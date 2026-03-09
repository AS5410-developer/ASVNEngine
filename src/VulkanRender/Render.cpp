#include <VulkanRender/Image.hpp>
#include <VulkanRender/Render.hpp>
#include <VulkanRender/Shader.hpp>
#include <VulkanRender/Sprite.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace AS::Engine;

IEngine* Render::EngineInstance = 0;
AS::Engine::IEngine* MainEngine = 0;
VkInstance MainInstance = 0;

void Render::OnLoaded() {
  MainEngine = EngineInstance;

  std::vector<char*> layers;
  std::vector<char*> extensions;
#ifdef DEBUG
  layers.push_back("VK_LAYER_KHRONOS_validation");
  extensions.push_back(const_cast<char*>(VK_EXT_DEBUG_UTILS_EXTENSION_NAME));
#endif
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
  auto ret = vkCreateInstance(&icInfo, nullptr, &MainInstance);
  if (ret == VK_ERROR_EXTENSION_NOT_PRESENT) {
    EngineInstance->GetConsole() << "Fucking extensions!" << EndLine;
    return;
  }

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

  SpriteQuad = VertexBuffer(MainDevice, {{{-1.0f, -1.0f}, {0, 1}},
                                         {{1.0f, -1.0f}, {1, 1}},
                                         {{1.0f, 1.0f}, {1, 0}},
                                         {{-1.0f, -1.0f}, {0, 1}},
                                         {{1.0f, 1.0f}, {1, 0}},
                                         {{-1.0f, 1.0f}, {0, 0}}});

  ShaderParams = new ShaderData[8196];
  Buffer = SSBO(MainDevice, static_cast<uint32_t>(sizeof(ShaderData)), 8196);

  float aspect = MainSwapchain.GetCurrentSize().width /
                 (float)MainSwapchain.GetCurrentSize().height;
  Proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

  View = glm::mat4(1);
  View = glm::translate(View, glm::vec3(0, 0, 0));
}
void Render::OnRegisterOptions() {}
void Render::OnUpdate() {}
void Render::OnTick() {}
void Render::OnEnabled() {}
void Render::OnDisabled() {}

IImage* Render::CreateImage() {
  auto img = new EImage;
  img->SetInitData(MainDevice, MainCommandBuffer);
  return img;
}
ISprite* Render::CreateSprite(IImage* image, IShader* shader,
                              Transform& transform) {
  Sprite* sprite = 0;
  if (!FreeShaderParamsId.empty()) {
    sprite = new Sprite(FreeShaderParamsId[0]);
    FreeShaderParamsId.erase(FreeShaderParamsId.begin());
  } else {
    sprite = new Sprite(ShaderParamsCount);
    ++ShaderParamsCount;
  }
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

void Render::BeginDraw() {
  MainCommandBuffer.WaitDeviceIdle();

  if (MainSwapchain.GetCurrentSize().width !=
          EngineInstance->GetMainWindow()->GetSize().x ||
      MainSwapchain.GetCurrentSize().height !=
          EngineInstance->GetMainWindow()->GetSize().y) {
    MainSwapchain.Release();
    MainSwapchain.Create();
    MainCommandBuffer.SetSwapchain(MainSwapchain);
    float aspect = MainSwapchain.GetCurrentSize().width /
                   (float)MainSwapchain.GetCurrentSize().height;
    Proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
  }

  MainCommandBuffer.StartDraw();
  if (CurrentShader)
    MainCommandBuffer.SetCurrentShader(*CurrentShader->GetShader());
}
void Render::BindShader(IShader& shader) {
  auto sh = dynamic_cast<EShader&>(shader);
  MainCommandBuffer.SetCurrentShader(*sh.GetShader());
  CurrentShader = &reinterpret_cast<EShader&>(shader);
}
void Render::DrawSprite(ISprite* sprite) {
  if (!sprite) return;
  auto sh = dynamic_cast<EShader*>(sprite->GetShader());
  if (sh != CurrentShader) BindShader(*sh);
  ShaderParams[dynamic_cast<const Sprite*>(sprite)->GetBufferID()] = {
      Proj, View, sprite->GetTransform().GetTransformMatrix()};
  float x = sprite->GetTransform().GetTransformMatrix()[1][1];
  Buffer.SetData(ShaderParams, 8196);
  Buffer.Update();
  auto img = reinterpret_cast<EImage*>(sprite->GetImage());
  if (!img->IsLoaded()) return;
  img->GetImage().BindToShader(*CurrentShader->GetShader());

  MainCommandBuffer.DrawVertexNotIndexedBuffer(
      SpriteQuad, dynamic_cast<const Sprite*>(sprite)->GetBufferID());
}
void Render::EndDraw() { MainCommandBuffer.EndDraw(); }