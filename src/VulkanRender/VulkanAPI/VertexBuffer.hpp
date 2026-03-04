#ifndef REF_VERTEXBUFFER_HPP
#define REF_VERTEXBUFFER_HPP

#include "Shader.hpp"
#include "Vulkan.hpp"

class VertexBuffer final {
 public:
  VertexBuffer() {}
  VertexBuffer(Device& dev, std::vector<Vertex> data);
  VertexBuffer(VkBuffer buf) : Buf(buf) {}

  void SetDevice(Device& dev) { Dev = dev; }
  void SetData(std::vector<Vertex> data) { Data = data; }
  bool Created();

  void Create();
  VkBuffer& GetBuffer() { return Buf; }
  VideoMemory& GetVideoMemory() { return VideoMem; }
  unsigned long long GetVertexCount() { return Data.size(); }
  void Release();

  virtual ~VertexBuffer() = default;

 private:
  VkBuffer Buf = nullptr;
  Device Dev;
  VideoMemory VideoMem;
  std::vector<Vertex> Data;
};

#endif
