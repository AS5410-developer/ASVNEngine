#include <Platform/SDL3/Mutex.hpp>
#include <Platform/SDL3/Platform.hpp>

using namespace AS::Engine;

IEngine* Platform::EngineInstance = 0;

void Platform::OnLoaded() {}
void Platform::OnRegisterOptions() {}
void Platform::OnUpdate() {}
void Platform::OnTick() {}
void Platform::OnEnabled() {}
void Platform::OnDisabled() {}

ResultOrError<IWindow*> Platform::CreateWindow() {
  return ResultOrError<IWindow*>(0, "No", true);
}
ResultOrError<IMutex*> Platform::CreateMutex() { return new Mutex; }

void Platform::SetCursorCoords(const CursorCoords& coords) {}
void Platform::SetCursorState(bool hide, bool locked) {}
CursorCoords Platform::GetCursorCoords() { return {0, 0}; }