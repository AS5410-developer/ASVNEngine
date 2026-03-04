find_package(Vulkan REQUIRED)

add_library(VideoVk MODULE Entry.cpp)

generate_export_header(VideoVk
    EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/VideoVkExport.h"
    BASE_NAME VIDEO_VK 
)

target_link_libraries(VideoVk PRIVATE Vulkan::Vulkan)