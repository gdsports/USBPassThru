# USBPassThru
Arduino USB pass through demos

All the demos use one of following hardware configurations.

* Arduino Leonardo + Arduino USB Host Shield
* Adafruit ItsyBitsy 32u4 @ 8 MHz and 3.3V + USB Host Shield mini

The demos do not decode the USB HID reports. They pass the HID reports from the
USB host shield unchanged to the USB device output.

Understanding pass through is required for keyboard/mouse extenders or IP KVMs.

Other uses are to make a hardware key macro that work on any OS. No
need to install key macro software.

Swap certain keys such as Caps Lock and Ctrl keys.

Swap the mouse left and right buttons.

## USBKbdPassThru

USB Keyboard -> USB Host Shield -> Leonardo -> Computer USB port

## USBMsePassThru

USB Mouse -> USB Host Shield -> Leonardo -> Computer USB port

## Alternative hardware option

A smaller and cheaper option is to use an Adafruit ItsyBitsy 32u4 @ 8 MHz and
3.3V logic levels. The 32u4 is the same microcontroller used in the Arduino
Leonardo. And a USB Host mini board. The UH mini uses 3.3V logic levels so the
32u4 board must also use 3.3V. Soldering and cutting traces is required.

https://www.circuitsathome.com/arduino_usb_host_shield_projects/

