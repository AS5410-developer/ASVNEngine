#include <Client.hpp>
using namespace AS::Engine;

IEngine* Client::EngineInstance = 0;

Client::Client() {}

void Client::OnLoaded() {
  EngineInstance->GetConsole() << "Client loaded and started!" << EndLine;
}
void Client::OnRegisterOptions() {}
void Client::OnUpdate() {}
void Client::OnTick() {}
void Client::OnEnabled() {}
void Client::OnDisabled() {}

void Client::SetEngine(IEngine* engine) { EngineInstance = engine; }