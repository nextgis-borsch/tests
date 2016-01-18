cmake_minimum_required (VERSION 2.8)
project (zlib_test)

set(WITH_ZLIB ON CACHE BOOL "set with szip on" FORCE)
set(WITH_ZLIB_EXTERNAL ON CACHE BOOL "set with szip external on" FORCE)

include(CheckTypeSize)
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable


include(find_anyproject)
find_anyproject(ZLIB REQUIRED)

add_executable(example example.c)
target_link_extlibraries (example)
add_test(${PROJECT_NAME}_example example)

add_executable(minigzip minigzip.c)
target_link_extlibraries (minigzip)

if(HAVE_OFF64_T)
    add_executable(example64 example.c)
    target_link_extlibraries (example64)
    set_target_properties(example64 PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64")
    add_test(${PROJECT_NAME}_example64 example64)

    add_executable(minigzip64 minigzip.c)
    target_link_extlibraries (minigzip64)
    set_target_properties(minigzip64 PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64")
endif()

