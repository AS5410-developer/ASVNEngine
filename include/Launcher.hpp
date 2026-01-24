#ifndef INC_LAUNCHER_HPP
#define INC_LAUNCHER_HPP

#include <engine_export.h>

#include <Engine.hpp>
#include <Launcher/ILauncher.hpp>
#include <string>

namespace AS::Engine {

class Launcher : public ILauncher {
 public:
  Launcher();

  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;

  virtual ResultOrError<LibraryHandle> SysLoadLibrary(
      const std::string& path) override;
  virtual ResultOrError<void*> SysGetFunc(LibraryHandle handle,
                                          const std::string& funcName) override;
  virtual void SysUnloadLibrary(LibraryHandle handle) override;

  virtual ResultOrError<GetModuleAPIFunc> SysGetModuleFunc(
      LibraryHandle handle) override;
  virtual ResultOrError<GetModuleAPIFunc> SysLoadModule(
      const std::string& path) override;

  void LaunchFailed(const char* reason, ...);

  virtual ~Launcher() = default;

 private:
  IEngine* EnginePtr;
};
}  // namespace AS::Engine

#endif