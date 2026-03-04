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
  delete this;
  CurrentInstance = 0;
}

PhysicalDevice& PhysicalDeviceManager::GetBestPhysicalDevice() { return Best; }

const std::vector<PhysicalDevice> PhysicalDeviceManager::GetDevices() const {
  return Devices;
}

void PhysicalDeviceManager::Enumerate() {
  auto devs = MainInstance.enumeratePhysicalDevices();
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
