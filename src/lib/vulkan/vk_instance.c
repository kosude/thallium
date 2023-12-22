/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "vk_instance.h"

#include "thallium/core/renderer.h"

#include "utils/utils.h"

#include <volk/volk.h>

#include <stdlib.h>
#include <string.h>

#define __DEFINE_REQUIRED_FLAG(flag) ret |= flag
#define __DEFINE_REQUIRED_LAYER(name) if (out_layer_names) { out_layer_names[count_ret] = name; } count_ret++
#define __DEFINE_REQUIRED_EXTENSION(name) if (out_extension_names) { out_extension_names[count_ret] = name; } count_ret++

#define __IF_EXTENSION_ENABLED(ext, fn)                     \
for (uint32_t i = 0; i < extensions.size; i++) {            \
    if (!strcmp((const char *) extensions.data[i], ext)) {  \
        fn                                                  \
        break;                                              \
    }                                                       \
}


static carray_t __ValidateInstanceLayers(const uint32_t count, const char *const *const names, bool *const out_missing_flag,
    const TL_Debugger_t *const debugger);

static carray_t __ValidateInstanceExtensions(const uint32_t count, const char *const *const names, const uint32_t lcount,
    const char *const *const lnames, bool *const out_missing_flag, const TL_Debugger_t *const debugger);

static VkInstanceCreateFlags __GetRequiredInstanceFlags(const TL_RendererFeatures_t requirements);

static void __EnumerateRequiredInstanceLayers(const TL_RendererFeatures_t requirements, const bool debug_utils, uint32_t *const out_layer_count,
    const char **out_layer_names);

static void __EnumerateRequiredInstanceExtensions(const TL_RendererFeatures_t requirements, const bool debug_utils,
    uint32_t *const out_extension_count, const char **out_extension_names);

static void __UpdateRendererFeaturesWithSupported(TL_RendererFeatures_t *const features, carray_t extensions, carray_t layers,
    const TL_Debugger_t *const debugger);


VkInstance TLVK_InstanceCreate(const VkApplicationInfo application_info, const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info,
    TL_RendererFeatures_t *const requirements, carray_t *const out_layer_names, carray_t *const out_extension_names,
    const TL_Debugger_t *const debugger)
{
    if (!requirements) {
        return VK_NULL_HANDLE;
    }

    VkInstanceCreateInfo instance_create_info;
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.pNext = NULL;

    instance_create_info.flags = __GetRequiredInstanceFlags(*requirements);

    bool debug_utils = (debug_messenger_info.sType != 0);

    // validate required layers...
    uint32_t required_layer_count = 0;
    __EnumerateRequiredInstanceLayers(*requirements, debug_utils, &required_layer_count, NULL);
    const char *required_layers[required_layer_count];
    __EnumerateRequiredInstanceLayers(*requirements, debug_utils, &required_layer_count, required_layers);

    TL_Log(debugger, "Validating Vulkan layers...");

    bool is_missing_layers = false;
    carray_t layers = __ValidateInstanceLayers(required_layer_count, required_layers, &is_missing_layers, debugger);
    if (is_missing_layers) {
        TL_Error(debugger, "Missing layers for Vulkan instance, some features may not be available");
    }

    // validate required extensions...
    uint32_t required_extension_count = 0;
    __EnumerateRequiredInstanceExtensions(*requirements, debug_utils, &required_extension_count, NULL);
    const char *required_extensions[required_extension_count];
    __EnumerateRequiredInstanceExtensions(*requirements, debug_utils, &required_extension_count, required_extensions);

    TL_Log(debugger, "Validating Vulkan instance-level extensions...");

    bool is_missing_extensions = false;
    carray_t extensions = __ValidateInstanceExtensions(required_extension_count, required_extensions, layers.size, (const char **) layers.data,
        &is_missing_extensions, debugger);
    if (is_missing_extensions) {
        TL_Error(debugger, "Missing extensions for Vulkan instance, some features may not be available");
    }

    // disable any renderer features if they could not be used
    __UpdateRendererFeaturesWithSupported(requirements, extensions, layers, debugger);

    // initialise the heap-allocated arrays of instance-level extensions and layers stored by the renderer system
    instance_create_info.enabledLayerCount = layers.size;
    instance_create_info.ppEnabledLayerNames = (const char **) layers.data;
    instance_create_info.enabledExtensionCount = extensions.size;
    instance_create_info.ppEnabledExtensionNames = (const char **) extensions.data;

    // validation features (will be populated if the extension was enabled)
    VkValidationFeatureEnableEXT validation_features_enabled[2] = {
        VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
        VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT
    };
    VkValidationFeaturesEXT validation_features = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
        .pNext = NULL,
        .enabledValidationFeatureCount = 2,
        .pEnabledValidationFeatures = validation_features_enabled,
        .disabledValidationFeatureCount = 0,
        .pDisabledValidationFeatures = NULL
    };

    // if validation features are available...
    __IF_EXTENSION_ENABLED(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
        // append the validation features description onto the pNext chain
        TLVK_AppendPNext(&instance_create_info.pNext, &validation_features);
    );

    // if debug utils are available...
    __IF_EXTENSION_ENABLED(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        // append debug messenger info to the pNext chain
        TLVK_AppendPNext(&instance_create_info.pNext, &debug_messenger_info);
    );

    // returning enabled layer names
    if (out_layer_names) {
        for (uint32_t i = 0; i < layers.size; i++) {
            char *curlayer_name = (char *) layers.data[i];

            // allocate layer name in heap
            size_t len = strlen(curlayer_name);
            char *str = malloc(sizeof(char) * (len + 1));
            strcpy(str, curlayer_name);

            // append layer name into renderer system store
            carraypush(out_layer_names, (carrayval_t) str);
        }
    }

    // returning enabled extension names
    if (out_extension_names) {
        for (uint32_t i = 0; i < extensions.size; i++) {
            char *curext_name = (char *) extensions.data[i];

            // allocate extension name in heap
            size_t len = strlen(curext_name);
            char *str = malloc(sizeof(char) * (len + 1));
            strcpy(str, curext_name);

            // append extension name into renderer system store
            carraypush(out_extension_names, (carrayval_t) str);
        }
    }

    // create instance
    VkInstance instance;
    if (vkCreateInstance(&instance_create_info, NULL, &instance)) {
        return VK_NULL_HANDLE;
    }

    carrayfree(&layers);
    carrayfree(&extensions);

    return instance;
}


// validate given layers and return an array of the layers that are deemed valid (which can be enabled)
static carray_t __ValidateInstanceLayers(const uint32_t count, const char *const *const names, bool *const out_missing_flag,
    const TL_Debugger_t *const debugger)
{
    // get available layers
    uint32_t available_count;
    vkEnumerateInstanceLayerProperties(&available_count, NULL);
    VkLayerProperties available[available_count];
    vkEnumerateInstanceLayerProperties(&available_count, available);

    carray_t ret = carraynew(count);

    for (uint32_t i = 0; i < count; i++) {
        bool found = false;

        const char *cur_name_req = names[i];

        for (uint32_t j = 0; j < available_count; j++) {
            const char *cur_name_av = available[j].layerName;

            if (!strcmp(cur_name_req, cur_name_av)) {
                // the layer is confirmed available
                found = true;
                carraypush(&ret, (carrayval_t) cur_name_req);

                break;
            }
        }

        // layer not found
        if (!found) {
            TL_Warn(debugger, "Could not find layer \"%s\"", cur_name_req);
            *out_missing_flag = true;
        }
    }

    return ret;
}

// validate given extensions and return an array of the extensions that are deemed valid (which can be enabled)
static carray_t __ValidateInstanceExtensions(const uint32_t count, const char *const *const names, const uint32_t lcount,
    const char *const *const lnames, bool *const out_missing_flag, const TL_Debugger_t *const debugger)
{
    // get available extensions
    uint32_t available_count;
    vkEnumerateInstanceExtensionProperties(NULL, &available_count, NULL);
    VkExtensionProperties available[available_count];
    vkEnumerateInstanceExtensionProperties(NULL, &available_count, available);

    carray_t ret = carraynew(count);

    for (uint32_t i = 0; i < count; i++) {
        bool found = false;

        const char *cur_name_req = names[i];

        for (uint32_t j = 0; j < available_count; j++) {
            const char *cur_name_av = available[j].extensionName;

            if (!strcmp(cur_name_req, cur_name_av)) {
                // the extension is confirmed available
                found = true;
                carraypush(&ret, (carrayval_t) cur_name_req);

                break;
            }
        }

        // if found at this point then the extension is provided by the implementation and we continue
        if (found) {
            continue;
        }

        for (uint32_t j = 0, br = 0; j < lcount && br == 0; j++) {
            const char *lname = lnames[j];

            // get available extensions for that layer
            uint32_t l_available_count;
            vkEnumerateInstanceExtensionProperties(lname, &l_available_count, NULL);
            VkExtensionProperties l_available[l_available_count];
            vkEnumerateInstanceExtensionProperties(lname, &l_available_count, l_available);

            for (uint32_t k = 0; k < l_available_count; k++) {
                if (!strcmp(cur_name_req, l_available[k].extensionName)) {
                    // the extension is confirmed available
                    found = true;
                    carraypush(&ret, (carrayval_t) cur_name_req);

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

// Get the instance create flags required to support the given features
static VkInstanceCreateFlags __GetRequiredInstanceFlags(const TL_RendererFeatures_t requirements) {
    VkInstanceCreateFlags ret = 0;

#   if defined(_APPLE)
        // as MoltenVK is non-compliant we need to enable its device enumeration with the portability flag
        __DEFINE_REQUIRED_FLAG(VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
#   endif

    return ret;
}

// Get the instance layers required to support the given features
static void __EnumerateRequiredInstanceLayers(const TL_RendererFeatures_t requirements, const bool debug_utils, uint32_t *const out_layer_count,
    const char **out_layer_names)
{
    if (!out_layer_count) {
        return;
    }

    uint32_t count_ret = 0;

    // we automatically enable validation layers if debug utils are enabled
    if (debug_utils) {
        __DEFINE_REQUIRED_LAYER("VK_LAYER_KHRONOS_validation");
    }

    *out_layer_count = count_ret;
}

// Get the instance extensions required to support the given features
static void __EnumerateRequiredInstanceExtensions(const TL_RendererFeatures_t requirements, const bool debug_utils,
    uint32_t *const out_extension_count, const char **out_extension_names)
{
    if (!out_extension_count) {
        return;
    }

    uint32_t count_ret = 0;

#   if defined(_APPLE)
        // __DEFINE_REQUIRED_EXTENSION(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); deprecated and added to Vulkan core 1.1
        __DEFINE_REQUIRED_EXTENSION(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#   endif

    // NOTE: some implementations may not support debug utils, but I won't implement support for debug report as a backup because it's deprecated.
    if (debug_utils) {
        __DEFINE_REQUIRED_EXTENSION(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        __DEFINE_REQUIRED_EXTENSION(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
    }

    // renderers that can present images
    if (requirements.presentation) {
        // implementation-independent surface extension
        __DEFINE_REQUIRED_EXTENSION(VK_KHR_SURFACE_EXTENSION_NAME);

#       if defined(_WIN32)
            // windows surface extension
            __DEFINE_REQUIRED_EXTENSION(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#       elif defined(_APPLE)
            // metal surface extension
            __DEFINE_REQUIRED_EXTENSION(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
#       elif defined(_UNIX)
            // unix surface extensions (assume linux)
            __DEFINE_REQUIRED_EXTENSION(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            __DEFINE_REQUIRED_EXTENSION(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#       endif

    }

    *out_extension_count = count_ret;
}

static void __UpdateRendererFeaturesWithSupported(TL_RendererFeatures_t *const features, carray_t extensions, carray_t layers,
    const TL_Debugger_t *const debugger)
{
    // presentation feature availability
    {
        bool pa = false, pb = false;
#       if defined(_UNIX)
            bool pc = false;
#       endif

        // look for surface extension
        for (uint32_t i = 0; i < extensions.size; i++) {
            const char *curext_name = (const char *) extensions.data[i];

            if (!strcmp((const char *) curext_name, VK_KHR_SURFACE_EXTENSION_NAME)) {
                pa = true;
                continue;
            }

#           if defined(_WIN32)
                if (!strcmp((const char *) curext_name, VK_KHR_WIN32_SURFACE_EXTENSION_NAME)) {
                    pb = true;
                    break;
                }
#           elif defined(_APPLE)
                if (!strcmp((const char *) curext_name, VK_EXT_METAL_SURFACE_EXTENSION_NAME)) {
                    pb = true;
                    break;
                }
#           elif defined(_UNIX)
                if (!strcmp((const char *) curext_name, VK_KHR_XCB_SURFACE_EXTENSION_NAME)) {
                    pb = true;
                    continue;
                }
                if (!strcmp((const char *) curext_name, VK_KHR_XLIB_SURFACE_EXTENSION_NAME)) {
                    pc = true;
                    continue;
                }
#           endif
        }

        // if any of the listed extensions above were not found then disable presentation
        if (!pa || !pb
#           if defined(_UNIX)
                || !pc
#           endif
        ) {
            features->presentation = false;
            TL_Error(debugger,
                "When creating Vulkan instance: RENDERER FEATURE UNAVAILABLE (missing instance extensions) - 'presentation' was disabled!");
        }
    }
}
