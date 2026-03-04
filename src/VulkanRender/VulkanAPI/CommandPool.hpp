#ifndef REF_COMMAND_POOL_HPP
#define REF_COMMAND_POOL_HPP
#include "Device.hpp"
#include "Vulkan.hpp"

class CommandPool final {
 public:
  CommandPool() {}
  CommandPool(Device& device, unsigned int queueFamily);
  CommandPool(VkCommandPool pool) : Pool(pool) {}

  VkCommandPool& operator*() { return Pool; }
  Device& GetDevice() { return Dev; }
  unsigned int GetQueueFamily() { return QueueFamily; }

  void Release();

  virtual ~CommandPool() = default;

 private:
  VkCommandPool Pool = nullptr;
  unsigned int QueueFamily = 0;
  Device Dev;
};

#endif
