/*
 * MIT License
 *
 * Copyright (c) 2018 gdsports625@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

/*
 * USB keyboard pass through. Works at HID report level. HID report is not
 * decoded.  Tested on Leonardo with Arduino USB host shield, IDE 1.8.5, and
 * generic keyboard.  This is a proof of concept so is not guaranteed to work
 * with all USB keyboards.
 */

// USB host keyboard from USB Host Shield Library. Install using Library Manager
#include <hidboot.h>

// USB device keyboard library included with Arduino IDE 1.8.5
#include "Keyboard.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class KbdRptParser : public KeyboardReportParser
{
  protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void KbdRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  // Run parent class method so keyboard LEDs are updated.
  KeyboardReportParser::Parse(hid, is_rpt_id, len, buf);

  Serial.print("KbdRptParser::Parse");
  // Show USB HID keyboard report
  for (uint8_t i = 0; i < len ; i++) {
    Serial.print(' '); Serial.print(buf[i], HEX);
  }
  Serial.println();

  // On error - return
  if (buf[2] == 1)
    return;

  if (len == 8) {
    //Keyboard.sendReport((KeyReport *)buf);
    HID().SendReport(2, buf, sizeof(KeyReport));
  }
}

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  uint8_t attempts = 30;
  while (!Serial && attempts--) {
    delay(100); // Wait for serial port to connect for up to 3 seconds
  }
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("USB host shield did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);

  Keyboard.begin();
}

void loop()
{
  Usb.Task();
}
