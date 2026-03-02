
add_library(Client MODULE Client.cpp Entry.cpp)

generate_export_header(Client
    EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/ClientExport.h"
    BASE_NAME CLIENT
)