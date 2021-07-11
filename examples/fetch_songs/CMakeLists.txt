cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED VCPKG_ROOT_PATH)
    set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)
endif()

message("The vcpkg root path ${VCPKG_ROOT_PATH}")


if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_PATH "${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Toolchain path")

    set(CMAKE_TOOLCHAIN_FILE
        "${TOOLCHAIN_PATH}"
        CACHE STRING "Vcpkg toolchain file")

    message("Using default toolchain file from VCPKG_ROOT_PATH variable ${CMAKE_TOOLCHAIN_FILE}")
else()
    message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")
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


project(fetch_songs)



set(ICARUS_DATA_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../../icarus_data/include") 
message("icarus_data include ${ICARUS_DATA_INCLUDE}")

include_directories(${ICARUS_DATA_INCLUDE})

# TODO: Get a working example and then merge this to the master/main branch

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tinyxml2)

# icarus_data lib
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/icarus_data)

set(ICARUS_DATA_BUILD_OUTPUT ${CMAKE_BINARY_DIR}/subdirs)

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../icarus_data ${CMAKE_BINARY_DIR}/subdir_bin)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../icarus_data ${ICARUS_DATA_BUILD_OUTPUT})

add_executable(fetch_songs fetch_songs.cpp)

target_link_libraries(fetch_songs PRIVATE icarus_data)
