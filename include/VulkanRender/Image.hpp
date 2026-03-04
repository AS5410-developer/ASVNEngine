#ifndef INC_VULKANRENDER_IMAGE_HPP
#define INC_VULKANRENDER_IMAGE_HPP

#include <Base/IError.hpp>
#include <Render/IRender.hpp>
#include <string>

namespace AS::Engine {
class Image : public IImage {
 public:
  Image() {}

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
};
}  // namespace AS::Engine

#endif