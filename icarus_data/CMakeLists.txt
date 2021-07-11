if(NOT DEFINED CMAKE_BUILD_TYPE)
    message("Building for debug")
    set(CMAKE_BUILD_TYPE DEBUG)

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message("Exporting compile commands")
elseif(DEFINED CMAKE_BUILD_TYPE)

    if(NOT DEFINED ${CMAKE_BUILD_TYPE})
        message("CMAKE_BUILD_TYPE has not been defined. Defaulting to debug")
        set(CMAKE_BUILD_TYPE DEBUG)

        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
        message("Exporting compile commands")
    elseif("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
        message("Building for debug")
    
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
        message("Exporting compile commands")
    elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
        message("Building for release")
    endif()

    message("CMAKE_BUILD_TYPE is defined as ${CMAKE_BUILD_TYPE}")
endif()


set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "enforcing c++ standard requirement" FORCE)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "setting c++ standard" FORCE)



set(LIBMYSQL_MAIN_H
    "${VCPKG_ROOT_PATH}/packages/libmysql_${VCPKG_TRIPLET}/include")

message("MySQL path ${LIBMYSQL_MAIN_H}")

find_package(libmysql REQUIRED)
# find_package(SOCI CONFIG REQUIRED)


if(NOT DEFINED ICARUS_DATA_BUILD_OUTPUT)
    set(SOCI_CONFIG_H_FILE
        "${CMAKE_BINARY_DIR}/libraries/icarus_data/3rdparty/soci/include")
else()
    set(SOCI_CONFIG_H_FILE
        "${ICARUS_DATA_BUILD_OUTPUT}/3rdparty/soci/include")
endif()


message("soci-config path ${SOCI_CONFIG_H_FILE}")

# Don't remember if I need this. Will eventually find out
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


# add_library(icarus_data INTERFACE SHARED ${SOURCES})
add_library(icarus_data INTERFACE)

# Needed for libmysql
target_link_libraries(icarus_data PRIVATE ${MYSQL_LIBRARIES})
target_link_libraries(icarus_data INTERFACE soci_core soci_mysql)
# target_link_libraries(icarus_data INTERFACE SOCI::soci_core SOCI::soci_odbc SOCI::soci_empty)
target_include_directories(icarus_data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include ${SOCI_CONFIG_H_FILE})
# target_include_directories(icarus_data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(icarus_data INTERFACE ${LIBMYSQL_MAIN_H})


