#include <Device.hpp>

Device::Device() {}
Device::Device(PhysicalDevice pdev, unsigned int queueFamily,
               vk::raii::SurfaceKHR* surface)
    : PDev(pdev), CurrentQueueFamily(queueFamily), Surface(surface) {
  Create();
}
Device::Device(vk::raii::Device dev) { Dev = std::move(dev); }

unsigned int Device::FindPresentQueueID() {
  unsigned int PresentID = CurrentQueueFamily;
  if (!PDev.GetDevice().getSurfaceSupportKHR(PresentID, **Surface)) {
    PresentIsGraphics = false;
    auto families = PDev.GetDevice().getQueueFamilyProperties();
    for (int i = 0; i < families.size(); ++i) {
      if (PDev.GetDevice().getSurfaceSupportKHR(i, **Surface)) {
        PresentID = i;
        break;
      }
    }
  }
  return PresentID;
}

unsigned int Device::GetPresentID() { return PresentID; }
unsigned int Device::GetGraphicsID() { return CurrentQueueFamily; }

void Device::Create() {
  vk::DeviceQueueCreateInfo dqInfo{.queueFamilyIndex = CurrentQueueFamily,
                                   .queueCount = 1,
                                   .pQueuePriorities = &Priority};

  std::vector extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
                            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                            VK_KHR_SPIRV_1_4_EXTENSION_NAME};

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan11Features,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      features = {{.features = {.samplerAnisotropy = true}},
                  {.shaderDrawParameters = true},
                  {.dynamicRendering = true},
                  {.extendedDynamicState = true}};

  vk::DeviceCreateInfo dcInfo{
      .pNext = &features.get<vk::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &dqInfo,
      .enabledExtensionCount = extensions.size(),
      .ppEnabledExtensionNames = extensions.data()};

  vk::raii::PhysicalDevice& dev = PDev.GetDevice();

  Dev = vk::raii::Device(dev, dcInfo);
  PresentID = FindPresentQueueID();

  Graphics = vk::raii::Queue(Dev, CurrentQueueFamily, 0);
  Present = vk::raii::Queue(Dev, PresentID, 0);
}
bool Device::IsPresentEqualsGraphics() { return PresentIsGraphics; }

void Device::SetPriority(float priority) {
  if (Dev != nullptr) {
    return;
  }
  Priority = priority;
}
float Device::GetPriority() const { return Priority; }

PhysicalDevice& Device::GetPhysicalDevice() { return PDev; }

vk::raii::Device& Device::GetDevice() { return Dev; }
vk::raii::Queue& Device::GetPresentQueue() { return Present; }
vk::raii::Queue& Device::GetGraphicsQueue() { return Graphics; }
void Device::Release() { Dev.release(); }
