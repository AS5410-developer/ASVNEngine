#include <Client.hpp>
#include <thread>

using namespace AS::Engine;

IEngine* Client::EngineInstance = 0;

Client::Client() {}

void Client::OnLoaded() {
  EngineInstance->GetConsole() << "Client loaded and started!" << EndLine;
  // std::thread windowThread(&Client::WindowThread, this);
  // windowThread.detach();
}
void Client::OnRegisterOptions() {}
void Client::OnUpdate() {}
void Client::OnTick() {}
void Client::OnEnabled() {}
void Client::OnDisabled() {}

void Client::OnWindowUpdate() {
  // EngineInstance->GetConsole() << "Window update" << EndLine;
}
void Client::WindowThread() {
  // EngineInstance->Quit();
}

void Client::SetEngine(IEngine* engine) { EngineInstance = engine; }