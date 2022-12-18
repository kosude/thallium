find_package("Doxygen" REQUIRED)

set(DOXYGEN_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(DOXYGEN_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")


# Public API

set(PUBLIC_DOCS_SRC "${DOXYGEN_SRC_DIR}/public")

configure_file("${PUBLIC_DOCS_SRC}/Doxyfile.pub.conf" "${DOXYGEN_OUT_DIR}/Doxyfile.pub.conf.out" @ONLY)

add_custom_target(${PROJECT_NAME}docs
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} "${DOXYGEN_OUT_DIR}/Doxyfile.pub.conf.out"
    WORKING_DIRECTORY "${DOXYGEN_OUT_DIR}"
    COMMENT "Building Thallium API documentation: public API"
    VERBATIM
)


# Internal API: Vulkan

set(INTERNAL_VULKAN_DOCS_SRC "${DOXYGEN_SRC_DIR}/internal/vulkan")

configure_file("${INTERNAL_VULKAN_DOCS_SRC}/Doxyfile.vk.conf" "${DOXYGEN_OUT_DIR}/Doxyfile.vk.conf.out" @ONLY)

add_custom_target(${PROJECT_NAME}docs_vk
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} "${DOXYGEN_OUT_DIR}/Doxyfile.vk.conf.out"
    WORKING_DIRECTORY "${DOXYGEN_OUT_DIR}"
    COMMENT "Building Thallium API documentation: internal - Vulkan API"
    VERBATIM
)
