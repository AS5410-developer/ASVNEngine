#include <Console.hpp>
#include <format>

using namespace AS::Engine;

Console::Console() { Buffer.clear(); }

void Console::RegisterConVar(IConVar& convar) {
  convars[convar.GetName()] = &convar;
}
void Console::RegisterConCmd(ConCMD& concmd) { concmds[concmd.Name] = &concmd; }

const IError* Console::ExecuteCommand(const std::string& command) { return 0; }
const IError* Console::Execute(const std::string& filePath) { return 0; }

IConsole& Console::operator<<(const std::string& text) {
  Buffer.append(text);
  return *this;
}
IConsole& Console::operator<<(unsigned long long number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(long long number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(unsigned int number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(int number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(unsigned short number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(short number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(unsigned char number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(char number) {
  Buffer.append(std::to_string(number));
  return *this;
}
IConsole& Console::operator<<(const char* text) {
  Buffer.append(text);
  return *this;
}
IConsole& Console::operator<<(char* text) {
  Buffer.append(text);
  return *this;
}
IConsole& Console::operator<<(void* address) {
  std::string s =
      std::format("0x{:08x}", reinterpret_cast<unsigned long long>(address));
  Buffer.append(s);
  return *this;
}
IConsole& Console::operator<<(IConsole& (*manipulator)(IConsole&)) {
  manipulator(*this);
  return *this;
}

void Console::NewLine() {
  Buffer.append("\n");
  Flush();
}
void Console::Flush() {
  std::cout << Buffer.c_str();
  if (handler) handler(Buffer);
  Buffer.clear();
}
Console::~Console() {}

IConsole& AS::Engine::EndLine(IConsole& console) {
  console.NewLine();
  return console;
}
IConsole& AS::Engine::Flush(IConsole& console) {
  console.Flush();
  return console;
}

void Console::SetFlushHandler(FlushHandler handler) { this->handler = handler; }
IConsole::FlushHandler Console::GetFlushHandler() { return handler; }