#ifndef INC_OPENGL_SPRITE_HPP
#define INC_OPENGL_SPRITE_HPP

#include <Engine/Transform.hpp>
#include <Render/IImage.hpp>
#include <Render/IRender.hpp>
#include <Render/IShader.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IImage;
class Sprite final : public ISprite {
 public:
  Sprite();

  virtual void SetImage(IImage& image) override;
  virtual void SetShader(IShader& shader) override;
  virtual void SetTransform(Transform& transform) override;

  virtual IImage& GetImage() const override;
  virtual IShader& GetShader() const override;
  virtual Transform& GetTransform() override;

  virtual void Destroy() override;

  virtual ~Sprite() = default;

 private:
  IImage* image = 0;
  IShader* shader = 0;
  Transform transform;
};
}  // namespace AS::Engine
#endif