#ifndef REF_CIMG_HPP
#define REF_CIMG_HPP

#include "AbstractImage.hpp"

class Image final : public AbstractImage {
 public:
  Image() {}
  Image(Device& dev, CommandBuffer& buffer, unsigned int width,
        unsigned int height, unsigned char channels, const char* data);
  Image(VkImage& image) { Img = image; }

  void Create(Device& dev, CommandBuffer& buffer, unsigned int width,
              unsigned int height, unsigned char channels, const char* data);
  void Release();

  VkSampler& GetSampler() { return Sampler; }

  void BindToShader(Shader& shader);

  virtual ~Image() = default;

 private:
  VkSampler Sampler;
  const char* Data;
};

#endif
