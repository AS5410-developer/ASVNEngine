#include <PlatformSDL3Export.h>

#include <Engine/IEngine.hpp>

using namespace AS::Engine;

extern "C" PLATFORM_SDL3_EXPORT IModule* GetModuleAPI(IEngine* engine) {
  return 0;
}