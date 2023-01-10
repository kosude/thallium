cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(thallium)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THALLIUM_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(THALLIUM_DOCS_DIR "${CMAKE_CURRENT_LIST_DIR}/docs")
set(THALLIUM_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(THALLIUM_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
set(THALLIUM_TESTS_DIR "${CMAKE_CURRENT_LIST_DIR}/tests")
# set(THALLIUM_EXAMPLES_DIR "${CMAKE_CURRENT_LIST_DIR}/examples")

set(CMAKE_MODULE_PATH "${THALLIUM_CMAKE_DIR}")

set(THALLIUM_LIBRARIES)

if (MSVC)
    add_compile_options(
        /W4 /WX

        /wd4100 # unused parameters
        /wd4180 # ignored qualifiers on return type
    )
else()
    add_compile_options(
        -Wall -Wextra -pedantic -Werror

        -Wno-unused-parameter
        -Wno-ignored-qualifiers
    )
endif()

macro(ADD_TEST_PROJECT TEST_NAME TEST_FILES LIB_FILES)
    add_executable(${TEST_NAME} ${TEST_FILES})
    target_link_libraries(${TEST_NAME} ${LIB_FILES})
endmacro()

set(SUMMARY_TARGET_PLATFORM "Unknown")
set(SUMMARY_TARGET_ARCH "Unknown")
set(SUMMARY_LIB_TYPE "Unknown")

option(BUILD_SHARED_LIBS "Build shared libraries (or DLLs if on Windows)" ON)
option(THALLIUM_BUILD_LIB "Build the core Thallium library" ON)
option(THALLIUM_BUILD_TESTS "Build Thallium tests" OFF)
# option(THALLIUM_BUILD_EXAMPLES "Build Thallium example projects" OFF)
option(THALLIUM_BUILD_DOCS "Build Thallium HTML documentation" OFF)

if (THALLIUM_BUILD_LIB)
    # apple isn't as intelligent as everyone else so their toy "computers" don't support vulkan
    if (NOT APPLE)
        option(THALLIUM_BUILD_MODULE_VULKAN "Include the Vulkan module in the compilation" OFF)
    endif()
endif()

option(THALLIUM_DEBUG_LAYER "Enable Thallium debugging utilities (applies to both Debug and Release configs)" ON)
if (${THALLIUM_DEBUG_LAYER})
    add_compile_definitions("THALLIUM_DEBUG_LAYER")
endif()

if (BUILD_SHARED_LIBS)
    set(LIB_BUILD_TYPE SHARED)
    set(SUMMARY_LIB_TYPE "Shared/dynamic")
else()
    set(LIB_BUILD_TYPE STATIC)
    set(SUMMARY_LIB_TYPE "Static")
endif()

set(CMAKE_CONFIGURATION_TYPES Debug Release)

if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Missing build configuration type! Reconfigure project with option -DCMAKE_BUILD_TYPE=(Debug|Release)")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" _upperBuildType)
set("BUILD_${_upperBuildType}" 1)
configure_file("${THALLIUM_CMAKE_DIR}/generate/platform.h" "${PROJECT_BINARY_DIR}/generated/cmake_platform.h")

if (THALLIUM_BUILD_LIB)
    add_subdirectory("${THALLIUM_LIB_DIR}/")
endif()

if (THALLIUM_BUILD_TESTS)
    add_subdirectory("${THALLIUM_TESTS_DIR}/")
endif()

if (THALLIUM_BUILD_DOCS)
    add_subdirectory("${THALLIUM_DOCS_DIR}/")
endif()

# if (THALLIUM_BUILD_EXAMPLES)
#     add_subdirectory("${THALLIUM_EXAMPLES_DIR}/")
# endif()

option(THALLIUM_SKIP_VERSION "Skip getting Thallium version dynamically" OFF)

if (NOT THALLIUM_SKIP_VERSION)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    add_custom_target(${PROJECT_NAME}version
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/utils/py/GetVersion.py cfg
        BYPRODUCTS "${PROJECT_BINARY_DIR}/generated/dyn_version.h"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    message("${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/utils/py/GetVersion.py cfg")

    execute_process(
        COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/utils/py/GetVersion.py"
        OUTPUT_VARIABLE THALLIUM_VERSION
    )
else()
    configure_file("${THALLIUM_CMAKE_DIR}/generate/dyn_version_skip.h" "${PROJECT_BINARY_DIR}/generated/dyn_version.h")
endif()

if (APPLE OR CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SUMMARY_TARGET_ARCH "AMD64 (x86_64)")
else()
    set(SUMMARY_TARGET_ARCH "i386 (x86)")
endif()

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SUMMARY_TARGET_PLATFORM "Win64")
    else()
        set(SUMMARY_TARGET_PLATFORM "Win32")
    endif()
elseif (APPLE)
    set(SUMMARY_TARGET_PLATFORM "macOS")
elseif (UNIX)
    set(SUMMARY_TARGET_PLATFORM "Unix")
endif()

message("")

message(STATUS "Target platform\t=>\t${SUMMARY_TARGET_PLATFORM}")
message(STATUS "Target architecture\t=>\t${SUMMARY_TARGET_ARCH}")
message(STATUS "Library type\t\t=>\t${SUMMARY_LIB_TYPE}")

message(STATUS "Configuration\t=>\t${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler\t\t=>\t${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} @ ${CMAKE_C_COMPILER}")

message("")

message(STATUS "BUILT core lib?\t=>\t${THALLIUM_BUILD_LIB}")

if (THALLIUM_BUILD_LIB)
    message(STATUS "  - Vulkan module?\t=>\t  ${THALLIUM_BUILD_MODULE_VULKAN}")
endif()

message(STATUS "BUILT tests?\t\t=>\t${THALLIUM_BUILD_TESTS}")
# message(STATUS "BUILT examples?\t=>\t${THALLIUM_BUILD_EXAMPLES}")

message(STATUS "BUILT docs?\t\t=>\t${THALLIUM_BUILD_DOCS}")

if (THALLIUM_BUILD_DOCS)
    message(STATUS "  - see output at ${PROJECT_BINARY_DIR}/docs/html/index.html")
endif()

message("")

message(STATUS "See the online documentation at https://kosude.github.io/thallium")
message(STATUS "Or contribute to the upstream repository at https://github.com/kosude/thallium")

message("")
