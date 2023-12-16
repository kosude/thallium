Home
====


.. toctree::
    :hidden:

    self

    pages/getting_started

    pages/api/_api


The Thallium Project is a library for C/C++ applications that will provide the ability to write cross-renderer applications with the same interface.
Cross-platform APIs (i.e. Vulkan and OpenGL) will be prioritised, but I plan to also implement dumber APIs such as Metal, DX11, and DX12 in the
far future.

.. note::
    The contents of this page can also be seen in the project's `README <https://github.com/kosude/thallium/blob/master/README.md/>`_.

For comprehensive function and struct documentation, see the :doc:`API reference <pages/api/_api>`.


Building
--------

CMake is used to generate build files. Ideally, you should build in a Linux environment - do note however that MinGW32 and subsystems like Cygwin
and MSYS2 have not yet been tested.


General options
^^^^^^^^^^^^^^^

The following is a table of available CMake configuration options for Thallium. Note that this list may be missing some, so check the appropriate
``CMakeLists.txt`` file to make sure.

===========================  ================================  =======
Option name                  Description                       Default
===========================  ================================  =======
``BUILD_SHARED_LIBS``        Build shared libraries            ON
``THALLIUM_BUILD_LIB``       Build the Thallium library        ON
``THALLIUM_BUILD_TESTS``     Build Thallium tests              OFF
``THALLIUM_BUILD_EXAMPLES``  Build Thallium example projects   OFF
``THALLIUM_BUILD_DOCS``      Build HTML documentation          OFF
===========================  ================================  =======

It's recommended to use the ``-DCMAKE_BUILD_TYPE=Debug`` flag when building Thallium for library development.


API modules
^^^^^^^^^^^

Thallium source compilation is split into **modules**, based on the graphics APIs you need support for. Each module can be manually enabled or
disabled with CMake flags, just like above. These flags are named ``THALLIUM_BUILD_MODULE_<API>``. For example, the Vulkan module can be compiled
using the ``-DTHALLIUM_BUILD_MODULE_VULKAN=ON`` build flag (this requires the Vulkan SDK to be installed).


WSI flags
^^^^^^^^^

Window system integration (WSI) is available depending on the target platform:

 - On Apple systems, ``THALLIUM_WSI_COCOA`` is available and enabled by default.
 - On other \*nix systems, ``THALLIUM_WSI_XCB`` and ``THALLIUM_WSI_XLIB`` are both available and enabled by default. *Wayland is not yet supported.*


Information for contributing
----------------------------


Document **all** new public functions or types with `Doxygen <https://www.doxygen.nl/>`_-style formatting. Also see the
`Breathe documentation <https://breathe.readthedocs.io/en/latest/index.html>`_ to find out how to include your code's documentation in the HTML
output.

I keep an upstream TO-DO list on `Trello <https://trello.com/b/ZHYGTiZr/thallium-development-tracking>`_. Submit issues and pull requests to the
`GitHub repo <https://github.com/kosude/thallium>`_.
