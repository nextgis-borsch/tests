cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(extlib_tests)

# some init settings
set(CMAKE_COLOR_MAKEFILE ON)
# set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

enable_testing()
include(CTest)
include(FindAnyProject)

set(BUILD_SHARED_LIBS OFF) # else we need to install test libraries somethere
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release" FORCE)
endif()

if(CMAKE_GENERATOR_TOOLSET MATCHES "v([0-9]+)_xp")
    add_definitions(-D_WIN32_WINNT=0x0501)
endif()

set(DART_TESTING_TIMEOUT 1200 CACHE INTEGER
       "Timeout in seconds for each test (default 1200=20minutes)")

file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/lib_*)
foreach(TEST ${TESTS})
    if(EXISTS ${TEST}/CMakeLists.txt) 
        add_subdirectory(${TEST})
    endif()
endforeach()
add_subdirectory(googletest)
