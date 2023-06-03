# build cutils
set(CUTILS_CLIST OFF CACHE BOOL "Build clist library" FORCE)

add_subdirectory("${THALLIUM_DEPS_DIR}/cutils" "${CMAKE_BINARY_DIR}/deps/cutils")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES}
    "core/placeholder.c"

    PARENT_SCOPE
)

set(THALLIUM_LIBRARY_LINKS ${THALLIUM_LIBRARY_LINKS}
    carray
    cio

    PARENT_SCOPE
)
