set(TEST_MAIN_NAME test_main)
file(GLOB TEST_SOURCES *.c)
set(TEST_MAIN_SOURCES internal/main.c ${TEST_SOURCES})

add_executable(${TEST_MAIN_NAME} ${TEST_MAIN_SOURCES})
target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME} testing)

set_property(TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
assign_source_group(${TEST_MAIN_SOURCES})
add_test(${TEST_MAIN_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_MAIN_NAME})
