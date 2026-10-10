# M5SerialPngCapture

Arduino library for capturing an `M5GFX` display as PNG and sending the image
over a `Stream`, such as `Serial`.

## Usage

Install `M5SerialPngCapture` and its `M5Unified` dependency, then connect the
library to the stream used by the host:

```cpp
#ifndef GDTOUCHKEYBOARD_ENABLE_SCREENSHOT
#define GDTOUCHKEYBOARD_ENABLE_SCREENSHOT 0
#endif

#include <M5Unified.h>
#if GDTOUCHKEYBOARD_ENABLE_SCREENSHOT
#include <M5SerialPngCapture.h>
#endif

#if GDTOUCHKEYBOARD_ENABLE_SCREENSHOT
M5SerialPngCapture screenshotCapture(Serial);
#endif

void setup()
{
  M5.begin();
  Serial.begin(115200);
}

void loop()
{
  M5.update();
#if GDTOUCHKEYBOARD_ENABLE_SCREENSHOT
  screenshotCapture.handleScreenshotRequest(M5.Display);
#endif
}
```

For applications such as GDTouchKeyboard, set
`GDTOUCHKEYBOARD_ENABLE_SCREENSHOT` to `1` in the sketch or build
configuration to enable screenshot capture.

The response frame is:

```text
PNG1 + 4-byte little-endian PNG length + PNG data
```

The library sends no response when the display cannot be read or PNG
allocation fails. Applications can report those failures separately.

## Host receiver

[`tools/capture_screenshot.py`](tools/capture_screenshot.py) requests a
screenshot and saves the received PNG:

```text
python3 -m pip install pyserial
python3 tools/capture_screenshot.py --port /dev/ttyACM0 --output screen.png
```

The receiver ignores human-readable serial output before the `PNG1` marker,
validates the PNG signature, limits the default payload to 8 MiB, and writes
through a temporary file before replacing the output.
