#ifndef REF_PDEV_HPP
#define REF_PDEV_HPP

#include "Vulkan.hpp"

class PhysicalDevice final {
 public:
  PhysicalDevice();
  PhysicalDevice(vk::raii::PhysicalDevice dev);

  unsigned long long GetScore() const;
  vk::raii::PhysicalDevice& GetDevice();

  unsigned int GetGraphicsQueueFamily() const;
  unsigned int GetComputeQueueFamily() const;

  void GetQueueFamilies();

  void RecalculateScore();

  virtual ~PhysicalDevice() = default;

 private:
  vk::raii::PhysicalDevice Device = nullptr;
  unsigned long long Score = 0;
  unsigned int GraphicsQueueFamily = 0;
  unsigned int ComputeQueueFamily = 0;
};

#endif
