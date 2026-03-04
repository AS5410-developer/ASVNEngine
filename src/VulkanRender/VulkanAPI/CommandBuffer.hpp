#ifndef REF_COMMAND_BUFFER_HPP
#define REF_COMMAND_BUFFER_HPP
#include "Vulkan.hpp"
#include "Device.hpp"
#include "CommandPool.hpp"
#include "Swapchain.hpp"
#include "VertexBuffer.hpp"

class CommandBuffer final
{
public:
	CommandBuffer( );
	CommandBuffer( CommandPool *pool, Swapchain *swapchain );
	CommandBuffer( vk::raii::CommandBuffer cb );

	vk::raii::CommandBuffer &GetBuffer( );

	void WaitDeviceIdle( );

	void StartDraw( vk::ImageView &depthBuffer, vk::Image &depthImg );
	void SetCurrentShader( Shader &shader );
	template <typename T>
	void PushConstant( T &data )
	{
		Buffer.pushConstants<T>(
			*CurrentShader->GetPipelineLayout( ),
			vk::ShaderStageFlagBits::eVertex,
			0,
			data
		);
	}
	vk::raii::CommandBuffer StartSTCommands( );
	void EndSTCommands( vk::raii::CommandBuffer &buffer );
	void DrawVertexNotIndexedBuffer( VertexBuffer &buffer );
	void EndDraw( );
	void TransitionImageLayout( vk::raii::CommandBuffer &buffer,
		vk::PipelineStageFlags srcStageMask,
		vk::PipelineStageFlags dstStageMask,
		vk::AccessFlags srcAccessMask,
		vk::AccessFlags dstAccessMask,
		vk::ImageLayout oldLayout,
		vk::ImageLayout newLayout,
		vk::Image image,
		vk::ImageAspectFlags aspect = vk::ImageAspectFlagBits::eColor );

	virtual ~CommandBuffer( ) = default;
private:


	vk::raii::CommandBuffer Buffer = nullptr;
	CommandPool *Pool;
	Swapchain *Swapch;

	unsigned int SwapchID = 0;

	vk::raii::Semaphore Present = nullptr;
	vk::raii::Semaphore Render = nullptr;
	vk::raii::Fence Draw = nullptr;
	Shader *CurrentShader;
};

#endif
