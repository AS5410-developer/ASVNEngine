#include <VulkanRender/Shader.hpp>
using namespace AS::Engine;
void EShader::SetShaderData(const char* filename) {
  ShaderInstance = new Shader(Dev, Swapch, filename);
}