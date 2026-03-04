#include <VideoVkExport.h>

#include <Engine/IEngine.hpp>
#include <VulkanRender/Render.hpp>

using namespace AS::Engine;

extern "C" VIDEO_VK_EXPORT IModule* GetModuleAPI(IEngine* engine) {
  Render::SetEngine(engine);
  return new Render;
}