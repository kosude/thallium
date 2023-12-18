/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "thallium/core/extent.h"

#include "utils/utils.h"

#include "types/core/renderer.h"
#include "types/core/wsi/window_surface.h"
#include "types/vulkan/vk_renderer_system.h"
#include "types/vulkan/vk_swapchain_system.h"

#include "lib/core/wsi/surface_platform_data.h"
#include "lib/vulkan/vk_context_block.h"

#include <volk/volk.h>

#include <stdlib.h>

static VkSurfaceKHR __CreateVkSurface(const VkInstance instance, const TL_WindowSurface_t *const tl_surface, const TL_Debugger_t *const debugger) {
    if (!tl_surface || !tl_surface->platform_data) {
        return VK_NULL_HANDLE;
    }

    TL_WSI_API_t wsiapi = tl_surface->wsi;

    VkSurfaceKHR surface;

    switch (wsiapi) {
        default:
            TL_Error(debugger, "Invalid TL_WindowSurface_t format encountered when attempting to create Vulkan surface");
            return VK_NULL_HANDLE;

        // ---------------- cocoa window surface ----------------
        case TL_WSI_API_COCOA:;
#           if defined(_THALLIUM_WSI_COCOA)
                TL_WindowSurfacePlatformDataCocoa_t *cocoa_native = (TL_WindowSurfacePlatformDataCocoa_t *) tl_surface->platform_data;

                VkMetalSurfaceCreateInfoEXT cocoa_mt_create_info;
                cocoa_mt_create_info.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
                cocoa_mt_create_info.pNext = NULL;
                cocoa_mt_create_info.flags = 0;
                cocoa_mt_create_info.pLayer = (CAMetalLayer *) cocoa_native->mt_layer; // note a CAMetalLayer forward decl is provided by Vulkan.

                vkCreateMetalSurfaceEXT(instance, &cocoa_mt_create_info, NULL, &surface);

                break;
#           else
                // Thallium XCB support not compiled
                TL_Error(debugger, "Attempted to create a Vulkan surface for Cocoa window, but Thallium Cocoa support was not compiled; "
                    "Recompile Thallium with the -DTHALLIUM_WSI_COCOA=ON flag!");
                return VK_NULL_HANDLE;
#           endif

        // ---------------- xcb window surface ----------------
        case TL_WSI_API_XCB:;
#           if defined(_THALLIUM_WSI_XCB)
                TL_WindowSurfacePlatformDataXCB_t *xcb_native = (TL_WindowSurfacePlatformDataXCB_t *) tl_surface->platform_data;

                VkXcbSurfaceCreateInfoKHR xcb_create_info;
                xcb_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
                xcb_create_info.pNext = NULL;
                xcb_create_info.flags = 0;
                xcb_create_info.connection = xcb_native->connection;
                xcb_create_info.window = xcb_native->window;

                vkCreateXcbSurfaceKHR(instance, &xcb_create_info, NULL, &surface);

                break;
#           else
                // Thallium XCB support not compiled
                TL_Error(debugger, "Attempted to create a Vulkan surface for XCB window, but Thallium XCB support was not compiled; "
                    "Recompile Thallium with the -DTHALLIUM_WSI_XCB=ON flag!");
                return VK_NULL_HANDLE;
#           endif

        // ---------------- xlib window surface ----------------
        case TL_WSI_API_XLIB:;
#           if defined(_THALLIUM_WSI_XLIB)
                TL_WindowSurfacePlatformDataXlib_t *xlib_native = (TL_WindowSurfacePlatformDataXlib_t *) tl_surface->platform_data;

                VkXlibSurfaceCreateInfoKHR xlib_create_info;
                xlib_create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
                xlib_create_info.pNext = NULL;
                xlib_create_info.flags = 0;
                xlib_create_info.dpy = xlib_native->display;
                xlib_create_info.window = xlib_native->window;

                vkCreateXlibSurfaceKHR(instance, &xlib_create_info, NULL, &surface);

                break;
#           else
                // Thallium XCB support not compiled
                TL_Error(debugger, "Attempted to create a Vulkan surface for Xlib window, but Thallium Xlib support was not compiled; "
                    "Recompile Thallium with the -DTHALLIUM_WSI_XLIB=ON flag!");
                return VK_NULL_HANDLE;
#           endif
    }

    return surface;
}

/**
 * @brief A struct containing information about what swapchains from a certain device can support.
 *
 * This struct contains info about what swapchains from a certain VkPhysicalDevice can support.
 */
typedef struct TLVK_SwapchainSupportInfo_t {
    /// @brief Basic surface capabilities supported - min/max images in swapchain, min/max size of images, etc
    VkSurfaceCapabilitiesKHR caps;

    /// @brief Supported surface formats (pixel format, colour space)
    VkSurfaceFormatKHR *formats;
    /// @brief Amount of elements in array `formats`
    uint32_t format_count;

    /// @brief Available presentation modes (fifo, mailbox, etc)
    VkPresentModeKHR *present_modes;
    /// @brief Amount of elements in array `present_modes`
    uint32_t present_mode_count;
} TLVK_SwapchainSupportInfo_t;

static TLVK_SwapchainSupportInfo_t __GetSwapchainSupportInfo(const VkPhysicalDevice physical_device, const VkSurfaceKHR surface) {
    TLVK_SwapchainSupportInfo_t details;

    // get surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.caps);

    // get supported surface formats
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details.format_count, NULL);
    if (details.format_count > 0) {
        details.formats = malloc(sizeof(VkSurfaceFormatKHR) * details.format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details.format_count, details.formats);
    }

    // get supported presentation modes
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details.present_mode_count, NULL);
    if (details.format_count > 0) {
        details.present_modes = malloc(sizeof(VkPresentModeKHR) * details.present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details.present_mode_count, details.present_modes);
    }

    return details;
}

// select most optimal available format for the swapchain to use, from the candidates specified.
static VkSurfaceFormatKHR __PickSwapSurfaceFormat(const VkSurfaceFormatKHR *const formats, const uint32_t format_count) {
    for (uint32_t i = 0; i < format_count; i++) {
        // ideally we use sRGB colour space with a BGRA which is a common format.
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return formats[i];
        }
    }

    // fallback
    return formats[0];
}

// select best presentation mode for the swapchain to use from the candidates specified.
static VkPresentModeKHR __PickSwapPresentMode(const VkPresentModeKHR *const modes, const uint32_t mode_count) {
    for (uint32_t i = 0; i < mode_count; i++) {
        // use mailbox if available
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return modes[i];
        }
    }

    // the Specification guarantees FIFO (vsync) to be available
    return VK_PRESENT_MODE_FIFO_KHR;
}

// `width` and `height` are clamped to min/max extents as specified in `caps`.
static VkExtent2D __PickSwapExtent(const VkSurfaceCapabilitiesKHR caps, const uint32_t width, const uint32_t height) {
    // if current extent width/height is *not* set to uint32_max by the wm, then we match the extent to the window size
    if (caps.currentExtent.width != UINT32_MAX) {
        return caps.currentExtent;
    }

    // clamp width
    uint32_t w = (width < caps.minImageExtent.width) ? caps.minImageExtent.width : width;
    w = (w > caps.maxImageExtent.width) ? caps.maxImageExtent.width : w;

    // clamp height
    uint32_t h = (height < caps.minImageExtent.height) ? caps.minImageExtent.height : height;
    h = (h > caps.maxImageExtent.height) ? caps.maxImageExtent.height : h;

    return (VkExtent2D) { w, h };
}

TLVK_SwapchainSystem_t *TLVK_SwapchainSystemCreate(const TLVK_RendererSystem_t *const renderer_system, const TL_Extent2D_t resolution,
    const TLVK_SwapchainSystemDescriptor_t descriptor, const TL_WindowSurface_t *const window_surface)
{
    // note that we assume the renderer has the presentation feature enabled because this is checked in TL_SwapchainCreate (abstraction layer above)

    if (!renderer_system) {
        return NULL;
    }

    const TLVK_FuncSet_t *devfs = &(renderer_system->devfs);

    const TL_Debugger_t *debugger = renderer_system->renderer->debugger;
    VkInstance instance = renderer_system->vk_context->vk_instance;

    VkPhysicalDevice physdev = renderer_system->vk_physical_device;
    VkDevice dev = renderer_system->vk_logical_device;
    TLVK_LogicalDeviceQueues_t queues = renderer_system->vk_queues;

    if (!window_surface && !descriptor.vk_surface) {
        TL_Error(debugger, "When attempting to create Vulkan swapchain system: either a Thallium window surface or a Vulkan VkSurfaceKHR must be "
            "supplied to TLVK_SwapchainSystemCreate");
        return NULL;
    }

    TLVK_SwapchainSystem_t *swapchain_system = malloc(sizeof(TLVK_SwapchainSystem_t));
    if (!swapchain_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_SwapchainSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan swapchain system at %p", swapchain_system);

    swapchain_system->vk_image_count = 0;
    swapchain_system->vk_images = NULL;

    swapchain_system->renderer_system = renderer_system;

    swapchain_system->vk_instance = instance;

    VkSurfaceKHR surface;

    if (descriptor.vk_surface) {
        // if caller directly passed an existing Vulkan surface, we use that
        surface = descriptor.vk_surface;
    } else {
        // if no surface was directly passed, we create a new one for the window (passed via Thallium window surface `window_surface`)
        surface = __CreateVkSurface(instance, window_surface, debugger);
        if (surface == VK_NULL_HANDLE) {
            TL_Error(debugger, "Failed to create Vulkan surface for new swapchain system at %p", swapchain_system);
            return NULL;
        }
    }

    swapchain_system->vk_surface = surface;

    TLVK_SwapchainSupportInfo_t support_info = __GetSwapchainSupportInfo(physdev, surface);

    // automatically select surface format if not explicitly chosen
    VkSurfaceFormatKHR surface_format = ((int) descriptor.vk_surface_format.format != -1) ?
        descriptor.vk_surface_format : __PickSwapSurfaceFormat(support_info.formats, support_info.format_count);

    // automatically select present mode if not explicitly chosen
    VkPresentModeKHR present_mode = ((int) descriptor.vk_present_mode != -1) ?
        descriptor.vk_present_mode : __PickSwapPresentMode(support_info.present_modes, support_info.present_mode_count);

    // clamp swap extent to capabilities
    VkExtent2D extent = __PickSwapExtent(support_info.caps, resolution.width, resolution.height);

    // get min image from support info
    uint32_t min_img_count = support_info.caps.minImageCount + 1;

    // transformation applied before presentation
    uint32_t pre_trans = support_info.caps.currentTransform; // no image transformation

    // be sure to free support info struct now that we have extracted the neceessary info.
    free(support_info.formats);
    free(support_info.present_modes);

    VkSwapchainCreateInfoKHR create_info;
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.pNext = NULL;
    create_info.flags = 0;

    create_info.surface = surface;
    create_info.minImageCount = min_img_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // queue families which images may be shared across (if the indices are different)
    int32_t graphicsfam = queues.graphics_family;
    int32_t presentfam = queues.present_family;

    if (graphicsfam != presentfam) {
        // if the queue families are different then allow sharing between them
        // (as the images need to be accessed for both drawing and presentation)
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = (uint32_t[]) { graphicsfam, presentfam };
    } else {
        // if the queue families are the same then specify exclusive image ownership
        // (as drawing and presentation will therefore occur within the same family)
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = NULL;
    }

    create_info.preTransform = pre_trans;

    // don't blend with other windows in the window system
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    create_info.oldSwapchain = VK_NULL_HANDLE; // TODO: Swapchain recreation: https://trello.com/c/l1Fe6hcf

    // create swapchain into system
    if (devfs->vkCreateSwapchainKHR(dev, &create_info, NULL, &swapchain_system->vk_swapchain)) {
        TL_Error(debugger, "Failed to create Vulkan swapchain object in swapchain system %p", swapchain_system);

        free(swapchain_system);
        return NULL;
    }

    VkSwapchainKHR swapchain = swapchain_system->vk_swapchain;

    // retrieve image handles
    devfs->vkGetSwapchainImagesKHR(dev, swapchain, &swapchain_system->vk_image_count, NULL);
    VkImage *images = malloc(sizeof(VkImage) * swapchain_system->vk_image_count);
    if (!images) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_SwapchainSystemCreate");
        return NULL;
    }
    swapchain_system->vk_images = images;
    devfs->vkGetSwapchainImagesKHR(dev, swapchain, &swapchain_system->vk_image_count, images);

    // debug output
    if (debugger) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physdev, &props);

        TL_Log(debugger, "Created Vulkan swapchain object at %p in Thallium Vulkan swapchain system %p", swapchain,
            swapchain_system);

        TL_Log(debugger, "  For use by physical device \"%s\"", props.deviceName);
        TL_Log(debugger, "  With extent resolution %dx%d", extent.width, extent.height);
        TL_Log(debugger, "  Image count %d (from minimum of %d)", swapchain_system->vk_image_count, min_img_count);
    }

    return swapchain_system;
}

void TLVK_SwapchainSystemDestroy(TLVK_SwapchainSystem_t *const swapchain_system) {
    if (!swapchain_system) {
        return;
    }

    const TLVK_RendererSystem_t *renderersys = swapchain_system->renderer_system;
    const TLVK_FuncSet_t *devfs = &(renderersys->devfs);

    devfs->vkDestroySwapchainKHR(renderersys->vk_logical_device, swapchain_system->vk_swapchain, NULL);
    vkDestroySurfaceKHR(swapchain_system->vk_instance, swapchain_system->vk_surface, NULL);

    free(swapchain_system->vk_images);

    free(swapchain_system);
}
