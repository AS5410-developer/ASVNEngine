#ifndef INC_RENDER_ISHADER_HPP
#define INC_RENDER_ISHADER_HPP

#include <Render/IRender.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IShader {
 public:
  virtual float& operator[](int Num) = 0;
  virtual bool Has(const char* uniformName) const = 0;
  virtual int Get(const char* uniformName) const = 0;

  virtual void Use() = 0;

  virtual ~IShader() = default;
};
}  // namespace AS::Engine

#endif