#include <Device.hpp>

Device::Device(PhysicalDevice pdev, unsigned int queueFamily,
               VkSurfaceKHR surface)
    : PDev(pdev), CurrentQueueFamily(queueFamily), Surface(surface) {
  Create();
}

unsigned int Device::FindPresentQueueID() {
  unsigned int PresentID = CurrentQueueFamily;
  VkBool32 supported = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(PDev.GetDevice(), CurrentQueueFamily,
                                       Surface, &supported);
  if (!supported) {
    PresentIsGraphics = false;
    unsigned int count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(PDev.GetDevice(), &count, 0);
    std::vector<VkQueueFamilyProperties> families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(PDev.GetDevice(), &count,
                                             families.data());
    for (int i = 0; i < families.size(); ++i) {
      vkGetPhysicalDeviceSurfaceSupportKHR(PDev.GetDevice(), i, Surface,
                                           &supported);
      if (supported) {
        PresentID = i;
        break;
      }
    }
  }
  return PresentID;
}

void Device::Create() {
  VkDeviceQueueCreateInfo dqInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .queueFamilyIndex = CurrentQueueFamily,
      .queueCount = 1,
      .pQueuePriorities = &Priority};

  std::vector extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
                            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                            VK_KHR_SPIRV_1_4_EXTENSION_NAME};

  VkPhysicalDeviceExtendedDynamicStateFeaturesEXT pdedsFeatures{
      .sType =
          VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
      .pNext = 0,
      .extendedDynamicState = true};

  VkPhysicalDeviceVulkan13Features pdv13Features{
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
      .pNext = &pdedsFeatures,
      .dynamicRendering = true};

  VkPhysicalDeviceVulkan11Features pdv11Features{
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
      .pNext = &pdv13Features,
      .shaderDrawParameters = true};

  VkPhysicalDeviceFeatures2 features{
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
      .pNext = &pdv11Features,
      .features = {.geometryShader = true, .samplerAnisotropy = true}};

  VkDeviceCreateInfo dcInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = &features,
      .flags = 0,
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &dqInfo,
      .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data()};

  VkPhysicalDevice dev = PDev.GetDevice();

  vkCreateDevice(dev, &dcInfo, 0, &Dev);
  PresentID = FindPresentQueueID();

  vkGetDeviceQueue(Dev, CurrentQueueFamily, 0, &Graphics);
  vkGetDeviceQueue(Dev, PresentID, 0, &Present);
}

void Device::SetPriority(float priority) {
  if (Dev != nullptr) {
    return;
  }
  Priority = priority;
}

void Device::Release() {
  if (Dev) vkDestroyDevice(Dev, 0);
}
