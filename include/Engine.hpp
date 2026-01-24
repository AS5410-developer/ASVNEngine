#ifndef INC_ENGINE_HPP
#define INC_ENGINE_HPP

#include <engine_export.h>

#include <Console.hpp>
#include <Engine/IEngine.hpp>
#include <map>

namespace AS::Engine {

class ENGINE_EXPORT Engine : public IEngine {
 public:
  Engine();

  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;
  virtual void OnEnabled() override;
  virtual void OnDisabled() override;

  virtual void Quit() override;
  virtual void QuitOnError(const IError& error) override;

  virtual ModuleID AddModule(IModule* module) override;
  virtual void RemoveModule(ModuleID module) override;

  virtual ResultOrError<ModuleID> LoadModule(const std::string& name) override;
  virtual ResultOrError<ModuleID> FindModuleByName(
      const std::string& name) const override;
  virtual const ModuleInfo* GetModuleInfo(ModuleID module) const override;
  virtual void DeactivateModule(ModuleID module) override;
  virtual void ActivateModule(ModuleID module) override;
  virtual void UnloadModule(ModuleID module) override;

  virtual void SetLauncherClass(ILauncher* launcher) override;

  virtual IConsole* GetConsole() override;

  virtual Tick GetCurrentTime() const override;
  virtual Tick GetTickrate() const override;

  virtual bool IsServer() const override;
  virtual bool IsSingleplayer() const override;

  virtual void SetIsSingleplayer(bool newFlag) override;

  virtual ~Engine() = default;

 private:
  std::map<ModuleID, ModuleInfo> Modules;
  Tick CurrentTime;
  Tick TickInSecond = 60;
  bool ServerFlag;
  bool Singleplayer;
  Console ConsoleInstance;
  ILauncher* LauncherInstance;
};
}  // namespace AS::Engine

#endif