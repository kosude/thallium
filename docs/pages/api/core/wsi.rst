WSI and Window Surfaces
=======================

This section describes the Thallium WSI system, including the window surface structure.


*****


Types
-----


Objects
^^^^^^^

.. doxygentypedef:: TL_WindowSurface_t


*****


Functions
---------


For X11 systems - via XCB
^^^^^^^^^^^^^^^^^^^^^^^^^

.. important::
    Definitions for these functions is compiled **only if** the ``-DTHALLIUM_WSI_XCB=ON`` CMake flag was present at build. This is the
    **default behaviour** on **Unix** *(not Apple)* systems.

.. doxygenfunction:: TL_WindowSurfaceCreateXCB


For X11 systems - via Xlib
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. important::
    Definitions for these functions is compiled **only if** the ``-DTHALLIUM_WSI_XLIB=ON`` CMake flag was present at build. This is the
    **default behaviour** on **Unix** *(not Apple)* systems.

.. doxygenfunction:: TL_WindowSurfaceCreateXlib