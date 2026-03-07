#include <Image.hpp>
#include <cstring>

Image::Image(Device& dev, CommandBuffer& buffer, unsigned int width,
             unsigned int height, unsigned char channels, const char* data)
    : Data(data) {
  Dev = dev;
  CBuffer = buffer;
  Create(dev, buffer, width, height, channels, data);
}

void Image::Create(Device& dev, CommandBuffer& buffer, unsigned int width,
                   unsigned int height, unsigned char channels,
                   const char* data) {
  CreateI(dev, buffer, width, height, channels, VK_FORMAT_R8G8B8A8_SRGB,
          VK_IMAGE_TILING_OPTIMAL,
          VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
  void* out = VideoMem.Map();
  std::memcpy(out, data, width * height * channels);
  VideoMem.Unmap();
  Apply(width, height, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  VkPhysicalDeviceProperties properties = {};
  vkGetPhysicalDeviceProperties(dev.GetPhysicalDevice().GetDevice(),
                                &properties);
  VkSamplerCreateInfo scInfo{
      .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
      .pNext = 0,
      .magFilter = VK_FILTER_LINEAR,
      .minFilter = VK_FILTER_LINEAR,
      .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
      .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
      .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
      .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
      .mipLodBias = 0.0f,
      .anisotropyEnable = VK_TRUE,
      .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
      .compareEnable = VK_FALSE,
      .compareOp = VK_COMPARE_OP_ALWAYS,
      .minLod = 0.0f,
      .maxLod = 0.0f,
      .borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
      .unnormalizedCoordinates = VK_FALSE,
  };
  vkCreateSampler(Dev.GetDevice(), &scInfo, nullptr, &Sampler);
}
void Image::BindToShader(Shader& shader) {
  VkDescriptorImageInfo dbInfo{
      .sampler = Sampler,
      .imageView = ImgView,
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
  VkWriteDescriptorSet wdSet{
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = 0,
      .dstSet = shader.GetDescriptorSet(),
      .dstBinding = 1,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .pImageInfo = &dbInfo};
  vkUpdateDescriptorSets(Dev.GetDevice(), 1, &wdSet, 0, 0);
}
void Image::Release() { vkDestroyImage(Dev.GetDevice(), Img, 0); }