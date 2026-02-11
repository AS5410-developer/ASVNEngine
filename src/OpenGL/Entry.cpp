#include <OpenGLExport.h>

#include <Engine/IEngine.hpp>
#include <OpenGL/Render.hpp>

using namespace AS::Engine;

extern "C" OPENGL_EXPORT IModule* GetModuleAPI(IEngine* engine) {
  Render::SetEngine(engine);
  return new Render;
}