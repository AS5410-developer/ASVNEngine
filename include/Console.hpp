#ifndef INC_ENGINE_CONSOLE_HPP
#define INC_ENGINE_CONSOLE_HPP

#include <engine_export.h>

#include <Base/IError.hpp>
#include <Engine/IConVar.hpp>
#include <Engine/IConsole.hpp>

namespace AS::Engine {

class ENGINE_EXPORT Console : public IConsole {
 public:
  Console() = default;

  virtual void RegisterConVar(const IConVar& convar) override;
  virtual void RegisterConCmd(const ConCMD& concmd) override;

  virtual const IError ExecuteCommand(const std::string& command) override;
  virtual const IError Execute(const std::string& filePath) override;

  virtual void operator<<(const std::string& text) override;
  virtual void operator<<(unsigned long long number) override;
  virtual void operator<<(long long number) override;
  virtual void operator<<(unsigned int number) override;
  virtual void operator<<(int number) override;
  virtual void operator<<(unsigned short number) override;
  virtual void operator<<(short number) override;
  virtual void operator<<(unsigned char number) override;
  virtual void operator<<(char number) override;
  virtual void operator<<(const char* text) override;
  virtual void operator<<(char* text) override;
  virtual void operator<<(void* address) override;

  virtual ~Console() = default;
};
}  // namespace AS::Engine

#endif