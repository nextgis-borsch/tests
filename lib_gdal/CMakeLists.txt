################################################################################
# Project:  CMake4GDAL
# Purpose:  CMake build scripts
# Authors:   Dmitry Baryshnikov (polimax@mail.ru), Mikhail Gusev (gusevmihs@gmail.com)
################################################################################
# Copyright (C) 2015, NextGIS <info@nextgis.com>
# Copyright (C) 2012,2013,2014 Dmitry Baryshnikov
# Copyright (C) 2016 Mikhail Gusev
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
################################################################################

cmake_minimum_required(VERSION 2.8.12)
project(gdal_test)

option(BUILD_GDAL_TESTING "Build GDAL Unit Testing" ON)
if(BUILD_GDAL_TESTING)

    set(WITH_GDAL ON CACHE BOOL "set with gdal on" FORCE)
    set(WITH_GDAL_EXTERNAL ON CACHE BOOL "set with gdal external on" FORCE)
    
    # NOTE: to use from python we need shared build of GDAL.
    # Otherwise it will not be available for python to call bound gdal methods from static gdal lib.
    set(BUILD_SHARED_LIBS ON CACHE BOOL "..." FORCE) 
    
    # Force setting required for ALL tests variables.
    set(WITH_PYTHON ON CACHE BOOL "..." FORCE)
    set(WITH_CURL ON CACHE BOOL "..." FORCE) 
    set(WITH_CURL_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_EXPAT ON CACHE BOOL "..." FORCE) 
    set(WITH_EXPAT_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_GEOS ON CACHE BOOL "..." FORCE) 
    set(WITH_GEOS_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_GeoTIFF ON CACHE BOOL "..." FORCE) 
    set(WITH_GeoTIFF_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_ICONV ON CACHE BOOL "..." FORCE) 
    set(WITH_ICONV_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_JBIG ON CACHE BOOL "..." FORCE) 
    set(WITH_JBIG_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_JPEG ON CACHE BOOL "..." FORCE) 
    set(WITH_JPEG_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_JPEG12 ON CACHE BOOL "..." FORCE) 
    set(WITH_JPEG12_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_JSONC ON CACHE BOOL "..." FORCE) 
    set(WITH_JSONC_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_LibLZMA ON CACHE BOOL "..." FORCE) 
    set(WITH_LibLZMA_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_LibXml2 ON CACHE BOOL "..." FORCE) 
    set(WITH_LibXml2_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_OpenSSL ON CACHE BOOL "..." FORCE) 
    set(WITH_OpenSSL_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_PNG OFF CACHE BOOL "..." FORCE) 
    set(WITH_PNG_EXTERNAL OFF CACHE BOOL "..." FORCE)
    set(WITH_PROJ4 ON CACHE BOOL "..." FORCE) 
    set(WITH_PROJ4_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_SQLite3 ON CACHE BOOL "..." FORCE) 
    set(WITH_SQLite3_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_TIFF ON CACHE BOOL "..." FORCE) 
    set(WITH_TIFF_EXTERNAL ON CACHE BOOL "..." FORCE)
    set(WITH_ZLIB ON CACHE BOOL "..." FORCE) 
    set(WITH_ZLIB_EXTERNAL ON CACHE BOOL "..." FORCE) 
    
    # Find GDAL.
    find_anyproject(GDAL REQUIRED CMAKE_ARGS # Build GDAL inside this command.
        -DENABLE_PNG=OFF # temporary
        -DENABLE_OZI=ON
    )
      
    # Find Python.
    find_package(PythonInterp REQUIRED)
    if(NOT PYTHONINTERP_FOUND)
        message(FATAL_ERROR "Cannot find python")
    endif()
    
    # Create separate directory into which we will put generated python libraries (because each OS will have unique
    # directory name). 
    set(TEST_LIB_GDAL_DIRECTORY ${CMAKE_BINARY_DIR}/Testing-lib_gdal)
    add_custom_command(TARGET GDAL_EP
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_LIB_GDAL_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_BINARY_DIR}/third-party/Install/GDAL_EP/lib/Python27/site-packages/. 
            ${TEST_LIB_GDAL_DIRECTORY}/.
        COMMENT "Copying GDAL python libraries to ${TEST_LIB_GDAL_DIRECTORY}")
    if(WIN32)
        # For windows copy GDAL dlls to the python modules' directory.
        add_custom_command(TARGET GDAL_EP
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
                ${CMAKE_BINARY_DIR}/third-party/Install/GDAL_EP/bin/. 
                ${TEST_LIB_GDAL_DIRECTORY}/osgeo/.
            COMMENT "Copying GDAL dlls to ${TEST_LIB_GDAL_DIRECTORY}")
    endif()
    
    # Finally we can do tests for each python test dir, adding this path for finding python modules 
    # in order not to touch the system GDAL if it exists.
    macro(do_gdal_test name)
        option(TEST_GDAL_${name} "Enable to test ${name} in GDAL. See results in /Testing/Temporary/LastTest.log" ON)
        if(TEST_GDAL_${name})
            add_test(NAME ${name} COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/run_all.py ${name})
            set_property(TEST ${name} PROPERTY ENVIRONMENT "PYTHONPATH=${TEST_LIB_GDAL_DIRECTORY}") # reset PYTHONPATH
        endif()
    endmacro()
    
    # TODO: customize CTest so it can correctly define tests' results.
    # https://cmake.org/Wiki/CMake/Testing_With_CTest
    
    # Run all python tests.
    do_gdal_test(gcore)
    do_gdal_test(gdrivers)
    do_gdal_test(alg)
    do_gdal_test(osr)
    do_gdal_test(ogr)
    do_gdal_test(utilities)
    do_gdal_test(pymod)
    do_gdal_test(pyscripts)
    do_gdal_test(gnm)
    
    # TODO: add_subdirectory(cpp).
    
endif()
