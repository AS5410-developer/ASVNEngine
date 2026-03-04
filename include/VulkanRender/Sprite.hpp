#ifndef INC_VULKANRENDER_SPRITE_HPP
#define INC_VULKANRENDER_SPRITE_HPP

#include <Engine/Transform.hpp>
#include <Render/IRender.hpp>

namespace AS::Engine {
class Sprite : public ISprite {
 public:
  Sprite() {}

  virtual void SetImage(IImage& image);
  virtual void SetShader(IShader& shader);
  virtual void SetTransform(Transform& transform);

  virtual IImage& GetImage() const;
  virtual IShader& GetShader() const;
  virtual Transform& GetTransform();

  virtual void Destroy();

  virtual ~Sprite() = default;

 private:
};
}  // namespace AS::Engine

#endif