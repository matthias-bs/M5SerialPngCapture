// Copyright (c) Matthias Prinke. All rights reserved.
// Licensed under the MIT license. See LICENCE file in the project root.

#include "M5SerialPngCapture.h"

#include <limits.h>

M5SerialPngCapture::M5SerialPngCapture(Stream& output)
  : _output(output)
{
}

bool M5SerialPngCapture::_write(const uint8_t* data, size_t length)
{
  while (length > 0)
  {
    const size_t written = _output.write(data, length);
    if (written == 0)
    {
      return false;
    }
    data += written;
    length -= written;
  }
  return true;
}

bool M5SerialPngCapture::captureAndSend(M5GFX& display)
{
  if (!display.isReadable())
  {
    return false;
  }

  size_t pngLength = 0;
  void* pngData = display.createPng(
      &pngLength, 0, 0, display.width(), display.height());
  if (pngData == nullptr || pngLength > UINT32_MAX)
  {
    if (pngData != nullptr)
    {
      display.releasePngMemory();
    }
    return false;
  }

  const uint32_t payloadLength = static_cast<uint32_t>(pngLength);
  const uint8_t lengthBytes[] =
  {
    static_cast<uint8_t>(payloadLength),
    static_cast<uint8_t>(payloadLength >> 8),
    static_cast<uint8_t>(payloadLength >> 16),
    static_cast<uint8_t>(payloadLength >> 24),
  };
  const bool success =
      _write(reinterpret_cast<const uint8_t*>("PNG1"), 4) &&
      _write(lengthBytes, sizeof(lengthBytes)) &&
      _write(static_cast<const uint8_t*>(pngData), pngLength);
  _output.flush();
  display.releasePngMemory();
  return success;
}

void M5SerialPngCapture::handleScreenshotRequest(M5GFX& display)
{
  if (_output.available() > 0 && _output.read() == 's' &&
      !captureAndSend(display))
  {
    _output.println("SCREENSHOT_ERROR");
  }
}
