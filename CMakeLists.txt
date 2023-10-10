cmake_minimum_required(VERSION 3.5)

project(ParallelParallel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed.(build in another folder)"
  )
endif()

add_subdirectory(Library)
add_subdirectory(Application)
