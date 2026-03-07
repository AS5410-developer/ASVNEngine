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
  virtual void DrawSprite(const ISprite* sprite) override;
  virtual void EndDraw() override;

  virtual void BindShader(IShader& shader) override;

  virtual void SetPostProcessingShader(IShader& shader) override {
    PostProcessShader = &shader;
  }

  virtual ~Render() = default;

 private:
  static IEngine* EngineInstance;
  IShader* PostProcessShader;
  IShader* CurrentShader;
  Device MainDevice;
  PhysicalDevice MainPhysicalDevice;
  Swapchain MainSwapchain;
  CommandBuffer MainCommandBuffer;
  VertexBuffer SpriteQuad;
  SSBO Buffer;
  std::vector<ShaderData> ShaderParams;
};
}  // namespace AS::Engine

#endif