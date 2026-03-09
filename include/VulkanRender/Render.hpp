#ifndef INC_VULKANRENDER_RENDER_HPP
#define INC_VULKANRENDER_RENDER_HPP

#include <Base/ErrorSuccess.hpp>
#include <CommandBuffer.hpp>
#include <Device.hpp>
#include <Engine/IEngine.hpp>
#include <PhysicalDeviceManager.hpp>
#include <Render/IRender.hpp>
#include <SSBO.hpp>
#include <Swapchain.hpp>
#include <VertexBuffer.hpp>
#include <Vulkan.hpp>
#include <VulkanRender/Sprite.hpp>

namespace AS::Engine {
class Render : public IRender {
 public:
  Render() {}

  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;
  virtual void OnTick() override;
  virtual void OnEnabled() override;
  virtual void OnDisabled() override;

  static void SetEngine(IEngine* engine) { EngineInstance = engine; }
  static IEngine* GetEngine() { return EngineInstance; }

  virtual IError* SetFunctionLoader(void* loader) override {
    return new ErrorSuccess;
  }

  virtual IImage* CreateImage() override;
  virtual ISprite* CreateSprite(IImage* image, IShader* shader,
                                Transform& transform) override;
  virtual IShader* CreateShader(const std::string& shader) override;

  virtual void BeginDraw() override;
  virtual void DrawSprite(ISprite* sprite) override;
  virtual void EndDraw() override;

  virtual void BindShader(IShader& shader) override;

  virtual void SetPostProcessingShader(IShader& shader) override {
    PostProcessShader = &reinterpret_cast<EShader&>(shader);
  }

  virtual ~Render() = default;

 private:
  static IEngine* EngineInstance;
  EShader* PostProcessShader = 0;
  EShader* CurrentShader = 0;
  Device MainDevice;
  PhysicalDevice MainPhysicalDevice;
  Swapchain MainSwapchain;
  CommandBuffer MainCommandBuffer;
  VertexBuffer SpriteQuad;
  glm::mat4 Proj;
  glm::mat4 View;
  SSBO Buffer;
  ShaderData* ShaderParams;
  unsigned int ShaderParamsCount = 0;
  std::vector<unsigned int> FreeShaderParamsId;
};
}  // namespace AS::Engine

#endif