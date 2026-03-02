#ifndef INC_OPENGL_IMAGE_HPP
#define INC_OPENGL_IMAGE_HPP

#include <Render/IRender.hpp>
#include <string>

namespace AS::Engine {
class Image final : public IImage {
 public:
  Image();

  virtual void SetPath(const std::string& path) override;
  virtual const std::string GetPath() override;

  virtual IError* Precache() override;
  virtual bool IsLoaded() override;
  virtual unsigned long long GetWidth() override;
  virtual unsigned long long GetHeight() override;
  virtual unsigned char GetDepth() override;
  virtual void Unload() override;
  virtual void Destroy() override;

  virtual ~Image() = default;

 private:
  std::string Path;
  bool Loaded = false;
  unsigned long long Width = 0;
  unsigned long long Height = 0;
  unsigned char Depth = 0;
  char* Buffer = 0;
  unsigned long long Size = 0;
};
}  // namespace AS::Engine

#endif