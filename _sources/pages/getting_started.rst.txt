Getting started
===============


Understanding Thallium contexts and renderers
---------------------------------------------

Each Thallium application must have **one** context object. This object stores state data for APIs that need it, such as the Vulkan instance, and is
created using the :c:func:`TL_ContextCreate()` function.

The context data is initialised as necessary when any renderers are created - in other words, if there are no Vulkan renderers created, then no
Vulkan instance will be created, for example. One or more renderers can be created in batch with the :c:func:`TL_RendererCreate()` function - this can
only be done **once** per context!


Debugging functions
-------------------

While the context is usually the first object to be created in production builds, you might opt for creating a debugger first instead. This is
necessary to recieve any debug output, and can be done with the :c:func:`TL_DebuggerCreate()` function. You'll need to specify the severities and
sources of messages you want to stream to the debugger - this configuration is available in the descriptor, as well as an optional callback function.

Some functions might explicitly take an optional debugger argument to send output to. However, it's recommended to use a **debugger attachment** to
get much better output from the context. This is a virtual structure that is defined with the :c:struct:`TL_DebuggerAttachmentDescriptor_t`
descriptor, and is specified at context creation. With a debugger attachment, you can recieve output directly from the graphics API(s) your
application uses!


Object systems
--------------

API-specific versions of objects are abstracted in the form of **object systems**. For example, a renderer created with the
:c:enum:`TL_RendererAPIFlags_t` flag of ``TL_RENDERER_API_VULKAN_BIT`` will itself contain a Vulkan renderer system; swapchains will contain an
API-specific swapchain system, etc.

Object systems are, by default, not provided in the public API. However, you can still access them by including the appropriate graphics API header,
such as ``thallium_vulkan.h`` for Vulkan applications. These headers provide, among other things, their respective object systems.


Interacting with windows
------------------------

Windows are passed to Thallium through the :c:struct:`TL_WindowSurface_t` abstraction. This allows for compatibility with various window systems; for
example, on Apple systems, this struct can be created with the :c:func:`TL_WindowSurfaceCreateCocoa()` function. X11 and XCB equivalents exist for
Linux systems - the functions that are compiled depend on which CMake flags are enabled at config-time (e.g. ``THALLIUM_WSI_COCOA`` is ON by default
when building on macOS).
