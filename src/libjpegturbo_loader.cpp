#include "snapdecision/libjpegturbo_loader.h"
#include <iostream>

extern "C"
{
#include <turbojpeg.h>
}
#include <QDebug>
#include <QImage>

static QImage loadJpegWithLibjpegTurbo(const QString& filename)
{
  tjhandle decompressor = tjInitDecompress();
  if (!decompressor)
  {
    throw std::runtime_error("Failed to initialize TurboJPEG decompressor");
  }

  FILE* jpegFile = fopen(filename.toStdString().c_str(), "rb");
  if (!jpegFile)
  {
    tjDestroy(decompressor);
    throw std::runtime_error("Failed to open JPEG file");
  }

  fseek(jpegFile, 0, SEEK_END);
  long size = ftell(jpegFile);
  fseek(jpegFile, 0, SEEK_SET);

  unsigned char* jpegBuffer = reinterpret_cast<unsigned char*>(tjAlloc(size));
  if (fread(jpegBuffer, 1, size, jpegFile) < size)
  {
    tjFree(jpegBuffer);
    fclose(jpegFile);
    tjDestroy(decompressor);
    throw std::runtime_error("Failed to read JPEG file");
  }

  fclose(jpegFile);

  int width, height;
  if (tjDecompressHeader(decompressor, jpegBuffer, size, &width, &height) < 0)
  {
    tjFree(jpegBuffer);
    tjDestroy(decompressor);
    throw std::runtime_error("Failed to read JPEG header");
  }

  QImage image(width, height, QImage::Format_RGB888);
  if (tjDecompress2(decompressor, jpegBuffer, size, image.bits(), width, image.bytesPerLine(), height, TJPF_RGB, 0) < 0)
  {
    tjFree(jpegBuffer);
    tjDestroy(decompressor);
    throw std::runtime_error("Failed to decompress JPEG");
  }

  tjFree(jpegBuffer);
  tjDestroy(decompressor);

  return image;
}

QImage libjpegturboOpen(const QString& filename)
{
  try
  {
    return loadJpegWithLibjpegTurbo(filename);
  }
  catch (const std::runtime_error&)
  {
    return QImage();
  }
}
