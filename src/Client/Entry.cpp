#include <ClientExport.h>

#include <Client.hpp>
#include <Engine/IEngine.hpp>

using namespace AS::Engine;

extern "C" CLIENT_EXPORT IModule* GetModuleAPI(IEngine* engine) {
  Client::SetEngine(engine);
  return new Client;
}