# Find json-c
# ~~~~~~~~~
# Copyright (c) 2012, Dmitry Baryshnikov <polimax at mail.ru>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for Libkml library
#
# If it's found it sets JSONC_FOUND to TRUE
# and following variables are set:
#    JSONC_INCLUDE_DIR
#    JSONC_LIBRARY

# FIND_PATH and FIND_LIBRARY normally search standard locations
# before the specified paths. To search non-standard paths first,
# FIND_* is invoked first with specified paths and NO_DEFAULT_PATH
# and then again with no specified paths to search the default
# locations. When an earlier FIND_* succeeds, subsequent FIND_*s
# searching for the same item do nothing. 

# try to use framework on mac
# want clean framework path, not unix compatibility path
IF (APPLE)
  IF (CMAKE_FIND_FRAMEWORK MATCHES "FIRST"
      OR CMAKE_FRAMEWORK_PATH MATCHES "ONLY"
      OR NOT CMAKE_FIND_FRAMEWORK)
    SET (CMAKE_FIND_FRAMEWORK_save ${CMAKE_FIND_FRAMEWORK} CACHE STRING "" FORCE)
    SET (CMAKE_FIND_FRAMEWORK "ONLY" CACHE STRING "" FORCE)
    #FIND_PATH(JSONC_INCLUDE_DIR JSONC/dom.h)
    FIND_LIBRARY(JSONC_LIBRARY JSONC)
    IF (JSONC_LIBRARY)
      # FIND_PATH doesn't add "Headers" for a framework
      SET (JSONC_INCLUDE_DIR ${JSONC_LIBRARY}/Headers CACHE PATH "Path to a file.")
    ENDIF (JSONC_LIBRARY)
    SET (CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK_save} CACHE STRING "" FORCE)
  ENDIF ()
ENDIF (APPLE)

FIND_PATH(JSONC_INCLUDE_DIR json.h
  "$ENV{LIB_DIR}/"
  "$ENV{LIB_DIR}/include/"
  "$ENV{JSONC_ROOT}/"
  /usr/include/json-c
  /usr/local/include/json-c
  #mingw
  c:/msys/local/include/json-c
  NO_DEFAULT_PATH
  )

if(PROJ4_INCLUDE_DIR)
    set(JSON_C_MAJOR_VERSION 0)
    set(JSON_C_MINOR_VERSION 0)
    set(JSON_C_MICRO_VERSION 0)

    if(EXISTS "${JSONC_INCLUDE_DIR}/json_c_version.h")
        file(READ ${JSONC_INCLUDE_DIR}/json_c_version.h _VERSION_H_CONTENTS)

        string(REGEX MATCH "JSON_C_MAJOR_VERSION[ \t]+([0-9]+)"
          JSON_C_MAJOR_VERSION ${_VERSION_H_CONTENTS})
        string (REGEX MATCH "([0-9]+)"
          JSON_C_MAJOR_VERSION ${JSON_C_MAJOR_VERSION})
        string(REGEX MATCH "JSON_C_MINOR_VERSION[ \t]+([0-9]+)"
          JSON_C_MINOR_VERSION ${_VERSION_H_CONTENTS})
        string (REGEX MATCH "([0-9]+)"
          JSON_C_MINOR_VERSION ${JSON_C_MINOR_VERSION})
        string(REGEX MATCH "JSON_C_MICRO_VERSION[ \t]+([0-9]+)"
          JSON_C_MICRO_VERSION ${_VERSION_H_CONTENTS})
        string (REGEX MATCH "([0-9]+)"
          JSON_C_MICRO_VERSION ${JSON_C_MICRO_VERSION})
          
        unset(_VERSION_H_CONTENTS)  
    endif()
    
    set(JSON_C_VERSION_STRING "${JSON_C_MAJOR_VERSION}.${JSON_C_MINOR_VERSION}.${JSON_C_MICRO_VERSION}") 
endif()

FIND_LIBRARY(JSONC_LIBRARY NAMES json-c libjson-c libjson PATHS
  "$ENV{LIB_DIR}/lib"
  "$ENV{JSONC_ROOT}/lib"
  /usr/lib
  /usr/local/lib
  #mingw
  c:/msys/local/lib
)  

# Handle the QUIETLY and REQUIRED arguments and set GEOS_FOUND to TRUE
# if all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JSONC REQUIRED_VARS JSONC_LIBRARY JSONC_INCLUDE_DIR
                                        VERSION_VAR JSON_C_VERSION_STRING)

if(JSONC_FOUND)
  set(JSONC_LIBRARIES ${JSONC_LIBRARY})
  set(JSONC_INCLUDE_DIRS ${JSONC_INCLUDE_DIR})
endif()

# Hide internal variables
mark_as_advanced(JSONC_LIBRARY JSONC_INCLUDE_DIR)
