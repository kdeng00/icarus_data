cmake_minimum_required(VERSION 3.12)


if(NOT DEFINED VCPKG_ROOT_PATH)
    set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)
endif()


message("The vcpkg root path ${VCPKG_ROOT_PATH}")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_PATH ${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake)

    set(CMAKE_TOOLCHAIN_FILE
        "${TOOLCHAIN_PATH}"
        CACHE STRING "Vcpkg toolchain file")

    message("Using default toolchain file")
endif()



if(NOT DEFINED VCPKG_TRIPLET)
    message("VCPKG_TRIPLET has not been defined...")
    set(VCPKG_TRIPLET "x64-linux")
    message("Using default ${VCPKG_TRIPLET} triplet")
endif()

project(icarus_data VERSION "0.0.1" DESCRIPTION "Database library used to power icarus")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(SOCI_CONFIG_H_FILE
    "${CMAKE_BINARY_DIR}/libraries/icarus_data/3rdparty/soci/include")

message("soci-config path ${SOCI_CONFIG_H_FILE}")

# include_directories("${CMAKE_CURRENT_LIST_DIR}/include")


set(SOCI_TESTS OFF CACHE BOOL "enable soci tests" FORCE)
set(SOCI_CXX11 ON CACHE BOOL "enable soci c++ 11" FORCE)
set(SOCI_EMPTY OFF CACHE BOOL "enable soci empty" FORCE)
set(WITH_MYSQL ON CACHE BOOL "enable mysql" FORCE)
set(WITH_ODBC OFF CACHE BOOL "enable ODBC" FORCE)
set(WITH_SQLITE3 OFF CACHE BOOL "enable sqlite3" FORCE)
set(WITH_BOOST OFF CACHE BOOL "enable boost" FORCE)
set(WITH_DB2 OFF CACHE BOOL "enable DB2" FORCE)
set(WITH_FIREBIRD OFF CACHE BOOL "enable firebird" FORCE)
set(WITH_ORACLE OFF CACHE BOOL "enable oracle" FORCE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/3rdparty/soci)


set(LIBMYSQL_MAIN_H
    "${VCPKG_ROOT_PATH}/packages/libmysql_${VCPKG_TRIPLET}/include/mysql")

# add_library(icarus_data INTERFACE SHARED ${SOURCES})
add_library(icarus_data INTERFACE)

target_link_libraries(icarus_data INTERFACE soci_core soci_mysql)
target_include_directories(icarus_data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include ${SOCI_CONFIG_H_FILE})
# target_include_directories(icarus_data INTERFACE ${SOCI_MYSQL_INCLUDE_DIRECTORIES})
target_include_directories(icarus_data INTERFACE ${LIBMYSQL_MAIN_H})
