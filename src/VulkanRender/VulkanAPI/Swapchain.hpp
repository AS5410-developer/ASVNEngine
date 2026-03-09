#ifndef REF_SWCN_HPP
#define REF_SWCN_HPP

#include "Device.hpp"
#include "Vulkan.hpp"

class Swapchain final {
 public:
  Swapchain() {}
  Swapchain(Device& dev, VkSurfaceKHR surface);
  Swapchain(VkSwapchainKHR swch) : Swapch(swch) {}

  void Create();
  VkSwapchainKHR& GetSwapchain() { return Swapch; }

  VkFormat BestFormat(VkColorSpaceKHR& colorSpace);
  VkPresentModeKHR BestPresentMode();

  VkFormat GetCurrentFormat() { return Format; }
  VkColorSpaceKHR GetCurrentColorSpace() { return ColorSpace; }
  VkPresentModeKHR GetCurrentPresentMode() { return PresentMode; }
  VkImage& GetImage(unsigned char i) { return Images[i]; }
  VkImageView& GetImageView(unsigned char i) { return ImageViews[i]; }

  void SetNewSurface(VkSurfaceKHR surface) { Surface = surface; }

  unsigned int NextImage(VkSemaphore& PresentSemaphore);

  VkExtent2D GetSize();
  VkExtent2D GetCurrentSize() { return CurrentSize; }

  void Release();

  virtual ~Swapchain() = default;

 private:
  VkSwapchainKHR Swapch = nullptr;
  Device Dev;
  VkSurfaceKHR Surface = nullptr;

  VkFormat Format;
  VkExtent2D CurrentSize;
  VkColorSpaceKHR ColorSpace;
  VkPresentModeKHR PresentMode;
  std::vector<VkImage> Images;
  std::vector<VkImageView> ImageViews;
};

extern Swapchain* MainSwapchain;

#endif
