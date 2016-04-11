cmake_minimum_required (VERSION 2.8)
project (png_test)

option (BUILD_PNG_TESTING "Build PNG Unit Testing" ON)
if(BUILD_PNG_TESTING)
    set(WITH_PNG ON CACHE BOOL "set with png on" FORCE)
    set(WITH_PNG_EXTERNAL ON CACHE BOOL "set with png external on" FORCE)

    find_anyproject(PNG REQUIRED)
    find_anyproject(ZLIB REQUIRED)
    
    if(NOT WIN32)
        find_library(M_LIBRARY
            NAMES m
            PATHS /usr/lib /usr/local/lib
        )
        if(NOT M_LIBRARY)
          message(STATUS "math lib 'libm' not found; floating point support disabled")
        endif()
    else()
      # not needed on windows
      set(M_LIBRARY "")
    endif()
        
    set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${M_LIBRARY})


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
    add_test(${PROJECT_NAME} ./pngtest ${CMAKE_CURRENT_SOURCE_DIR}/pngtest.png)
    #
    add_executable(pngvalid ${pngvalid_sources})
    target_link_extlibraries(pngvalid)
    add_test(${PROJECT_NAME}_valid ./pngvalid)
    add_executable(pngstest ${pngstest_sources})
    target_link_extlibraries(pngstest)
    add_test(${PROJECT_NAME}_s ./pngstest
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
endif()    
