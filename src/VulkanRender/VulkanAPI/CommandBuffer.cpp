#include "CommandBuffer.hpp"

CommandBuffer::CommandBuffer(CommandPool& pool, Swapchain& swapchain)
    : Swapch(swapchain), Pool(pool) {
  VkCommandBufferAllocateInfo cbaInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext = 0,
      .commandPool = *pool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1};
  vkAllocateCommandBuffers(Pool.GetDevice().GetDevice(), &cbaInfo, &Buffer);

  const VkSemaphoreCreateInfo scInfo{
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = 0, .flags = 0};
  VkFenceCreateInfo fcInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                           .pNext = 0,
                           .flags = VK_FENCE_CREATE_SIGNALED_BIT};

  vkCreateSemaphore(Pool.GetDevice().GetDevice(), &scInfo, 0, &Present);
  vkCreateSemaphore(Pool.GetDevice().GetDevice(), &scInfo, 0, &Render);
  vkCreateFence(Pool.GetDevice().GetDevice(), &fcInfo, 0, &Draw);
}

void CommandBuffer::WaitDeviceIdle() {
  vkDeviceWaitIdle(Pool.GetDevice().GetDevice());
}

void CommandBuffer::StartDraw() {
  vkWaitForFences(Pool.GetDevice().GetDevice(), 1, &Draw, VK_TRUE, UINT64_MAX);

  if (Present == VK_NULL_HANDLE) {
    return;
  }

  SwapchID = Swapch.NextImage(Present);

  VkCommandBufferBeginInfo cbbInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = 0,
      .flags = 0};
  vkBeginCommandBuffer(Buffer, &cbbInfo);

  auto image = Swapch.GetImage(SwapchID);

  TransitionImageLayout(Buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, {},
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, image);
  VkRenderingAttachmentInfo raInfo{
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .pNext = 0,
      .imageView = Swapch.GetImageView(SwapchID),
      .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = {0.0f, 0.0f, 0.0f, 1.0f}};

  VkRenderingInfo rInfo{
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .pNext = 0,
      .flags = 0,
      .renderArea = {.offset = {0, 0}, .extent = Swapch.GetCurrentSize()},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &raInfo};
  vkCmdBeginRendering(Buffer, &rInfo);

  VkViewport viewport{
      .x = 0.0f,
      .y = static_cast<float>(Swapch.GetSize().height),
      .width = static_cast<float>(Swapch.GetSize().width),
      .height = static_cast<float>(-(int32_t)(Swapch.GetSize().height)),
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
  };
  vkCmdSetViewport(Buffer, 0, 1, &viewport);

  VkRect2D scissor = {VkOffset2D{0, 0}, Swapch.GetSize()};
  vkCmdSetScissor(Buffer, 0, 1, &scissor);
}
void CommandBuffer::SetCurrentShader(Shader& shader) {
  CurrentShader = &shader;
  vkCmdBindPipeline(Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    CurrentShader->GetPipeline());
}
void CommandBuffer::DrawVertexNotIndexedBuffer(VertexBuffer& buffer,
                                               unsigned int id) {
  VkDeviceSize offset = 0;
  vkCmdBindDescriptorSets(Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          CurrentShader->GetPipelineLayout(), 0, 1,
                          &CurrentShader->GetDescriptorSet(), 0, 0);
  vkCmdBindVertexBuffers(Buffer, 0, 1, &buffer.GetBuffer(), &offset);
  vkCmdDraw(Buffer, buffer.GetVertexCount(), 1, 0, id);
}
void CommandBuffer::EndDraw() {
  vkCmdEndRendering(Buffer);
  TransitionImageLayout(Buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, {},
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                        Swapch.GetImage(SwapchID));
  vkEndCommandBuffer(Buffer);

  vkResetFences(Pool.GetDevice().GetDevice(), 1, &Draw);
  VkPipelineStageFlags waitDestinationStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  const VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                .pNext = 0,
                                .waitSemaphoreCount = 1,
                                .pWaitSemaphores = &Present,
                                .pWaitDstStageMask = &waitDestinationStageMask,
                                .commandBufferCount = 1,
                                .pCommandBuffers = &Buffer,
                                .signalSemaphoreCount = 1,
                                .pSignalSemaphores = &Render};
  vkQueueSubmit(Pool.GetDevice().GetGraphicsQueue(), 1, &submitInfo, Draw);

  const VkPresentInfoKHR presentInfoKHR{
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .pNext = 0,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &Render,
      .swapchainCount = 1,
      .pSwapchains = &Swapch.GetSwapchain(),
      .pImageIndices = &SwapchID};
  vkQueuePresentKHR(Pool.GetDevice().GetPresentQueue(), &presentInfoKHR);
}
void CommandBuffer::TransitionImageLayout(
    VkCommandBuffer& buffer, VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask,
    VkAccessFlags dstAccessMask, VkImageLayout oldLayout,
    VkImageLayout newLayout, VkImage image, VkImageAspectFlags aspect) {
  VkImageMemoryBarrier mb{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                          .pNext = 0,
                          .srcAccessMask = srcAccessMask,
                          .dstAccessMask = dstAccessMask,
                          .oldLayout = oldLayout,
                          .newLayout = newLayout,
                          .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                          .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                          .image = image,
                          .subresourceRange = {.aspectMask = aspect,
                                               .baseMipLevel = 0,
                                               .levelCount = 1,
                                               .baseArrayLayer = 0,
                                               .layerCount = 1}};
  vkCmdPipelineBarrier(buffer, srcStageMask, dstStageMask, {}, 0, nullptr, 0,
                       nullptr, 1, &mb);
}

VkCommandBuffer CommandBuffer::StartSTCommands() {
  VkCommandBuffer buffer = 0;
  VkCommandBufferAllocateInfo cbaInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext = 0,
      .commandPool = *Pool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1};
  vkAllocateCommandBuffers(Pool.GetDevice().GetDevice(), &cbaInfo, &buffer);

  VkCommandBufferBeginInfo cbbInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = 0,
      .flags = 0};
  vkBeginCommandBuffer(buffer, &cbbInfo);

  return buffer;
}
void CommandBuffer::EndSTCommands(VkCommandBuffer& buffer) {
  vkEndCommandBuffer(buffer);

  VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                          .pNext = 0,
                          .commandBufferCount = 1,
                          .pCommandBuffers = &buffer};
  vkQueueSubmit(Pool.GetDevice().GetGraphicsQueue(), 1, &submitInfo, nullptr);
  vkQueueWaitIdle(Pool.GetDevice().GetGraphicsQueue());
}
void CommandBuffer::Release() {
  vkDestroySemaphore(Pool.GetDevice().GetDevice(), Present, 0);
  vkDestroySemaphore(Pool.GetDevice().GetDevice(), Render, 0);
  vkDestroyFence(Pool.GetDevice().GetDevice(), Draw, 0);
  vkFreeCommandBuffers(Pool.GetDevice().GetDevice(), *Pool, 1, &Buffer);
}
