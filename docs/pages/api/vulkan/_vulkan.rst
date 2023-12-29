Vulkan Module
=============

Objects and functions defined within the Thallium Vulkan module (compiled using ``-DTHALLIUM_BUILD_MODULE_VULKAN=ON``).

.. note::
    These Vulkan-specific abstractions are **not** exposed by default. You will need to #include the additional ``thallium_vulkan.h`` file in order to
    access them.

.. toctree::
    :caption: Contents
    :maxdepth: 1

    vk_pipeline_system
    vk_renderer_system
    vk_swapchain_system
