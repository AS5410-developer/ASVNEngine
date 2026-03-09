#include <FileSystemExport.h>

#include <Engine/IEngine.hpp>
#include <FileSystem/DefaultImpl/FileSystem.hpp>

using namespace AS::Engine;

extern "C" FILESYSTEM_EXPORT IModule* GetModuleAPI(IEngine* engine) {
  FileSystem::SetEngine(engine);
  return new FileSystem;
}