#ifndef INC_FYLESYSTEM_IFYLESYSTEM_HPP
#define INC_FYLESYSTEM_IFYLESYSTEM_HPP

#include <engine_export.h>

#include <Base/IError.hpp>
#include <Base/IModule.hpp>
#include <string>

namespace AS::Engine {
class ENGINE_EXPORT IFileSystem : public IModule {
 public:
  virtual const std::string& GetGamePath() const = 0;
  virtual const std::string& GetExecutablePath() const = 0;

  virtual ~IFileSystem() = default;
};
}  // namespace AS::Engine

#endif