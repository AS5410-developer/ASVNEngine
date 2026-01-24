#include <Engine.hpp>
#include <iostream>
using namespace AS::Engine;

ModuleInfo& ModuleInfo::operator=(const ModuleInfo& other) {
  ModuleInfo* info = new ModuleInfo{.ID = other.ID,
                                    .Handle = other.Handle,
                                    .Name = other.Name,
                                    .Module = other.Module,
                                    .Activated = other.Activated,
                                    .LoadedByEngine = other.LoadedByEngine};
  return *info;
}

ModuleInfo& ModuleInfo::operator=(ModuleInfo&& other) {
  ModuleInfo* info = new ModuleInfo{.ID = other.ID,
                                    .Handle = other.Handle,
                                    .Name = other.Name,
                                    .Module = other.Module,
                                    .Activated = other.Activated,
                                    .LoadedByEngine = other.LoadedByEngine};
  delete[] &other;
  return *info;
}

Engine::Engine(ILauncher* launcherInstance)
    : LauncherInstance(launcherInstance) {}

void Engine::OnLoaded() {}
void Engine::OnRegisterOptions() {}
void Engine::OnUpdate() {}

void Engine::Quit() { TickInSecond = -1; }
void Engine::QuitOnError(const IError& error) {
  std::cerr << "FATAL ENGINE ERROR CALLED: " << error.What() << std::endl;
  Quit();
}

ModuleID Engine::AddModule(IModule* module) {
  std::string* name = new std::string(std::to_string(Modules.size()));

  ModuleInfo info = {.ID = Modules.size(), .Name = name};
  info.Module = module;
  info.LoadedByEngine = false;
  info.Activated = true;
  info.Handle = nullptr;

  Modules[info.ID] = std::move(info);

  return info.ID;
}
void Engine::RemoveModule(ModuleID module) { Modules.erase(module); }

ResultOrError<ModuleID> Engine::LoadModule(const std::string& name) {
  std::string path = "./bin/" + name;
  ModuleInfo info = {.ID = Modules.size(), .Name = new std::string(name)};

  auto Lib = LauncherInstance->SysLoadLibrary(path);
  if (Lib.Failed()) {
    return ResultOrError<ModuleID>(Lib.What());
  }

  auto Proc = LauncherInstance->SysGetModuleFunc(Lib.GetResult());
  if (Proc.Failed()) {
    return ResultOrError<ModuleID>(Lib.What());
  }

  info.Module = Proc.GetResult()(this);
  info.LoadedByEngine = true;
  info.Activated = true;
  info.Handle = Lib.GetResult();

  Modules[info.ID] = std::move(info);

  return info.ID;
}
ResultOrError<ModuleID> Engine::FindModuleByName(
    const std::string& name) const {}
const ModuleInfo Engine::GetModuleInfo(ModuleID module) const {
  return Modules.at(module);
}
void Engine::DeactivateModule(ModuleID module) {}
void Engine::ActivateModule(ModuleID module) {}
void Engine::UnloadModule(ModuleID module) {}

void Engine::SetLauncherClass(ILauncher* launcher) {
  LauncherInstance = launcher;
}

IConsole* Engine::GetConsole() { return &ConsoleInstance; }

Tick Engine::GetCurrentTime() const { return CurrentTime; }
Tick Engine::GetTickrate() const { return TickInSecond; }

bool Engine::IsServer() const { return ServerFlag; }
bool Engine::IsSingleplayer() const { return Singleplayer; }

void Engine::SetIsSingleplayer(bool newFlag) { Singleplayer = newFlag; }