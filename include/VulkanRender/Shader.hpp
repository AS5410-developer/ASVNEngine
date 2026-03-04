#ifndef INC_VULKANRENDER_SHADER_HPP
#define INC_VULKANRENDER_SHADER_HPP

#include <VulkanRender/Render.hpp>

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

  virtual ~EShader() {
    if (ShaderInstance) ShaderInstance->Release();
  }

 private:
  Shader* ShaderInstance;
  Device Dev;
  Swapchain Swapch;
};
}  // namespace AS::Engine

#endif