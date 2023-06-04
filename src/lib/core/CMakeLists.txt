set(SOURCES
    "context.c"
    "debugger.c"
    "renderer.c"
)

# add path prefix to each filename in sources
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES} ${SOURCES} PARENT_SCOPE)
