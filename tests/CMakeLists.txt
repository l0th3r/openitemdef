file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_custom_target(openitemdef_testdata ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/data"
            "${CMAKE_CURRENT_BINARY_DIR}/data"
    COMMENT "Copied data needed for test"
)

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party/unity/src
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(${TEST_NAME} PRIVATE
        unity
        openitemdef
    )

    if(OPENITEMDEF_ENABLE_COVERAGE)
        openitemdef_enable_coverage_for(${TEST_NAME})
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    set_tests_properties(${TEST_NAME} PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()