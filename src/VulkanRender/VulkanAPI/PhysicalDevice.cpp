#include "PhysicalDevice.hpp"

#include <algorithm>
PhysicalDevice::PhysicalDevice() {}
PhysicalDevice::PhysicalDevice(VkPhysicalDevice dev) : Device(dev) {
  RecalculateScore();
  GetQueueFamilies();
}

void PhysicalDevice::RecalculateScore() {
  Score = 0;

  VkPhysicalDeviceProperties properties = {};
  VkPhysicalDeviceFeatures features = {};
  vkGetPhysicalDeviceProperties(Device, &properties);
  vkGetPhysicalDeviceFeatures(Device, &features);

  if (!features.geometryShader) return;

  if (properties.apiVersion < VK_API_VERSION_1_4) return;
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
    Score += 100;
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    Score += 200;

  Score += properties.limits.maxImageDimension2D * 10;
  Score += properties.limits.maxDrawIndirectCount * 10;
  Score += properties.limits.maxDrawIndexedIndexValue;
  Score += properties.limits.maxFramebufferHeight *
           properties.limits.maxFramebufferWidth *
           properties.limits.maxFramebufferLayers;
}

void PhysicalDevice::GetQueueFamilies() {
  unsigned int queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> families(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      Device, &queueFamilyCount,
      reinterpret_cast<VkQueueFamilyProperties*>(families.data()));

  GraphicsQueueFamily = static_cast<uint32_t>(std::distance(
      families.begin(), std::find_if(families.begin(), families.end(),
                                     [](VkQueueFamilyProperties const& input) {
                                       return input.queueFlags &
                                              VK_QUEUE_GRAPHICS_BIT;
                                     })));
  ComputeQueueFamily = static_cast<uint32_t>(std::distance(
      families.begin(), std::find_if(families.begin(), families.end(),
                                     [](VkQueueFamilyProperties const& input) {
                                       return input.queueFlags &
                                              VK_QUEUE_COMPUTE_BIT;
                                     })));
}
