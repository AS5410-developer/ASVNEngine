#include <Console.hpp>

using namespace AS::Engine;

Console::Console() {}

void Console::RegisterConVar(IConVar& convar) {
  convars[convar.GetName()] = &convar;
}
void Console::RegisterConCmd(ConCMD& concmd) { concmds[concmd.Name] = &concmd; }

const IError* Console::ExecuteCommand(const std::string& command) { return 0; }
const IError* Console::Execute(const std::string& filePath) { return 0; }

void Console::operator<<(const std::string& text) {}
void Console::operator<<(unsigned long long number) {}
void Console::operator<<(long long number) {}
void Console::operator<<(unsigned int number) {}
void Console::operator<<(int number) {}
void Console::operator<<(unsigned short number) {}
void Console::operator<<(short number) {}
void Console::operator<<(unsigned char number) {}
void Console::operator<<(char number) {}
void Console::operator<<(const char* text) {}
void Console::operator<<(char* text) {}
void Console::operator<<(void* address) {}