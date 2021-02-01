option(ENABLE_COVERAGE_REPORT "Enables gcc coverage reporting." ON)

set(TEST_MAIN_NAME test_main)
file(GLOB TEST_SOURCES *.c)
set(TEST_MAIN_SOURCES internal/main.c ${TEST_SOURCES})

add_executable(${TEST_MAIN_NAME} ${TEST_MAIN_SOURCES})
target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME} testing)

set_property(TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
assign_source_group(${TEST_MAIN_SOURCES})
add_test(${TEST_MAIN_NAME} ${CMAKE_BINARY_DIR}/bin/${TEST_MAIN_NAME})

# Enable coverage report
if(NOT MSVC AND ENABLE_COVERAGE_REPORT)
  target_compile_options(
    ${TEST_MAIN_NAME}
    PUBLIC -g
           -Wall
           -W
           -Wshadow
           -Wunused-variable
           -Wunused-parameter
           -Wunused-function
           -Wunused
           -Wno-system-headers
           -Wno-deprecated
           -Wwrite-strings
           -O0
           -fprofile-arcs
           -ftest-coverage)

  target_link_options(${TEST_MAIN_NAME} PUBLIC -g -O0 -fprofile-arcs
                      -ftest-coverage)
endif()
