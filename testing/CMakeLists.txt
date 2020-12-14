file(GLOB_RECURSE TESTING_HEADERS include/*.h)
file(GLOB_RECURSE TESTING_SOURCES source/*.c)

add_library(testing ${TESTING_SOURCES})
target_include_directories(
  testing
  PUBLIC include
  PRIVATE source)
