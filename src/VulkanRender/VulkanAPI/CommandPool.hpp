#ifndef REF_COMMAND_POOL_HPP
#define REF_COMMAND_POOL_HPP
#include "Vulkan.hpp"
#include "Device.hpp"

class CommandPool final
{
public:
	CommandPool( );
	CommandPool( Device *device, unsigned int queueFamily );
	CommandPool( vk::raii::CommandPool *pool );

	vk::raii::CommandPool &operator*( );
	Device *GetDevice( );
	unsigned int GetQueueFamily( );

	void Release( );

	virtual ~CommandPool( ) = default;
private:
	vk::raii::CommandPool *Pool = nullptr;
	unsigned int QueueFamily = 0;
	Device *Dev = 0;
};

#endif
