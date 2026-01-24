#ifndef INC_RENDER_IRENDER_HPP
#define INC_RENDER_IRENDER_HPP

#include <Base/IModule.hpp>
#include <Render/IFramebuffer.hpp>
#include <Render/ISprite.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IRender : public IModule {
 public:
  virtual ISprite CreateSprite(const IImage& image, const IShader& shader,
                               Transform& transform) = 0;
  virtual IShader CreateShader(const std::string& vertex,
                               const std::string& fragment) = 0;
  virtual IFramebuffer CreatFramebuffer() = 0;

  virtual void DrawSprite(const ISprite& sprite) = 0;
  virtual void DrawFramebuffer(const IFramebuffer& fbo) = 0;

  virtual ~IRender();
};
}  // namespace AS::Engine

#endif