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
 * USB Mouse pass through. Works at HID report level. HID report is not
 * decoded.  Tested on Leonardo with Arduino USB host shield, IDE 1.8.5, and
 * Logitech Marble trackball/mouse. This is a proof of concept so is not
 * guaranteed to work with all USB mice/trackballs.
 */

// USB host mouse from USB Host Shield Library. Install using Library Manager
#include <hidboot.h>

// USB device mouse library included with Arduino IDE 1.8.5
#include <Mouse.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class MouseRptParser : public MouseReportParser
{
protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
#if 0
  // Demo -- Swap left and right buttons on 4 button Kensington trackball
  if (len > 0) {
    uint8_t button1 = buf[0] & 0x01;
    uint8_t button2 = buf[0] & 0x02;
    uint8_t button3 = buf[0] & 0x04;
    uint8_t button4 = buf[0] & 0x08;
    buf[0] = (buf[0] & 0xF0) | (button1 << 1) | (button2 >> 1) |
                               (button3 << 1) | (button4 >> 1);
  }
#endif
  // Run parent class method.
  MouseReportParser::Parse(hid, is_rpt_id, len, buf);

  Serial.print("MouseRptParser::Parse");
  // Show USB HID mouse report
  for (uint8_t i = 0; i < len ; i++) {
    Serial.print(' '); Serial.print(buf[i], HEX);
  }
  Serial.println();

  if (len > 2) {
    uint8_t mouseRpt[4];
    mouseRpt[0] = buf[0];
    mouseRpt[1] = buf[1];
    mouseRpt[2] = buf[2];
    // If the mouse/trackball has a scroll wheel, send the value
    if (len > 3) {
      mouseRpt[3] = buf[3];
    }
    else {
      mouseRpt[3] = 0;
    }
    HID().SendReport(1,mouseRpt,sizeof(mouseRpt));
  }
}

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  uint8_t attempts = 30;
  while (!Serial && attempts--) {
    delay(100); // Wait for serial port to connect for up to 3 seconds
  }
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("USB host shield did not start.");
  }
  delay( 200 );

  HidMouse.SetReportParser(0, &Prs);

  Mouse.begin();
}

void loop()
{
  Usb.Task();
}
