#ifndef INC_BASE_FILE_NOT_FOUND_HPP
#define INC_BASE_FILE_NOT_FOUND_HPP

#include <Base/IError.hpp>
#include <string>

namespace AS::Engine {
class ENGINE_EXPORT FileNotFoundError : public IError {
 public:
  FileNotFoundError();
  FileNotFoundError(const std::string& name);

  virtual const std::string& What() const override;
  virtual bool Failed() const override;
  virtual LogLevel GetLogLevel() const override;

  virtual ~FileNotFoundError() = default;

 private:
  std::string Path;
  bool IsFailed = false;
};
}  // namespace AS::Engine

#endif