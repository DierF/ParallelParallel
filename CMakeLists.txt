cmake_minimum_required(VERSION 3.5)

project(PParallel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if (NOT WIN32)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed.(build in another folder)"
  )
endif()

file(GLOB_RECURSE CoreFiles "Core/*")
file(GLOB_RECURSE RenderFiles "Render/*")
file(GLOB_RECURSE ApplicationFiles "Application/*")
file(GLOB gladc "Library/glad/src/glad.c")

source_group("Core" FILES ${CoreFiles})
source_group("Render" FILES ${RenderFiles})
source_group("Application" FILES ${ApplicationFiles})

include_directories(".")

add_executable(PParallel main.cpp ${CoreFiles} ${RenderFiles} ${ApplicationFiles} ${gladc})

target_link_libraries(PParallel PUBLIC glfw glm)

target_include_directories(PParallel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Library/glad/include)

add_subdirectory(Library)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)