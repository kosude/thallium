cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project("thallium" C CXX)

set(THALLIUM_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(THALLIUM_DEPS_DIR "${CMAKE_CURRENT_LIST_DIR}/deps")
set(THALLIUM_DOCS_DIR "${CMAKE_CURRENT_LIST_DIR}/docs")
set(THALLIUM_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(THALLIUM_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(THALLIUM_TESTS_DIR "${CMAKE_CURRENT_LIST_DIR}/tests")
set(THALLIUM_UTILS_DIR "${CMAKE_CURRENT_LIST_DIR}/utils")

set(CMAKE_MODULE_PATH "${THALLIUM_CMAKE_DIR}")

set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
    message(WARNING "Missing build configuration type! Defaulted to RELEASE, reconfigure project with option -DCMAKE_BUILD_TYPE=(Debug|Release)")
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(THALLIUM_DEBUG true)
    set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "DEBUG")
endif()

if (WIN32)
    set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "WIN32")
endif()
if (APPLE)
    set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "APPLE")
endif()
if (UNIX)
    set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "UNIX")
endif()

# treating warnings as errors may be problematic when compiling for prod use.
# we are assuming here that lib users will be compiling in release config and lib developers/contributors in debug config.
if (THALLIUM_DEBUG)
    if (MSVC)
        set(THALLIUM_COMPILE_OPTIONS
            /W4 /WX)
    else()
        set(THALLIUM_COMPILE_OPTIONS
            -Wall -Wextra -pedantic -Werror
            -Wno-error=unused-parameter
            -Wno-error=unused-function)
    endif()
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries (or DLLs if on Windows)" ON)
if (BUILD_SHARED_LIBS)
    set(THALLIUM_LIB_TYPE SHARED)
else()
    set(THALLIUM_LIB_TYPE STATIC)
endif()

option(THALLIUM_BUILD_TESTS "Build Thallium tests" OFF)
option(THALLIUM_BUILD_DOCS "Build Thallium HTML documentation" OFF)

option(THALLIUM_BUILD_MODULE_VULKAN "Include the Vulkan module in the compilation" OFF)

add_subdirectory("${THALLIUM_SRC_DIR}")

if (THALLIUM_BUILD_TESTS)
    add_subdirectory("${THALLIUM_TESTS_DIR}")
endif()

# TODO: docs
# if (THALLIUM_BUILD_DOCS)
#     add_subdirectory("${THALLIUM_DOCS_DIR}/")
# endif()

if (BUILD_SHARED_LIBS)
    set(THALLIUM_SUMMARY_LIB_TYPE "Shared/dynamic")
else()
    set(THALLIUM_SUMMARY_LIB_TYPE "Static")
endif()

if (APPLE OR CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(THALLIUM_SUMMARY_TARGET_ARCH "AMD64 (x86_64)")
else()
    set(THALLIUM_SUMMARY_TARGET_ARCH "i386 (x86)")
endif()

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(THALLIUM_SUMMARY_TARGET_PLATFORM "Win64")
    else()
        set(THALLIUM_SUMMARY_TARGET_PLATFORM "Win32")
    endif()
elseif (APPLE)
    set(THALLIUM_SUMMARY_TARGET_PLATFORM "macOS")
elseif (UNIX)
    set(THALLIUM_SUMMARY_TARGET_PLATFORM "Unix")
endif()

message("")

message(STATUS "Target platform\t=>\t${THALLIUM_SUMMARY_TARGET_PLATFORM}")
message(STATUS "Target architecture\t=>\t${THALLIUM_SUMMARY_TARGET_ARCH}")
message(STATUS "Library type\t\t=>\t${THALLIUM_SUMMARY_LIB_TYPE}")

message(STATUS "Configuration\t=>\t${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler\t\t=>\t${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} @ ${CMAKE_C_COMPILER}")

message("")

message(STATUS "BUILT core lib")
message(STATUS "  - Vulkan module?\t=>\t  ${THALLIUM_BUILD_MODULE_VULKAN}")

message(STATUS "BUILT tests?\t\t=>\t${THALLIUM_BUILD_TESTS}")
# message(STATUS "BUILT docs?\t\t=>\t${THALLIUM_BUILD_DOCS}")

# if (THALLIUM_BUILD_DOCS)
#     message(STATUS "  - see output at ${PROJECT_BINARY_DIR}/docs/html/index.html")
# endif()

message("")

message(STATUS "See the online documentation at https://kosude.github.io/thallium")
message(STATUS "Or contribute to the upstream repository at https://github.com/kosude/thallium")
