#ifndef INC_RENDER_IRENDER_HPP
#define INC_RENDER_IRENDER_HPP

#include <Base/IError.hpp>
#include <Base/IModule.hpp>
#include <Render/IImage.hpp>
#include <Render/IShader.hpp>
#include <Render/ISprite.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IRender : public IModule {
 public:
  virtual IError* SetFunctionLoader(void* loader) = 0;

  virtual IImage* CreateImage() = 0;
  virtual ISprite* CreateSprite(IImage* image, IShader* shader,
                                Transform& transform) = 0;
  virtual IShader* CreateShader(const std::string& shader) = 0;

  virtual void BeginDraw() = 0;
  virtual void DrawSprite(const ISprite* sprite) = 0;
  virtual void EndDraw() = 0;

  virtual void BindShader(IShader& shader) = 0;

  virtual void SetPostProcessingShader(IShader& shader) = 0;

  virtual ~IRender() = default;
};
}  // namespace AS::Engine

#endif