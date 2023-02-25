/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "vk_instance.h"

#include "thallium/core/debugger.h"
#include "thallium/core/renderer_config.h"

#include "thallium/vulkan/vk_extension.h"

#include "assert.h"
#include "types.h"
#include "utils/log.h"
#include "utils/primitive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FMT_VK_VERSION(vers) \
    VK_MAKE_VERSION( \
        vers.major, \
        vers.minor, \
        vers.patch \
    )

#define MAX_INSTANCE_LAYER_NAME_COUNT 128
#define MAX_INSTANCE_EXTENSION_NAME_COUNT 256

// Callback for instance debug messengers - redirects messages to Thallium debugger
// userData is a pointer to the Thallium debugger struct
static VKAPI_ATTR VkBool32 VKAPI_CALL _InstanceDebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    // convert the type to a string
    char type_str[32];
    switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        default:
            snprintf(type_str, 32, "GENERAL");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            snprintf(type_str, 32, "VALIDATION");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            snprintf(type_str, 32, "PERFORMANCE");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            snprintf(type_str, 32, "DEVICE_ADDRESS_BINDING");
            break;
    }

    // redirect to appropriate logging function based on severity
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            th_Warn_Vk(user_data, "%s: %s", type_str, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            th_Error_Vk(user_data, "%s: %s", type_str, callback_data->pMessage);
            break;
        default:
            th_Log_Vk(user_data, "%s: %s", type_str, callback_data->pMessage);
            break;
    }

    return VK_FALSE;
}

// Convert debug message severity flags from Thallium format to Vulkan format.
static VkDebugUtilsMessageSeverityFlagsEXT _ThalliumDebugSeveritiesToVulkanFlags(th_DebugSeverity_t severities) {
    // NOTIF severity is not considerered as the only appropriate Vulkan severities (VERBOSE and INFO) are
    // output via th_Log_*(), which would not show if the VERBOSE severity is not enabled anyway.
    // i.e., even when considering NOTIF, nothing different is output, so there's no point.
    return
          (severities & THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT) >> 4   // VERBOSE  --> VERBOSE
        | (severities & THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT)        //              + INFO
        | (severities & THALLIUM_DEBUG_SEVERITY_WARNING_BIT) << 6   // WARNING  --> WARNING
        | (severities & THALLIUM_DEBUG_SEVERITY_ERROR_BIT) << 11    // ERROR    --> ERROR
        | (severities & THALLIUM_DEBUG_SEVERITY_FATAL_BIT) << 12;   // FATAL    --> ERROR
}

// Create a debug messenger (return its descriptor)
static VkDebugUtilsMessengerCreateInfoEXT _CreateDebugMessenger(const th_Debugger_t *debugger) {
    return (VkDebugUtilsMessengerCreateInfoEXT) {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0, // NOTE: "reserved for future use"
        .messageSeverity = _ThalliumDebugSeveritiesToVulkanFlags(debugger->debug_severity_filter),
        .messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
        .pfnUserCallback = _InstanceDebugMessengerCallback,
        .pUserData = (void *) debugger
    };
}

int thvk_CreateInstance(thvk_RenderSystem_t *render_system) {
    VkApplicationInfo app_info;
    VkInstanceCreateInfo instance_info;
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info;

    const th_Debugger_t *debugger = render_system->debugger;

    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.flags = 0;

    // populate application info based on config if it was given
    if (render_system->config_specified) {
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = NULL;
        app_info.pApplicationName = render_system->config.application_name;
        app_info.applicationVersion = FMT_VK_VERSION(render_system->config.application_version);
        app_info.pEngineName = render_system->config.engine_name;
        app_info.engineVersion = FMT_VK_VERSION(render_system->config.engine_version);
        app_info.apiVersion = render_system->api_version;

        instance_info.pApplicationInfo = &app_info;
    } else {
        instance_info.pApplicationInfo = NULL;
    }

    // if a debugger was passed, then we can use it to describe a debug messenger.
    // this will require the VK_EXT_debug_utils to be enabled; this is handled as part of the
    // required layers and instance extensions checking.
    if (debugger) {
        debug_messenger_info = _CreateDebugMessenger(debugger);
        instance_info.pNext = &debug_messenger_info;
    } else {
        instance_info.pNext = NULL;
    }

    unsigned int layer_count = 0;
    unsigned int extension_count = 0;
    const char *layer_names[MAX_INSTANCE_LAYER_NAME_COUNT];
    const char *extension_names[MAX_INSTANCE_EXTENSION_NAME_COUNT];


    // handling layers...

    unsigned int available_layer_count, required_layer_count;

    // get available layers
    TH_ASSERT_VK(vkEnumerateInstanceLayerProperties(&available_layer_count, NULL));
    VkLayerProperties available_layer_properties[available_layer_count];
    TH_ASSERT_VK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layer_properties));

    // + convert layer properties to layer names (strings)
    char *available_layer_names[available_layer_count];
    for (unsigned int i = 0; i < available_layer_count; i++) {
        available_layer_names[i] = available_layer_properties[i].layerName;
    }

    // get required layers
    thvk_GetRequiredLayers(render_system, &required_layer_count, NULL);
    const char *required_layer_names[required_layer_count];
    thvk_GetRequiredLayers(render_system, &required_layer_count, required_layer_names);

    // filter supported layers against required ones
    for (unsigned int i = 0; i < required_layer_count; i++) {
        // if required layer is available
        if (th_StringValueInArray(required_layer_names[i], (const char *const *) available_layer_names, available_layer_count)) {
            if (layer_count++ > MAX_INSTANCE_LAYER_NAME_COUNT) {
                th_Error(debugger, "Count of enabled layer names for VkInstance exceeds max (%d)", MAX_INSTANCE_LAYER_NAME_COUNT);
                break;
            }

            layer_names[layer_count - 1] = required_layer_names[i];
        } else {
            th_Warn(debugger, "Could not find required or specified layer \"%s\"", required_layer_names[i]);
        }
    }


    // handling extensions...

    unsigned int required_extension_count, available_extension_count;

    // get available extensions
    TH_ASSERT_VK(vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, NULL));
    VkExtensionProperties available_extension_properties[available_extension_count];
    TH_ASSERT_VK(vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, available_extension_properties));

    // + convert extension properties to extension names (strings)
    char *available_extension_names[available_extension_count];
    for (unsigned int i = 0; i < available_extension_count; i++) {
        available_extension_names[i] = available_extension_properties[i].extensionName;
    }

    // get required extensions
    thvk_GetRequiredInstanceExtensions(render_system, debugger != NULL, &required_extension_count, NULL);
    const char *required_extension_names[required_extension_count];
    thvk_GetRequiredInstanceExtensions(render_system, debugger != NULL, &required_extension_count, required_extension_names);

    // filter supported extensions against required ones
    for (unsigned int i = 0; i < required_extension_count; i++) {
        // if required extension is available
        if (th_StringValueInArray(required_extension_names[i], (const char *const *) available_extension_names, available_extension_count)) {
            if (extension_count++ > MAX_INSTANCE_EXTENSION_NAME_COUNT) {
                th_Error(debugger, "Count of enabled Vulkan extension names exceeds max (%d)", MAX_INSTANCE_EXTENSION_NAME_COUNT);
                break;
            }

            extension_names[extension_count - 1] = required_extension_names[i];
        } else {
            th_Note(debugger, "Specified extension \"%s\" might not be valid, or it could be a device extension.", required_extension_names[i]);
        }
    }

    // apply layers and extensions
    instance_info.enabledLayerCount = layer_count;
    instance_info.enabledExtensionCount = extension_count;
    instance_info.ppEnabledLayerNames = layer_names;
    instance_info.ppEnabledExtensionNames = extension_names;

    // then we create the instance
    TH_ASSERT_VK(vkCreateInstance(&instance_info, NULL, &(render_system->instance)));

    th_Note(debugger, "Created Vulkan instance at %p", &(render_system->instance));

#   ifdef THALLIUM_DEBUG_LAYER
        char debug_str[1024];
        snprintf(debug_str, 1024, "==== INSTANCE INFO ====\n     Enabled layers (%d):\t\t\tEnabled extensions (%d):\n", layer_count, extension_count);

        unsigned int max_count = (layer_count > extension_count) ? layer_count : extension_count;
        for (unsigned int i = 0; i < max_count; i++) {
            char current_line[256];
            snprintf(current_line, 256, "     %s%s%s",
                (i < layer_count) ? layer_names[i] : "",
                (i < layer_count) ? "\t\t" : "\t\t\t\t\t\t",
                (i < extension_count) ? extension_names[i] : "");

            if (i < max_count - 1) {
                strncat(current_line, "\n", 256);
            }

            strncat(debug_str, current_line, 1024);
        }

        th_Log(debugger, debug_str);
#   endif

    return 1;
}
