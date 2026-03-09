#include <Base/ErrorSuccess.hpp>
#include <FileSystem/DefaultImpl/File.hpp>

using namespace AS::Engine;

const IError& Open(const char* path, unsigned long flags) {
  return ErrorSuccess();
}

ResultOrError<char*> ReadAll() {}
ResultOrError<char*> ReadAllFromCurrentPos(unsigned long size) {}
ResultOrError<char*> ReadFromCurrentPos(unsigned long size) {}
ResultOrError<char*> Read(unsigned long size) {}

void SetCurrentPos(unsigned long position) {}

IError* WriteFromCurrentPos(unsigned long size, const char* data) {}
IError* WriteFromStart(unsigned long size, const char* data) {}

void Close() {}