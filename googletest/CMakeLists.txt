cmake_minimum_required (VERSION 2.8)
project (gtest_test)

# Tweaks CMake's default compiler/linker settings to suit Google Test's needs.
#
# This must be a macro(), as inside a function string() can only
# update variables in the function scope.
macro(fix_default_compiler_settings_)
  if (MSVC)
    # For MSVC, CMake sets certain flags to defaults we want to override.
    # This replacement code is taken from sample in the CMake Wiki at
    # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if (NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)
        # When Google Test is built as a shared library, it should also use
        # shared runtime libraries.  Otherwise, it may end up with multiple
        # copies of runtime library data in different modules, resulting in
        # hard-to-find crashes. When it is built as a static library, it is
        # preferable to use CRT as static libraries, as we don't have to rely
        # on CRT DLLs being available. CMake always defaults to using shared
        # CRT libraries, so we override that default here.
        string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
      endif()

      # We prefer more strict warning checking for building Google Test.
      # Replaces /W3 with /W4 in defaults.
      string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

# cxx_executable_with_flags(name cxx_flags srcs...)
#
# creates a named C++ executable that depends on the given libraries and
# is built from the given source files with the given compiler flags.
function(cxx_executable_with_flags name cxx_flags libs)
  add_executable(${name} ${ARGN})
  if (cxx_flags)
    set_target_properties(${name}
      PROPERTIES
      COMPILE_FLAGS "${cxx_flags}")
  endif()
  if (BUILD_SHARED_LIBS)
    set_target_properties(${name}
      PROPERTIES
      COMPILE_DEFINITIONS "GTEST_LINKED_AS_SHARED_LIBRARY=1")
  endif()
  # To support mixing linking in static and dynamic libraries, link each
  # library in with an extra call to target_link_libraries.
 
    if(DEPENDENCY_LIB)
        add_dependencies(${name} ${DEPENDENCY_LIB})  
    endif()    
    if(TARGET_LINK_LIB)
        #list(REMOVE_DUPLICATES TARGET_LINK_LIB) debug;...;optimised;... etc. if filter out
        target_link_libraries(${name} ${libs})
    endif()
endfunction()

# cxx_executable(name dir lib srcs...)
#
# creates a named target that depends on the given libs and is built
# from the given source files.  dir/name.cc is implicitly included in
# the source file list.
function(cxx_executable name dir libs)
  cxx_executable_with_flags(
    ${name} "${cxx_default}" "${libs}" "${dir}/${name}.cc" ${ARGN})
endfunction()

# cxx_test_with_flags(name cxx_flags libs srcs...)
#
# creates a named C++ test that depends on the given libs and is built
# from the given source files with the given compiler flags.
function(cxx_test_with_flags name cxx_flags libs)
  cxx_executable_with_flags(${name} "${cxx_flags}" "${libs}" ${ARGN})
  add_test(${name} ${name})
endfunction()

# cxx_test(name libs srcs...)
#
# creates a named test target that depends on the given libs and is
# built from the given source files.  Unlike cxx_test_with_flags,
# test/name.cc is already implicitly included in the source file list.
function(cxx_test name libs)
  cxx_test_with_flags("${name}" "${cxx_default}" "${libs}"
    "test/${name}.cc" ${ARGN})
endfunction()


# Defines the compiler/linker flags used to build Google Test and
# Google Mock.  You can tweak these definitions to suit your need.  A
# variable's value is empty before it's explicitly assigned to.
macro(config_compiler_and_linker)
  fix_default_compiler_settings_()
  if (MSVC)
    # Newlines inside flags variables break CMake's NMake generator.
    # TODO(vladl@google.com): Add -RTCs and -RTCu to debug builds.
    set(cxx_base_flags "-GS -W4 -WX -wd4251 -wd4275 -nologo -J -Zi")
    if (MSVC_VERSION LESS 1400)  # 1400 is Visual Studio 2005
      # Suppress spurious warnings MSVC 7.1 sometimes issues.
      # Forcing value to bool.
      set(cxx_base_flags "${cxx_base_flags} -wd4800")
      # Copy constructor and assignment operator could not be generated.
      set(cxx_base_flags "${cxx_base_flags} -wd4511 -wd4512")
      # Compatibility warnings not applicable to Google Test.
      # Resolved overload was found by argument-dependent lookup.
      set(cxx_base_flags "${cxx_base_flags} -wd4675")
    endif()
    if (MSVC_VERSION LESS 1500)  # 1500 is Visual Studio 2008
      # Conditional expression is constant.
      # When compiling with /W4, we get several instances of C4127
      # (Conditional expression is constant). In our code, we disable that
      # warning on a case-by-case basis. However, on Visual Studio 2005,
      # the warning fires on std::list. Therefore on that compiler and earlier,
      # we disable the warning project-wide.
      set(cxx_base_flags "${cxx_base_flags} -wd4127")
    endif()
    if (NOT (MSVC_VERSION LESS 1700))  # 1700 is Visual Studio 2012.
      # Suppress "unreachable code" warning on VS 2012 and later.
      # http://stackoverflow.com/questions/3232669 explains the issue.
      set(cxx_base_flags "${cxx_base_flags} -wd4702")
    endif()

    set(cxx_base_flags "${cxx_base_flags} -D_UNICODE -DUNICODE -DWIN32 -D_WIN32")
    set(cxx_base_flags "${cxx_base_flags} -DSTRICT -DWIN32_LEAN_AND_MEAN")
    set(cxx_exception_flags "-EHsc -D_HAS_EXCEPTIONS=1")
    set(cxx_no_exception_flags "-D_HAS_EXCEPTIONS=0")
    set(cxx_no_rtti_flags "-GR-")
  elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(cxx_base_flags "-Wall -Wshadow -fPIC")
    set(cxx_exception_flags "-fexceptions")
    set(cxx_no_exception_flags "-fno-exceptions")
    # Until version 4.3.2, GCC doesn't define a macro to indicate
    # whether RTTI is enabled.  Therefore we define GTEST_HAS_RTTI
    # explicitly.
    set(cxx_no_rtti_flags "-fno-rtti -DGTEST_HAS_RTTI=0")
    set(cxx_strict_flags
      "-Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
    set(cxx_exception_flags "-features=except")
    # Sun Pro doesn't provide macros to indicate whether exceptions and
    # RTTI are enabled, so we define GTEST_HAS_* explicitly.
    set(cxx_no_exception_flags "-features=no%except -DGTEST_HAS_EXCEPTIONS=0")
    set(cxx_no_rtti_flags "-features=no%rtti -DGTEST_HAS_RTTI=0")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "VisualAge" OR
      CMAKE_CXX_COMPILER_ID STREQUAL "XL")
    # CMake 2.8 changes Visual Age's compiler ID to "XL".
    set(cxx_exception_flags "-qeh")
    set(cxx_no_exception_flags "-qnoeh")
    # Until version 9.0, Visual Age doesn't define a macro to indicate
    # whether RTTI is enabled.  Therefore we define GTEST_HAS_RTTI
    # explicitly.
    set(cxx_no_rtti_flags "-qnortti -DGTEST_HAS_RTTI=0")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "HP")
    set(cxx_base_flags "-AA -mt")
    set(cxx_exception_flags "-DGTEST_HAS_EXCEPTIONS=1")
    set(cxx_no_exception_flags "+noeh -DGTEST_HAS_EXCEPTIONS=0")
    # RTTI can not be disabled in HP aCC compiler.
    set(cxx_no_rtti_flags "")
  endif()

  if (CMAKE_USE_PTHREADS_INIT)  # The pthreads library is available and allowed.
    set(cxx_base_flags "${cxx_base_flags} -DGTEST_HAS_PTHREAD=1")
  else()
    set(cxx_base_flags "${cxx_base_flags} -DGTEST_HAS_PTHREAD=0")
  endif()

  # For building gtest's own tests and samples.
  set(cxx_exception "${CMAKE_CXX_FLAGS} ${cxx_base_flags} ${cxx_exception_flags}")
  set(cxx_no_exception
    "${CMAKE_CXX_FLAGS} ${cxx_base_flags} ${cxx_no_exception_flags}")
  set(cxx_default "${cxx_exception}")
  set(cxx_no_rtti "${cxx_default} ${cxx_no_rtti_flags}")
  set(cxx_use_own_tuple "${cxx_default} -DGTEST_USE_OWN_TR1_TUPLE=1")

  # For building the gtest libraries.
  set(cxx_strict "${cxx_default} ${cxx_strict_flags}")
endmacro()

# Defines the gtest & gtest_main libraries.  User tests should link
# with one of them.
function(cxx_library_with_type name type cxx_flags)
  # type can be either STATIC or SHARED to denote a static or shared library.
  # ARGN refers to additional arguments after 'cxx_flags'.
  add_library(${name} ${type} ${ARGN})
  set_target_properties(${name}
    PROPERTIES
    COMPILE_FLAGS "${cxx_flags}")
  if (BUILD_SHARED_LIBS OR type STREQUAL "SHARED")
    set_target_properties(${name}
      PROPERTIES
      COMPILE_DEFINITIONS "GTEST_CREATE_SHARED_LIBRARY=1")
  endif()
  if (CMAKE_USE_PTHREADS_INIT)
    target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
  endif()
endfunction()

########################################################################
#
# Helper functions for creating build targets.

function(cxx_shared_library name cxx_flags)
  cxx_library_with_type(${name} SHARED "${cxx_flags}" ${ARGN})
endfunction()

function(cxx_library name cxx_flags)
  cxx_library_with_type(${name} "" "${cxx_flags}" ${ARGN})
endfunction()

# py_test(name)
#
# creates a Python test with the given name whose main module is in
# test/name.py.  It does nothing if Python is not installed.
function(py_test name)
  # We are not supporting Python tests on Linux yet as they consider
  # all Linux environments to be google3 and try to use google3 features.
  if (PYTHONINTERP_FOUND)
    # ${CMAKE_BINARY_DIR} is known at configuration time, so we can
    # directly bind it from cmake. ${CTEST_CONFIGURATION_TYPE} is known
    # only at ctest runtime (by calling ctest -c <Configuration>), so
    # we have to escape $ to delay variable substitution here.
    add_test(${name}
      ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/${name}.py
          --build_dir=${CMAKE_CURRENT_BINARY_DIR}/\${CTEST_CONFIGURATION_TYPE})
  endif()
endfunction()

option (BUILD_GTEST_TESTING "Build Gtest Unit Testing" ON)
if(BUILD_GTEST_TESTING)
  set(WITH_GTest ON CACHE BOOL "set with gtest on" FORCE)
  set(WITH_GTest_EXTERNAL ON CACHE BOOL "set with gtest external on" FORCE)
  
  find_anyproject(GTest REQUIRED)
  find_package(PythonInterp)
  
  if (NOT gtest_disable_pthreads)
    # Defines CMAKE_USE_PTHREADS_INIT and CMAKE_THREAD_LIBS_INIT.
    find_package(Threads)
    set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${CMAKE_THREAD_LIBS_INIT})
  endif()
  
  config_compiler_and_linker()    
  ############################################################
  # C++ tests built with standard compiler flags.

  cxx_test(gtest-death-test_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_environment_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-filepath_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-linked_ptr_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-listener_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_main_unittest "${TARGET_LINK_LIB}")
  cxx_test(gtest-message_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_no_test_unittest "${TARGET_LINK_LIB}")
  cxx_test(gtest-options_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-param-test_test  "${TARGET_LINK_LIB}" 
    test/gtest-param-test2_test.cc)
  cxx_test(gtest-port_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_pred_impl_unittest  "${TARGET_LINK_LIB}")
  cxx_test(gtest_premature_exit_test  "${TARGET_LINK_LIB}"
    test/gtest_premature_exit_test.cc)
  cxx_test(gtest-printers_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_prod_test "${TARGET_LINK_LIB}"
    test/production.cc)
  cxx_test(gtest_repeat_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_sole_header_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_stress_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-test-part_test "${TARGET_LINK_LIB}")
  cxx_test(gtest_throw_on_failure_ex_test "${TARGET_LINK_LIB}")
  cxx_test(gtest-typed-test_test "${TARGET_LINK_LIB}"
    test/gtest-typed-test2_test.cc)
  cxx_test(gtest_unittest "${TARGET_LINK_LIB}")
  cxx_test(gtest-unittest-api_test "${TARGET_LINK_LIB}")

  ############################################################
  # C++ tests built with non-standard compiler flags.
  
  set(SRC_PATH "${CMAKE_BINARY_DIR}/third-party/Source/GTest_EP")
  include_directories(
      ${SRC_PATH}/include
      ${SRC_PATH})
  link_directories(${CMAKE_BINARY_DIR}/third-party/Build/GTest_EP/src)    
  
  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)
    cxx_library(gtest_no_exception "${cxx_no_exception}"
      ${SRC_PATH}/src/gtest-all.cc)
    cxx_library(gtest_main_no_exception "${cxx_no_exception}"
      ${SRC_PATH}/src/gtest-all.cc ${SRC_PATH}/src/gtest_main.cc)
  endif()
  cxx_library(gtest_main_no_rtti "${cxx_no_rtti}"
    ${SRC_PATH}/src/gtest-all.cc ${SRC_PATH}/src/gtest_main.cc)

  cxx_test_with_flags(gtest-death-test_ex_nocatch_test
    "${cxx_exception} -DGTEST_ENABLE_CATCH_EXCEPTIONS_=0"
    gtest test/gtest-death-test_ex_test.cc)
  cxx_test_with_flags(gtest-death-test_ex_catch_test
    "${cxx_exception} -DGTEST_ENABLE_CATCH_EXCEPTIONS_=1"
    gtest test/gtest-death-test_ex_test.cc)

  cxx_test_with_flags(gtest_no_rtti_unittest "${cxx_no_rtti}"
    gtest_main_no_rtti test/gtest_unittest.cc)

  cxx_shared_library(gtest_dll "${cxx_default}"
    ${SRC_PATH}/src/gtest-all.cc ${SRC_PATH}/src/gtest_main.cc)

  cxx_executable_with_flags(gtest_dll_test_ "${cxx_default}"
    gtest_dll test/gtest_all_test.cc)
  set_target_properties(gtest_dll_test_
                        PROPERTIES
                        COMPILE_DEFINITIONS "GTEST_LINKED_AS_SHARED_LIBRARY=1")

  if (NOT MSVC OR MSVC_VERSION LESS 1600)  # 1600 is Visual Studio 2010.
    # Visual Studio 2010, 2012, and 2013 define symbols in std::tr1 that
    # conflict with our own definitions. Therefore using our own tuple does not
    # work on those compilers.
    cxx_library(gtest_main_use_own_tuple "${cxx_use_own_tuple}"
      ${SRC_PATH}/src/gtest-all.cc ${SRC_PATH}/src/gtest_main.cc)

    cxx_test_with_flags(gtest-tuple_test "${cxx_use_own_tuple}"
      "gtest_main_use_own_tuple;${CMAKE_THREAD_LIBS_INIT}" test/gtest-tuple_test.cc)

    cxx_test_with_flags(gtest_use_own_tuple_test "${cxx_use_own_tuple}"
      "gtest_main_use_own_tuple;${CMAKE_THREAD_LIBS_INIT}"
      test/gtest-param-test_test.cc test/gtest-param-test2_test.cc)
  endif()

  ############################################################
  # Python tests.

  cxx_executable(gtest_break_on_failure_unittest_ test "${TARGET_LINK_LIB}")
  py_test(gtest_break_on_failure_unittest)

  # Visual Studio .NET 2003 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)  # 1310 is Visual Studio .NET 2003
    cxx_executable_with_flags(
      gtest_catch_exceptions_no_ex_test_
      "${cxx_no_exception}" gtest_main_no_exception     
      test/gtest_catch_exceptions_test_.cc)
  endif()

  cxx_executable_with_flags(
    gtest_catch_exceptions_ex_test_
    "${cxx_exception}" "${TARGET_LINK_LIB}"
    test/gtest_catch_exceptions_test_.cc)
  py_test(gtest_catch_exceptions_test)

  cxx_executable(gtest_color_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_color_test)

  cxx_executable(gtest_env_var_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_env_var_test)

  cxx_executable(gtest_filter_unittest_ test "${TARGET_LINK_LIB}")
  py_test(gtest_filter_unittest)

  cxx_executable(gtest_help_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_help_test)

  cxx_executable(gtest_list_tests_unittest_ test "${TARGET_LINK_LIB}")
  py_test(gtest_list_tests_unittest)

  cxx_executable(gtest_output_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_output_test)

  cxx_executable(gtest_shuffle_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_shuffle_test)

  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)
    cxx_executable(gtest_throw_on_failure_test_ test gtest_no_exception)
    set_target_properties(gtest_throw_on_failure_test_
      PROPERTIES
      COMPILE_FLAGS "${cxx_no_exception}")
    py_test(gtest_throw_on_failure_test)
  endif()

  cxx_executable(gtest_uninitialized_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_uninitialized_test)

  cxx_executable(gtest_xml_outfile1_test_ test "${TARGET_LINK_LIB}")
  cxx_executable(gtest_xml_outfile2_test_ test "${TARGET_LINK_LIB}")
  py_test(gtest_xml_outfiles_test)

  cxx_executable(gtest_xml_output_unittest_ test "${TARGET_LINK_LIB}")
  py_test(gtest_xml_output_unittest)
    
endif()    
