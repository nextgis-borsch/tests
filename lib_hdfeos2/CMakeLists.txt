cmake_minimum_required (VERSION 2.8)
project (hdfeos2_test)

option (BUILD_HDFEOS2_TESTING "Build HDF-EOS2 Unit Testing" ON)
if(BUILD_HDFEOS2_TESTING)

    set(WITH_HDFEOS2 ON CACHE BOOL "set with hdf-eos2 on" FORCE)
    set(WITH_HDFEOS2_EXTERNAL ON CACHE BOOL "set with hdf-eos2 external on" FORCE)
    set(WITH_HDF4 ON CACHE BOOL "set with hdf42 on" FORCE)
    set(WITH_HDF4_EXTERNAL ON CACHE BOOL "set with hdf4 external on" FORCE)

    set(WITH_ZLIB ON CACHE BOOL "set with zlib on" FORCE)
    set(WITH_ZLIB_EXTERNAL ON CACHE BOOL "set with zlib external on" FORCE)
    set(WITH_JPEG ON CACHE BOOL "set with jpeg on" FORCE)
    set(WITH_JPEG_EXTERNAL ON CACHE BOOL "set with jpeg external on" FORCE)
    set(WITH_SZIP ON CACHE BOOL "set with jpeg on" FORCE)
    set(WITH_SZIP_EXTERNAL ON CACHE BOOL "set with jpeg external on" FORCE)


    find_anyproject(HDFEOS2 REQUIRED)
    find_anyproject(HDF4 REQUIRED)
    find_anyproject(ZLIB REQUIRED)
    find_anyproject(JPEG REQUIRED)
    find_anyproject(SZIP REQUIRED)

    include (CheckLibraryExists) 
    check_library_exists (m ceil ""        HAVE_LIBM) 
    if(HAVE_LIBM)
        set(TARGET_LINK_LIB ${TARGET_LINK_LIB} m) 
    endif()

    set (XLATE_UTILITY "xlatefile")

    #-----------------------------------------------------------------------------
    # Define Sources
    #-----------------------------------------------------------------------------
    SET (samples
        xlatefile
        SetupSwath
        DefineFields
        WriteFields
        AppendField
        DefineGDflds
        DefineLevels
        InquireGrid
        InquireSwath
        ReadFields
        ReadGDflds
        ReadLevels
        SetupGrid
        SetupPoint
        SubsetGrid
        SubsetPoint
        SubsetSwath
        UpdateLevels
        WriteGDflds
        WriteLevels
    )

    foreach (sample ${samples})
      add_executable (${sample} ${CMAKE_CURRENT_SOURCE_DIR}/${sample}.c)
      target_link_extlibraries (${sample})
    endforeach ()

    set (POINT_REFERENCE_FILES
      fixedBuoy0.txt
      fixedBuoy1.txt
      fixedBuoy1s.txt
      floatBuoy0.txt
      floatBuoy1.txt
      simple.txt
    )
    foreach (out_file ${POINT_REFERENCE_FILES})
      set (outdest "${CMAKE_CURRENT_BINARY_DIR}/${out_file}")
      add_custom_command (
          TARGET     SetupPoint
          POST_BUILD
          COMMAND    ${XLATE_UTILITY}
          ARGS       ${CMAKE_CURRENT_SOURCE_DIR}/${out_file} ${outdest}
      )
    endforeach ()

    ADD_TEST (
        NAME ${PROJECT_NAME}_swath_demo-clearall-objects
        COMMAND    ${CMAKE_COMMAND} -E remove  SwathFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf
    )
    SET (sd_last_test "swath_demo-clearall-objects")

    SET (swath_demo
        SetupSwath
        DefineFields
        WriteFields
        ReadFields
        InquireSwath
        AppendField
        SubsetSwath
    )
    FOREACH (sample ${swath_demo})
        ADD_TEST (NAME ${PROJECT_NAME}_swath_demo_${sample} COMMAND $<TARGET_FILE:${sample}>)
        IF (NOT "${sd_last_test}" STREQUAL "")
            SET_TESTS_PROPERTIES (${PROJECT_NAME}_swath_demo_${sample} PROPERTIES DEPENDS ${sd_last_test})
        ENDIF (NOT "${sd_last_test}" STREQUAL "")
        SET (sd_last_test "swath_demo_${sample}")
    ENDFOREACH (sample ${swath_demo})

    # Remove any output file left over from previous test run
    ADD_TEST (
        NAME ${PROJECT_NAME}_grid_demo-clearall-objects
        COMMAND    ${CMAKE_COMMAND} -E remove GridFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf
    )
    SET (gd_last_test "grid_demo-clearall-objects")

    SET (grid_demo
        SetupGrid
        DefineGDflds
        WriteGDflds
        ReadGDflds
        InquireGrid
        SubsetGrid
    )
    FOREACH (sample ${grid_demo})
        ADD_TEST (NAME ${PROJECT_NAME}_grid_demo_${sample} COMMAND $<TARGET_FILE:${sample}>)
        IF (NOT "${gd_last_test}" STREQUAL "")
            SET_TESTS_PROPERTIES (${PROJECT_NAME}_grid_demo_${sample} PROPERTIES DEPENDS ${gd_last_test})
        ENDIF (NOT "${gd_last_test}" STREQUAL "")
        SET (gd_last_test "grid_demo_${sample}")
    ENDFOREACH (sample ${grid_demo})

    # Remove any output file left over from previous test run
    ADD_TEST (
        NAME ${PROJECT_NAME}_point_demo-clearall-objects
        COMMAND    ${CMAKE_COMMAND} -E remove  PointFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf
    )
    SET (pd_last_test "point_demo-clearall-objects")

    SET (point_demo
        SetupPoint
        DefineLevels
        WriteLevels
        ReadLevels
    )

    FOREACH (sample ${point_demo})
        ADD_TEST (NAME ${PROJECT_NAME}_point_demo_${sample} COMMAND $<TARGET_FILE:${sample}>)
        IF (NOT "${pd_last_test}" STREQUAL "")
            SET_TESTS_PROPERTIES (${PROJECT_NAME}_point_demo_${sample} PROPERTIES DEPENDS ${pd_last_test})
        ENDIF (NOT "${pd_last_test}" STREQUAL "")
        SET (pd_last_test "point_demo_${sample}")
    ENDFOREACH (sample ${point_demo})
endif()
