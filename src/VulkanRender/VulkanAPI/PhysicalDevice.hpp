#ifndef REF_PDEV_HPP
#define REF_PDEV_HPP

#include "Vulkan.hpp"

class PhysicalDevice final {
 public:
  PhysicalDevice();
  PhysicalDevice(VkPhysicalDevice dev);

  unsigned long long GetScore() const { return Score; }
  VkPhysicalDevice& GetDevice() { return Device; }

  unsigned int GetGraphicsQueueFamily() const { return GraphicsQueueFamily; }
  unsigned int GetComputeQueueFamily() const { return ComputeQueueFamily; }

  void GetQueueFamilies();

  void RecalculateScore();

  virtual ~PhysicalDevice() = default;

 private:
  VkPhysicalDevice Device = nullptr;
  unsigned long long Score = 0;
  unsigned int GraphicsQueueFamily = 0;
  unsigned int ComputeQueueFamily = 0;
};

#endif
