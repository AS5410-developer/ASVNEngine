#ifndef REF_IMG_HPP
#define REF_IMG_HPP

#include "CommandBuffer.hpp"
#include "Memory.hpp"
#include "Vulkan.hpp"

class AbstractImage {
 public:
  vk::raii::Image& GetImage();
  vk::raii::ImageView& GetImageView();
  vk::raii::Buffer& GetBuffer();

  virtual ~AbstractImage() = default;

 protected:
  void CreateI(Device* dev, CommandBuffer* buffer, unsigned int width,
               unsigned int height, unsigned char channels, vk::Format format,
               vk::ImageTiling tilling, vk::ImageUsageFlags usage,
               vk::MemoryPropertyFlags properties, vk::ImageAspectFlags aspect);
  void Apply(unsigned int width, unsigned int height, vk::ImageLayout dest);

  vk::raii::Image Img = nullptr;
  vk::raii::ImageView ImgView = nullptr;
  vk::raii::Buffer Buffer = nullptr;
  vk::raii::Buffer IBuffer = nullptr;
  VideoMemory* VideoMem;
  VideoMemory* ImageMem;
  CommandBuffer* CBuffer;
  Device* Dev;
};

#endif
