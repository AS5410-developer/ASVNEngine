#ifndef INC_RENDER_IFRAMEBUFFER_HPP
#define INC_RENDER_IFRAMEBUFFER_HPP

#include <Render/IRender.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IFramebuffer {
 public:
  virtual void Bind() = 0;
  virtual int GetTexture() = 0;
  virtual void SetTexture(int ID) = 0;
  virtual void Unbind() = 0;
  virtual void Destroy() = 0;

  virtual ~IFramebuffer() = default;
};
}  // namespace AS::Engine

#endif