#ifndef REF_SWCN_HPP
#define REF_SWCN_HPP

#include "Device.hpp"
#include "Vulkan.hpp"

class Swapchain final {
 public:
  Swapchain();
  Swapchain(Device* dev, vk::raii::SurfaceKHR* surface);
  Swapchain(vk::raii::SwapchainKHR swch);

  void Create();
  vk::raii::SwapchainKHR& GetSwapchain();

  vk::Format BestFormat(vk::ColorSpaceKHR& colorSpace);
  vk::PresentModeKHR BestPresentMode();

  vk::Format GetCurrentFormat();
  vk::ColorSpaceKHR GetCurrentColorSpace();
  vk::PresentModeKHR GetCurrentPresentMode();
  vk::Image& GetImage(unsigned char i);
  vk::raii::ImageView& GetImageView(unsigned char i);

  void SetNewSurface(vk::raii::SurfaceKHR* surface);

  unsigned int NextImage(vk::raii::Semaphore& PresentSemaphore);

  vk::Extent2D GetSize();

  void Release();

  virtual ~Swapchain() = default;

 private:
  vk::raii::SwapchainKHR Swapch = nullptr;
  Device* Dev;
  vk::raii::SurfaceKHR* Surface = nullptr;

  vk::Format Format;
  vk::ColorSpaceKHR ColorSpace;
  vk::PresentModeKHR PresentMode;
  std::vector<vk::Image> Images;
  std::vector<vk::raii::ImageView> ImageViews;
};

extern Swapchain* MainSwapchain;

#endif
