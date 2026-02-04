#ifndef INC_GUI_IELEMENT_HPP
#define INC_GUI_IELEMENT_HPP

#include <Base/IModule.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IGUIElement {
 public:
  virtual IGUIElement& GetChild(unsigned long long num) = 0;
  virtual IGUIElement& operator[](unsigned long long num) = 0;
  virtual IGUIElement& operator[](const char* name) = 0;

  virtual unsigned long long FindChildByName(const char* name) = 0;

  virtual void SetShow(bool isShowing) = 0;
  virtual bool GetShow() = 0;

  virtual ~IGUIElement() = default;
};
}  // namespace AS::Engine

#endif