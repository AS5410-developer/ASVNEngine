#include <OpenGL/glad/gl.h>

#include <OpenGL/Image.hpp>
#include <OpenGL/Render.hpp>
#include <OpenGL/Shader.hpp>
#include <OpenGL/Sprite.hpp>
using namespace AS::Engine;

IEngine* Render::EngineInstance = 0;

Render::Render() {}

void Render::OnLoaded() {}
void Render::OnRegisterOptions() {}
void Render::OnUpdate() {}
void Render::OnTick() {}
void Render::OnEnabled() {}
void Render::OnDisabled() {}

void Render::SetEngine(IEngine* engine) {}

IError* Render::SetFunctionLoader(void* loader) {}

IImage* Render::CreateImage() { return new Image; }
ISprite* Render::CreateSprite(IImage& image, IShader& shader,
                              Transform& transform) {
  Sprite* sprite = new Sprite;
  sprite->SetImage(image);
  sprite->SetShader(shader);
  sprite->SetTransform(transform);
  return sprite;
}
IShader* Render::CreateShader(const std::string& vertex,
                              const std::string& fragment) {
  Shader* shader = new Shader;

  unsigned int v = glCreateShader(GL_VERTEX_SHADER);
  unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);

  const char* srcV = vertex.c_str();
  const char* srcF = fragment.c_str();

  GLint isCompiled = 0;
  GLint maxLength = 0;
  std::vector<GLchar> infoLog;

  glShaderSource(v, 1, &srcV, 0);
  glCompileShader(v);

  glGetShaderiv(v, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
    infoLog = std::vector<GLchar>(maxLength);
    glGetShaderInfoLog(v, maxLength, &maxLength, &infoLog[0]);
    EngineInstance->GetConsole() << infoLog.data() << EndLine;
    glDeleteShader(v);
    return 0;
  }

  glShaderSource(f, 1, &srcF, 0);
  glCompileShader(f);

  glGetShaderiv(f, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
    infoLog = std::vector<GLchar>(maxLength);
    glGetShaderInfoLog(f, maxLength, &maxLength, &infoLog[0]);
    EngineInstance->GetConsole() << infoLog.data() << EndLine;
    glDeleteShader(f);
    return 0;
  }

  GLuint program = glCreateProgram();

  glAttachShader(program, v);
  glAttachShader(program, f);

  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    infoLog = std::vector<GLchar>(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    EngineInstance->GetConsole() << infoLog.data() << EndLine;

    glDeleteProgram(program);
    glDeleteShader(v);
    glDeleteShader(f);

    return 0;
  }

  glDetachShader(program, v);
  glDetachShader(program, f);

  shader->SetProgramID(program);

  return shader;
}
IFramebuffer* Render::CreateFramebuffer() {}

void Render::DrawSprite(const ISprite& sprite) {}
void Render::DrawFramebuffer(const IFramebuffer& fbo) {}