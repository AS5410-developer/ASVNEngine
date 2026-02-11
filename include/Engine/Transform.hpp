#ifndef INC_ENGINE_TRANSFORM_HPP
#define INC_ENGINE_TRANSFORM_HPP

#include <engine_export.h>

#include <glm/glm.hpp>

namespace AS::Engine {
class ENGINE_EXPORT Transform {
 public:
  Transform() = default;

  void SetParent(const Transform* parent);
  void SetPosition(const glm::vec2& pos);
  void SetRotation(float rot);
  void SetScale(const glm::vec2& scale);

  glm::vec2 GetPosition() const;
  float GetRotation() const;
  glm::vec2 GetScale() const;
  glm::vec2 GetAbsolutePosition() const;
  float GetAbsoluteRotation() const;
  glm::vec2 GetAbsoluteScale() const;

  glm::mat4x4 GetTransformMatrix() const;

  const Transform* GetParent();

  virtual ~Transform() = default;

 private:
  glm::vec2 Position;
  float Rotation;
  glm::vec2 Scale;
  const Transform* Parent;
};
}  // namespace AS::Engine

#endif