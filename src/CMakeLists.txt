add_subdirectory(Engine)
add_subdirectory(Launcher)
add_subdirectory(Client)
add_subdirectory(Platform/SDL3)
add_subdirectory(VulkanRender)

link_libraries(Engine)