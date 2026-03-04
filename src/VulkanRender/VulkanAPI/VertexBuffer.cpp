#include "VertexBuffer.hpp"

#include <cstring>

VertexBuffer::VertexBuffer(Device& dev, std::vector<Vertex> data)
    : Dev(dev), Data(data) {
  Create();
}

void VertexBuffer::Create() {
  VkBufferCreateInfo bcInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                            .pNext = nullptr,
                            .flags = 0,
                            .size = Data.size() * sizeof(Vertex),
                            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            .sharingMode = VK_SHARING_MODE_EXCLUSIVE};
  vkCreateBuffer(Dev.GetDevice(), &bcInfo, nullptr, &Buf);
  VideoMem = new VideoMemory(Dev, bcInfo.size, Buf->getMemoryRequirements(),
                             vk::MemoryPropertyFlagBits::eHostVisible |
                                 vk::MemoryPropertyFlagBits::eHostCoherent);
  vkBindBufferMemory(Dev.GetDevice(), Buf, VideoMem->GetDeviceMemory(), 0);
  void* mem = VideoMem->Map();
  std::memcpy(mem, Data.data(), Data.size() * sizeof(Vertex));
  VideoMem->Unmap();
}
bool VertexBuffer::Created() {
  if (!this) return false;
  return Buf != nullptr;
}
void VertexBuffer::Release() {
  VideoMem.Release();
  vkDestroyBuffer(Dev.GetDevice(), Buf, nullptr);
}
