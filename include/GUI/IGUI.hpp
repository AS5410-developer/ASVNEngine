#ifndef INC_GUI_IGUI_HPP
#define INC_GUI_IGUI_HPP

#include <Base/IModule.hpp>
#include <GUI/IScreen.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IGUI : public IModule {
 public:
  virtual IScreen* LoadScreen(const char* name) = 0;

  virtual ~IGUI() = default;
};
}  // namespace AS::Engine

#endif