#ifndef INC_VULKANRENDER_SHADER_HPP
#define INC_VULKANRENDER_SHADER_HPP

#include <Device.hpp>
#include <Render/IRender.hpp>
#include <Shader.hpp>
#include <Swapchain.hpp>

namespace AS::Engine {
class EShader : public IShader {
 public:
  EShader() {}

  virtual void SetShaderData(const char* filename) override;
  void SetInitData(Device& dev, Swapchain& swapchain) {
    Dev = dev;
    Swapch = swapchain;
  }
  Shader* GetShader() { return ShaderInstance; }

  virtual ~EShader() = default;

 private:
  Shader* ShaderInstance;
  Device Dev;
  Swapchain Swapch;
};
}  // namespace AS::Engine

#endif