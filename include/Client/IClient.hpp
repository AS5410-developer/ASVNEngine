#ifndef INC_CLIENT_ICLIENT_HPP
#define INC_CLIENT_ICLIENT_HPP

#include <Base/IModule.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IClient : public IModule {
 public:
  virtual ~IClient() = default;
};
}  // namespace AS::Engine

#endif