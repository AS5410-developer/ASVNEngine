#ifndef REF_SSBO_HPP
#define REF_SSBO_HPP

#include "Shader.hpp"
#include "Vulkan.hpp"

class SSBO final {
 public:
  SSBO() {}
  SSBO(Device& dev, unsigned int sizeOfEntry, unsigned int count);
  SSBO(VkBuffer buf) : Buf(buf) {}

  void SetDevice(Device& dev) { Dev = dev; }
  void SetData(void* data, unsigned int count) {
    Data = data;
    Count = count;
  }
  void Update();
  bool Created();

  void Create();
  VkBuffer& GetBuffer() { return Buf; }
  VideoMemory& GetVideoMemory() { return VideoMem; }
  unsigned long long GetVertexCount() { return Count; }
  void BindToShader(Shader& shader);
  void Release();

  virtual ~SSBO() = default;

 private:
  VkBuffer Buf = nullptr;
  Device Dev;
  VideoMemory VideoMem;
  unsigned int SizeOfEntry;
  unsigned int Count;
  void* Data;
};

#endif
