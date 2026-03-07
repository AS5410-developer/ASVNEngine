find_package(Vulkan REQUIRED)

file(GLOB VulkanSources VulkanAPI/*.cpp)
add_library(VideoVk MODULE Entry.cpp ${VulkanSources} Render.cpp Shader.cpp Image.cpp)
target_include_directories(VideoVk PRIVATE VulkanAPI)

generate_export_header(VideoVk
    EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/VideoVkExport.h"
    BASE_NAME VIDEO_VK 
)

target_link_libraries(VideoVk PRIVATE Vulkan::Vulkan)