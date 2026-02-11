#include <Engine/Transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/mat4x4.hpp>

using namespace AS::Engine;

void Transform::SetParent(const Transform* parent) { Parent = parent; }
void Transform::SetPosition(const glm::vec2& pos) { Position = pos; }
void Transform::SetRotation(float rot) { Rotation = rot; }
void Transform::SetScale(const glm::vec2& scale) { Scale = scale; }

glm::vec2 Transform::GetPosition() const { return Position; }
float Transform::GetRotation() const { return Rotation; }
glm::vec2 Transform::GetScale() const { return Scale; }
glm::vec2 Transform::GetAbsolutePosition() const {
  if (!Parent) return Position;
  glm::vec4 vector = glm::vec4(Position, 0, 1);
  return GetTransformMatrix() * vector;
}
float Transform::GetAbsoluteRotation() const {
  if (!Parent) return Rotation;
  return Parent->GetAbsoluteRotation() + Rotation;
}
glm::vec2 Transform::GetAbsoluteScale() const {
  if (!Parent) return Scale;
  return Parent->GetAbsoluteScale() * Scale;
}

glm::mat4x4 Transform::GetTransformMatrix() const {
  glm::mat4x4 transform;
  transform = glm::scale(transform, glm::vec3(Scale, 1));
  transform = glm::rotate(transform, Rotation, glm::vec3(1, 0, 0));
  transform = glm::translate(transform, glm::vec3(Position, 0));
  return transform;
}

const Transform* Transform::GetParent() { return Parent; }