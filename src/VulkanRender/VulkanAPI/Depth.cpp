#include "Depth.hpp"

Depth::Depth(Device& dev, CommandBuffer& buffer, unsigned int width,
             unsigned int height) {
  Dev = dev;
  CBuffer = buffer;
  Create(dev, buffer, width, height);
}

void Depth::Create(Device& dev, CommandBuffer& buffer, unsigned int width,
                   unsigned int height) {
  CurrentFormat = FindSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  CreateI(dev, buffer, width, height, 1, CurrentFormat, VK_IMAGE_TILING_OPTIMAL,
          VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
}
void Depth::Release() {
  Img = nullptr;
  ImgView = nullptr;
  Buffer = nullptr;
  IBuffer = nullptr;
  VideoMem.Release();
  ImageMem.Release();
}

VkFormat Depth::FindSupportedFormat(const std::vector<VkFormat>& candidates,
                                    VkImageTiling tiling,
                                    VkFormatFeatureFlags features) {
  for (const auto format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(Dev.GetPhysicalDevice().GetDevice(),
                                        format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    }
    if (tiling == VK_IMAGE_TILING_OPTIMAL &&
        (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  return VK_FORMAT_D32_SFLOAT;
}
