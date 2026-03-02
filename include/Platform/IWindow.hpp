#ifndef INC_PLATFORM_IWINDOW_HPP
#define INC_PLATFORM_IWINDOW_HPP

#include <Base/IModule.hpp>
#include <Base/ResultOrError.hpp>

namespace AS::Engine {
struct WindowSize {
  unsigned int x;
  unsigned int y;
};
class ENGINE_EXPORT IWindow {
 public:
  virtual void SetSize(const WindowSize& size) = 0;

  virtual WindowSize GetSize() const = 0;
  virtual ResultOrError<void*> GetSurface() = 0;

  virtual void Destroy() = 0;

  virtual ~IWindow() = default;
};
}  // namespace AS::Engine

#endif