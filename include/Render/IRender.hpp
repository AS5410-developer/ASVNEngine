#ifndef INC_RENDER_IRENDER_HPP
#define INC_RENDER_IRENDER_HPP

#include <Base/IModule.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IRender : public IModule {
 public:
  virtual void DrawMesh();
};
}  // namespace AS::Engine

#endif