cmake_minimum_required(VERSION 3.0.0)

project ("niok" C CXX)

set(CMAKE_SKIP_RPATH TRUE)

option(ENABLE_TESTING "Whether to turn unit test ON or OFF" ON)

if (ENABLE_TESTING)
    enable_testing()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "_build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "_build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "_build")

# Set the project home dir
set(NIOK_HOME ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DNIOK_HOME=${NIOK_HOME})

# To include customized FindXXX.cmake modules
set(CMAKE_MODULE_PATH "${NIOK_HOME}/cmake" ${CMAKE_MODULE_PATH})

# Possible values are Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Set macro _FORTIFY_SOURCE=2")
    add_definitions(-D_FORTIFY_SOURCE=2)
endif()

find_package(Leveldb REQUIRED)
# find_package(GMP REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

macro(niok_link_libraries target)
    target_link_libraries(
        ${target}
        leveldb
        gtest_main
        ntl
        cpisync
        gmp
        -pthread
    )
endmacro(niok_link_libraries)

macro(niok_add_executable)
    cmake_parse_arguments(
        niok_exec                   # prefix
        ""                          # <options>
        "NAME"                      # <one_value_args>
        "SOURCES;OBJECTS;LIBRARIES" # <multi_value_args>
        ${ARGN}
    )
    add_executable(
        ${niok_exec_NAME}
        ${niok_exec_SOURCES}
        ${niok_exec_OBJECTS}
    )
    niok_link_libraries(
        ${niok_exec_NAME}
        ${niok_exec_LIBRARIES}
    )
endmacro()

macro(niok_add_test)
    cmake_parse_arguments(
        niok_test                 # prefix
        "DISABLED"                  # <options>
        "NAME"                      # <one_value_args>
        "SOURCES;OBJECTS;LIBRARIES" # <multi_value_args>
        ${ARGN}
    )
    niok_add_executable(
        NAME ${niok_test_NAME}
        SOURCES ${niok_test_SOURCES}
        OBJECTS ${niok_test_OBJECTS}
        LIBRARIES ${niok_test_LIBRARIES}
    )

    if (NOT ${niok_test_DISABLED})
        string(REGEX REPLACE "${CMAKE_SOURCE_DIR}/src/(.*)/test" "\\1" test_group ${CMAKE_CURRENT_SOURCE_DIR})
        add_test(NAME ${niok_test_NAME} COMMAND ${niok_test_NAME})
        set_tests_properties(${niok_test_NAME} PROPERTIES LABELS ${test_group})
        # e.g. cmake -DNEBULA_ASAN_PRELOAD=/path/to/libasan.so
        # or,  cmake -DNEBULA_ASAN_PRELOAD=`/path/to/gcc --print-file-name=libasan.so`
    endif()
endmacro()

macro(niok_add_library name type)
    add_library(${name} ${type} ${ARGN})
endmacro()

function(niok_add_subdirectory dir_name)
    if ((NOT ENABLE_TESTING) AND (${dir_name} STREQUAL test))
        return()
    endif()
    add_subdirectory(${dir_name})
endfunction()

set(LEVELDB_LIBRARIES ${Leveldb_LIBRARY})
set(GOOGLETEST_LIBRARIES ${Googletest_LIBRARY})
set(GMP_LIBRARIES ${GMP_LIBRARY})

add_custom_target(
    clean-build
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND "find" "." "-name" "Testing" "|" "xargs" "rm" "-fr"
    DEPENDS clean-interface clean-pch clean-hbase
)

add_custom_target(
    clean-all
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND "find" "." "-name" "Testing" "|" "xargs" "rm" "-fr"
    DEPENDS ${NEBULA_CLEAN_ALL_DEPS}
)

add_custom_target(
    distclean
    COMMAND "find" "." "-name" "CMakeFiles" "|" "xargs" "rm" "-fr"
    COMMAND "find" "." "-name" "CMakeCache.txt" "|" "xargs" "rm" "-f"
    COMMAND "find" "." "-name" "cmake_install.cmake" "|" "xargs" "rm" "-f"
    COMMAND "find" "." "-name" "CTestTestfile.cmake" "|" "xargs" "rm" "-f"
    COMMAND "find" "." "-name" "Makefile" "|" "xargs" "rm" "-f"
    DEPENDS clean-all
)

add_subdirectory(src)

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)