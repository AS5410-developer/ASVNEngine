#include <Launcher.hpp>
#ifdef __linux__
#include <dlfcn.h>
#endif
#include <cstdarg>
#include <iostream>

using namespace AS::Engine;

Launcher::Launcher() {}

void Launcher::OnLoaded() {
  auto result = SysLoadModule("./bin/libEngine.so");
  if (result.Failed()) {
    LaunchFailed("Loading Engine module failed. %s", result.What().c_str());
    return;
  }

  EnginePtr = dynamic_cast<IEngine*>(result.GetResult()(nullptr));
  EnginePtr->SetLauncherClass(this);
  EnginePtr->OnLoaded();
}
void Launcher::OnRegisterOptions() {}
void Launcher::OnUpdate() {
  for (;;) {
    EnginePtr->OnUpdate();
    if (EnginePtr->GetTickrate() == -1) {
      break;
    }
  }
}
void Launcher::OnTick() {}
void Launcher::OnEnabled() {}
void Launcher::OnDisabled() {}

void Launcher::LaunchFailed(const char* reason, ...) {
  va_list args;
  va_start(args, reason);

  fprintf(stderr, "FATAL LAUNCH ERROR: ");
  fprintf(stderr, reason, args);
  fprintf(stderr, "\n");

  Failed = true;

  va_end(args);
}

ResultOrError<LibraryHandle> Launcher::SysLoadLibrary(const std::string& path) {
#ifdef __linux__
  void* Lib = dlopen(path.c_str(), RTLD_NOW);
  if (!Lib) {
    char* err = dlerror();
    std::cerr
        << "Launcher fatal error: cannot load launcher library\ndlerror(): "
        << err << std::endl;
    return ResultOrError<LibraryHandle>(0, err, true);
  }
  return ResultOrError<LibraryHandle>(Lib);
#endif
}
ResultOrError<void*> Launcher::SysGetFunc(LibraryHandle handle,
                                          const std::string& funcName) {
#ifdef __linux__
  void* Func = dlsym(handle, funcName.c_str());
  if (!Func) {
    char* err = dlerror();
    std::cerr << "Launcher fatal error: cannot get \"Launch\" symbol from "
                 "loaded launcher library!\ndlerror(): "
              << err << std::endl;
    return ResultOrError<LibraryHandle>(0, err, true);
  }
  return ResultOrError<LibraryHandle>(Func);
#endif
}
void Launcher::SysUnloadLibrary(LibraryHandle handle) {
#ifdef __linux__
  if (handle) dlclose(handle);
#endif
}

ResultOrError<GetModuleAPIFunc> Launcher::SysGetModuleFunc(
    LibraryHandle handle) {
  ResultOrError<void*> Func = SysGetFunc(handle, "GetModuleAPI");

  if (Func.Failed()) {
    return ResultOrError<GetModuleAPIFunc>(0, Func.What(), true);
  }

  return ResultOrError<GetModuleAPIFunc>(
      reinterpret_cast<GetModuleAPIFunc>(Func.GetResult()));
}
ResultOrError<GetModuleAPIFunc> Launcher::SysLoadModule(
    const std::string& path) {
  ResultOrError<LibraryHandle> Handle = SysLoadLibrary(path);
  if (Handle.Failed())
    return ResultOrError<GetModuleAPIFunc>(0, Handle.What(), true);

  return SysGetModuleFunc(Handle.GetResult());
}
bool Launcher::IsFailed() { return Failed; }