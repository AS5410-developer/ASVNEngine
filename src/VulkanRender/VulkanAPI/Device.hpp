#ifndef REF_DEV_HPP
#define REF_DEV_HPP

#include "PhysicalDevice.hpp"
#include "Vulkan.hpp"

class Device final {
 public:
  Device();
  Device(PhysicalDevice pdev, unsigned int queueFamily,
         vk::raii::SurfaceKHR* surface);
  Device(vk::raii::Device dev);

  void Create();
  unsigned int FindPresentQueueID();
  vk::raii::Device& GetDevice();
  vk::raii::Queue& GetPresentQueue();
  vk::raii::Queue& GetGraphicsQueue();
  PhysicalDevice& GetPhysicalDevice();
  void Release();

  unsigned int GetPresentID();
  unsigned int GetGraphicsID();

  bool IsPresentEqualsGraphics();

  void SetPriority(float priority);
  float GetPriority() const;

  virtual ~Device() = default;

 private:
  vk::raii::Device Dev = nullptr;
  vk::raii::Queue Present = nullptr;
  vk::raii::Queue Graphics = nullptr;
  vk::raii::SurfaceKHR* Surface = nullptr;
  unsigned int CurrentQueueFamily = 0;
  unsigned int PresentID = 0;
  PhysicalDevice PDev;
  float Priority = 1.0f;
  bool PresentIsGraphics = true;
};

extern Device* MainDevice;

#endif
