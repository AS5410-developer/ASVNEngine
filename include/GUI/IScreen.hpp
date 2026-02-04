#ifndef INC_GUI_ISCREEN_HPP
#define INC_GUI_ISCREEN_HPP

#include <Base/IModule.hpp>
#include <GUI/IGUIElement.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IScreen : public IGUIElement {
 public:
  virtual ~IScreen() = default;
};
}  // namespace AS::Engine

#endif