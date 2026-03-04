#ifndef REF_VERTEXBUFFER_HPP
#define REF_VERTEXBUFFER_HPP

#include "Shader.hpp"
#include "Vulkan.hpp"

class VertexBuffer final {
 public:
  VertexBuffer();
  VertexBuffer(Device* dev, std::vector<Vertex> data);
  VertexBuffer(vk::raii::Buffer& buf);

  void SetDevice(Device* dev);
  void SetData(std::vector<Vertex> data);
  bool Created();

  void Create();
  vk::raii::Buffer& GetBuffer();
  VideoMemory& GetVideoMemory();
  unsigned long long GetVertexCount();
  void Release();

  virtual ~VertexBuffer() = default;

 private:
  vk::raii::Buffer* Buf = nullptr;
  Device* Dev = 0;
  VideoMemory* VideoMem;
  std::vector<Vertex> Data;
};

#endif
