set(SOURCES_CORE
    "utils/io_colour.c"     "utils/io_colour.h"
    "utils/log.c"           "utils/log.h"

    "test.c"
)


# Core library

add_library(${PROJECT_NAME}_core ${LIB_BUILD_TYPE} ${SOURCES_CORE})
set(THALLIUM_LIBRARIES ${THALLIUM_LIBRARIES} ${PROJECT_NAME}_core)

target_include_directories(
${PROJECT_NAME}_core
PUBLIC
    "${THALLIUM_INCLUDE_DIR}"
PRIVATE
    "${THALLIUM_LIB_DIR}" # this directory for internal headers
    "${PROJECT_BINARY_DIR}/generated" # for generated resources and headers
)


# Interface library

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${THALLIUM_LIBRARIES})
