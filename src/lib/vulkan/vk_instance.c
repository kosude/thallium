/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_instance.h"

#include "thallium.h"

#include "utils/utils.h"

#include <stdlib.h>
#include <string.h>

#include <volk/volk.h>

// TODO ..:: Room for (probable) optimisation ::..
// Currently the __ValidateInstanceLayers and __ValidateInstanceExtensions use linear searching algorithms to
// look for layer and extension names. This is not very efficient, so maybe something like some sort of binary
// search would be better after alphabetically sorting the available layers/extensions. This doesn't need too
// much priority as it's not like it's being done every frame, but it should probably be looked into at some
// point.

// validate given layers and return an array of the layers that are deemed valid (which can be enabled)
static carray_t __ValidateInstanceLayers(const uint32_t count, const char *const *const names, const TL_Debugger_t *const debugger,
    bool *const out_missing_flag)
{
    // get available layers
    uint32_t available_count;
    vkEnumerateInstanceLayerProperties(&available_count, NULL);
    VkLayerProperties available[available_count];
    vkEnumerateInstanceLayerProperties(&available_count, available);

    carray_t ret = carraynew(count);

    for (uint32_t i = 0; i < count; i++) {
        bool found = false;

        for (uint32_t j = 0; j < available_count; j++) {
            if (!strcmp(names[i], available[j].layerName)) {
                // the layer is confirmed available
                found = true;
                carraypush(&ret, (carrayval_t) names[i]);

                break;
            }
        }

        // layer not found
        if (!found) {
            TL_Warn(debugger, "Could not find layer \"%s\"", names[i]);

            *out_missing_flag = true;
        }
    }

    return ret;
}

// validate given extensions and return an array of the extensions that are deemed valid (which can be enabled)
static carray_t __ValidateInstanceExtensions(const uint32_t count, const char *const *const names, const uint32_t lcount,
    const char *const *const lnames, const TL_Debugger_t *const debugger, bool *const out_missing_flag)
{
    // get available extensions
    uint32_t available_count;
    vkEnumerateInstanceExtensionProperties(NULL, &available_count, NULL);
    VkExtensionProperties available[available_count];
    vkEnumerateInstanceExtensionProperties(NULL, &available_count, available);

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

        // if found at this point then the extension is provided by the implementation and we continue
        if (found) {
            continue;
        }

        for (uint32_t j = 0, br = 0; j < lcount && br == 0; j++) {
            // get available extensions for that layer
            uint32_t l_available_count;
            vkEnumerateInstanceExtensionProperties(lnames[j], &l_available_count, NULL);
            VkExtensionProperties l_available[l_available_count];
            vkEnumerateInstanceExtensionProperties(lnames[j], &l_available_count, l_available);

            for (uint32_t k = 0; k < l_available_count; k++) {
                if (!strcmp(names[i], l_available[k].extensionName)) {
                    // the extension is confirmed available
                    found = true;
                    carraypush(&ret, (carrayval_t) names[i]);

                    // br variable to break nest parent for-loop
                    br = 1;
                    break;
                }
            }
        }

        if (!found) {
            // extension not found
            TL_Warn(debugger, "Could not find extension \"%s\"", names[i]);
            *out_missing_flag = true;
        }
    }

    return ret;
}

#define __DEFINE_REQUIRED_FLAG(flag) ret |= flag;
#define __DEFINE_REQUIRED_LAYER(name) if (out_layer_names) { out_layer_names[count_ret] = name; } count_ret++;
#define __DEFINE_REQUIRED_EXTENSION(name) if (out_extension_names) { out_extension_names[count_ret] = name; } count_ret++;

// Get the instance create flags required to support the given features
static VkInstanceCreateFlags __GetRequiredInstanceFlags(const TL_RendererFeatures_t requirements) {
    VkInstanceCreateFlags ret = 0;

#   if defined(APPLE)
        // as MoltenVK is non-compliant we need to enable its device enumeration with the portability flag
        __DEFINE_REQUIRED_FLAG(VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
#   endif

    return ret;
}

// Get the instance layers required to support the given features
static void __EnumerateRequiredInstanceLayers(const TL_RendererFeatures_t requirements, const TL_Debugger_t *const debugger,
    uint32_t *const out_layer_count, const char **out_layer_names)
{
    if (!out_layer_count) {
        return;
    }

    uint32_t count_ret = 0;

    // TODO: define validation layers under some condition to do with debugger and debug attachment

    *out_layer_count = count_ret;
}

// Get the instance extensions required to support the given features
static void __EnumerateRequiredInstanceExtensions(const TL_RendererFeatures_t requirements, const TL_Debugger_t *const debugger,
    uint32_t *const out_extension_count, const char **out_extension_names)
{
    if (!out_extension_count) {
        return;
    }

    uint32_t count_ret = 0;

#   if defined(APPLE)
        // __DEFINE_REQUIRED_EXTENSION(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); deprecated and added to Vulkan core 1.1
        __DEFINE_REQUIRED_EXTENSION(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#   endif

    // TODO: define debug utils and validation features under some condition to do with debugger and debug attachment. Debug utils are present on
    // ~47% of devices. On those where it is not present, debug report (on ~89% devices) should be attempted to be used instead.

    *out_extension_count = count_ret;
}

#define __IF_EXTENSION_ENABLED(ext, fn)                     \
for (uint32_t i = 0; i < extensions.size; i++) {            \
    if (!strcmp((const char *) extensions.data[i], ext)) {  \
        fn                                                  \
        break;                                              \
    }                                                       \
}

VkInstance TLVK_CreateInstance(const VkApplicationInfo application_info, const TL_RendererFeatures_t requirements, carray_t *const out_layer_names,
    carray_t *const out_extension_names, const TL_Debugger_t *const debugger)
{
    VkInstanceCreateInfo instance_create_info;
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.pNext = NULL;

    instance_create_info.flags = __GetRequiredInstanceFlags(requirements);

    // validate required layers...
    uint32_t required_layer_count = 0;
    __EnumerateRequiredInstanceLayers(requirements, debugger, &required_layer_count, NULL);
    const char *required_layers[required_layer_count];
    __EnumerateRequiredInstanceLayers(requirements, debugger, &required_layer_count, required_layers);

    bool is_missing_layers = false;
    carray_t layers = __ValidateInstanceLayers(required_layer_count, required_layers, debugger, &is_missing_layers);
    if (is_missing_layers) {
        TL_Warn(debugger, "Missing layers for Vulkan instance, some features may not be available");
    }

    // validate required extensions...
    uint32_t required_extension_count = 0;
    __EnumerateRequiredInstanceExtensions(requirements, debugger, &required_extension_count, NULL);
    const char *required_extensions[required_extension_count];
    __EnumerateRequiredInstanceExtensions(requirements, debugger, &required_extension_count, required_extensions);

    bool is_missing_extensions = false;
    carray_t extensions = __ValidateInstanceExtensions(required_extension_count, required_extensions, layers.size, (const char **) layers.data,
        debugger, &is_missing_extensions);
    if (is_missing_extensions) {
        TL_Warn(debugger, "Missing extensions for Vulkan instance, some features may not be available");
    }

    // initialise the heap-allocated arrays of instance-level extensions and layers stored by the render system
    instance_create_info.enabledLayerCount = layers.size;
    instance_create_info.ppEnabledLayerNames = (const char **) layers.data;
    instance_create_info.enabledExtensionCount = extensions.size;
    instance_create_info.ppEnabledExtensionNames = (const char **) extensions.data;

    // TODO: define validation feature enableds and validation features struct

    // returning enabled layer names
    if (out_layer_names) {
        for (uint32_t i = 0; i < layers.size; i++) {
            // allocate layer name in heap
            size_t len = strlen((char *) layers.data[i]);
            char *str = malloc(sizeof(char) * len);
            strcpy(str, (char *) layers.data[i]);

            // append layer name into render system store
            carraypush(out_layer_names, (carrayval_t) str);
        }
    }

    // returning enabled extension names
    if (out_extension_names) {
        for (uint32_t i = 0; i < extensions.size; i++) {
            // allocate extension name in heap
            size_t len = strlen((char *) extensions.data[i]);
            char *str = malloc(sizeof(char) * len);
            strcpy(str, (char *) extensions.data[i]);

            // append extension name into render system store
            carraypush(out_extension_names, (carrayval_t) str);
        }
    }

    // TODO: append validation features struct to create info pNext if its extension is enabled
    // TODO: append debug utils messenger create info to create info pNext if its extension is enabled

    // create instance
    VkInstance instance;
    if (vkCreateInstance(&instance_create_info, NULL, &instance)) {
        return VK_NULL_HANDLE;
    }

    // load vulkan functions
    volkLoadInstance(instance);

    carrayfree(&layers);
    carrayfree(&extensions);

    return instance;
}