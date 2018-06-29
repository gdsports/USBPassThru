# USBPassThru
Arduino USB pass through demos

All the demos use the following hardware configuration.

* Arduino Leonardo
* Arduino USB Host Shield

The demos do not decode the USB HID reports. They pass the HID reports from the
USB host shield unchanged to the USB device output.

This is useful for keyboard/mouse extenders or IP KVMs.

## USBKbdPassThru

USB Keyboard -> USB Host Shield -> Leonardo -> Computer USB port

## USBMsePassThru

USB Mouse -> USB Host Shield -> Leonardo -> Computer USB port

