#include <VulkanRender/Image.hpp>
#include <VulkanRender/Render.hpp>
#include <VulkanRender/Shader.hpp>
#include <VulkanRender/Sprite.hpp>
using namespace AS::Engine;

IEngine* Render::EngineInstance = 0;
AS::Engine::IEngine* MainEngine = 0;
VkInstance MainInstance = 0;

void Render::OnLoaded() {
  MainEngine = EngineInstance;

  std::vector<char*> layers;
  std::vector<char*> extensions;
#ifdef _DEBUG
  layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
  extensions.push_back(const_cast<char*>(VK_KHR_SURFACE_EXTENSION_NAME));
  extensions.push_back(const_cast<char*>(VK_EXT_DEBUG_UTILS_EXTENSION_NAME));

  auto exts = EngineInstance->GetPlatform().GetExtensions();

  extensions.insert(extensions.end(), exts.begin(), exts.end());

  constexpr VkApplicationInfo aInfo{
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = nullptr,
      .pApplicationName = "ASVN Application",
      .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
      .pEngineName = "ASVN Engine",
      .engineVersion = VK_MAKE_VERSION(0, 1, 0),
      .apiVersion = VK_API_VERSION_1_4,
  };

  VkInstanceCreateInfo icInfo{
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .pApplicationInfo = &aInfo,
      .enabledLayerCount = static_cast<unsigned int>(layers.size()),
      .ppEnabledLayerNames = layers.data(),
      .enabledExtensionCount = static_cast<unsigned int>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data(),
  };
  vkCreateInstance(&icInfo, nullptr, &MainInstance);

  if (!MainInstance) {
    EngineInstance->GetConsole()
        << "Error on creating vulkan instance!" << EndLine;
    return;
  }

  PhysicalDeviceManager devManager = PhysicalDeviceManager::Create();
  MainPhysicalDevice = devManager.GetBestPhysicalDevice();

  auto surf = MainEngine->GetMainWindow()->GetSurface(MainInstance);
  if (surf.Failed()) {
    MainEngine->GetConsole()
        << "Failed to get surface from main window: " << surf.What() << EndLine;
    return;
  }
  MainDevice =
      Device(MainPhysicalDevice, MainPhysicalDevice.GetGraphicsQueueFamily(),
             reinterpret_cast<VkSurfaceKHR>(surf.GetResult()));

  MainSwapchain =
      Swapchain(MainDevice, reinterpret_cast<VkSurfaceKHR>(surf.GetResult()));
  auto mainPool = new CommandPool(MainDevice, MainDevice.GetGraphicsID());
  MainCommandBuffer = CommandBuffer(*mainPool, MainSwapchain);

  SpriteQuad = VertexBuffer(MainDevice, {{{-1, -1}, {0, 0}},
                                         {{-1, 1}, {0, 1}},
                                         {{1, 1}, {1, 1}},
                                         {{-1, -1}, {0, 0}},
                                         {{1, -1}, {1, 0}},
                                         {{1, 1}, {1, 1}}});

  ShaderParams.resize(8196);
  Buffer = SSBO(MainDevice, static_cast<uint32_t>(sizeof(ShaderData)),
                ShaderParams.size());
}
void Render::OnRegisterOptions() {}
void Render::OnUpdate() {}
void Render::OnTick() {}
void Render::OnEnabled() {}
void Render::OnDisabled() {}

IImage* Render::CreateImage() { return new EImage; }
ISprite* Render::CreateSprite(IImage* image, IShader* shader,
                              Transform& transform) {
  auto sprite = new Sprite();
  sprite->SetImage(image);
  sprite->SetShader(shader);
  sprite->SetTransform(transform);
  return sprite;
}
IShader* Render::CreateShader(const std::string& shader) {
  EShader* shaderObj = new EShader;
  shaderObj->SetInitData(MainDevice, MainSwapchain);
  shaderObj->SetShaderData(shader.c_str());
  Buffer.BindToShader(*shaderObj->GetShader());
  return shaderObj;
}

void Render::BeginDraw() { MainCommandBuffer.StartDraw(); }
void Render::BindShader(IShader& shader) {
  auto sh = dynamic_cast<EShader&>(shader);
  MainCommandBuffer.SetCurrentShader(*sh.GetShader());
  CurrentShader = &shader;
}
void Render::DrawSprite(ISprite* sprite) {
  if (!sprite) return;
  auto sh = dynamic_cast<EShader*>(sprite->GetShader());
  if (sh != CurrentShader) BindShader(*sh);
  MainCommandBuffer.DrawVertexNotIndexedBuffer(
      SpriteQuad, dynamic_cast<const Sprite*>(sprite)->GetBufferID());
}
void Render::EndDraw() { MainCommandBuffer.EndDraw(); }