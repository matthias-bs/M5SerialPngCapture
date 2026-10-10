// Copyright (c) Matthias Prinke. All rights reserved.
// Licensed under the MIT license. See LICENCE file in the project root.

#ifndef M5_SERIAL_PNG_CAPTURE_H
#define M5_SERIAL_PNG_CAPTURE_H

#include <M5GFX.h>
#include <Stream.h>

/**
 * @brief Captures an M5GFX display and sends it as a framed PNG stream.
 */
class M5SerialPngCapture
{
public:
  /**
   * @param output Stream receiving the screenshot frame.
   */
  explicit M5SerialPngCapture(Stream& output);

  /**
   * @brief Capture the complete display and send it over the configured stream.
   *
   * The frame format is `PNG1`, a 32-bit little-endian payload length, and the
   * PNG payload. The display's temporary PNG buffer is released before return.
   *
   * @param display Display to capture.
   * @return true if the image was captured and written successfully.
   */
  bool captureAndSend(M5GFX& display);

  /**
   * @brief Handle a pending screenshot request from the configured stream.
   *
   * The method consumes one byte when available. The `s` command captures the
   * display; failed captures are reported as `SCREENSHOT_ERROR`.
   *
   * @param display Display to capture.
   */
  void handleScreenshotRequest(M5GFX& display);

private:
  bool _write(const uint8_t* data, size_t length);

  Stream& _output;
};

#endif
