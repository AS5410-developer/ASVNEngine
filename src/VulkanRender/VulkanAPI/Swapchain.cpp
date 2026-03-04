#include "Swapchain.hpp"

#include <algorithm>

#include "Engine.hpp"

Swapchain::Swapchain(Device& dev, VkSurfaceKHR surface)
    : Dev(dev), Surface(surface) {
  Create();
}

VkFormat Swapchain::BestFormat(VkColorSpaceKHR& colorSpace) {
  unsigned int count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(Dev.GetPhysicalDevice().GetDevice(),
                                       Surface, &count, 0);
  std::vector<VkSurfaceFormatKHR> formats(count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(Dev.GetPhysicalDevice().GetDevice(),
                                       Surface, &count, formats.data());
  for (const auto& format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      colorSpace = format.colorSpace;
      return format.format;
    }
  }
  colorSpace = formats[0].colorSpace;
  return formats[0].format;
}

VkExtent2D Swapchain::GetSize() {
  VkSurfaceCapabilitiesKHR caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Dev.GetPhysicalDevice().GetDevice(),
                                            Surface, &caps);
  return VkExtent2D{std::clamp<unsigned int>(MainEngine->GetWindowSize().x,
                                             caps.minImageExtent.width,
                                             caps.maxImageExtent.width),
                    std::clamp<unsigned int>(MainEngine->GetWindowSize().y,
                                             caps.minImageExtent.height,
                                             caps.maxImageExtent.height)};
}

void Swapchain::Create() {
  if (!Surface) {
    return;
  }
  unsigned int count = 0;
  Format = BestFormat(ColorSpace);
  PresentMode = BestPresentMode();
  VkSurfaceCapabilitiesKHR caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Dev.GetPhysicalDevice().GetDevice(),
                                            Surface, &caps);

  VkSwapchainCreateInfoKHR swInfo{
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .pNext = 0,
      .flags = VkSwapchainCreateFlagsKHR(),
      .surface = Surface,
      .minImageCount =
          caps.maxImageCount > 0
              ? std::min(std::max(3u, caps.minImageCount), caps.maxImageCount)
              : caps.minImageCount,
      .imageFormat = Format,
      .imageColorSpace = ColorSpace,
      .imageExtent = GetSize(),
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
      .preTransform = caps.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = PresentMode,
      .clipped = true,
      .oldSwapchain = nullptr,
  };
  if (!Dev.IsPresentEqualsGraphics()) {
    unsigned int queues[2] = {Dev.GetGraphicsID(), Dev.GetPresentID()};
    swInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swInfo.queueFamilyIndexCount = 2;
    swInfo.pQueueFamilyIndices = queues;
  }
  vkCreateSwapchainKHR(Dev.GetDevice(), &swInfo, 0, &Swapch);

  vkGetSwapchainImagesKHR(Dev.GetDevice(), Swapch, &count, 0);
  Images.reserve(count);
  vkGetSwapchainImagesKHR(Dev.GetDevice(), Swapch, &count, Images.data());

  count = 0;
  VkImageViewCreateInfo ivcInfo{
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = Format,
      .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};
  for (auto& image : Images) {
    ivcInfo.image = image;
    vkCreateImageView(Dev.GetDevice(), &ivcInfo, 0, &ImageViews[count]);
    ++count;
  }
}

unsigned int Swapchain::NextImage(VkSemaphore& PresentSemaphore) {
  unsigned int id;
  vkAcquireNextImageKHR(Dev.GetDevice(), Swapch, UINT64_MAX, PresentSemaphore,
                        0, &id);
  return id;
}

VkPresentModeKHR Swapchain::BestPresentMode() {
  unsigned int count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(Dev.GetPhysicalDevice().GetDevice(),
                                            Surface, &count, 0);
  std::vector<VkPresentModeKHR> modes(count);
  vkGetPhysicalDeviceSurfacePresentModesKHR(Dev.GetPhysicalDevice().GetDevice(),
                                            Surface, &count, modes.data());

  for (auto& mode : modes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return mode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}
void Swapchain::Release() {
  for (auto ImageView : ImageViews) {
    vkDestroyImageView(Dev.GetDevice(), ImageView, 0);
  }
  vkDestroySwapchainKHR(Dev.GetDevice(), Swapch, 0);
  Swapch = 0;
}
