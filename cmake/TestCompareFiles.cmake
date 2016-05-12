
execute_process(COMMAND ${TEST_PROG} 
                RESULT_VARIABLE HAD_ERROR
                OUTPUT_FILE ${BINARY_DIR}/${TEST_NAME}.current)
if(HAD_ERROR)
    message(FATAL_ERROR "Test ${TEST_NAME} failed")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files 
    ${BINARY_DIR}/${TEST_NAME}.current    
    ${SOURCES_DIR}/${TEST_NAME}.expected
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test ${TEST_NAME} failed - files differ")
endif()