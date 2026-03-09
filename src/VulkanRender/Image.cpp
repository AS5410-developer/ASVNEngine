#include <webp/decode.h>

#include <Base/ErrorSuccess.hpp>
#include <Base/FileSystemError.hpp>
#include <VulkanRender/Image.hpp>
#include <fstream>

AS::Engine::IError* AS::Engine::EImage::Precache() {
  auto file = MainEngine->GetFileSystem().CreateFile();
  auto& err = file->Open(Path.c_str(), AS_ENGINE_FILE_SYSTEM_GAMEDIR |
                                           AS_ENGINE_FILE_SYSTEM_RAW |
                                           AS_ENGINE_FILE_SYSTEM_READ);
  if (err.Failed()) return new ErrorFileSystem(err.What());
  auto size = file->GetFileSize();
  auto result = file->ReadAll();
  file->Close();
  if (result.Failed()) return new ErrorFileSystem(result.What());

  Data = reinterpret_cast<char*>(WebPDecodeRGBA(
      reinterpret_cast<uint8_t*>(result.GetResult()), size, &Width, &Height));

  delete[] result.GetResult();
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