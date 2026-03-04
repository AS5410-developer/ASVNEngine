#include "PhysicalDevice.hpp"
PhysicalDevice::PhysicalDevice() {}
PhysicalDevice::PhysicalDevice(vk::raii::PhysicalDevice dev) {
  Device = dev;
  RecalculateScore();
  GetQueueFamilies();
}

unsigned long long PhysicalDevice::GetScore() const { return Score; }

void PhysicalDevice::RecalculateScore() {
  Score = 0;
  auto features = Device.getFeatures();
  auto properties = Device.getProperties();
  if (!features.geometryShader) return;

  if (properties.apiVersion < vk::ApiVersion14) return;
  if (properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu)
    Score += 100;
  if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
    Score += 200;

  Score += properties.limits.maxImageDimension2D * 10;
  Score += properties.limits.maxDrawIndirectCount * 10;
  Score += properties.limits.maxDrawIndexedIndexValue;
  Score += properties.limits.maxFramebufferHeight *
           properties.limits.maxFramebufferWidth *
           properties.limits.maxFramebufferLayers;
}

unsigned int PhysicalDevice::GetGraphicsQueueFamily() const {
  return GraphicsQueueFamily;
}
unsigned int PhysicalDevice::GetComputeQueueFamily() const {
  return ComputeQueueFamily;
}

void PhysicalDevice::GetQueueFamilies() {
  auto families = Device.getQueueFamilyProperties();

  GraphicsQueueFamily = static_cast<uint32_t>(std::distance(
      families.begin(),
      std::find_if(families.begin(), families.end(),
                   [](vk::QueueFamilyProperties const& input) {
                     return input.queueFlags & vk::QueueFlagBits::eGraphics;
                   })));
  ComputeQueueFamily = static_cast<uint32_t>(std::distance(
      families.begin(),
      std::find_if(families.begin(), families.end(),
                   [](vk::QueueFamilyProperties const& input) {
                     return input.queueFlags & vk::QueueFlagBits::eCompute;
                   })));
}

vk::raii::PhysicalDevice& PhysicalDevice::GetDevice() { return Device; }
