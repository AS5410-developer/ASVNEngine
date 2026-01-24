#ifndef INC_RENDER_ISPRITE_HPP
#define INC_RENDER_ISPRITE_HPP

#include <Engine/Transform.hpp>
#include <Render/IImage.hpp>
#include <Render/IRender.hpp>
#include <Render/IShader.hpp>

namespace AS::Engine {
class ENGINE_EXPORT ISprite {
 public:
  virtual void SetImage(const IImage& image) = 0;
  virtual void SetShader(const IShader& shader) = 0;
  virtual void SetTranform(const Transform& transform) = 0;

  virtual IImage& GetImage() const = 0;
  virtual IShader& GetShader() const = 0;
  virtual Transform& GetTranform() const = 0;

  virtual void Destroy() = 0;

  virtual ~ISprite() = default;
};
}  // namespace AS::Engine

#endif