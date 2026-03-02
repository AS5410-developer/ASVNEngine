#ifndef INC_ENGINE_TRANSFORM_HPP
#define INC_ENGINE_TRANSFORM_HPP

#include <engine_export.h>

#include <glm/glm.hpp>

namespace AS::Engine {
class ENGINE_EXPORT Transform {
 public:
  Transform();

  void SetParent(const Transform* parent) { Parent = parent; }
  void SetPosition(const glm::vec2& pos) { Position = pos; }
  void SetRotation(float rot) { Rotation = rot; }
  void SetScale(const glm::vec2& scale) { Scale = scale; }

  glm::vec2 GetPosition() const { return Position; }
  float GetRotation() const { return Rotation; }
  glm::vec2 GetScale() const { return Scale; }
  glm::vec2 GetAbsolutePosition() const;
  float GetAbsoluteRotation() const;
  glm::vec2 GetAbsoluteScale() const;

  glm::mat4x4 GetTransformMatrix() const;

  const Transform* GetParent() { return Parent; }

  virtual ~Transform() = default;

 private:
  glm::vec2 Position;
  float Rotation;
  glm::vec2 Scale;
  const Transform* Parent;
};
}  // namespace AS::Engine

#endif