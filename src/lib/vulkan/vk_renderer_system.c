/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/vulkan/vk_renderer_system.h"
#include "types/vulkan/vk_renderer_system.h"

#include "types/core/context.h"
#include "types/core/renderer.h"
#include "utils/io/log.h"

#include "vk_context_block.h"
#include "vk_device.h"

#include <volk/volk.h>

#include <stdlib.h>

static VkPhysicalDevice __SelectRendererSystemPhysicalDevice(const TLVK_RendererSystem_t *const renderer_system,
    const TLVK_RendererSystemDescriptor_t *const descriptor, carray_t *const out_exts, VkPhysicalDeviceFeatures *const out_feats,
    const TL_Debugger_t *const debugger);


TLVK_RendererSystem_t *TLVK_RendererSystemCreate(TL_Renderer_t *const renderer, const TLVK_RendererSystemDescriptor_t descriptor) {
    // if renderer is not NULL then we can assume the context was populated, as renderers are only created in core after populating their context
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;
    TL_RendererFeatures_t rendfeatures = renderer->features;

    TLVK_RendererSystem_t *renderer_system = malloc(sizeof(TLVK_RendererSystem_t));
    if (!renderer_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_RendererSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan renderer system at %p", renderer_system);

    renderer_system->renderer = renderer;
    renderer_system->vk_context = (TLVK_ContextBlock_t *) ((char *) renderer->context->data + renderer->context->vulkan_offset);

    VkPhysicalDevice physdev = __SelectRendererSystemPhysicalDevice(renderer_system, &descriptor, &renderer_system->device_extensions,
        &renderer_system->vk_device_features, debugger);
    if (physdev == VK_NULL_HANDLE) {
        TL_Error(debugger, "Failed to select physical device for Vulkan renderer system %p", renderer_system);
        return NULL;
    }
    renderer_system->vk_physical_device = physdev;

    VkPhysicalDeviceProperties props;

    // print information about the selected physical device
    if (debugger) {
        vkGetPhysicalDeviceProperties(physdev, &props);

        TL_Note(debugger, "Vulkan renderer system %p physical device selection - this renderer system will use the following GPU:", renderer_system);
        TL_Note(debugger, "  %s", props.deviceName);

        TL_Log(debugger, "  Supports Vulkan API version %d.%d.%d", VK_API_VERSION_MAJOR(props.apiVersion), VK_API_VERSION_MINOR(props.apiVersion),
            VK_API_VERSION_PATCH(props.apiVersion));
        TL_Log(debugger, "  VkPhysicalDevice handle at %p", physdev);
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

    carray_t exts = renderer_system->device_extensions;
    VkPhysicalDeviceFeatures feats = renderer_system->vk_device_features;
    TLVK_PhysicalDeviceQueueFamilyIndices_t qf = TLVK_PhysicalDeviceQueueFamilyIndicesGetEnabled(physdev, rendfeatures);

    VkDevice dev = TLVK_LogicalDeviceCreate(
        physdev, exts, feats, qf, &renderer_system->vk_queues, &rendfeatures, &renderer_system->devfs, debugger);
    if (dev == VK_NULL_HANDLE) {
        TL_Error(debugger, "Failed to create Vulkan logical device object in renderer system %p", renderer_system);
        return NULL;
    }
    renderer_system->vk_logical_device = dev;

    // store queue family indices
    renderer_system->vk_queues.graphics_family = qf.graphics;
    renderer_system->vk_queues.compute_family = qf.compute;
    renderer_system->vk_queues.transfer_family = qf.transfer;
    renderer_system->vk_queues.present_family = qf.present;

    if (debugger) {
        TL_Log(debugger, "Created Vulkan device object at %p in Thallium Vulkan renderer system %p", dev, renderer_system);

        TL_Log(debugger, "  Interfacing physical device \"%s\"", props.deviceName);

        TL_Log(debugger, "  %d extensions", exts.size);
        for (uint32_t i = 0; i < exts.size; i++) {
            TL_Log(debugger, "    - extension #%d: %s", i, exts.data[i]);
        }

        TLVK_LogicalDeviceQueues_t queues = renderer_system->vk_queues;

        uint32_t queue_count =
            queues.graphics.size +
            queues.compute.size +
            queues.transfer.size +
            queues.present.size;

        TL_Log(debugger, "  %d queues", queue_count);
        if (qf.graphics > -1)
            TL_Log(debugger, "    - %d graphics (family index: %d)", queues.graphics.size, qf.graphics);
        if (qf.compute > -1)
            TL_Log(debugger, "    - %d compute  (family index: %d)", queues.compute.size, qf.compute);
        if (qf.transfer > -1)
            TL_Log(debugger, "    - %d transfer (family index: %d)", queues.transfer.size, qf.transfer);
        if (qf.present > -1)
            TL_Log(debugger, "    - %d present  (family index: %d)", queues.present.size, qf.present);
    }

    return renderer_system;
}

void TLVK_RendererSystemDestroy(TLVK_RendererSystem_t *const renderer_system) {
    if (!renderer_system) {
        return;
    }

    const TLVK_FuncSet_t *devfs = &(renderer_system->devfs);

    devfs->vkDestroyDevice(renderer_system->vk_logical_device, NULL);

    carrayfree(&renderer_system->device_extensions);

    // free arrays of queue handles
    if (renderer_system->vk_queues.graphics.size) carrayfree(&(renderer_system->vk_queues.graphics));
    if (renderer_system->vk_queues.compute.size)  carrayfree(&(renderer_system->vk_queues.compute));
    if (renderer_system->vk_queues.transfer.size) carrayfree(&(renderer_system->vk_queues.transfer));
    if (renderer_system->vk_queues.present.size)  carrayfree(&(renderer_system->vk_queues.present));

    free(renderer_system);
}


static VkPhysicalDevice __SelectRendererSystemPhysicalDevice(const TLVK_RendererSystem_t *const renderer_system,
    const TLVK_RendererSystemDescriptor_t *const descriptor, carray_t *const out_exts, VkPhysicalDeviceFeatures *const out_feats,
    const TL_Debugger_t *const debugger)
{
    if (!out_exts || !out_feats || !descriptor) {
        return VK_NULL_HANDLE;
    }

    VkInstance instance = renderer_system->vk_context->vk_instance;
    TL_RendererFeatures_t requirements = renderer_system->renderer->features;

    TLVK_PhysicalDeviceSelectionMode_t selectmode = descriptor->physical_device_mode;

    // init array of all physical device
    uint32_t candidate_count;
    vkEnumeratePhysicalDevices(instance, &candidate_count, NULL);

    if (!candidate_count) {
        TL_Error(debugger, "No Vulkan-capable GPUs found!");
        return VK_NULL_HANDLE;
    }

    carray_t candidates = carraynew(candidate_count);
    candidates.size = candidates.capacity; // we know the size but we aren't 'pushing' to the array, so set it manually instead
    vkEnumeratePhysicalDevices(instance, &candidate_count, (VkPhysicalDevice *) candidates.data);

    // remove unsuitable devices
    for (uint32_t i = 0; i < candidates.size; i++) {
        if (!TLVK_PhysicalDeviceCheckCandidacy((VkPhysicalDevice) candidates.data[i], requirements, debugger)) {
            carrayremove(&candidates, i--);
        }
    }

    if (!candidates.size) {
        TL_Error(debugger, "No physical devices were determined suitable for use in Vulkan renderer system at %p", renderer_system);
        return VK_NULL_HANDLE;
    }

    VkPhysicalDevice ret;

    switch (selectmode) {
        case TLVK_PHYSICAL_DEVICE_SELECTION_MODE_OPTIMAL:
        default:
            // select optimal physical device (with score etc)
            ret = TLVK_PhysicalDeviceSelect(candidates, requirements, selectmode, out_exts, out_feats, debugger);
            break;
    }

    carrayfree(&candidates);

    return ret;
}
