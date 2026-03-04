#include "CommandBuffer.hpp"

CommandBuffer::CommandBuffer() {}
CommandBuffer::CommandBuffer(CommandPool* pool, Swapchain* swapchain)
    : Swapch(swapchain), Pool(pool) {
  vk::CommandBufferAllocateInfo cbaInfo{
      .commandPool = **pool,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = 1};
  Buffer = std::move(
      vk::raii::CommandBuffers(pool->GetDevice()->GetDevice(), cbaInfo)
          .front());

  Present = vk::raii::Semaphore(pool->GetDevice()->GetDevice(),
                                vk::SemaphoreCreateInfo{});
  Render = vk::raii::Semaphore(pool->GetDevice()->GetDevice(),
                               vk::SemaphoreCreateInfo{});
  Draw = vk::raii::Fence(pool->GetDevice()->GetDevice(),
                         {.flags = vk::FenceCreateFlagBits::eSignaled});
}
CommandBuffer::CommandBuffer(vk::raii::CommandBuffer cb)
    : Buffer(std::move(cb)) {}

vk::raii::CommandBuffer& CommandBuffer::GetBuffer() { return Buffer; }

void CommandBuffer::WaitDeviceIdle() {
  Pool->GetDevice()->GetDevice().waitIdle();
}

void CommandBuffer::StartDraw(vk::ImageView& depthBuffer, vk::Image& depthImg) {
  Pool->GetDevice()->GetDevice().waitForFences(*Draw, vk::True, UINT64_MAX);

  if (*Present == VK_NULL_HANDLE) {
    return;
  }

  SwapchID = Swapch->NextImage(Present);
  Buffer.begin({});

  TransitionImageLayout(
      Buffer, vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eColorAttachmentOutput, {},
      vk::AccessFlagBits::eColorAttachmentWrite, vk::ImageLayout::eUndefined,
      vk::ImageLayout::eColorAttachmentOptimal, Swapch->GetImage(SwapchID));
  TransitionImageLayout(Buffer,
                        vk::PipelineStageFlagBits::eEarlyFragmentTests |
                            vk::PipelineStageFlagBits::eLateFragmentTests,
                        vk::PipelineStageFlagBits::eEarlyFragmentTests |
                            vk::PipelineStageFlagBits::eLateFragmentTests,
                        vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                        vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eDepthAttachmentOptimal, depthImg,
                        vk::ImageAspectFlagBits::eDepth);
  vk::RenderingAttachmentInfo raInfo{
      .imageView = Swapch->GetImageView(SwapchID),
      .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
      .clearValue = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::RenderingAttachmentInfo radInfo = {
      .imageView = depthBuffer,
      .imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eDontCare,
      .clearValue = vk::ClearDepthStencilValue{1.0f, 0}};
  vk::RenderingInfo rInfo{
      .renderArea = {.offset = {0, 0}, .extent = Swapch->GetSize()},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &raInfo,
      .pDepthAttachment = &radInfo};
  Buffer.beginRendering(rInfo);
  Buffer.setViewport(
      0, vk::Viewport{0.0f, static_cast<float>(Swapch->GetSize().height),
                      static_cast<float>(Swapch->GetSize().width),
                      static_cast<float>(-(int32_t)(Swapch->GetSize().height)),
                      0.0f, 1.0f});
  Buffer.setScissor(0, vk::Rect2D{vk::Offset2D{0, 0}, Swapch->GetSize()});
}
void CommandBuffer::SetCurrentShader(Shader& shader) {
  CurrentShader = &shader;
  Buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, shader.GetPipeline());
}
void CommandBuffer::DrawVertexNotIndexedBuffer(VertexBuffer& buffer) {
  Buffer.bindVertexBuffers(0, *buffer.GetBuffer(), {0});
  Buffer.draw(buffer.GetVertexCount(), 1, 0, 0);
}
void CommandBuffer::EndDraw() {
  Buffer.endRendering();
  TransitionImageLayout(
      Buffer, vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eBottomOfPipe,
      vk::AccessFlagBits::eColorAttachmentWrite, {},
      vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR,
      Swapch->GetImage(SwapchID));
  Buffer.end();

  Pool->GetDevice()->GetDevice().resetFences(*Draw);
  vk::PipelineStageFlags waitDestinationStageMask(
      vk::PipelineStageFlagBits::eColorAttachmentOutput);
  const vk::SubmitInfo submitInfo{
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &*Present,
      .pWaitDstStageMask = &waitDestinationStageMask,
      .commandBufferCount = 1,
      .pCommandBuffers = &*Buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = &*Render};
  Pool->GetDevice()->GetGraphicsQueue().submit(submitInfo, *Draw);

  const vk::PresentInfoKHR presentInfoKHR{
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &*Render,
      .swapchainCount = 1,
      .pSwapchains = &*Swapch->GetSwapchain(),
      .pImageIndices = &SwapchID};
  Pool->GetDevice()->GetPresentQueue().presentKHR(presentInfoKHR);
}
void CommandBuffer::TransitionImageLayout(
    vk::raii::CommandBuffer& buffer, vk::PipelineStageFlags srcStageMask,
    vk::PipelineStageFlags dstStageMask, vk::AccessFlags srcAccessMask,
    vk::AccessFlags dstAccessMask, vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout, vk::Image image, vk::ImageAspectFlags aspect) {
  vk::ImageMemoryBarrier mb{.srcAccessMask = srcAccessMask,
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
  buffer.pipelineBarrier(srcStageMask, dstStageMask, {}, nullptr, nullptr, mb);
}

vk::raii::CommandBuffer CommandBuffer::StartSTCommands() {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = **Pool,
      .level = vk::CommandBufferLevel::ePrimary,
      .commandBufferCount = 1};
  vk::raii::CommandBuffer buffer = std::move(
      Pool->GetDevice()->GetDevice().allocateCommandBuffers(allocInfo).front());

  vk::CommandBufferBeginInfo beginInfo{
      .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
  buffer.begin(beginInfo);

  return buffer;
}
void CommandBuffer::EndSTCommands(vk::raii::CommandBuffer& buffer) {
  buffer.end();

  vk::SubmitInfo submitInfo{.commandBufferCount = 1,
                            .pCommandBuffers = &*buffer};
  Pool->GetDevice()->GetGraphicsQueue().submit(submitInfo, nullptr);
  Pool->GetDevice()->GetGraphicsQueue().waitIdle();
}
