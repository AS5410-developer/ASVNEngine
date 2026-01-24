#ifndef INC_ENGINE_ICONSOLE_HPP
#define INC_ENGINE_ICONSOLE_HPP

#include <engine_export.h>

#include <Base/IError.hpp>
#include <Engine/IConVar.hpp>

namespace AS::Engine {
typedef void (*ConsoleFunc)(int argc, char** argv);

struct ConCMD {
  std::string Name;
  ConsoleFunc Func;
  std::string Description;
};

class ENGINE_EXPORT IConsole {
 public:
  virtual void RegisterConVar(IConVar& convar) = 0;
  virtual void RegisterConCmd(ConCMD& concmd) = 0;

  virtual const IError* ExecuteCommand(const std::string& command) = 0;
  virtual const IError* Execute(const std::string& filePath) = 0;

  virtual void operator<<(const std::string& text) = 0;
  virtual void operator<<(unsigned long long number) = 0;
  virtual void operator<<(long long number) = 0;
  virtual void operator<<(unsigned int number) = 0;
  virtual void operator<<(int number) = 0;
  virtual void operator<<(unsigned short number) = 0;
  virtual void operator<<(short number) = 0;
  virtual void operator<<(unsigned char number) = 0;
  virtual void operator<<(char number) = 0;
  virtual void operator<<(const char* text) = 0;
  virtual void operator<<(char* text) = 0;
  virtual void operator<<(void* address) = 0;

  virtual ~IConsole() = default;
};
}  // namespace AS::Engine

#endif