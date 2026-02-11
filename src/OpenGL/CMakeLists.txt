find_package(WebP)

add_library(OpenGL MODULE Render.cpp Entry.cpp Image.cpp Shader.cpp gl.c Sprite.cpp)

generate_export_header(OpenGL
    EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/OpenGLExport.h"
    BASE_NAME OPENGL
)

target_include_directories(OpenGL PRIVATE ${CMAKE_BINARY_DIR} ${WebP_INCLUDE_DIRS})

target_link_libraries(OpenGL PRIVATE ${WebP_LIBRARIES})