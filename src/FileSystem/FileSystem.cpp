#include <FileSystem/DefaultImpl/File.hpp>
#include <FileSystem/DefaultImpl/FileSystem.hpp>
#include <cstring>
#include <filesystem>
using namespace AS::Engine;

void FileSystem::OnLoaded() {
  File::SetInstance(*this);
#ifdef __linux__
  auto path = std::filesystem::canonical("/proc/self/exe").parent_path();

  char* exePath = path.string().data();
  ExePath = new char[strlen(exePath)];
  strcpy(ExePath, exePath);

  char* binPath = path.append("bin").string().data();
  BinPath = new char[strlen(binPath)];
  strcpy(BinPath, binPath);
#endif
}

void FileSystem::SetupGamePath(const char* gameDir) {
#ifdef __linux__
  auto path = std::filesystem::path(ExePath).append(gameDir);

  char* gamePath = path.string().data();
  GamePath = new char[strlen(gamePath)];
  strcpy(GamePath, gamePath);
#endif
}
std::string FileSystem::GetPath(const char* path, unsigned long pathType) {
  try {
    if (pathType & AS_ENGINE_FILE_SYSTEM_EXEDIR) {
      return std::filesystem::path(ExePath).append(path).string();
    } else if (pathType & AS_ENGINE_FILE_SYSTEM_BINDIR) {
      return std::filesystem::path(BinPath).append(path).string();
    } else if (pathType & AS_ENGINE_FILE_SYSTEM_GAMEDIR) {
      return std::filesystem::path(GamePath).append(path).string();
    }
  } catch (std::exception&) {
    return path;
  }
  return path;
}

IFile* FileSystem::CreateFile() { return new File; }
bool FileSystem::FileExists(const char* path, unsigned long pathType) {
  return std::filesystem::exists(GetPath(path, pathType));
}