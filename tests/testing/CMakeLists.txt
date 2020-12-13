add_library(testing ${CMAKE_CURRENT_SOURCE_DIR}/test.c)
target_include_directories(testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
