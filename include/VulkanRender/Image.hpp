#ifndef INC_VULKANRENDER_IMAGE_HPP
#define INC_VULKANRENDER_IMAGE_HPP

#include <Base/IError.hpp>
#include <VulkanRender/Render.hpp>
#include <string>

namespace AS::Engine {
class Image : public IImage {
 public:
  Image() {}

  virtual void SetPath(const std::string& path) override { Path = path; }
  virtual const std::string GetPath() override { return Path; }

  virtual IError* Precache() override;
  virtual bool IsLoaded() override { return Loaded; }
  virtual unsigned long long GetWidth() override { return Width; }
  virtual unsigned long long GetHeight() override { return Height; }
  virtual unsigned char GetDepth() override { return Depth; }
  virtual void Unload() override;
  virtual void Destroy() override;

  void SetInitData(Device& dev, CommandBuffer& cbuffer) {
    Dev = dev;
    CBuffer = cbuffer;
  }

  virtual ~Image() { Destroy(); }

 private:
  Device Dev;
  CommandBuffer CBuffer;
  std::string Path;
  bool Loaded = false;
  unsigned int Width = 0;
  unsigned int Height = 0;
  unsigned int Depth = 1;
};
}  // namespace AS::Engine

#endif