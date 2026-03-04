#ifndef INC_RENDER_ISHADER_HPP
#define INC_RENDER_ISHADER_HPP

#include <Render/IRender.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IShader {
 public:
  virtual void SetShaderData(const char* filename) = 0;

  virtual ~IShader() = default;
};
}  // namespace AS::Engine

#endif