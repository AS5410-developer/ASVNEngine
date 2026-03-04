#ifndef REF_DEPTH_HPP
#define REF_DEPTH_HPP

#include "AbstractImage.hpp"
#include "Vulkan.hpp"

class Depth : public AbstractImage {
 public:
  Depth();
  Depth(Device* dev, CommandBuffer* buffer, unsigned int width,
        unsigned int height);

  void Create(Device* dev, CommandBuffer* buffer, unsigned int width,
              unsigned int height);
  vk::Format GetFormat();
  void Release();

  virtual ~Depth() = default;

 private:
  vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates,
                                 vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features);
  vk::Format CurrentFormat;
};

#endif
