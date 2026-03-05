#ifndef REF_COMMAND_BUFFER_HPP
#define REF_COMMAND_BUFFER_HPP
#include "CommandPool.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "VertexBuffer.hpp"
#include "Vulkan.hpp"

class CommandBuffer final {
 public:
  CommandBuffer() {}
  CommandBuffer(CommandPool& pool, Swapchain& swapchain);
  CommandBuffer(VkCommandBuffer cb) : Buffer(cb) {}

  VkCommandBuffer& GetBuffer() { return Buffer; }

  void WaitDeviceIdle();

  void StartDraw();
  void SetCurrentShader(Shader& shader);
  template <typename T>
  void PushConstant(T& data) {
    vkCmdPushConstants(Buffer, *CurrentShader->GetPipelineLayout(),
                       VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(T), &data);
  }
  VkCommandBuffer StartSTCommands();
  void EndSTCommands(VkCommandBuffer& buffer);
  void DrawVertexNotIndexedBuffer(VertexBuffer& buffer, unsigned int id);
  void EndDraw();
  void TransitionImageLayout(
      VkCommandBuffer& buffer, VkPipelineStageFlags srcStageMask,
      VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask,
      VkAccessFlags dstAccessMask, VkImageLayout oldLayout,
      VkImageLayout newLayout, VkImage image,
      VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);

  void Release();

  virtual ~CommandBuffer() = default;

 private:
  VkCommandBuffer Buffer = nullptr;
  CommandPool Pool;
  Swapchain Swapch;

  unsigned int SwapchID = 0;

  VkSemaphore Present = nullptr;
  VkSemaphore Render = nullptr;
  VkFence Draw = nullptr;
  Shader* CurrentShader;
};

#endif
