When using RC663 Blue board, it is possible that the program running
on LPC1769 may also drive SPI pins.

This will break the system, because the Host Microcontroller is also
trying to talk to LPC1769.

lpcWfi is a very simple program, it is just continually looping on __WFI()
This ensures that it is not driving any RC663 signals