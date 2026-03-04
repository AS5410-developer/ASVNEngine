#ifndef REF_DEV_HPP
#define REF_DEV_HPP

#include "PhysicalDevice.hpp"
#include "Vulkan.hpp"

class Device final {
 public:
  Device() {}
  Device(PhysicalDevice pdev, unsigned int queueFamily, VkSurfaceKHR surface);
  Device(VkDevice dev) : Dev(dev) {}

  void Create();
  unsigned int FindPresentQueueID();
  VkDevice& GetDevice() { return Dev; }
  VkQueue& GetPresentQueue() { return Present; }
  VkQueue& GetGraphicsQueue() { return Graphics; }
  PhysicalDevice& GetPhysicalDevice() { return PDev; }
  void Release();

  unsigned int GetPresentID() { return PresentID; }
  unsigned int GetGraphicsID() { return CurrentQueueFamily; }

  bool IsPresentEqualsGraphics() { return PresentIsGraphics; }

  void SetPriority(float priority);
  float GetPriority() const { return Priority; }

  virtual ~Device() = default;

 private:
  VkDevice Dev = nullptr;
  VkQueue Present = nullptr;
  VkQueue Graphics = nullptr;
  VkSurfaceKHR Surface = nullptr;
  unsigned int CurrentQueueFamily = 0;
  unsigned int PresentID = 0;
  PhysicalDevice PDev;
  float Priority = 1.0f;
  bool PresentIsGraphics = true;
};

extern Device* MainDevice;

#endif
