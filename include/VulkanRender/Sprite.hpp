#ifndef INC_VULKANRENDER_SPRITE_HPP
#define INC_VULKANRENDER_SPRITE_HPP

#include <Engine/Transform.hpp>
#include <Render/IRender.hpp>

namespace AS::Engine {
struct ShaderData {
  glm::mat4 Proj;
  glm::mat4 View;
  glm::mat4 Model;
};
class Sprite : public ISprite {
 public:
  Sprite() {}
  Sprite(unsigned int id) : ID(id) {}

  virtual void SetImage(IImage* image) override {
    Img = dynamic_cast<Image*>(image);
  }
  virtual void SetShader(IShader* shader) override {
    Shadr = dynamic_cast<EShader*>(shader);
  }
  virtual void SetTransform(Transform& transform) override {
    Local = transform;
  }

  virtual IImage* GetImage() const override { return Img; }
  virtual IShader* GetShader() const override { return Shadr; }
  virtual Transform& GetTransform() override { return Local; }

  unsigned int GetBufferID() const { return ID; }

  virtual void Destroy() override {
    if (Img) Img->Destroy();
  }

  virtual ~Sprite() { Destroy(); }

 private:
  Image* Img = 0;
  EShader* Shadr = 0;
  Transform Local;
  unsigned int ID = 0;
};
}  // namespace AS::Engine

#endif