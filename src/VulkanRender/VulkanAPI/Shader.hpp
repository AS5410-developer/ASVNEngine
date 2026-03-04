#ifndef REF_SHADER_HPP
#define REF_SHADER_HPP

#include <map>

#include "Device.hpp"
#include "Memory.hpp"
#include "Swapchain.hpp"
#include "Vulkan.hpp"

#define SHADER_MOD_LOCATION "shaders/"

struct ShaderVar {
  char* Name;
  unsigned long long Size;
  unsigned long long Offset;
};

struct Vertex {
  float pos[3];
  float color[3];
  float texCoord[2];
};

class Shader {
 public:
  Shader();
  Shader(Device* dev, Swapchain* swapchain, const char* filename,
         bool enableDepthTest = true,
         vk::CullModeFlags cullMode = vk::CullModeFlagBits::eBack);

  void SetCullMode(vk::CullModeFlags cullMode);
  vk::CullModeFlags GetCullMode();

  void SetBlending(bool enabled);
  bool GetBlending();

  void DrawGeometry(const Vertex* vertex);

  vk::raii::Pipeline& GetPipeline();
  vk::raii::PipelineLayout& GetPipelineLayout();
  vk::raii::DescriptorSetLayout& GetDescriptorSetLayout();

  void RebuildPipeline();

  virtual ~Shader() = default;

 private:
  vk::raii::Pipeline Pipeline = nullptr;
  vk::raii::PipelineLayout PipelineLayout = nullptr;
  vk::raii::ShaderModule Module = nullptr;
  vk::raii::DescriptorSetLayout DescriptorSetLayout = nullptr;
  vk::ShaderModuleCreateInfo ShaderModuleInfo;
  vk::CullModeFlags CullMode = vk::CullModeFlagBits::eBack;
  bool Blending = false;
  bool EnableDepthTest = true;
  Swapchain* Swapch = 0;
  Device* Dev = 0;
};

#endif
