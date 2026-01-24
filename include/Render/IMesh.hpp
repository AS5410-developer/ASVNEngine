#ifndef INC_RENDER_IMESH_HPP
#define INC_RENDER_IMESH_HPP

#include <engine_export.h>

namespace AS::Engine {
class ENGINE_EXPORT IMesh {
 public:
  virtual void SetShader() = 0;
  virtual void SetTexture() = 0;

  virtual void GetShader() = 0;
  virtual void GetTexture() = 0;
  virtual ~IMesh() = default;
};
}  // namespace AS::Engine

#endif