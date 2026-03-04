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
  Shader() {}
  Shader(Device dev, Swapchain swapchain, const char* filename,
         VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT);

  void SetCullMode(VkCullModeFlags cullMode) { CullMode = cullMode; }
  VkCullModeFlags GetCullMode() { return CullMode; }

  void SetBlending(bool enabled) { Blending = enabled; }
  bool GetBlending() { return Blending; }

  VkPipeline& GetPipeline() { return Pipeline; }
  VkPipelineLayout& GetPipelineLayout() { return PipelineLayout; }
  VkDescriptorSetLayout& GetDescriptorSetLayout() {
    return DescriptorSetLayout;
  }

  void RebuildPipeline();
  void Release();

  virtual ~Shader() = default;

 private:
  VkPipeline Pipeline = nullptr;
  VkPipelineLayout PipelineLayout = nullptr;
  VkShaderModule Module = nullptr;
  VkDescriptorSetLayout DescriptorSetLayout = nullptr;
  VkShaderModuleCreateInfo ShaderModuleInfo;
  VkCullModeFlags CullMode = VK_CULL_MODE_BACK_BIT;
  bool Blending = false;
  Swapchain Swapch = 0;
  Device Dev = 0;
};

#endif
