/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_device.h"

#include "utils/io/log.h"

#include "types/vulkan/vk_renderer_system.h"
#include "types/core/renderer.h"

#include "lib/vulkan/vk_context_block.h"

#include <stdlib.h>
#include <string.h>

#include <volk/volk.h>

#define __DEFINE_REQUIRED_QUEUE_FAMILY(fam) required.fam = true
#define __DEFINE_REQUIRED_EXTENSION(name) if (out_extension_names) { out_extension_names[count_ret] = name; } count_ret++
#define __DEFINE_REQUIRED_DEVICE_FEATURE(name) out_features.name = VK_TRUE

#define __CHECK_REQUIRED_PHYSICAL_DEVICE_QUEUE_FAMILY(name)                                                                                 \
{                                                                                                                                           \
    if (required_fam_flags.name && available_fams.name <= -1) {                                                                             \
        TL_Error(debugger, "Device candidacy rejected: \"%s\" is missing a required %s-supporting queue family.",  props.deviceName, #name);\
        return false;                                                                                                                       \
    }                                                                                                                                       \
}

#define __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(name)                                                      \
{                                                                                                           \
    if (features.name && !available.name) {                                                                 \
        TL_Warn(debugger, "Graphics device \"%s\" missing feature: \"%s\"", props.deviceName, #name);       \
        supported.name = VK_FALSE;                                                                          \
        *out_missing_flag = true;                                                                           \
    }                                                                                                       \
}

#define __MAX_QUEUE_CREATE_INFO_COUNT 16

#define __STORE_QUEUE_HANDLE(name)                                                  \
{                                                                                   \
    if (queue_families.name > -1 && name ## _queue_count > 0) {                     \
        out_queues->name = carraynew(name ## _queue_count);                         \
        if (!out_queues->name.capacity) {                                           \
            TL_Fatal(debugger, "MALLOC fault in call to __CreateLogicalDevice");    \
            return VK_NULL_HANDLE;                                                  \
        }                                                                           \
                                                                                    \
        for (uint32_t i = 0; i < name ## _queue_count; i++) {                       \
            VkQueue qptr;                                                           \
            vkGetDeviceQueue(device, queue_families.name, i, &qptr);                \
            carraypush(&out_queues->name, (carrayval_t) qptr);                      \
        }                                                                           \
    }                                                                               \
}

// out_queue_score will be higher if, say, the transfer queue is independent/dedicated
static TLVK_PhysicalDeviceQueueFamilyIndices __GetDeviceQueueFamilyIndices(const VkPhysicalDevice physical_device, uint64_t *out_queue_score) {
    TLVK_PhysicalDeviceQueueFamilyIndices indices;

    // -1 indicates not found
    indices.graphics = -1;
    indices.compute = -1;
    indices.transfer = -1;
    indices.present = -1;

    uint64_t queue_score = 0;

    // get queue families
    uint32_t fam_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &fam_count, NULL);
    VkQueueFamilyProperties fams[fam_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &fam_count, fams);

    // this score is reset after each queue family, and is incremented each time one has a flag other than
    // VK_QUEUE_TRANSFER_BIT. finally, the family with the lowest score is identified as the transfer queue family,
    // as it is the most likely one to be a dedicated transfer queue.
    uint8_t min_trans_score = -1;

    for (uint32_t i = 0; i < fam_count; i++) {
        uint8_t cur_trans_score = 0;

        if (fams[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            // use any family that supports graphics operations as a graphics family
            indices.graphics = i;

            cur_trans_score++;
        }

        if (fams[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            // use any family that supports compute operations as a compute family
            indices.compute = i;

            cur_trans_score++;
        }

        if (fams[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            if (cur_trans_score <= min_trans_score) {
                indices.transfer = i;

                // aim for dedicated transfer queue on next iteration
                min_trans_score = cur_trans_score;
            }
        }

        indices.present = i;

        queue_score += fams[i].queueCount;
    }

    queue_score += min_trans_score;

    if (out_queue_score) {
        *out_queue_score = queue_score;
    }

    return indices;
}

// Get the queue family types required to support the given features
// Members of the returned __QueueFamilyIndices_t struct will be set to 1 if required, 0 if not.
static TLVK_PhysicalDeviceQueueFamilyIndices __GetRequiredQueueFamilies(const TL_RendererFeatures_t requirements) {
    TLVK_PhysicalDeviceQueueFamilyIndices required = { false };

    __DEFINE_REQUIRED_QUEUE_FAMILY(graphics); // always require graphics queue

    if (requirements.presentation) {
        __DEFINE_REQUIRED_QUEUE_FAMILY(present);
    }

    // TODO: conditions to require transfer families (and maybe compute)

    return required;
}

static carray_t __ValidateExtensions(const VkPhysicalDevice physical_device, const uint32_t count, const char *const *const names,
    bool *const out_missing_flag, const TL_Debugger_t *const debugger)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physical_device, &props);

    // get available extensions
    uint32_t available_count;
    vkEnumerateDeviceExtensionProperties(physical_device, NULL, &available_count, NULL);
    VkExtensionProperties available[available_count];
    vkEnumerateDeviceExtensionProperties(physical_device, NULL, &available_count, available);

    carray_t ret = carraynew(count);

    for (uint32_t i = 0; i < count; i++) {
        bool found = false;

        for (uint32_t j = 0; j < available_count; j++) {
            if (!strcmp(names[i], available[j].extensionName)) {
                // the extension is confirmed available
                found = true;
                carraypush(&ret, (carrayval_t) names[i]);

                break;
            }

            // VkDeviceCreateInfo: "If the VK_KHR_portability_subset extension is included in pProperties of vkEnumerateDeviceExtensionProperties,
            // ppEnabledExtensionNames must include "VK_KHR_portability_subset"" - from the Vulkan Specification
            if (!strcmp(available[j].extensionName, "VK_KHR_portability_subset")) {
                carraypush(&ret, (carrayval_t) "VK_KHR_portability_subset");
                continue;
            }
        }

        // extension not found
        if (!found) {
            TL_Warn(debugger, "Validating physical device \"%s\": could not find extension \"%s\"", props.deviceName, names[i]);
            *out_missing_flag = true;
        }
    }

    return ret;
}

// Get the extensions required to support the given features
static void __EnumerateRequiredExtensions(const TL_RendererFeatures_t requirements, uint32_t *const out_extension_count,
    const char **out_extension_names)
{
    if (!out_extension_count) {
        return;
    }

    uint32_t count_ret = 0;

    // renderers that can present images
    if (requirements.presentation) {
        // swapchain creation
        __DEFINE_REQUIRED_EXTENSION(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    *out_extension_count = count_ret;
}

static VkPhysicalDeviceFeatures __ValidateDeviceFeatures(const VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures features,
    bool *const out_missing_flag, const TL_Debugger_t *const debugger)
{
    // This function is pretty shit but oh well

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physical_device, &props);

    VkPhysicalDeviceFeatures available;
    vkGetPhysicalDeviceFeatures(physical_device, &available);

    VkPhysicalDeviceFeatures supported = features; // will be updated with the macro invokations below

    // scope for IDE collapsing :3
    {
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(robustBufferAccess);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fullDrawIndexUint32);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(imageCubeArray);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(independentBlend);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(geometryShader);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(tessellationShader);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sampleRateShading);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(dualSrcBlend);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(logicOp);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiDrawIndirect);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(drawIndirectFirstInstance);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthClamp);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBiasClamp);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fillModeNonSolid);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBounds);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(wideLines);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(largePoints);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(alphaToOne);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiViewport);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(samplerAnisotropy);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionETC2);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionASTC_LDR);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionBC);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(occlusionQueryPrecise);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(pipelineStatisticsQuery);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(vertexPipelineStoresAndAtomics);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fragmentStoresAndAtomics);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderTessellationAndGeometryPointSize);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderImageGatherExtended);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageExtendedFormats);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageMultisample);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageReadWithoutFormat);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageWriteWithoutFormat);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderUniformBufferArrayDynamicIndexing);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderSampledImageArrayDynamicIndexing);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageBufferArrayDynamicIndexing);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageArrayDynamicIndexing);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderClipDistance);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderCullDistance);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderFloat64);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt64);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt16);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceResidency);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceMinLod);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseBinding);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyBuffer);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage2D);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage3D);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency2Samples);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency4Samples);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency8Samples);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency16Samples);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyAliased);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(variableMultisampleRate);
        __CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(inheritedQueries);
    }

    return supported;
}

// Get the device features required to support the given renderer features
static VkPhysicalDeviceFeatures __EnumerateRequiredDeviceFeatures(const TL_RendererFeatures_t requirements) {
    VkPhysicalDeviceFeatures feat = { 0 };

    return feat;
}

// Return required extensions and features that can be safely requested from the device.
// The score is based on:
//    - supported extensions
//    - supported features
//    - device type (discrete, integrated, etc)
//    - available VRAM
static uint64_t __ScorePhysicalDevice(const VkPhysicalDevice physical_device, const TL_RendererFeatures_t requirements,
    carray_t *out_exts, VkPhysicalDeviceFeatures *out_features, const TL_Debugger_t *debugger)
{
    if (!out_exts || !out_features) {
        return 0;
    }

    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memprops;

    vkGetPhysicalDeviceProperties(physical_device, &props);
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memprops);

    uint64_t score = 1;

    TL_Log(debugger, "Scoring suitability of device \"%s\"", props.deviceName);

    // get required extensions
    uint32_t required_ext_count = 0;
    __EnumerateRequiredExtensions(requirements, &required_ext_count, NULL);
    const char *required_exts[required_ext_count];
    __EnumerateRequiredExtensions(requirements, &required_ext_count, required_exts);

    TL_Log(debugger, "  %s: Validating Vulkan extensions...", props.deviceName);

    bool is_missing_exts = false;

    // validate extensions - an array of supported extensions is returned (heap ptr) into out_exts.
    *out_exts = __ValidateExtensions(physical_device, required_ext_count, required_exts, &is_missing_exts, debugger);

    if (is_missing_exts) {
        TL_Error(debugger, "Missing device-level extensions for Vulkan physical device \"%s\", some features may not be available", props.deviceName);
    }

    // increase score relative to the amount of required extensions that are supported
    // this helps in cases where e.g. if no devices support all extensions; prioritise device with the *most* extensions supported out of required.
    // note that the casts to floats are to avoid integer division.
    if (required_ext_count > 0) {
        score += (uint64_t) (((float) out_exts->size / (float) required_ext_count) * 80);
    }

    if (out_exts->size == required_ext_count) {
        score += 250;
    }

    // get required features
    VkPhysicalDeviceFeatures required_feats = __EnumerateRequiredDeviceFeatures(requirements);

    TL_Log(debugger, "  %s: Validating Vulkan features...", props.deviceName);

    bool is_missing_features = false;

    // validate features - supported features is returned (heap ptr) into out_features.
    *out_features = __ValidateDeviceFeatures(physical_device, required_feats, &is_missing_features, debugger);

    if (is_missing_features) {
        TL_Error(debugger, "Missing device features for Vulkan physical device \"%s\", some renderer features may not be available",
            props.deviceName);
    }

    // further increase score if all features supported
    score += !is_missing_features * 250;

    // further increase score if all features supported
    score += !is_missing_features * 250;

    switch (props.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            // increase score for typically discrete GPUs
            // (significant performance advantage over integrated ofc)
            score += 1000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 200;
            break;
        default:
            score += 0;
            break;
    }

    // get memory size
    for (unsigned int i = 0; i < memprops.memoryHeapCount; i++) {
        if (memprops.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            // device local heap: .size will be size of VRAM in bytes.
            // we add this size to the score (converted to smaller number)
            score += memprops.memoryHeaps[i].size / (1024 * 1024 * 1024);
        }
    }

    return score;
}

VkDevice TLVK_LogicalDeviceCreate(const VkPhysicalDevice physical_device, const carray_t extensions, const VkPhysicalDeviceFeatures features,
    const TLVK_PhysicalDeviceQueueFamilyIndices queue_families, TLVK_LogicalDeviceQueues_t *const out_queues, const TL_Debugger_t *const debugger)
{
    if (!out_queues) {
        return VK_NULL_HANDLE;
    }

    VkDeviceCreateInfo device_create_info;
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.flags = 0;

    // deprecated and ignored
    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = NULL;

    device_create_info.enabledExtensionCount = extensions.size;
    device_create_info.ppEnabledExtensionNames = (const char *const *) extensions.data;

    device_create_info.pEnabledFeatures = &features;

    // array of unique indices
    carray_t unique_family_indices = carraynew(6);
        carraypush(&unique_family_indices, queue_families.graphics);
        carraypush(&unique_family_indices, queue_families.compute);
        carraypush(&unique_family_indices, queue_families.transfer);
        carraypush(&unique_family_indices, queue_families.present);
    // further array creation -- removing duplicate indices
    for (uint32_t i = 0; i < unique_family_indices.size - 1; i++) {
        for (uint32_t j = i + 1; j < unique_family_indices.size; j++) {
            if (unique_family_indices.data[i] == unique_family_indices.data[j]) {
                carrayremove(&unique_family_indices, i--);
                break;
            }
        }
    }

    VkDeviceQueueCreateInfo queue_create_infos[__MAX_QUEUE_CREATE_INFO_COUNT];
    uint32_t queue_create_info_count = 0;

    // amounts of queues to create from each family (if that family is required by the application)
    uint32_t graphics_queue_count = 1;
    uint32_t compute_queue_count = 1;
    uint32_t transfer_queue_count = 1;
    uint32_t present_queue_count = 1;

    float queue_priority = 1.0f;

    // set up queue create infos to create queue(s) from each queue family index...
    // note that an index of -1 means no queues are to be created from that family
    for (uint32_t i = 0; i < unique_family_indices.size; i++) {
        int32_t family_index = unique_family_indices.data[i];

        // -1 means no queues created for this family
        if (family_index <= -1) {
            continue;
        }

        VkDeviceQueueCreateInfo *queue_create_info = &(queue_create_infos[queue_create_info_count++]);
        queue_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info->pNext = NULL;
        queue_create_info->flags = 0;

        // amount of queues to create for this index, keeping in mind the index could be the same for multiple queue families.
        uint32_t queue_count =
            (family_index == queue_families.graphics) * graphics_queue_count +
            (family_index == queue_families.compute ) * compute_queue_count  +
            (family_index == queue_families.transfer) * transfer_queue_count +
            (family_index == queue_families.present ) * ((queue_families.graphics == queue_families.present) ? 0 : present_queue_count);

        queue_create_info->queueCount = queue_count;
        queue_create_info->queueFamilyIndex = family_index;

        queue_create_info->pQueuePriorities = &queue_priority;
    }

    carrayfree(&unique_family_indices);

    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.queueCreateInfoCount = queue_create_info_count;

    // create device
    VkDevice device;
    if (vkCreateDevice(physical_device, &device_create_info, NULL, &device)) {
        return VK_NULL_HANDLE;
    };

    // init arrays to be empty
    out_queues->graphics.size = 0;
    out_queues->compute.size = 0;
    out_queues->transfer.size = 0;
    out_queues->present.size = 0;

    __STORE_QUEUE_HANDLE(graphics);
    __STORE_QUEUE_HANDLE(compute);
    __STORE_QUEUE_HANDLE(transfer);
    __STORE_QUEUE_HANDLE(present);

    return device;
}

bool TLVK_PhysicalDeviceCheckCandidacy(const VkPhysicalDevice physical_device, const TL_RendererFeatures_t requirements,
    const TL_Debugger_t *const debugger)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physical_device, &props);

    // get available and required queue families
    TLVK_PhysicalDeviceQueueFamilyIndices required_fam_flags = __GetRequiredQueueFamilies(requirements);
    TLVK_PhysicalDeviceQueueFamilyIndices available_fams = __GetDeviceQueueFamilyIndices(physical_device, NULL);

    // validate queue families...
    __CHECK_REQUIRED_PHYSICAL_DEVICE_QUEUE_FAMILY(graphics);
    __CHECK_REQUIRED_PHYSICAL_DEVICE_QUEUE_FAMILY(compute);
    __CHECK_REQUIRED_PHYSICAL_DEVICE_QUEUE_FAMILY(transfer);
    __CHECK_REQUIRED_PHYSICAL_DEVICE_QUEUE_FAMILY(present);

    return true;
}

TLVK_PhysicalDeviceQueueFamilyIndices TLVK_PhysicalDeviceQueueFamilyIndicesGetEnabled(const VkPhysicalDevice physical_device,
    const TL_RendererFeatures_t requirements)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physical_device, &props);

    // get available and required queue families
    TLVK_PhysicalDeviceQueueFamilyIndices required_fam_flags = __GetRequiredQueueFamilies(requirements);
    TLVK_PhysicalDeviceQueueFamilyIndices available_fams = __GetDeviceQueueFamilyIndices(physical_device, NULL);

    // when returning family indices to use, -1 is used to indicate that no queues need to be created from a certain queue family.
    return (TLVK_PhysicalDeviceQueueFamilyIndices) {
        (required_fam_flags.graphics) ? available_fams.graphics : -1,
        (required_fam_flags.compute ) ? available_fams.compute  : -1,
        (required_fam_flags.transfer) ? available_fams.transfer : -1,
        (required_fam_flags.present ) ? available_fams.present  : -1
    };
}

VkPhysicalDevice TLVK_PhysicalDeviceSelect(const carray_t candidates, const TL_RendererFeatures_t requirements, carray_t *const out_extensions,
    VkPhysicalDeviceFeatures *const out_features, const TL_Debugger_t *const debugger)
{
    if (!out_extensions || !out_features) {
        return 0;
    }

    if (candidates.size == 1) {
        return (VkPhysicalDevice) candidates.data[0];
    }

    // get the physical device with highest score
    uint64_t min_score = 0;
    VkPhysicalDevice best_physical_device = VK_NULL_HANDLE;

    for (uint32_t i = 0; i < candidates.size; i++) {
        VkPhysicalDevice dev = (VkPhysicalDevice) candidates.data[i];

        carray_t cur_dev_exts;
        VkPhysicalDeviceFeatures cur_dev_feats;

        // score device
        uint64_t cur_score = __ScorePhysicalDevice(dev, requirements, &cur_dev_exts, &cur_dev_feats, debugger);

        if (debugger) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(dev, &props);

            TL_Note(debugger, "%s scored Thallium score of [%lu]", props.deviceName, cur_score);
        }

        // best device so far
        if (cur_score > min_score) {
            min_score = cur_score;
            best_physical_device = dev;

            // free old extensions array, we only do this after the first iteration as otherwise it won't have been initialised yet.
            if (i > 0) {
                carrayfree(out_extensions);
            }

            // store the extensions and features that can be enabled for logical devices interfacing this physical device
            *out_extensions = cur_dev_exts;
            *out_features = cur_dev_feats;
        } else {
            // this array can be discarded
            carrayfree(&cur_dev_exts);
        }
    }

    return best_physical_device;
}