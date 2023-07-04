find_package("Doxygen" REQUIRED)
find_package("Sphinx" REQUIRED)


# doxygen: XML documentation

configure_file("${CMAKE_CURRENT_LIST_DIR}/Doxyfile.conf" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.conf" @ONLY)

add_custom_target(${PROJECT_NAME}xmldoc
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.conf"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating Thallium XML documentation backend with Doxygen"
    VERBATIM)


# sphinx: HTML frontend

add_custom_target(${PROJECT_NAME}htmldoc
    ALL
    COMMAND ${SPHINX_EXECUTABLE} "-q" "-b" "html" ${CMAKE_CURRENT_LIST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/output/html"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Building Thallium HTML documentation with Sphinx"
    VERBATIM)
