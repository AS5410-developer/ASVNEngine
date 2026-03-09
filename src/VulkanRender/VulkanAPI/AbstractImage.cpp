#include "AbstractImage.hpp"

void AbstractImage::CreateI(Device& dev, CommandBuffer& buffer,
                            unsigned int width, unsigned int height,
                            unsigned char channels, VkFormat format,
                            VkImageTiling tilling, VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties,
                            VkImageAspectFlags aspect) {
  VkImageCreateInfo icInfo{
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext = 0,
      .imageType = VK_IMAGE_TYPE_2D,
      .format = format,
      .extent = {.width = width, .height = height, .depth = 1},
      .mipLevels = 1,
      .arrayLayers = 1,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = tilling,
      .usage = usage,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE};

  vkCreateImage(Dev.GetDevice(), &icInfo, 0, &Img);

  VkMemoryRequirements req;
  vkGetImageMemoryRequirements(Dev.GetDevice(), Img, &req);

  VkBufferCreateInfo bcInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                            .pNext = 0,
                            .flags = 0,
                            .size = req.size,
                            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            .sharingMode = VK_SHARING_MODE_EXCLUSIVE};
  vkCreateBuffer(Dev.GetDevice(), &bcInfo, 0, &Buffer);

  vkGetBufferMemoryRequirements(Dev.GetDevice(), Buffer, &req);
  VideoMem = VideoMemory(dev, bcInfo.size, req,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  vkBindBufferMemory(Dev.GetDevice(), Buffer, VideoMem.GetDeviceMemory(), 0);

  vkGetImageMemoryRequirements(Dev.GetDevice(), Img, &req);

  ImageMem = VideoMemory(dev, req.size, req, properties);
  vkBindImageMemory(Dev.GetDevice(), Img, ImageMem.GetDeviceMemory(), 0);

  VkImageViewCreateInfo ivcInfo{
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .image = Img,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = format,
      .subresourceRange = {aspect, 0, 1, 0, 1}};
  vkCreateImageView(Dev.GetDevice(), &ivcInfo, 0, &ImgView);
}
void AbstractImage::Apply(unsigned int width, unsigned int height,
                          VkImageLayout dest) {
  VkCommandBuffer copyBuffer = CBuffer.StartSTCommands();
  VkBufferImageCopy biCopy{
      .bufferOffset = 0,
      .bufferRowLength = 0,
      .bufferImageHeight = 0,
      .imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
      .imageOffset = {0, 0, 0},
      .imageExtent = {width, height, 1}};
  CBuffer.TransitionImageLayout(
      copyBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT, {}, VK_ACCESS_TRANSFER_WRITE_BIT,
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, Img);
  vkCmdCopyBufferToImage(copyBuffer, Buffer, Img,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &biCopy);
  CBuffer.TransitionImageLayout(
      copyBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT,
      VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dest,
      Img);
  CBuffer.EndSTCommands(copyBuffer);
}
