#include <LauncherExport.h>

#include <Launcher.hpp>
#include <iostream>

extern "C" void Launch(int argc, char** argv) {
  std::cout << "Launcher Library started!\nStarting Engine..." << std::endl;
  AS::Engine::Launcher Launcher;
  Launcher.OnRegisterOptions();
  Launcher.OnLoaded();
  for (;;) {
    Launcher.OnUpdate();
  }
}