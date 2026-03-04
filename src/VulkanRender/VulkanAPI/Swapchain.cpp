#include "Swapchain.hpp"

#include "Engine.hpp"

Swapchain::Swapchain() {}
Swapchain::Swapchain(Device* dev, vk::raii::SurfaceKHR* surface)
    : Dev(dev), Surface(std::move(surface)) {
  Create();
}
Swapchain::Swapchain(vk::raii::SwapchainKHR swch) : Swapch(std::move(swch)) {}

vk::Format Swapchain::BestFormat(vk::ColorSpaceKHR& colorSpace) {
  auto formats =
      Dev->GetPhysicalDevice().GetDevice().getSurfaceFormatsKHR(**Surface);
  for (const auto& format : formats) {
    if (format.format == vk::Format::eB8G8R8A8Srgb &&
        format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      colorSpace = format.colorSpace;
      return format.format;
    }
  }
  colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
  return vk::Format::eB8G8R8A8Srgb;
}
vk::Image& Swapchain::GetImage(unsigned char i) { return Images[i]; }
vk::raii::ImageView& Swapchain::GetImageView(unsigned char i) {
  return ImageViews[i];
}

vk::Extent2D Swapchain::GetSize() {
  auto caps =
      Dev->GetPhysicalDevice().GetDevice().getSurfaceCapabilitiesKHR(**Surface);
  return vk::Extent2D{
      std::clamp<uint32_t>(Engine::GetInstance().GetGlobals()->width,
                           caps.minImageExtent.width,
                           caps.maxImageExtent.width),
      std::clamp<uint32_t>(Engine::GetInstance().GetGlobals()->height,
                           caps.minImageExtent.height,
                           caps.maxImageExtent.height)};
}

void Swapchain::Create() {
  if (!**Surface) {
    return;
  }
  Format = BestFormat(ColorSpace);
  PresentMode = BestPresentMode();
  auto caps =
      Dev->GetPhysicalDevice().GetDevice().getSurfaceCapabilitiesKHR(**Surface);

  vk::SwapchainCreateInfoKHR swInfo{
      .flags = vk::SwapchainCreateFlagsKHR(),
      .surface = **Surface,
      .minImageCount =
          caps.maxImageCount > 0
              ? std::min(std::max(3u, caps.minImageCount), caps.maxImageCount)
              : caps.minImageCount,
      .imageFormat = Format,
      .imageColorSpace = ColorSpace,
      .imageExtent = GetSize(),
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
      .preTransform = caps.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode = PresentMode,
      .clipped = true,
      .oldSwapchain = nullptr,
  };
  if (!Dev->IsPresentEqualsGraphics()) {
    unsigned int queues[2] = {Dev->GetGraphicsID(), Dev->GetPresentID()};
    swInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    swInfo.queueFamilyIndexCount = 2;
    swInfo.pQueueFamilyIndices = queues;
  }
  Swapch = vk::raii::SwapchainKHR(Dev->GetDevice(), swInfo);
  Images = Swapch.getImages();

  vk::ImageViewCreateInfo ivcInfo{
      .viewType = vk::ImageViewType::e2D,
      .format = Format,
      .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};
  for (auto& image : Images) {
    ivcInfo.image = image;
    ImageViews.emplace_back(Dev->GetDevice(), ivcInfo);
  }
}

unsigned int Swapchain::NextImage(vk::raii::Semaphore& PresentSemaphore) {
  auto [result, id] =
      Swapch.acquireNextImage(UINT64_MAX, PresentSemaphore, nullptr);
  return id;
}

void Swapchain::SetNewSurface(vk::raii::SurfaceKHR* surface) {
  Surface = surface;
}

vk::PresentModeKHR Swapchain::BestPresentMode() {
  auto modes =
      Dev->GetPhysicalDevice().GetDevice().getSurfacePresentModesKHR(**Surface);

  for (auto& mode : modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      return mode;
    }
  }

  return vk::PresentModeKHR::eFifo;
}

vk::Format Swapchain::GetCurrentFormat() { return Format; }
vk::ColorSpaceKHR Swapchain::GetCurrentColorSpace() { return ColorSpace; }
vk::raii::SwapchainKHR& Swapchain::GetSwapchain() { return Swapch; }
void Swapchain::Release() {
  ImageViews.clear();
  Swapch = nullptr;
}
