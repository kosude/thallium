find_package("Doxygen" REQUIRED)
find_package("Sphinx" REQUIRED)


# doxygen: XML documentation

set(DOXYGEN_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(DOXYGEN_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/output")

configure_file("${DOXYGEN_SRC_DIR}/Doxyfile.conf" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.conf" @ONLY)

add_custom_target(${PROJECT_NAME}xmldoc
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.conf"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating Thallium XML documentation backend with Doxygen"
    VERBATIM)


# sphinx: HTML frontend

set(SPHINX_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/sphinx")
set(SPHINX_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/output/html")

add_custom_target(${PROJECT_NAME}htmldoc
    ALL
    COMMAND ${SPHINX_EXECUTABLE} "-q" "-b" "html" ${SPHINX_SRC_DIR} ${SPHINX_OUT_DIR}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Building Thallium HTML documentation with Sphinx"
    VERBATIM)
