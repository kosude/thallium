/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

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

    VkSurfaceKHR surface;

    switch (tl_surface->wsi) {
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
                return VK_NULL_HANDLE:
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
                return VK_NULL_HANDLE:
#           endif

        default:
            TL_Error(debugger, "Invalid TL_WindowSurface_t format encountered when attempting to create Vulkan surface");
            return VK_NULL_HANDLE;
    }

    return surface;
}

TLVK_SwapchainSystem_t *TLVK_SwapchainSystemCreate(const TLVK_RendererSystem_t *const renderer_system,
    const TLVK_SwapchainSystemDescriptor_t descriptor, const TL_WindowSurface_t *const window_surface)
{
    if (!renderer_system) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer_system->renderer->debugger;

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

    swapchain_system->renderer_system = renderer_system;

    // get device queues for graphics and presentation from the renderer system
    // TODO: check back here as storing these here might be unnecessary
    swapchain_system->vk_graphics_queue = (VkQueue) renderer_system->vk_queues.graphics.data[0];
    swapchain_system->vk_present_queue = (VkQueue) renderer_system->vk_queues.present.data[0];

    swapchain_system->vk_instance = renderer_system->vk_context->vk_instance;

    VkSurfaceKHR surface;

    if (descriptor.vk_surface) {
        // if caller directly passed an existing Vulkan surface, we use that
        surface = descriptor.vk_surface;
    } else {

        // if no surface was directly passed, we create a new one for the window (passed via Thallium window surface `window_surface`)
        surface = __CreateVkSurface(swapchain_system->vk_instance, window_surface, debugger);
        if (surface == VK_NULL_HANDLE) {
            TL_Error(debugger, "Failed to create Vulkan surface for new swapchain system at %p", swapchain_system);
            return NULL;
        }
    }

    swapchain_system->vk_surface = surface;

    return swapchain_system;
}

void TLVK_SwapchainSystemDestroy(TLVK_SwapchainSystem_t *const swapchain_system) {
    if (!swapchain_system) {
        return;
    }

    vkDestroySurfaceKHR(swapchain_system->vk_instance, swapchain_system->vk_surface, NULL);

    free(swapchain_system);
}
