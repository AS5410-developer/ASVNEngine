find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

qt_standard_project_setup()

qt_add_library(Client SHARED Client.cpp Entry.cpp)

generate_export_header(Client
    EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/ClientExport.h"
    BASE_NAME CLIENT
)

target_include_directories(Client PRIVATE ${CMAKE_BINARY_DIR} include)

qt_add_qml_module(Client
    URI AS
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml/
    QML_FILES
        Screens/Console.qml
    SOURCES
        QEngineViewport.cpp)


target_link_libraries(Client PRIVATE Qt6::Core Qt6::Quick Qt6::Qml)