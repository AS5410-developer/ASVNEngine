#include "PhysicalDeviceManager.hpp"

PhysicalDeviceManager* PhysicalDeviceManager::CurrentInstance = 0;

PhysicalDeviceManager& PhysicalDeviceManager::Create() {
  PhysicalDeviceManager* result;
  if (CurrentInstance)
    result = CurrentInstance;
  else
    result = new PhysicalDeviceManager;
  return *result;
}
void PhysicalDeviceManager::Release() {
  CurrentInstance = 0;
  delete this;
}

const std::vector<PhysicalDevice> PhysicalDeviceManager::GetDevices() const {
  return Devices;
}

void PhysicalDeviceManager::Enumerate() {
  unsigned int count = 0;
  vkEnumeratePhysicalDevices(MainInstance, &count, nullptr);
  std::vector<VkPhysicalDevice> devs(count);
  vkEnumeratePhysicalDevices(MainInstance, &count,
                             reinterpret_cast<VkPhysicalDevice*>(devs.data()));
  unsigned long long maxScore = 0;
  for (auto dev : devs) {
    PhysicalDevice Device(dev);
    if (Device.GetScore() > maxScore) {
      maxScore = Device.GetScore();
      Best = Device;
    }
    Devices.push_back(Device);
  }
}

PhysicalDeviceManager& PhysicalDeviceManager::GetInstance() {
  return *CurrentInstance;
}

PhysicalDeviceManager::PhysicalDeviceManager() {
  CurrentInstance = this;
  Enumerate();
}
