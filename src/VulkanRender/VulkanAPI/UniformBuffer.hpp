#ifndef REF_UNIFORMBUFFER_HPP
#define REF_UNIFORMBUFFER_HPP

#include "CommandBuffer.hpp"
#include "Shader.hpp"
#include "Vulkan.hpp"

class UniformBuffer final {
 public:
  UniformBuffer();
  UniformBuffer(Device* dev, Shader* shader, std::vector<char> data);
  UniformBuffer(vk::raii::Buffer buf);

  void Create();
  vk::raii::Buffer& GetBuffer();
  void* GetPointer();
  VideoMemory& GetVideoMemory();
  void Bind(CommandBuffer& cb);

  void Release();

  virtual ~UniformBuffer() = default;

 private:
  vk::raii::Buffer Buf = nullptr;
  Device* Dev = 0;
  VideoMemory* VideoMem;
  std::vector<char> Data;
  Shader* Shad;
  vk::raii::DescriptorPool DescriptorPool = nullptr;
  vk::raii::DescriptorSet DescriptorSet = nullptr;
  void* Pointer = 0;
};

#endif
