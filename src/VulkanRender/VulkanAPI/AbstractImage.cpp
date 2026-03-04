#include "AbstractImage.hpp"

vk::raii::Image &AbstractImage::GetImage( )
{
	return Img;
}
vk::raii::ImageView &AbstractImage::GetImageView( )
{
	return ImgView;
}
vk::raii::Buffer &AbstractImage::GetBuffer( )
{
	return Buffer;
}

void AbstractImage::CreateI( Device *dev, CommandBuffer *buffer, unsigned int width, unsigned int height, unsigned char channels, vk::Format format, vk::ImageTiling tilling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::ImageAspectFlags aspect )
{
	vk::ImageCreateInfo icInfo{
		.imageType = vk::ImageType::e2D,
		.format = format,
		.extent = {
			.width = width,
			.height = height,
			.depth = channels
		},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = vk::SampleCountFlagBits::e1,
		.tiling = tilling,
		.usage = usage,
		.sharingMode = vk::SharingMode::eExclusive
	};

	Img = vk::raii::Image( dev->GetDevice( ), icInfo );

	vk::BufferCreateInfo bcInfo{
		.size = Img.getMemoryRequirements( ).size,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.sharingMode = vk::SharingMode::eExclusive
	};
	Buffer = vk::raii::Buffer( Dev->GetDevice( ), bcInfo );
	VideoMem = new VideoMemory( dev, bcInfo.size, Buffer.getMemoryRequirements( ), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
	Buffer.bindMemory( VideoMem->GetDeviceMemory( ), 0 );

	ImageMem = new VideoMemory( dev, Img.getMemoryRequirements( ).size, Img.getMemoryRequirements( ), properties );
	Img.bindMemory( ImageMem->GetDeviceMemory( ), 0 );

	vk::ImageViewCreateInfo ivcInfo{ .image = Img,
		.viewType = vk::ImageViewType::e2D,
		.format = format,
		.subresourceRange = { aspect, 0, 1, 0, 1 }
	};
	ImgView = vk::raii::ImageView( Dev->GetDevice( ), ivcInfo );

}
void AbstractImage::Apply( unsigned int width, unsigned int height, vk::ImageLayout dest )
{
	vk::raii::CommandBuffer copyBuffer = CBuffer->StartSTCommands( );
	vk::BufferImageCopy biCopy{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {
			vk::ImageAspectFlagBits::eColor,
			0, 0, 1
		},
		.imageOffset = {0, 0, 0}, .imageExtent = {width, height, 1}
	};
	CBuffer->TransitionImageLayout( copyBuffer, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, vk::AccessFlagBits::eTransferWrite, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, Img );
	copyBuffer.copyBufferToImage( Buffer, Img, vk::ImageLayout::eTransferDstOptimal, biCopy );
	CBuffer->TransitionImageLayout( copyBuffer, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead, vk::ImageLayout::eTransferDstOptimal, dest, Img );
	CBuffer->EndSTCommands( copyBuffer );
}
