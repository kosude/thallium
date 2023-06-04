# build cutils
set(CUTILS_CLIST OFF CACHE BOOL "Build clist library" FORCE)

add_subdirectory("${THALLIUM_DEPS_DIR}/cutils" "${CMAKE_BINARY_DIR}/deps/cutils")

set(THALLIUM_LIBRARY_LINKS ${THALLIUM_LIBRARY_LINKS}
    carray
    cio

    PARENT_SCOPE
)

set(SOURCES
    "context.c"
    "debugger.c"
)

# add path prefix to each filename in sources
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES} ${SOURCES} PARENT_SCOPE)
