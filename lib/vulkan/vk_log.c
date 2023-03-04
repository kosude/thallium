/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "vk_log.h"

#include "thallium/core/version.h"
#include "utils/log.h"

#include <volk/volk.h>
#include <string.h>
#include <stdio.h>

#define PRINT_ENABLED_PHYSICAL_DEVICE_FEATURE(feature) \
{ \
    if (features.feature) { \
        snprintf(current_feature_name, 63, "\t\t- %s", #feature); \
        strncat(debug_str, current_feature_name, 1023); \
    } \
}

void thvk_PrintVkInstanceInfo(const th_Debugger_t *dbg, const unsigned int layer_count, const char **layer_names, const unsigned int extension_count,
    const char **extension_names)
{
#   ifndef THALLIUM_DEBUG_LAYER
        return;
#   endif

    unsigned int instance_version_bit = VK_API_VERSION_1_0;

    // vkEnumerateInstanceVersion provided by Vulkan 1.1
    if (vkEnumerateInstanceVersion) {
        vkEnumerateInstanceVersion(&instance_version_bit);
    }

    th_Version_t instance_version_struct = {
        VK_VERSION_MAJOR(instance_version_bit),
        VK_VERSION_MINOR(instance_version_bit),
        VK_VERSION_PATCH(instance_version_bit),
    };

    char debug_str[1024];
    snprintf(debug_str, 1023, "===== VULKAN INSTANCE INFO =====\n"
        "     Supported instance version: %d.%d.%d\n"
        "     Enabled layers (%d):\t\t\tEnabled extensions (%d):\n",
        instance_version_struct.major, instance_version_struct.minor, instance_version_struct.patch, layer_count, extension_count);

    unsigned int max_count = (layer_count > extension_count) ? layer_count : extension_count;
    for (unsigned int i = 0; i < max_count; i++) {
        char current_line[256];
        snprintf(current_line, 255, "     %s%s%s%s%s",
            (i < layer_count) ? "- " : "",
            (i < layer_count) ? layer_names[i] : "",
            (i < layer_count) ? "\t\t" : "\t\t\t\t\t\t",
            (i < extension_count) ? "- " : "",
            (i < extension_count) ? extension_names[i] : "");

        if (i < max_count - 1) {
            strncat(current_line, "\n", 255);
        }

        strncat(debug_str, current_line, 1023);
    }

    th_Log(dbg, debug_str);
}

void thvk_PrintVkDeviceInfo(const th_Debugger_t *dbg, const char *physical_device_name, const unsigned int extension_count,
    const char **extension_names)
{
#   ifndef THALLIUM_DEBUG_LAYER
        return;
#   endif

    char debug_str[1024];
    snprintf(debug_str, 1023, "===== VULKAN LOGICAL DEVICE INFO =====\n"
        "     Using device: \"%s\"\n"
        "     Enabled extensions (%d):\n",
        physical_device_name, extension_count);

    for (unsigned int i = 0; i < extension_count; i++) {
        char current_line[256];
        snprintf(current_line, 255, "     - %s",
            extension_names[i]);

        if (i < extension_count - 1) {
            strncat(current_line, "\n", 255);
        }

        strncat(debug_str, current_line, 1023);
    }

    th_Log(dbg, debug_str);
}
