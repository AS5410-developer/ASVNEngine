#include "Depth.hpp"
Depth::Depth( ) { }

Depth::Depth( Device *dev, CommandBuffer *buffer, unsigned int width, unsigned int height )
{
	Dev = dev;
	CBuffer = buffer;
	Create( dev, buffer, width, height );
}
vk::Format Depth::GetFormat( )
{
	return CurrentFormat;
}
void Depth::Create( Device *dev, CommandBuffer *buffer, unsigned int width, unsigned int height )
{
	CurrentFormat = FindSupportedFormat(
			{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
			vk::ImageTiling::eOptimal,
			vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
	CreateI(
		dev,
		buffer,
		width,
		height,
		1,
		CurrentFormat,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		vk::ImageAspectFlagBits::eDepth
	);
}
void Depth::Release( )
{
	Img = nullptr;
	ImgView = nullptr;
	Buffer = nullptr;
	IBuffer = nullptr;
	VideoMem->Release( );
	ImageMem->Release( );
}

vk::Format Depth::FindSupportedFormat( const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features )
{
	for( const auto format : candidates )
	{
		vk::FormatProperties props = Dev->GetPhysicalDevice( ).GetDevice( ).getFormatProperties( format );

		if( tiling == vk::ImageTiling::eLinear && ( props.linearTilingFeatures & features ) == features )
		{
			return format;
		}
		if( tiling == vk::ImageTiling::eOptimal && ( props.optimalTilingFeatures & features ) == features )
		{
			return format;
		}
	}
	return vk::Format::eD32Sfloat;
}
