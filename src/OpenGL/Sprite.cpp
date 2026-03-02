#include <OpenGL/Sprite.hpp>

using namespace AS::Engine;

Sprite::Sprite() {}

void Sprite::SetImage(IImage& image) { this->image = &image; }
void Sprite::SetShader(IShader& shader) { this->shader = &shader; }
void Sprite::SetTransform(Transform& transform) { transform = transform; }

IImage& Sprite::GetImage() const { return *image; }
IShader& Sprite::GetShader() const { return *shader; }
Transform& Sprite::GetTransform() { return transform; }

void Sprite::Destroy() {
  if (image) image->Destroy();
}