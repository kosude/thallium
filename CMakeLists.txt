cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(thallium)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THALLIUM_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(THALLIUM_DEPS_DIR "${CMAKE_CURRENT_LIST_DIR}/deps")
set(THALLIUM_DOCS_DIR "${CMAKE_CURRENT_LIST_DIR}/docs")
set(THALLIUM_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(THALLIUM_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
set(THALLIUM_TESTS_DIR "${CMAKE_CURRENT_LIST_DIR}/tests")
# set(THALLIUM_EXAMPLES_DIR "${CMAKE_CURRENT_LIST_DIR}/examples")

set(CMAKE_MODULE_PATH "${THALLIUM_CMAKE_DIR}")

set(THALLIUM_LIBRARIES)
set(THALLIUM_LIBRARY_DEFS)
set(THALLIUM_COMPILE_OPTIONS)

if (MSVC)
    set(THALLIUM_COMPILE_OPTIONS
        /W4 /WX
    )
else()
    set(THALLIUM_COMPILE_OPTIONS
        -Wall -Wextra -pedantic -Werror
        -Wno-strict-prototypes
    )
endif()

set(SUMMARY_TARGET_PLATFORM "Unknown")
set(SUMMARY_TARGET_ARCH "Unknown")
set(SUMMARY_LIB_TYPE "Unknown")

option(BUILD_SHARED_LIBS "Build shared libraries (or DLLs if on Windows)" ON)
option(THALLIUM_BUILD_LIB "Build the core Thallium library" ON)

if (THALLIUM_BUILD_LIB)
    option(THALLIUM_BUILD_TESTS "Build Thallium tests" OFF)
    # option(THALLIUM_BUILD_EXAMPLES "Build Thallium example projects" OFF)
endif()

option(THALLIUM_BUILD_DOCS "Build Thallium HTML documentation" OFF)

if (THALLIUM_BUILD_LIB)
    # apple isn't as intelligent as everyone else so their toy "computers" don't support vulkan
    if (NOT APPLE)
        option(THALLIUM_BUILD_MODULE_VULKAN "Include the Vulkan module in the compilation" OFF)
    endif()
endif()

option(THALLIUM_DEBUG_LAYER "Enable Thallium debugging utilities (applies to both Debug and Release configs)" ON)
if (THALLIUM_DEBUG_LAYER)
    set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "THALLIUM_DEBUG_LAYER")
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

set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "THALLIUM_BUILD_${CMAKE_BUILD_TYPE}")

option(THALLIUM_SKIP_VERSION "Skip getting Thallium version dynamically" OFF)

# either building docs or otherwise not skipping version retrieval
if (NOT THALLIUM_SKIP_VERSION OR THALLIUM_BUILD_DOCS)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    execute_process(
        COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/utils/py/GetVersion.py"
        OUTPUT_VARIABLE THALLIUM_VERSION
    )
else()
    set(THALLIUM_VERSION "unknown_version")
endif()

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

message(STATUS "Debug layer enabled?\t=>\t${THALLIUM_DEBUG_LAYER}")

message("")

message(STATUS "See the online documentation at https://kosude.github.io/thallium")
message(STATUS "Or contribute to the upstream repository at https://github.com/kosude/thallium")

message("")

message(STATUS "Thallium version ${THALLIUM_VERSION}")
