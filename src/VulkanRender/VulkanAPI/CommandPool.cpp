#include "CommandPool.hpp"

CommandPool::CommandPool(Device& device, unsigned int queueFamily)
    : Dev(device), QueueFamily(queueFamily) {
  VkCommandPoolCreateInfo cpcInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .pNext = 0,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = queueFamily};
  vkCreateCommandPool(Dev.GetDevice(), &cpcInfo, 0, &Pool);
}

void CommandPool::Release() {
  vkDestroyCommandPool(Dev.GetDevice(), Pool, 0);
  Pool = 0;
}
