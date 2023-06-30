/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/io/log.h"

#include "types/vulkan/vk_device_manager.h"
#include "types/vulkan/vk_render_system.h"
#include "types/core/renderer.h"

#include <stdlib.h>
#include <string.h>

#include <volk/volk.h>

typedef struct __QueueFamilyIndices_t {
    int32_t graphics;
    int32_t compute;
    int32_t transfer;
    int32_t present;
} __QueueFamilyIndices_t;

#define CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(name)                                                        \
{                                                                                                           \
    if (features.name && !available.name) {                                                                 \
        TL_Warn(debugger, "Graphics device \"%s\" missing feature: \"%s\"", props.deviceName, #name);       \
        supported.name = VK_FALSE;                                                                          \
        *out_missing_flag = true;                                                                           \
    }                                                                                                       \
}

#define __DEFINE_REQUIRED_EXTENSION(name) if (out_extension_names) { out_extension_names[count_ret] = name; } count_ret++
#define __DEFINE_REQUIRED_DEVICE_FEATURE(name) out_features.name = VK_TRUE

// out_queue_score will be higher if, say, the transfer queue is independent/dedicated
static __QueueFamilyIndices_t __GetDeviceQueueFamilyIndices(const VkPhysicalDevice physical_device, uint64_t *out_queue_score) {
    __QueueFamilyIndices_t indices;

    // -1 indicates not found
    indices.graphics = -1;
    indices.compute = -1;
    indices.transfer = -1;
    indices.present = -1;

    if (!out_queue_score) {
        return indices;
    }

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

        // TODO: check for present queue
        indices.present = i;

        queue_score += fams[i].queueCount;
    }

    queue_score += min_trans_score;
    *out_queue_score = queue_score;

    return indices;
}

// Get the queue family types required to support the given features
// Members of the returned __QueueFamilyIndices_t struct will be set to 1 if required, 0 if not.
static __QueueFamilyIndices_t __GetRequiredQueueFamilies(const TL_RendererFeatures_t requirements) {
    __QueueFamilyIndices_t required = { false };

    required.graphics = true; // always require graphics queue

    if (requirements.presentation) {
        required.present = true;
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
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(robustBufferAccess);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fullDrawIndexUint32);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(imageCubeArray);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(independentBlend);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(geometryShader);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(tessellationShader);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sampleRateShading);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(dualSrcBlend);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(logicOp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiDrawIndirect);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(drawIndirectFirstInstance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthClamp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBiasClamp);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fillModeNonSolid);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(depthBounds);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(wideLines);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(largePoints);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(alphaToOne);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(multiViewport);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(samplerAnisotropy);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionETC2);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionASTC_LDR);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(textureCompressionBC);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(occlusionQueryPrecise);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(pipelineStatisticsQuery);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(vertexPipelineStoresAndAtomics);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(fragmentStoresAndAtomics);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderTessellationAndGeometryPointSize);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderImageGatherExtended);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageExtendedFormats);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageMultisample);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageReadWithoutFormat);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageWriteWithoutFormat);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderUniformBufferArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderSampledImageArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageBufferArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderStorageImageArrayDynamicIndexing);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderClipDistance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderCullDistance);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderFloat64);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt64);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderInt16);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceResidency);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(shaderResourceMinLod);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseBinding);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyBuffer);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage2D);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyImage3D);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency2Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency4Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency8Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidency16Samples);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(sparseResidencyAliased);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(variableMultisampleRate);
        CHECK_REQUIRED_PHYSICAL_DEVICE_FEATURE(inheritedQueries);
    }

    return supported;
}

// Get the device features required to support the given renderer features
static VkPhysicalDeviceFeatures __EnumerateRequiredDeviceFeatures(const TL_RendererFeatures_t requirements) {
    VkPhysicalDeviceFeatures feat = { 0 };

    return feat;
}

// If 0 is returned then the device is unsuitable.
// Return required extensions and features that can be safely requested from the device.
// score is based on:
//    - supported queue families
//    - supported extensions
//    - supported features
//    - device type (discrete, integrated, etc)
//    - available VRAM
static uint64_t __ScorePhysicalDevice(const VkPhysicalDevice physical_device, const TL_RendererFeatures_t requirements,
    carray_t *out_exts, VkPhysicalDeviceFeatures *out_features, __QueueFamilyIndices_t *out_families, const TL_Debugger_t *debugger)
{
    if (!out_exts || !out_features || !out_families) {
        return 0;
    }

    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceMemoryProperties memprops;

    vkGetPhysicalDeviceProperties(physical_device, &props);
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memprops);

    uint64_t score = 0;

    TL_Log(debugger, "Scoring suitability of device \"%s\"", props.deviceName);

    TL_Log(debugger, "  %s: Validating Vulkan queue families...", props.deviceName);

    uint64_t queue_fam_score;

    // get available and required queue families
    __QueueFamilyIndices_t required_fams = __GetRequiredQueueFamilies(requirements);
    __QueueFamilyIndices_t available_fams = __GetDeviceQueueFamilyIndices(physical_device, &queue_fam_score);

    TL_Log(debugger, "    Scored queue family score of %lu", queue_fam_score);

    // validate queue families...
    // as it is difficult to keep a program running without the required queue families, we return early.
    if (required_fams.graphics && available_fams.graphics <= -1) {
        TL_Warn(debugger, "Device \"%s\" is missing a graphics-supporting queue family, which the application requires; this device cannot be used.",
            props.deviceName);
        return 0;
    }
    if (required_fams.compute && available_fams.compute <= -1) {
        TL_Warn(debugger, "Device \"%s\" is missing a compute-supporting queue family, which the application requires; this device cannot be used.",
            props.deviceName);
        return 0;
    }
    if (required_fams.transfer && available_fams.transfer <= -1) {
        TL_Warn(debugger, "Device \"%s\" is missing a transfer-supporting queue family, which the application requires; this device cannot be used.",
            props.deviceName);
        return 0;
    }
    if (required_fams.present && available_fams.present <= -1) {
        TL_Warn(debugger, "Device \"%s\" is missing a present-supporting queue family, which the application requires; this device cannot be used.",
            props.deviceName);
        return 0;
    }

    // when returning family indices to use, -1 is used to indicate that no queues need to be created from a certain queue family.
    out_families->graphics = (required_fams.graphics) ? available_fams.graphics : -1;
    out_families->compute  = (required_fams.compute ) ? available_fams.compute  : -1;
    out_families->transfer = (required_fams.transfer) ? available_fams.transfer : -1;
    out_families->present  = (required_fams.present ) ? available_fams.present  : -1;

    score += queue_fam_score;

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
        TL_Warn(debugger, "Missing device-level extensions for Vulkan physical device \"%s\", some features may not be available", props.deviceName);
    }

    // increase score relative to the amount of required extensions that are supported
    // this helps in cases where e.g. if no devices support all extensions; prioritise device with the *most* extensions supported out of required.
    // note that the casts to floats are to avoid integer division.
    if (required_ext_count > 0) {
        score += (uint64_t) (((float) out_exts->size / (float) required_ext_count) * 80);
    }

    if (out_exts->size == required_ext_count)
        score += 250;

    // get required features
    VkPhysicalDeviceFeatures required_feats = __EnumerateRequiredDeviceFeatures(requirements);

    TL_Log(debugger, "  %s: Validating Vulkan features...", props.deviceName);

    bool is_missing_features = false;

    // validate features - supported features is returned (heap ptr) into out_features.
    *out_features = __ValidateDeviceFeatures(physical_device, required_feats, &is_missing_features, debugger);

    if (is_missing_features) {
        TL_Warn(debugger, "Missing device features for Vulkan physical device \"%s\", some renderer features may not be available", props.deviceName);
    }

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

TLVK_DeviceManager_t *TLVK_CreateDeviceManager(const TLVK_RenderSystem_t *const render_system, const TLVK_DeviceManagerDescriptor_t descriptor) {
    if (!render_system || !render_system->renderer || !render_system->vk_context) {
        return NULL;
    }

    const TL_Debugger_t *debugger = render_system->renderer->debugger;
    const VkInstance instance = render_system->vk_context->vk_instance;

    TLVK_DeviceManager_t *device_manager = malloc(sizeof(TLVK_DeviceManager_t));
    if (!device_manager) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_CreateDeviceManager");
        return NULL;
    }

    device_manager->extensions.data = NULL; // we initialise to null array as it will be allocated and populated later.

    TL_Log(debugger, "Allocated memory for Vulkan device manager at %p", device_manager);

    // get all physical devices
    uint32_t pd_count;
    vkEnumeratePhysicalDevices(instance, &pd_count, NULL);
    if (pd_count == 0)
        TL_Error(debugger, "The Vulkan API failed to enumerate any available physical devices");
    VkPhysicalDevice pd_arr[pd_count];
    vkEnumeratePhysicalDevices(instance, &pd_count, pd_arr);

    // get the physical device with highest score
    uint64_t min_score = 0;
    VkPhysicalDevice best_physical_device = VK_NULL_HANDLE;

    __QueueFamilyIndices_t best_device_queue_fams;

    for (uint32_t i = 0; i < pd_count; i++) {
        VkPhysicalDevice dev = pd_arr[i];

        carray_t cur_dev_exts;
        VkPhysicalDeviceFeatures cur_dev_feats;

        // score device
        uint64_t cur_score = __ScorePhysicalDevice(dev, render_system->features, &cur_dev_exts, &cur_dev_feats, &best_device_queue_fams, debugger);

        if (debugger) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(dev, &props);

            TL_Note(debugger, "%s scored Thallium score of %lu", props.deviceName, cur_score);
        }

        // best device so far
        if (cur_score > min_score) {
            min_score = cur_score;
            best_physical_device = dev;

            // store the extensions and features that can be enabled for logical devices interfacing this physical device
            carrayfree(&device_manager->extensions); // freeing old extensions array
            device_manager->extensions = cur_dev_exts;
            device_manager->vk_features = cur_dev_feats;
        } else {
            // this array can be discarded
            carrayfree(&cur_dev_exts);
        }
    }

    // no device was suitable
    if (best_physical_device == VK_NULL_HANDLE) {
        TL_Error(debugger, "No GPU was determined suitable for Vulkan device manager creation in render system %p (in renderer %p)", render_system,
            render_system->renderer);

        return NULL;
    }

    device_manager->vk_physical_device = best_physical_device;

    // print information about the selected physical device
    if (debugger) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device_manager->vk_physical_device, &props);

        TL_Note(debugger, "Vulkan device manager %p creation: using best suitable Vulkan physical device based on configuration of render"
            " system at %p", device_manager, render_system);
        TL_Note(debugger, "  %s", props.deviceName);

        TL_Log(debugger, "  (Thallium score: %lu)", min_score);
        TL_Log(debugger, "  Supports Vulkan API version %d.%d.%d", VK_API_VERSION_MAJOR(props.apiVersion), VK_API_VERSION_MINOR(props.apiVersion),
            VK_API_VERSION_PATCH(props.apiVersion));
        TL_Log(debugger, "  VkPhysicalDevice handle at %p", device_manager->vk_physical_device);
        TL_Log(debugger, "  Device ID: 0x%04hhx", props.deviceID);

        // printing device vendor from known PCI IDs
        switch (props.vendorID) {
            case 0x1002:
                TL_Log(debugger, "  Device vendor: AMD");
                break;
            case 0x1010:
                TL_Log(debugger, "  Device vendor: ImgTec");
                break;
            case 0x10de:
                TL_Log(debugger, "  Device vendor: NVIDIA");
                break;
            case 0x13b5:
                TL_Log(debugger, "  Device vendor: ARM");
                break;
            case 0x5143:
                TL_Log(debugger, "  Device vendor: Qualcomm");
                break;
            case 0x8086:
                TL_Log(debugger, "  Device vendor: Intel");
                break;
            default:
                TL_Log(debugger, "  Device vendor: unknown");
                break;
        }
        TL_Log(debugger, "    (Vendor id = 0x%04hhx)", props.vendorID);
    }

    // TODO: create device manager's logical device using the selected physical device

    return device_manager;
}

void TLVK_DestroyDeviceManager(TLVK_DeviceManager_t *const device_manager) {
    if (!device_manager) {
        return;
    }

    free(device_manager);
}
