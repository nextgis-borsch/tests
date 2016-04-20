cmake_minimum_required (VERSION 2.8)
project (jsonc_test)

option (BUILD_JSONC_TESTING "Build PNG Unit Testing" ON)
if(BUILD_JSONC_TESTING)

    set(WITH_JSONC ON CACHE BOOL "set with json-c on" FORCE)
    set(WITH_JSONC_EXTERNAL ON CACHE BOOL "set with json-c external on" FORCE)

    include (CheckIncludeFile)
    check_include_file("locale.h" HAVE_LOCALE_H)
    if(HAVE_LOCALE_H)
        add_definitions(-DHAVE_LOCALE_H -DHAVE_SETLOCALE)
    endif()    

    find_anyproject(JSONC REQUIRED)

    macro(ADD_JSONC_TEST name)
        add_executable(${name} ${name}.c parse_flags.c)
        target_link_extlibraries (${name})
        add_test(NAME jsonc_${name} 
                 COMMAND ${CMAKE_COMMAND}
                 -DTEST_PROG=$<TARGET_FILE:${name}>
                 -DTEST_NAME=${name}
                 -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
                 -DSOURCES_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                 -P ${CMAKE_SOURCE_DIR}/cmake/TestCompareFiles.cmake)
    endmacro()

    add_jsonc_test(test1)
    add_jsonc_test(test2)
    add_jsonc_test(test4)
    add_jsonc_test(testReplaceExisting)
    add_jsonc_test(test_parse_int64)
    add_jsonc_test(test_null)
    add_jsonc_test(test_cast)
    add_jsonc_test(test_parse)
    add_jsonc_test(test_locale)
    add_jsonc_test(test_charcase)
    add_jsonc_test(test_printbuf)
    add_jsonc_test(test_set_serializer)
endif()    
