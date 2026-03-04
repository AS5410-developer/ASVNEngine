#ifndef REF_PDEV_MGR_HPP
#define REF_PDEV_MGR_HPP

#include "PhysicalDevice.hpp"
#include "Vulkan.hpp"

class PhysicalDeviceManager final {
 public:
  static PhysicalDeviceManager& Create();
  void Release();

  PhysicalDevice& GetBestPhysicalDevice() { return Best; }

  const std::vector<PhysicalDevice> GetDevices() const;

  void Enumerate();

  static PhysicalDeviceManager& GetInstance();

  virtual ~PhysicalDeviceManager() = default;

 private:
  PhysicalDeviceManager();

  static PhysicalDeviceManager* CurrentInstance;

  std::vector<PhysicalDevice> Devices;
  PhysicalDevice Best;
};

#endif
