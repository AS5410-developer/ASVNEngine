#ifndef INC_VULKANRENDER_SHADER_HPP
#define INC_VULKANRENDER_SHADER_HPP

#include <Render/IRender.hpp>

namespace AS::Engine {
class Shader : public IShader {
 public:
  Shader() {}

  virtual void SetShaderData(const void* data,
                             unsigned long long size) override;

  virtual void Bind() override;
  virtual void Unbind() override;

  virtual ~Shader() = default;
};
}  // namespace AS::Engine

#endif