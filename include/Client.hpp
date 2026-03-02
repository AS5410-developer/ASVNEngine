#ifndef INC_CLIENT_HPP
#define INC_CLIENT_HPP

#include <Client/IClient.hpp>
#include <Engine/IEngine.hpp>

namespace AS::Engine {
class Client final : public IClient {
 public:
  Client();

  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;
  virtual void OnTick() override;
  virtual void OnEnabled() override;
  virtual void OnDisabled() override;

  virtual void OnWindowUpdate() override;
  void WindowThread();

  static void SetEngine(IEngine* engine);

  virtual ~Client() = default;

 private:
  static IEngine* EngineInstance;
};
}  // namespace AS::Engine

#endif