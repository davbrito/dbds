enable_testing()

add_subdirectory(testing)

set(TEST_MAIN_NAME test_main)
file(GLOB_RECURSE TEST_HEADERS *.h)
file(GLOB_RECURSE TEST_SOURCES *.c)
set(TEST_MAIN_SOURCES main.c ${TEST_HEADERS} ${TEST_SOURCES})

add_executable(${TEST_MAIN_NAME} ${TEST_MAIN_SOURCES})
target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME} testing)
set_property(TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
assign_source_group(${TEST_MAIN_SOURCES})
add_test(${TEST_MAIN_NAME} bin/${TEST_MAIN_NAME})
