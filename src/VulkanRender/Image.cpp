#include <webp/decode.h>

#include <Base/ErrorSuccess.hpp>
#include <VulkanRender/Image.hpp>
#include <fstream>

AS::Engine::IError* AS::Engine::EImage::Precache() {
  std::ifstream image(Path, std::ios::ate | std::ios::binary);
  size_t fileSize = image.tellg();
  image.seekg(std::ios::beg);
  char* fileData = new char[fileSize];
  image.read(fileData, fileSize);
  image.close();

  Data = reinterpret_cast<char*>(WebPDecodeRGBA(
      reinterpret_cast<uint8_t*>(fileData), fileSize, &Width, &Height));

  delete[] fileData;
  Loaded = true;
  Depth = 4;

  Img = ::Image(Dev, CBuffer, Width, Height, Depth, Data);

  return new ErrorSuccess;
}
void AS::Engine::EImage::Unload() {
  if (Loaded) WebPFree(Data);
  Width = 0;
  Height = 0;
}
void AS::Engine::EImage::Destroy() {
  if (Loaded) Unload();
  if (Img.GetImage()) Img.Release();
}