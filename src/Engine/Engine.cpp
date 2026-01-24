#include <Engine.hpp>
#include <chrono>
#include <iostream>
#include <thread>
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

Engine::Engine() {}

std::chrono::steady_clock::time_point PrepareTick() {
  std::chrono::high_resolution_clock::now();
  return std::chrono::high_resolution_clock::now();
}
std::chrono::steady_clock::time_point EndTick() {
  return std::chrono::high_resolution_clock::now();
}

void Engine::OnLoaded() {
  std::thread tickThread([&]() { OnTick(); });
  tickThread.detach();
  ConsoleInstance << "Current engine's address in process virtual memory: "
                  << this << EndLine;
}
void Engine::OnRegisterOptions() {}
void Engine::OnUpdate() {
  for (uint64_t i = 0; i < Modules.size(); ++i) {
    if (Modules[i].Activated) {
      std::thread updateCall([&]() { Modules[i].Module->OnUpdate(); });
      updateCall.detach();
    }
  }
}
void Engine::OnTick() {
  std::chrono::steady_clock::time_point startTime;
  std::chrono::steady_clock::time_point endTime;
  int64_t end;
  std::chrono::steady_clock::time_point maxEndTime;
  while (GetTickrate() != -1) {
    startTime = PrepareTick();

    for (uint64_t i = 0; i < Modules.size(); ++i) {
      if (Modules[i].Activated) Modules[i].Module->OnTick();
    }
    ConsoleInstance << "Tickrate: " << TickInSecond << " "
                    << "Tick: " << CurrentTime << EndLine;
    ++CurrentTime;

    endTime = EndTick();

    end = std::chrono::duration_cast<std::chrono::milliseconds>(
              endTime.time_since_epoch())
              .count();
    maxEndTime = startTime + std::chrono::milliseconds(
                                 (int64_t)((1 / (double)TickInSecond) * 1000));
    if (std::chrono::duration_cast<std::chrono::milliseconds>(
            maxEndTime.time_since_epoch())
            .count() > end) {
      std::this_thread::sleep_until(maxEndTime);
    }
  }
}
void Engine::OnEnabled() {}
void Engine::OnDisabled() {}

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
  module->OnLoaded();

  return info.ID;
}
void Engine::RemoveModule(ModuleID module) {
  if (!Modules.contains(module)) return;
  DeactivateModule(module);
  Modules.erase(module);
}

ResultOrError<ModuleID> Engine::LoadModule(const std::string& name) {
  std::string path = "./bin/" + name;
  ModuleInfo info = {.ID = Modules.size(), .Name = new std::string(name)};

  auto Lib = LauncherInstance->SysLoadLibrary(path);
  if (Lib.Failed()) {
    return ResultOrError<ModuleID>(0, Lib.What(), true);
  }

  auto Proc = LauncherInstance->SysGetModuleFunc(Lib.GetResult());
  if (Proc.Failed()) {
    return ResultOrError<ModuleID>(0, Lib.What(), true);
  }

  info.Module = Proc.GetResult()(this);
  info.LoadedByEngine = true;
  info.Activated = true;
  info.Handle = Lib.GetResult();

  Modules[info.ID] = std::move(info);
  info.Module->OnLoaded();

  return info.ID;
}
ResultOrError<ModuleID> Engine::FindModuleByName(
    const std::string& name) const {
  ModuleID moduleID = 0;
  auto ID = std::find_if(
      Modules.begin(), Modules.end(),
      [&name](
          const std::pair<const AS::Engine::ModuleID, AS::Engine::ModuleInfo>&
              entry) { return *entry.second.Name == name; });

  if (ID == Modules.end())
    return ResultOrError<ModuleID>(0, "Module not found", true);
  return ID->first;
}
const ModuleInfo* Engine::GetModuleInfo(ModuleID module) const {
  return &Modules.at(module);
}
void Engine::DeactivateModule(ModuleID module) {
  if (!Modules.contains(module)) return;
  if (!Modules[module].Activated) return;
  Modules[module].Module->OnDisabled();
  Modules[module].Activated = false;
}
void Engine::ActivateModule(ModuleID module) {
  if (!Modules.contains(module)) return;
  if (Modules[module].Activated) return;
  Modules[module].Activated = true;
  Modules[module].Module->OnEnabled();
}
void Engine::UnloadModule(ModuleID module) {
  DeactivateModule(module);
  if (!Modules[module].LoadedByEngine) {
    Modules.erase(module);
    return;
  }
  LauncherInstance->SysUnloadLibrary(Modules[module].Handle);
  Modules.erase(module);
}

void Engine::SetLauncherClass(ILauncher* launcher) {
  LauncherInstance = launcher;
}

IConsole* Engine::GetConsole() { return &ConsoleInstance; }

Tick Engine::GetCurrentTime() const { return CurrentTime; }
Tick Engine::GetTickrate() const { return TickInSecond; }

bool Engine::IsServer() const { return ServerFlag; }
bool Engine::IsSingleplayer() const { return Singleplayer; }

void Engine::SetIsSingleplayer(bool newFlag) { Singleplayer = newFlag; }