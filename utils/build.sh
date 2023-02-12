#!/usr/bin/env sh

#
#   Copyright (c) 2023 Jack Bennett
#   All rights reserved.
#
#   Please see the LICENCE file for more information.
#

# build.sh
# ----
# An interactive script to make the Thallium build process simpler.
# Note that # enabling some modules requires installation of other software. (For example,
# the Vulkan module requires the Vulkan SDK to be installed.)
# Building documenation requires Doxygen.

# dependencies:
# cmake (https://cmake.org/)

exit_on_invalid() {
    echo "invalid: $1"
    echo

    echo "Exiting script on error..."
    exit
}

CMAKE_EXEC="/usr/bin/cmake"

CMAKE_CONFIG_CMD="${CMAKE_EXEC}"
CMAKE_BUILD_CMD="${CMAKE_EXEC} --build"

PROJECT_PATH="$(dirname -- "$(dirname -- "$(readlink -f "${BASH_SOURCE}")")")"
BUILD_PATH="${PROJECT_PATH}/build"

# add project path to config command
CMAKE_CONFIG_CMD+=" ${PROJECT_PATH}"

# add build path to commands
CMAKE_CONFIG_CMD+=" -B ${BUILD_PATH}"
CMAKE_BUILD_CMD+=" ${BUILD_PATH}"

echo "  build.sh: Thallium interactive build configuration script"
echo "  Note that input strings are case-sensitive."
echo
echo "  An asterisk next to an option means it is the default."
echo "  Full forms of options with shorthands are inside square brackets."
echo "  Options separated by a pipe (|) are interchangable."
echo

#
# get build configuration type.

read -p "? Choose build configuration (d[ebug], r[elease]*) > " CHOICE
BUILD_CONFIGURATION_TYPE=

case $CHOICE in
    "d" | "debug")
        BUILD_CONFIGURATION_TYPE="Debug"
        echo "    Building with DEBUG config"
        ;;
    "r" | "release" | "")
        BUILD_CONFIGURATION_TYPE="Release"
        echo "    Building with RELEASE config"
        ;;
    *)
        exit_on_invalid $CHOICE
        ;;
esac

CMAKE_CONFIG_CMD+=" -DCMAKE_BUILD_TYPE=${BUILD_CONFIGURATION_TYPE}"

#
# get library build type.

read -p "? Choose output library type (st[atic], sh[ared]*) > " CHOICE
SHARED_LIBRARIES_FLAG=

case $CHOICE in
    "st" | "static")
        SHARED_LIBRARIES_FLAG="OFF"
        echo "    Building STATIC libraries"
        ;;
    "sh" | "shared" | "")
        SHARED_LIBRARIES_FLAG="ON"
        echo "    Building SHARED/DYNAMIC libraries"
        ;;
    *)
        exit_on_invalid $CHOICE
        ;;
esac

CMAKE_CONFIG_CMD+=" -DBUILD_SHARED_LIBS=${SHARED_LIBRARIES_FLAG}"

#
# prompt for debug layer

read -p "? Enable debug layer? (y[es]*, n[o]) > " CHOICE
DEBUG_LAYER_FLAG=

case $CHOICE in
    "y" | "yes" | "")
        DEBUG_LAYER_FLAG="ON"
        echo "    ENABLING debug layer"
        ;;
    "n" | "no")
        DEBUG_LAYER_FLAG="OFF"
        echo "    DISABLING debug layer"
        ;;
    *)
        exit_on_invalid $CHOICE
        ;;
esac

CMAKE_CONFIG_CMD+=" -DTHALLIUM_DEBUG_LAYER=${DEBUG_LAYER_FLAG}"

#
# prompt for core library build.

read -p "? Build core library? (y[es]*, n[o]) > " CHOICE
CORE_LIBRARY_FLAG=

case $CHOICE in
    "y" | "yes" | "")
        CORE_LIBRARY_FLAG="ON"
        echo "    Building core library"
        ;;
    "n" | "no")
        CORE_LIBRARY_FLAG="OFF"
        echo "    Skipping core library"
        ;;
    *)
        exit_on_invalid $CHOICE
        ;;
esac

CMAKE_CONFIG_CMD+=" -DTHALLIUM_BUILD_LIB=${CORE_LIBRARY_FLAG}"

#
# build Vulkan module? (if core library enabled)

if [ "${CORE_LIBRARY_FLAG}" = "ON" ]
then
    read -p "? Build Vulkan library module? (y[es], n[o]*) > " CHOICE
    VULKAN_MOD_FLAG=

    case $CHOICE in
        "y" | "yes")
            VULKAN_MOD_FLAG="ON"
            echo "    Building Vulkan module"
            ;;
        "n" | "no" | "")
            VULKAN_MOD_FLAG="OFF"
            echo "    Skipping Vulkan module"
            ;;
        *)
            exit_on_invalid $CHOICE
            ;;
    esac

    CMAKE_CONFIG_CMD+=" -DTHALLIUM_BUILD_MODULE_VULKAN=${VULKAN_MOD_FLAG}"
fi

#
# build tests? (if core library enabled)

if [ "${CORE_LIBRARY_FLAG}" = "ON" ]
then
    read -p "? Build test executables? (y[es], n[o]*) > " CHOICE
    TESTS_FLAG=

    case $CHOICE in
        "y" | "yes")
            TESTS_FLAG="ON"
            echo "    Building tests"
            ;;
        "n" | "no" | "")
            TESTS_FLAG="OFF"
            echo "    Skipping tests"
            ;;
        *)
            exit_on_invalid $CHOICE
            ;;
    esac

    CMAKE_CONFIG_CMD+=" -DTHALLIUM_BUILD_TESTS=${TESTS_FLAG}"
fi

#
# build examples? (if core library enabled)

if [ "${CORE_LIBRARY_FLAG}" = "ON" ]
then
    read -p "? Build example projects? (y[es], n[o]*) > " CHOICE
    EXAMPLES_FLAG=

    case $CHOICE in
        "y" | "yes")
            EXAMPLES_FLAG="ON"
            echo "    Building examples"
            ;;
        "n" | "no" | "")
            EXAMPLES_FLAG="OFF"
            echo "    Skipping examples"
            ;;
        *)
            exit_on_invalid $CHOICE
            ;;
    esac

    CMAKE_CONFIG_CMD+=" -DTHALLIUM_BUILD_EXAMPLES=${EXAMPLES_FLAG}"
fi

#
# build docs?

read -p "? Build HTML documentation? (y[es]*, n[o]) > " CHOICE
DOCS_FLAG=

case $CHOICE in
    "y" | "yes")
        DOCS_FLAG="ON"
        echo "    Building documentation"
        ;;
    "n" | "no" | "")
        DOCS_FLAG="OFF"
        echo "    Skipping documentation"
        ;;
    *)
        exit_on_invalid $CHOICE
        ;;
esac

CMAKE_CONFIG_CMD+=" -DTHALLIUM_BUILD_DOCS=${DOCS_FLAG}"

#
# final cmake command execution

echo
echo "| CONFIGURATION COMMAND:"
echo "| ${CMAKE_CONFIG_CMD}"
echo "|"
echo "| BUILD COMMAND:"
echo "| ${CMAKE_BUILD_CMD}"
echo

echo "Executing..."
${CMAKE_CONFIG_CMD}
${CMAKE_BUILD_CMD}
