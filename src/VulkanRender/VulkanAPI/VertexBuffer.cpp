#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {}
VertexBuffer::VertexBuffer(Device* dev, std::vector<Vertex> data)
    : Dev(dev), Data(data) {
  Create();
}
VertexBuffer::VertexBuffer(vk::raii::Buffer& buf) : Buf(&buf) {}

void VertexBuffer::Create() {
  vk::BufferCreateInfo bcInfo{.size = Data.size() * sizeof(Vertex),
                              .usage = vk::BufferUsageFlagBits::eVertexBuffer,
                              .sharingMode = vk::SharingMode::eExclusive};
  Buf = new vk::raii::Buffer(Dev->GetDevice(), bcInfo);
  VideoMem = new VideoMemory(Dev, bcInfo.size, Buf->getMemoryRequirements(),
                             vk::MemoryPropertyFlagBits::eHostVisible |
                                 vk::MemoryPropertyFlagBits::eHostCoherent);
  Buf->bindMemory(VideoMem->GetDeviceMemory(), 0);
  void* mem = VideoMem->Map();
  memcpy(mem, Data.data(), Data.size() * sizeof(Vertex));
  VideoMem->Unmap();
}
void VertexBuffer::SetDevice(Device* dev) { Dev = dev; }
void VertexBuffer::SetData(std::vector<Vertex> data) { Data = data; }
vk::raii::Buffer& VertexBuffer::GetBuffer() { return *Buf; }
bool VertexBuffer::Created() {
  if (!this) return false;
  return Buf != nullptr;
}
VideoMemory& VertexBuffer::GetVideoMemory() { return *VideoMem; }
unsigned long long VertexBuffer::GetVertexCount() { return Data.size(); }
void VertexBuffer::Release() {
  VideoMem->Release();
  delete Buf;
}
