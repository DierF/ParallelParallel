cmake_minimum_required(VERSION 3.4)

project(PParallel LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce out-of-source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please build in another folder.")
endif()

# Include FetchContent module
include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Fetch GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Set binary directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Organize source files into groups
file(GLOB_RECURSE CoreFiles "${CMAKE_SOURCE_DIR}/Core/*.cpp" "${CMAKE_SOURCE_DIR}/Core/*.h")
file(GLOB_RECURSE RenderFiles "${CMAKE_SOURCE_DIR}/Render/*.cpp" "${CMAKE_SOURCE_DIR}/Render/*.h")
file(GLOB_RECURSE ApplicationFiles "${CMAKE_SOURCE_DIR}/Application/*.cpp" "${CMAKE_SOURCE_DIR}/Application/*.h")

file(GLOB gladc "Glad/src/gl.c")

source_group("Core" FILES ${CoreFiles})
source_group("Render" FILES ${RenderFiles})
source_group("Application" FILES ${ApplicationFiles})

include_directories(".")

# Create executable
add_executable(PParallel ${CMAKE_SOURCE_DIR}/main.cpp ${CoreFiles} ${RenderFiles} ${ApplicationFiles} ${gladc})

# Link dependencies
target_link_libraries(PParallel PRIVATE glfw glm::glm)

# Include directories
target_include_directories(PParallel PRIVATE
    ${glfw_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Glad/include
    ${glm_SOURCE_DIR}
)

# Set preprocessor definitions
target_compile_definitions(PParallel PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Set executable output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
