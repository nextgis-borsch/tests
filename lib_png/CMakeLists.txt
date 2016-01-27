cmake_minimum_required (VERSION 2.8)
project (png_test)

set(WITH_PNG ON CACHE BOOL "set with png on" FORCE)
set(WITH_PNG_EXTERNAL ON CACHE BOOL "set with png external on" FORCE)

include(FindAnyProject)
find_anyproject(PNG REQUIRED)


#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set(pngtest_sources
  pngtest.c
)
set(pngvalid_sources
  contrib/libtests/pngvalid.c
)
set(pngstest_sources
  contrib/libtests/pngstest.c
)

add_definitions(-DPNG_FREESTANDING_TESTS)
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/contrib/visupng)

add_executable(pngtest ${pngtest_sources})
target_link_extlibraries(pngtest)
add_test(pngtest ./pngtest ${CMAKE_CURRENT_SOURCE_DIR}/pngtest.png)
#
add_executable(pngvalid ${pngvalid_sources})
target_link_extlibraries(pngvalid)
add_test(pngvalid ./pngvalid)
add_executable(pngstest ${pngstest_sources})
target_link_extlibraries(pngstest)
add_test(pngstest ./pngstest
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn0g01.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn0g02.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn0g04.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn0g08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn0g16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn2c08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn2c16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn3p01.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn3p02.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn3p04.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn4a08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn4a16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn6a08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/basn6a16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbbn0g01.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbbn0g02.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbbn0g04.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbbn2c16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbbn3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbgn2c16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbgn3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbrn2c08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbwn0g16.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbwn3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftbyn3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftp0n0g08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftp0n2c08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftp0n3p08.png
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/pngsuite/ftp1n3p08.png
)
