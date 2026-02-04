#ifndef INC_ENGINE_CONSOLE_HPP
#define INC_ENGINE_CONSOLE_HPP

#include <engine_export.h>

#include <Base/IError.hpp>
#include <Engine/IConVar.hpp>
#include <Engine/IConsole.hpp>
#include <map>

namespace AS::Engine {

class ENGINE_EXPORT Console : public IConsole {
 public:
  Console();

  virtual void RegisterConVar(IConVar& convar) override;
  virtual void RegisterConCmd(ConCMD& concmd) override;

  virtual const IError* ExecuteCommand(const std::string& command) override;
  virtual const IError* Execute(const std::string& filePath) override;

  virtual IConsole& operator<<(const std::string& text) override;
  virtual IConsole& operator<<(unsigned long long number) override;
  virtual IConsole& operator<<(long long number) override;
  virtual IConsole& operator<<(unsigned int number) override;
  virtual IConsole& operator<<(int number) override;
  virtual IConsole& operator<<(unsigned short number) override;
  virtual IConsole& operator<<(short number) override;
  virtual IConsole& operator<<(unsigned char number) override;
  virtual IConsole& operator<<(char number) override;
  virtual IConsole& operator<<(const char* text) override;
  virtual IConsole& operator<<(char* text) override;
  virtual IConsole& operator<<(void* address) override;
  virtual IConsole& operator<<(IConsole& (*manipulator)(IConsole&)) override;
  virtual void NewLine() override;
  virtual void Flush() override;

  virtual void SetFlushHandler(FlushHandler handler) override;
  virtual FlushHandler GetFlushHandler() override;

  virtual ~Console();

 private:
  std::map<std::string, IConVar*> convars;
  std::map<std::string, ConCMD*> concmds;
  std::string Buffer;
  FlushHandler handler;
};
}  // namespace AS::Engine

#endif