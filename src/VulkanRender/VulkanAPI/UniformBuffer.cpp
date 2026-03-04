#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer() {}
UniformBuffer::UniformBuffer(Device* dev, Shader* shader,
                             std::vector<char> data)
    : Dev(dev), Shad(shader), Data(data) {
  Create();
}
UniformBuffer::UniformBuffer(vk::raii::Buffer buf) : Buf(std::move(buf)) {}

void UniformBuffer::Create() {
  vk::BufferCreateInfo bcInfo{.size = Data.size(),
                              .usage = vk::BufferUsageFlagBits::eUniformBuffer,
                              .sharingMode = vk::SharingMode::eExclusive};
  Buf = vk::raii::Buffer(Dev->GetDevice(), bcInfo);
  VideoMem = new VideoMemory(Dev, bcInfo.size, Buf.getMemoryRequirements(),
                             vk::MemoryPropertyFlagBits::eHostVisible |
                                 vk::MemoryPropertyFlagBits::eHostCoherent);
  Buf.bindMemory(VideoMem->GetDeviceMemory(), 0);
  Pointer = VideoMem->Map();
  memcpy(Pointer, Data.data(), Data.size() * sizeof(Vertex));

  vk::DescriptorPoolSize poolSize{vk::DescriptorType::eUniformBuffer, 1};
  vk::DescriptorPoolCreateInfo poolInfo{
      .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
      .maxSets = 1,
      .poolSizeCount = 1,
      .pPoolSizes = &poolSize};
  DescriptorPool = vk::raii::DescriptorPool(Dev->GetDevice(), poolInfo);
  std::vector<vk::DescriptorSetLayout> layouts(1,
                                               *Shad->GetDescriptorSetLayout());
  vk::DescriptorSetAllocateInfo allocInfo{
      .descriptorPool = DescriptorPool,
      .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
      .pSetLayouts = layouts.data()};
  DescriptorSet =
      std::move(Dev->GetDevice().allocateDescriptorSets(allocInfo).front());
  vk::DescriptorBufferInfo bufferInfo{
      .buffer = Buf, .offset = 0, .range = Data.size()};
  vk::WriteDescriptorSet descriptorWrite{
      .dstSet = DescriptorSet,
      .dstBinding = 0,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = vk::DescriptorType::eUniformBuffer,
      .pBufferInfo = &bufferInfo};
  Dev->GetDevice().updateDescriptorSets(descriptorWrite, {});
}
vk::raii::Buffer& UniformBuffer::GetBuffer() { return Buf; }
VideoMemory& UniformBuffer::GetVideoMemory() { return *VideoMem; }
void* UniformBuffer::GetPointer() { return Pointer; }
void UniformBuffer::Bind(CommandBuffer& cb) {
  cb.GetBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                    Shad->GetPipelineLayout(), 0,
                                    *DescriptorSet, nullptr);
}
void UniformBuffer::Release() {
  VideoMem->Release();
  Buf = nullptr;
}
