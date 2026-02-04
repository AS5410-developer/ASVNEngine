#ifndef INC_QT_GUI_HPP
#define INC_QT_GUI_HPP

#include <GUI/IGUI.hpp>

namespace AS::Engine {
class ENGINE_EXPORT GUI : public IGUI {
 public:
  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;
  virtual void OnTick() override;
  virtual void OnEnabled() override;
  virtual void OnDisabled() override;

  virtual IScreen* LoadScreen(const char* name) override;

  virtual ~GUI() = default;
};
}  // namespace AS::Engine

#endif