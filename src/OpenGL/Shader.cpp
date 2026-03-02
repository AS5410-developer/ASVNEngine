#include <OpenGL/glad/gl.h>

#include <OpenGL/Shader.hpp>
using namespace AS::Engine;

Shader::Shader() {}

void Shader::SetUniform(int Num, float value) {
  Bind();
  glUniform1fv(programID, Num, &value);
  Unbind();
}
void Shader::SetUniform(int Num, const glm::vec2& value) {
  Bind();
  glUniform2fv(programID, Num, &value[0]);
  Unbind();
}
void Shader::SetUniform(int Num, const glm::vec3& value) {
  Bind();
  glUniform3fv(programID, Num, &value[0]);
  Unbind();
}
void Shader::SetUniform(int Num, const glm::vec4& value) {
  Bind();
  glUniform4fv(programID, Num, &value[0]);
  Unbind();
}
void Shader::SetUniform(int Num, const glm::mat4& value) {
  Bind();
  glUniformMatrix4fv(programID, Num, false, &value[0][0]);
  Unbind();
}
void Shader::SetUniform(int Num, int value) {}

float Shader::GetUniformF(int Num) {
  Bind();
  float f = 0;
  glGetUniformfv(programID, Num, &f);
  Unbind();
  return f;
}
int Shader::GetUniformI(int Num) {
  Bind();
  int f = 0;
  glGetUniformiv(programID, Num, &f);
  Unbind();
  return f;
}
glm::mat4 Shader::GetUniformMat4(int Num) {
  Bind();
  glm::mat4 f;
  glGetUniformfv(programID, Num, &f[0][0]);
  Unbind();
  return f;
}
glm::vec2 Shader::GetUniformVec2(int Num) {
  Bind();
  glm::vec2 f;
  glGetUniformfv(programID, Num, &f[0]);
  Unbind();
  return f;
}
glm::vec3 Shader::GetUniformVec3(int Num) {
  Bind();
  glm::vec3 f;
  glGetUniformfv(programID, Num, &f[0]);
  Unbind();
  return f;
}
glm::vec4 Shader::GetUniformVec4(int Num) {
  Bind();
  glm::vec4 f;
  glGetUniformfv(programID, Num, &f[0]);
  Unbind();
  return f;
}

bool Shader::Has(const char* uniformName) {
  Bind();
  bool result = glGetUniformLocation(programID, uniformName) != -1;
  Unbind();
  return result;
}
int Shader::Get(const char* uniformName) {
  Bind();
  int result = glGetUniformLocation(programID, uniformName);
  Unbind();
  return result;
}

void Shader::SetProgramID(unsigned int ID) { programID = ID; }

void Shader::Bind() { glUseProgram(programID); }
void Shader::Unbind() { glUseProgram(0); }