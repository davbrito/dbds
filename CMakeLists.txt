if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# Project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(
  dbds
  DESCRIPTION "A library of basic data structures."
  VERSION 0.1.0
  LANGUAGES C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include(set_max_warning_level)
set_max_warning_level()

# Options
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." ON)

# Sources
file(GLOB_RECURSE PROJECT_HEADERS include/*.h)
file(GLOB_RECURSE PROJECT_SOURCES source/*.c)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set(PROJECT_EXPORT_H include/${PROJECT_NAME}/export.h)
set(PROJECT_FILES ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_CMAKE_UTILS}
                  ${PROJECT_MISC} ${PROJECT_EXPORT_HPP})

include(assign_source_group)
assign_source_group(${PROJECT_FILES})

# Dependencies

# Targets
add_library(${PROJECT_NAME} ${PROJECT_FILES})
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:include>
  PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME}
                           PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

if(NOT BUILD_SHARED_LIBS)
  string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME}
                        PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

# Postbuild
include(GenerateExportHeader)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(
  ${PROJECT_NAME}
  EXPORT_FILE_NAME
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/export.h
  EXPORT_MACRO_NAME
  ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE
  ${PROJECT_NAME_UPPER}_STATIC)

# Testing
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(testing)
  add_subdirectory(tests)
endif()

# Installation  #
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-config
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT ${PROJECT_NAME}-config DESTINATION cmake)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}-config.cmake)
