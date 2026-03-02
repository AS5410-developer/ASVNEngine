#include <SDL3/SDL.h>

#include <Platform/SDL3/Mutex.hpp>
#include <Platform/SDL3/Platform.hpp>
#include <Platform/SDL3/Window.hpp>

using namespace AS::Engine;

IEngine* Platform::EngineInstance = 0;

void Platform::OnLoaded() { SDL_Init(SDL_INIT_VIDEO); }
void Platform::OnRegisterOptions() {}
void Platform::OnUpdate() {}
void Platform::OnTick() {}
void Platform::OnEnabled() {}
void Platform::OnDisabled() { SDL_Quit(); }

ResultOrError<IWindow*> Platform::CreateWindow() { return new Window; }
ResultOrError<IMutex*> Platform::CreateMutex() { return new Mutex; }

void Platform::SetCursorCoords(const CursorCoords& coords) {
  SDL_WarpMouseGlobal(coords.x, coords.y);
}
void Platform::SetCursorState(bool hide) {
  if (hide)
    SDL_HideCursor();
  else
    SDL_ShowCursor();
}
CursorCoords Platform::GetCursorCoords() {
  float x, y;
  SDL_GetGlobalMouseState(&x, &y);
  return {(uint)x, (uint)y};
}