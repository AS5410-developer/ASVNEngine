#ifndef INC_RENDER_IRENDER_HPP
#define INC_RENDER_IRENDER_HPP

#include <Base/IError.hpp>
#include <Base/IModule.hpp>
#include <Render/IFramebuffer.hpp>
#include <Render/IShader.hpp>
#include <Render/ISprite.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IShader;
class ENGINE_EXPORT IImage;
class ENGINE_EXPORT IRender : public IModule {
 public:
  virtual IError* SetFunctionLoader(void* loader) = 0;

  virtual IImage* CreateImage() = 0;
  virtual ISprite* CreateSprite(IImage& image, IShader& shader,
                                Transform& transform) = 0;
  virtual IShader* CreateShader(const std::string& vertex,
                                const std::string& fragment) = 0;
  virtual IFramebuffer* CreateFramebuffer() = 0;

  virtual void DrawSprite(const ISprite& sprite) = 0;
  virtual void DrawFramebuffer(const IFramebuffer& fbo) = 0;

  virtual ~IRender() = default;
};
}  // namespace AS::Engine

#endif