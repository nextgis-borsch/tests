################################################################################
# Project:  CMake4GDAL
# Purpose:  CMake build scripts
# Author:   Dmitry Baryshnikov, polimax@mail.ru
################################################################################
# Copyright (C) 2015, NextGIS <info@nextgis.com>
# Copyright (C) 2012,2013,2014 Dmitry Baryshnikov
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

cmake_minimum_required (VERSION 2.8)
project (gdal_test)

option (BUILD_GDAL_TESTING "Build GDAL Unit Testing" ON)
if(BUILD_GDAL_TESTING)
    #TODO: to use from python we need shared build of gdal
    
    set(WITH_GDAL ON CACHE BOOL "set with gdal on" FORCE)
    set(WITH_GDAL_EXTERNAL ON CACHE BOOL "set with gdal external on" FORCE)

    find_anyproject(GDAL REQUIRED)

    #check for python
    find_package(PythonInterp REQUIRED)
    if(NOT PYTHONINTERP_FOUND)
        message(FATAL_ERROR "Cannot find python")
    endif()

    #TODO: add to PYTHON_PATH build swig diretory (temporary)
    # build/bindings/python/build/lib.linux-x86_64-2.7

    #add tests
    add_test(gcore ${PYTHON_EXECUTABLE} run_all.py gcore)
    add_test(gdrivers ${PYTHON_EXECUTABLE} run_all.py gdrivers)
    add_test(alg ${PYTHON_EXECUTABLE} run_all.py alg)
    add_test(osr ${PYTHON_EXECUTABLE} run_all.py osr)
    add_test(ogr ${PYTHON_EXECUTABLE} run_all.py ogr)
    add_test(utilities ${PYTHON_EXECUTABLE} run_all.py utilities)
    add_test(pymod ${PYTHON_EXECUTABLE} run_all.py pymod)
    add_test(pyscripts ${PYTHON_EXECUTABLE} run_all.py pyscripts)
    add_test(gnm ${PYTHON_EXECUTABLE} run_all.py gnm)
    
    #TODO: add_subdirectory(cpp)
endif()
