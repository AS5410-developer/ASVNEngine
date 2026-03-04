#ifndef REF_IMG_HPP
#define REF_IMG_HPP

#include "CommandBuffer.hpp"
#include "Memory.hpp"
#include "Vulkan.hpp"

class AbstractImage {
 public:
  VkImage& GetImage() { return Img; }
  VkImageView& GetImageView() { return ImgView; }
  VkBuffer& GetBuffer() { return Buffer; }

  virtual ~AbstractImage() = default;

 protected:
  void CreateI(Device& dev, CommandBuffer& buffer, unsigned int width,
               unsigned int height, unsigned char channels, VkFormat format,
               VkImageTiling tilling, VkImageUsageFlags usage,
               VkMemoryPropertyFlags properties, VkImageAspectFlags aspect);
  void Apply(unsigned int width, unsigned int height, VkImageLayout dest);

  VkImage Img = nullptr;
  VkImageView ImgView = nullptr;
  VkBuffer Buffer = nullptr;
  VkBuffer IBuffer = nullptr;
  VideoMemory VideoMem;
  VideoMemory ImageMem;
  CommandBuffer CBuffer;
  Device Dev;
};

#endif
