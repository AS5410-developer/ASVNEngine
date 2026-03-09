#include <Client.hpp>
#include <thread>

using namespace AS::Engine;

IEngine* Client::EngineInstance = 0;

IRender* render = 0;
ISprite* sprite = 0;

Client::Client() {}

void Client::OnLoaded() {
  EngineInstance->GetConsole() << "Client loaded and started!" << EndLine;

  render = EngineInstance->GetRender();
  IImage* img = render->CreateImage();
  img->SetPath("test.webp");
  img->Precache();
  IShader* shad = render->CreateShader("Default.spv");
  Transform pos;
  pos.SetPosition(glm::vec2(0, 0));
  pos.SetRotation(0);
  pos.SetScale(glm::vec2(img->GetWidth() / (float)img->GetHeight(), 1));
  sprite = render->CreateSprite(img, shad, pos);

  std::thread windowThread(&Client::WindowThread, this);
  windowThread.detach();
}
void Client::OnRegisterOptions() {}
void Client::OnUpdate() {}
void Client::OnTick() {
  float deltaTime = 1 / (float)EngineInstance->GetTickrate();
  sprite->GetTransform().SetRotation(sprite->GetTransform().GetRotation() +
                                     deltaTime);
}
void Client::OnEnabled() {}
void Client::OnDisabled() {}

void Client::OnWindowUpdate() {
  // EngineInstance->GetConsole() << "Window update" << EndLine;
  render->BeginDraw();
  if (sprite) render->DrawSprite(sprite);
  render->EndDraw();
}
void Client::WindowThread() {
  while (EngineInstance->GetTickrate() != -1) {
    OnWindowUpdate();
  }
  // EngineInstance->Quit();
}

void Client::SetEngine(IEngine* engine) { EngineInstance = engine; }