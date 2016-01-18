cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(extlib_tests)

# some init settings
set(CMAKE_COLOR_MAKEFILE ON)
# set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(LIB_TYPE STATIC)

set (DART_TESTING_TIMEOUT 1200 CACHE INTEGER
       "Timeout in seconds for each test (default 1200=20minutes)")

enable_testing ()
include (CTest)

option (BUILD_SZIP_TESTING "Build SZIP Unit Testing" ON)
if(BUILD_SZIP_TESTING)
    add_subdirectory (${CMAKE_SOURCE_DIR}/lib_szip)
endif()
