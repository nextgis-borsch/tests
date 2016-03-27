 #################################################################################
#
# GEOS tests build configuration for CMake build system
#
# Copyright (C) 2009 Mateusz Loskot <mateusz@loskot.net>
#
# This is free software; you can redistribute and/or modify it under
# the terms of the GNU Lesser General Public Licence as published
# by the Free Software Foundation. 
# See the COPYING file for more information.
#
#################################################################################

project (geos_test)

option (BUILD_GEOS_TESTING "Build GEOS Unit Testing" ON)
if(BUILD_GEOS_TESTING)

    set(WITH_GEOS ON CACHE BOOL "set with expat on" FORCE)
    set(WITH_GEOS_EXTERNAL ON CACHE BOOL "set with expat external on" FORCE)

    find_anyproject(GEOS REQUIRED)
    
    add_definitions(-DGEOS_DLL_IMPORT)

    add_subdirectory(unit)
    add_subdirectory(xmltester)
    add_subdirectory(bigtest)
    # TODO: add other test programs

endif()
