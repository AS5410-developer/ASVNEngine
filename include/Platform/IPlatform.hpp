#ifndef INC_PLATFORM_IPLATFORM_HPP
#define INC_PLATFORM_IPLATFORM_HPP

#include <Base/IModule.hpp>

namespace AS::Engine {
class IPlatform : public IModule {
 public:
  virtual ~IPlatform() = default;
};
}  // namespace AS::Engine

#endif