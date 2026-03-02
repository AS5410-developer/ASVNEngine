#ifndef INC_RENDER_ISHADER_HPP
#define INC_RENDER_ISHADER_HPP

#include <Render/IRender.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace AS::Engine {
class ENGINE_EXPORT IShader {
 public:
  virtual void SetUniform(int Num, float value) = 0;
  virtual void SetUniform(int Num, const glm::vec2& value) = 0;
  virtual void SetUniform(int Num, const glm::vec3& value) = 0;
  virtual void SetUniform(int Num, const glm::vec4& value) = 0;
  virtual void SetUniform(int Num, const glm::mat4& value) = 0;
  virtual void SetUniform(int Num, int value) = 0;

  virtual float GetUniformF(int Num) = 0;
  virtual int GetUniformI(int Num) = 0;
  virtual glm::mat4 GetUniformMat4(int Num) = 0;
  virtual glm::vec2 GetUniformVec2(int Num) = 0;
  virtual glm::vec3 GetUniformVec3(int Num) = 0;
  virtual glm::vec4 GetUniformVec4(int Num) = 0;

  virtual bool Has(const char* uniformName) = 0;
  virtual int Get(const char* uniformName) = 0;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual ~IShader() = default;
};
}  // namespace AS::Engine

#endif