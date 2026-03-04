#include "CommandPool.hpp"

CommandPool::CommandPool( ) { }

CommandPool::CommandPool( Device *device, unsigned int queueFamily ) : QueueFamily( queueFamily )
{
	Dev = device;
	vk::CommandPoolCreateInfo cpcInfo{
		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = queueFamily
	};
	Pool = new vk::raii::CommandPool( device->GetDevice( ), cpcInfo );
}
CommandPool::CommandPool( vk::raii::CommandPool *pool ) : Pool( pool )
{ }

vk::raii::CommandPool &CommandPool::operator *( )
{
	return *Pool;
}
Device *CommandPool::GetDevice( )
{
	return Dev;
}
void CommandPool::Release( )
{
	Pool->release( );
	Pool = 0;
}
unsigned int CommandPool::GetQueueFamily( )
{
	return QueueFamily;
}
