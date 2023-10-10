set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ALL_FILES "*.*")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALL_FILES})

add_executable(Application ${ALL_FILES})

find_package(Vulkan REQUIRED)

target_link_libraries(Application PUBLIC glfw glm)
target_link_libraries(Application PRIVATE Vulkan::Vulkan)
