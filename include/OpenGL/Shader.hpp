#ifndef INC_OPENGL_SHADER_HPP
#define INC_OPENGL_SHADER_HPP

#include <Render/IShader.hpp>

namespace AS::Engine {
class Shader final : public IShader {
 public:
  Shader();

  virtual void SetUniform(int Num, float value) override;
  virtual void SetUniform(int Num, const glm::vec2& value) override;
  virtual void SetUniform(int Num, const glm::vec3& value) override;
  virtual void SetUniform(int Num, const glm::vec4& value) override;
  virtual void SetUniform(int Num, const glm::mat4& value) override;
  virtual void SetUniform(int Num, int value) override;

  virtual float GetUniformF(int Num) override;
  virtual int GetUniformI(int Num) override;
  virtual glm::mat4 GetUniformMat4(int Num) override;
  virtual glm::vec2 GetUniformVec2(int Num) override;
  virtual glm::vec3 GetUniformVec3(int Num) override;
  virtual glm::vec4 GetUniformVec4(int Num) override;

  virtual bool Has(const char* uniformName) override;
  virtual int Get(const char* uniformName) override;

  void SetProgramID(unsigned int ID);

  virtual void Bind() override;
  virtual void Unbind() override;

  virtual ~Shader() = default;

 private:
  unsigned int programID = 0;
};
}  // namespace AS::Engine

#endif