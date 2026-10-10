/**
 * @file M5SerialPngCapture.ino
 * @brief Demonstrate framed PNG screenshots over Serial.
 */

#include <M5Unified.h>
#include <M5SerialPngCapture.h>

M5SerialPngCapture screenshotCapture(Serial);

void setup()
{
  M5.begin();
  Serial.begin(115200);
}

void loop()
{
  M5.update();
  screenshotCapture.handleScreenshotRequest(M5.Display);
}
