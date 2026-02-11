#include <webp/decode.h>

#include <Base/FileNotFound.hpp>
#include <OpenGL/Image.hpp>
#include <fstream>

using namespace AS::Engine;

Image::Image() {}

void Image::SetPath(const std::string& path) { Path = path; }
const std::string Image::GetPath() { return Path; }

IError* Image::Precache() {
  if (Path.empty() || Loaded) return new FileNotFoundError;

  std::ifstream file("image\\" + Path, std::ios::binary);
  if (file.is_open()) return new FileNotFoundError("image\\" + Path);

  file.seekg(0, std::ios::end);

  unsigned long long WebPSize = file.tellg();

  file.seekg(0, std::ios::beg);

  char* WebPBuffer = new char[Size];

  file.read(Buffer, Size);

  file.close();

  Buffer = reinterpret_cast<char*>(WebPDecodeRGBA(
      reinterpret_cast<uint8_t*>(WebPBuffer), WebPSize,
      reinterpret_cast<int*>(&Width), reinterpret_cast<int*>(&Height)));
  Depth = 4;
  Size = Width * Height * Depth;
  Loaded = true;
}
bool Image::IsLoaded() { return Loaded; }
unsigned long long Image::GetWidth() { return Width; }
unsigned long long Image::GetHeight() { return Height; }
unsigned char Image::GetDepth() { return Depth; }
void Image::Unload() {
  if (!Loaded) return;
  Height = 0;
  Width = 0;
  Depth = 0;
  WebPFree(Buffer);
  Size = 0;
  Loaded = false;
}
void Image::Destroy() {
  if (Loaded) Unload();
}