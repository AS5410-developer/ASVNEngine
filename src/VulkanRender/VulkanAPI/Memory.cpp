#include "Memory.hpp"

VideoMemory::VideoMemory() {}
VideoMemory::VideoMemory(Device* dev, unsigned long long size,
                         vk::MemoryRequirements req,
                         vk::MemoryPropertyFlags props)
    : Dev(dev), Size(size) {
  vk::MemoryAllocateInfo maInfo{
      .allocationSize = Size,
      .memoryTypeIndex = GetMemType(req.memoryTypeBits, props)

  };
  Mem = vk::raii::DeviceMemory(dev->GetDevice(), maInfo);
}

unsigned int VideoMemory::GetMemType(uint32_t typeFilter,
                                     vk::MemoryPropertyFlags properties) {
  auto t = Dev->GetPhysicalDevice().GetDevice().getMemoryProperties();

  for (unsigned int i = 0; i < t.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (t.memoryTypes[i].propertyFlags & properties)) {
      return i;
    }
  }
  return 0;
}
vk::raii::DeviceMemory& VideoMemory::GetDeviceMemory() { return Mem; }

void* VideoMemory::Map() { return Mem.mapMemory(0, Size); }
void VideoMemory::Unmap() { Mem.unmapMemory(); }
void VideoMemory::Release() {
  Mem.clear();
  Mem = nullptr;
}
