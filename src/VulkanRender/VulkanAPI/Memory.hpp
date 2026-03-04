#ifndef REF_MEM_HPP
#define REF_MEM_HPP

#include "Device.hpp"
#include "Vulkan.hpp"

// Free with delete function
template <typename T>
T* Copy(T* data) {
  T* nData = new T;
  memcpy(nData, data, sizeof(T));
  return nData;
}

class VideoMemory final {
 public:
  VideoMemory();
  VideoMemory(Device* dev, unsigned long long size, vk::MemoryRequirements req,
              vk::MemoryPropertyFlags props);

  void* Map();
  void Unmap();

  vk::raii::DeviceMemory& GetDeviceMemory();

  void Release();

  virtual ~VideoMemory() = default;

 private:
  unsigned int GetMemType(uint32_t typeFilter,
                          vk::MemoryPropertyFlags properties);

  vk::raii::DeviceMemory Mem = nullptr;
  Device* Dev;
  unsigned long long Size;
};

#endif
