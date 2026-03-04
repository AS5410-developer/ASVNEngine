#ifndef INC_VULKANRENDER_RENDER_HPP
#define INC_VULKANRENDER_RENDER_HPP

#include <Base/IError.hpp>
#include <Base/IModule.hpp>
#include <Render/IRender.hpp>

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

  virtual IError* SetFunctionLoader(void* loader) override;

  virtual IImage* CreateImage() override;
  virtual ISprite* CreateSprite(IImage& image, IShader& shader,
                                Transform& transform) override;
  virtual IShader* CreateShader(const std::string& shader) override;

  virtual void DrawSprite(const ISprite& sprite) override;

  virtual ~Render() = default;

 private:
  static IEngine* EngineInstance;
};
}  // namespace AS::Engine

#endif