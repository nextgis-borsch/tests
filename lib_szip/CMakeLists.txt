cmake_minimum_required (VERSION 2.8)
project (szip_test)

set(WITH_SZIP ON CACHE BOOL "set with szip on" FORCE)
set(WITH_SZIP_EXTERNAL ON CACHE BOOL "set with szip external on" FORCE)

include(find_anyproject)
find_anyproject(SZIP REQUIRED)


#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set (TEST_LIB_SRCS
    mcgill.c
)

set (TEST_LIB_HEADERS
    mcgill.h
)

SET (SZIP_TEST_LIB_TARGET         "mcgill")

add_library (${SZIP_TEST_LIB_TARGET} ${LIB_TYPE} ${TEST_LIB_SRCS} ${TEST_LIB_HEADERS})
target_link_extlibraries (${SZIP_TEST_LIB_TARGET})

set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${SZIP_TEST_LIB_TARGET}) 

add_executable (gentest gentest.c)
if (WIN32 AND NOT MINGW)
if (BUILD_SHARED_LIBS)
  set_target_properties (gentest PROPERTIES OUTPUT_NAME "gentestdll")
endif ()
endif ()  
if (WIN32)
if (MSVC)
  if (NOT BUILD_SHARED_LIBS)
    set_target_properties (gentest
        PROPERTIES
            LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
    ) 
  endif ()
endif ()
endif ()
target_link_extlibraries (gentest)

add_test (NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:gentest>)
