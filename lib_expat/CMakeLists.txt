cmake_minimum_required (VERSION 2.8)
project (expat_test)

option (BUILD_EXPAT_TESTING "Build Expat Unit Testing" ON)
if(BUILD_EXPAT_TESTING)

    set(WITH_EXPAT ON CACHE BOOL "set with expat on" FORCE)
    set(WITH_EXPAT_EXTERNAL ON CACHE BOOL "set with expat external on" FORCE)

    include(FindAnyProject)
    find_anyproject(EXPAT REQUIRED)

    add_executable(runtests runtests.c chardata.c minicheck.c)
    target_link_extlibraries(runtests)
    add_test(${PROJECT_NAME}_runtests runtests)

    add_executable(runtestspp runtestspp.cpp chardata.c minicheck.c)
    target_link_extlibraries(runtestspp)
    add_test(${PROJECT_NAME}_runtestspp runtestspp)
endif()
