#ifndef REF_DEPTH_HPP
#define REF_DEPTH_HPP

#include "AbstractImage.hpp"
#include "Vulkan.hpp"

class Depth : public AbstractImage {
 public:
  Depth() {}
  Depth(Device& dev, CommandBuffer& buffer, unsigned int width,
        unsigned int height);

  void Create(Device& dev, CommandBuffer& buffer, unsigned int width,
              unsigned int height);
  VkFormat GetFormat() { return CurrentFormat; }
  void Release();

  virtual ~Depth() = default;

 private:
  VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat CurrentFormat;
};

#endif
