cmake_minimum_required(VERSION 3.10.2)

if(NOT DEFINED VCPKG_ROOT_PATH)
    set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)
endif()

message("The vcpkg root path ${VCPKG_ROOT_PATH}")


set(SOFTWARE_DESCRIPTION
    "Database access library for icarus")
set(SOFTWARE_VERSION
    "0.1.0")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_PATH ${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake)

    set(CMAKE_TOOLCHAIN_FILE
        "${TOOLCHAIN_PATH}"
        CACHE STRING "Vcpkg toolchain file")

    message("Using default toolchain file from VCPKG_ROOT_PATH variable ${CMAKE_TOOLCHAIN_FILE}")
endif()



if(NOT DEFINED VCPKG_TRIPLET)
    message("System name: ${CMAKE_HOST_SYSTEM_NAME}")

    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
        set(VCPKG_TRIPLET "x64-osx")
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        # Adding Visual C++ support
        set(WINDOWS TRUE)
        set(VCPKG_TRIPLET "x64-windows")
    else ()
        set(VCPKG_TRIPLET "x64-linux")
    endif()

    message("Using default ${VCPKG_TRIPLET} triplet")
endif()

# C++ 14 is the target C++ standard
project(icarus_data VERSION "0.1.0" DESCRIPTION "Database library used to power icarus")

# TODO: Build or have the option to build the examples

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/icarus_data)
