#ifndef INC_VULKANRENDER_IMAGE_HPP
#define INC_VULKANRENDER_IMAGE_HPP

#include <Base/IError.hpp>
#include <Image.hpp>
#include <string>

namespace AS::Engine {
class EImage : public IImage {
 public:
  EImage() {}

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
  ::Image& GetImage() { return Img; }

  virtual ~EImage() { Destroy(); }

 private:
  Device Dev;
  CommandBuffer CBuffer;
  std::string Path;
  bool Loaded = false;
  int Width = 0;
  int Height = 0;
  unsigned int Depth = 4;
  char* Data = 0;
  ::Image Img;
};
}  // namespace AS::Engine

#endif