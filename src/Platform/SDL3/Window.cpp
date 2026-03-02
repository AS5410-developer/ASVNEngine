#include <Platform/SDL3/Window.hpp>

using namespace AS::Engine;

void Window::Initialize() {
  window = SDL_CreateWindow(Title, Size.x, Size.y, Flag | SDL_WINDOW_RESIZABLE);
}

void Window::SetSize(const WindowSize& size) {
  if (!window) return;
  Size = size;
  SDL_SetWindowSize(window, size.x, size.y);
}
void Window::SetFullscreen(bool isItTrue) {
  Fullscreen = isItTrue;
  if (window) SDL_SetWindowFullscreen(window, isItTrue);
}
void Window::SetTitle(const char* title) {
  if (window) SDL_SetWindowTitle(window, title);
  Title = title;
}

ResultOrError<void*> Window::GetSurface() {
  if (!window) return ResultOrError<void*>(0, "Window = 0", true);
  if (Flag != AS_ENGINE_IWINDOW_SURFACETYPE_VULKAN)
    return ResultOrError<void*>(0,
                                "You trying get surface for OPENGL??? Are you "
                                "serious??? This is only vulkan function!!!",
                                true);
  return SDL_GetWindowSurface(window);
}

void Window::Update() {
  if (!window) return;
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        Destroy();
        break;
      case SDL_EVENT_WINDOW_RESIZED:
        int x, y;
        SDL_GetWindowSize(window, &x, &y);
        Size.x = x;
        Size.y = y;
        break;
      default:
        break;
    }
  }
}

void Window::Destroy() {
  if (window) SDL_DestroyWindow(window);
}

Window::~Window() {
  if (window) Destroy();
}