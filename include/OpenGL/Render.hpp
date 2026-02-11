#ifndef INC_OPENGL_RENDER_HPP
#define INC_OPENGL_RENDER_HPP

#include <Engine/IEngine.hpp>
#include <Render/IRender.hpp>

namespace AS::Engine {
class Render final : public IRender {
 public:
  Render();

  virtual void OnLoaded() override;
  virtual void OnRegisterOptions() override;
  virtual void OnUpdate() override;
  virtual void OnTick() override;
  virtual void OnEnabled() override;
  virtual void OnDisabled() override;

  static void SetEngine(IEngine* engine);

  virtual IError* SetFunctionLoader(void* loader) override;

  virtual IImage* CreateImage() override;
  virtual ISprite* CreateSprite(IImage& image, IShader& shader,
                                Transform& transform) override;
  virtual IShader* CreateShader(const std::string& vertex,
                                const std::string& fragment) override;
  virtual IFramebuffer* CreateFramebuffer() override;

  virtual void DrawSprite(const ISprite& sprite) override;
  virtual void DrawFramebuffer(const IFramebuffer& fbo) override;

  virtual ~Render() = default;

 private:
  static IEngine* EngineInstance;
};
}  // namespace AS::Engine

#endif