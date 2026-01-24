#ifndef INC_RENDER_IIMAGE_HPP
#define INC_RENDER_IIMAGE_HPP

#include <Render/IRender.hpp>
#include <string>

namespace AS::Engine {
class ENGINE_EXPORT IImage {
 public:
  virtual void SetPath(const std::string& path) = 0;
  virtual const std::string GetPath() = 0;

  virtual void Precache() = 0;
  virtual bool IsLoaded() = 0;
  virtual unsigned long long GetWidth() = 0;
  virtual unsigned long long GetHeight() = 0;
  virtual unsigned char GetDepth() = 0;
  virtual void Unload() = 0;
  virtual void Destroy() = 0;

  virtual ~IImage() = default;
};
}  // namespace AS::Engine

#endif