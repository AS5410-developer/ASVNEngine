#include "SSBO.hpp"

#include <cstring>

SSBO::SSBO(Device& dev, unsigned int sizeOfEntry, unsigned int count)
    : Dev(dev), SizeOfEntry(sizeOfEntry), Count(count) {
  Create();
}

void SSBO::Create() {
  VkMemoryRequirements mr;
  VkBufferCreateInfo bcInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                            .pNext = nullptr,
                            .flags = 0,
                            .size = SizeOfEntry * Count,
                            .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                     VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                            .sharingMode = VK_SHARING_MODE_EXCLUSIVE};
  vkCreateBuffer(Dev.GetDevice(), &bcInfo, nullptr, &Buf);
  vkGetBufferMemoryRequirements(Dev.GetDevice(), Buf, &mr);
  VideoMem = VideoMemory(Dev, mr.size, mr, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  vkBindBufferMemory(Dev.GetDevice(), Buf, VideoMem.GetDeviceMemory(), 0);
  Update();
}
void SSBO::Update() {
  void* mem = VideoMem.Map();
  std::memcpy(mem, Data, Count * SizeOfEntry);
  VideoMem.Unmap();
}
bool SSBO::Created() { return Buf != nullptr; }
void SSBO::Release() {
  VideoMem.Release();
  vkDestroyBuffer(Dev.GetDevice(), Buf, nullptr);
}
void SSBO::BindToShader(Shader& shader) {
  VkDescriptorBufferInfo dbInfo{
      .buffer = Buf, .offset = 0, .range = VK_WHOLE_SIZE};
  VkWriteDescriptorSet wdSet{
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = 0,
      .dstSet = shader.GetDescriptorSet(),
      .dstBinding = 0,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .pBufferInfo = &dbInfo};
  vkUpdateDescriptorSets(Dev.GetDevice(), 1, &wdSet, 0, 0);
}