#include "Memory.hpp"

VideoMemory::VideoMemory(Device& dev, unsigned long long size,
                         VkMemoryRequirements req, VkMemoryPropertyFlags props)
    : Dev(dev), Size(size) {
  VkMemoryAllocateInfo maInfo{
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .pNext = 0,
      .allocationSize = Size,
      .memoryTypeIndex = GetMemType(req.memoryTypeBits, props)

  };
  vkAllocateMemory(dev.GetDevice(), &maInfo, 0, &Mem);
}

unsigned int VideoMemory::GetMemType(uint32_t typeFilter,
                                     VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties t;
  vkGetPhysicalDeviceMemoryProperties(Dev.GetPhysicalDevice().GetDevice(), &t);

  for (unsigned int i = 0; i < t.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (t.memoryTypes[i].propertyFlags & properties)) {
      return i;
    }
  }
  return 0;
}

void* VideoMemory::Map() {
  void* data;
  vkMapMemory(Dev.GetDevice(), Mem, 0, Size, 0, &data);
  return data;
}
void VideoMemory::Release() {
  vkFreeMemory(Dev.GetDevice(), Mem, 0);
  Mem = nullptr;
}
