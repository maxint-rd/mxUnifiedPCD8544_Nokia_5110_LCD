DESCRIPTION
===========
This is a library for the Monochrome Nokia 5110 LCD Displays

These displays normally use SPI to communicate. 3 to 5 data-pins are required to  
interface with the MCU. When using the minimum of three data-pins (DC/DIN/CLK),
the remaining pins are fixed. By using the mxUnifiedPCF8574 library for an I2C
interface or the mxUnified74HC595 library for shift-registers, the display can
be connected to share the I2C bus (only two pins) or the SPI pins (three wires).

This library is based on the Adafruit PCD8544 Nokia 5110 LCD library, but 
instead of directly using SPI it allows the display to be driven via the 
expanded pins of an I2C I/O expander or a shift-register.


Using SPI shift-register
========================
The (shared) SPI bus requires three data-lines to connect one or more shift
registers (SS, MOSI and SCK).
On the Arduino UNO, Nano and Pro Mini, the pins for (fast) hardware SPI are
D10, D12 and D13. On the ESP8266 the default SPI pins are GPIO15, GPIO13 and
GPIO14. When using (slower) software SPI others can be selected too.


Using I2C I/O expander
======================
The (shared) I2C bus only requires two data-lines (SDA and SCL).
On the Arduino UNO, Nano and Pro Mini, these can be found on pins A4 and A5.
On the ESP8266 the default pins are GPIO4 and GPIO5, but others can be
selected too.

The I2C data is expanded to the recommended 5 pins using the PCF8274 I2C I/O
expander. A very affordable board and abundantly available board that
features a PCF8274 chip is the I2C backpack that is most commonly used
as interface to the 1602 and 2004 character LCD display modules. 
Using only slightly different connections this interface board can be
used to connect the 48-84 Nokia 5110 dot-matrix LCD display module.


Module Pinout
=============

Nokia 5110 LCD Display module:
     +------------------+
     |     ........     |
     | +--------------+ |
     | +--------------+ |
     | |              | |
     | |              | |
     | |              | |
     | |              | |
     | +--------------+ |
     |     ........     |
     +------------------+
           12345678

Pin Description
 1.  RST--------- reset
 2.  CE---------- chip selection
 3.  DC---------- data/commands choice
 4.  DIN--------- serial data line
 5.  CLK--------- serial Clock Speed
 6.  3.3V-------- VCC
 7.  LIGHT------- backlight control terminal
 8.  GND--------- power negative

PCF8574 I2C LCD driver:
     +---------------------+
-SCL-+   ::: +---+         |
-SCA-+   +-+ |PCF| ''': LED+-
-VCC-+   |*| |   |  ..- jmp+-
-GND-+   +-+ +---+         |
     +---------------------+
        1234567890123456
        ||||||||||||||||

Pin Description
 1.  GND
 2.  VCC
 3.  Contrast (adjustable VCC->Pot->Pin3)
 4.  P0
 5.  P1
 6.  P2
 7.  not connected
 8.  not connected
 9.  not connected
10.  not connected
11.  P4
12.  P5
13.  P6
14.  P7
15.  VCC (for LED)
16.  P3 (connected via LED jumper to J3Y (=S8050) transistor, sink)

See the included example for suggested connections of the display module
to the I2C LCD driver interface.


CREDITS
========
1. Credits to Adafruit for creating the original PCD8544 library and the
required Adafruit GFX library.
Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

  Pick a Nokia 5110 display up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

Original version written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

2. Credits to Rob Tillaart for creating the PCF8574 library. 
Check out his large collection of Arduino libraries at 
https://github.com/RobTillaart/Arduino

3. Modified library by Maxint R&D 2017. 
BSD license applies. See below for more information 


INSTALLATION
============
Download the required libraries (see links below).
Use the Arduino IDE to add the .ZIP library file via the menu.
Alternatively copy all library files into a folder under your
<arduinosketchfolder>/libraries/ folder and restart the IDE.

You will also have to install the Adafruit GFX Graphics core library
which does all the circles, text, rectangles, etc; as well as the
mxUnifiedIO library. Also install either the mxUnifiedPCF8574
library or the mxUnified74HC595 librraries, which are used
to drive the PCD8544 LCD chip.

See the included examples to learn more about using this library.


TROUBLESHOOTING TIPS
====================
- Use the I2C scanner sketch to see if the PCF8574 I2C LCD driver
  is properly connected and responding.
- Test the Nokia 5110 module using the SPI connections and the 
  original library and pcdtest example.
- Check if all data-pins between display and interface modules are
  properly connected, according the pins as defined in your sketch.
- Use a multimeter to check pin voltages. Note: most Nokia 5110 and
  PCF8574 modules are both 3.3V and 5V complient. However, the
  contrast level on some 5110 modules needs to be higher when powered
  by 3.3V.
  

LINKS
=====
Required libraries:
- This library:
    https://github.com/maxint-rd/mxUnifiedPCD8544_Nokia_5110_LCD
- Supporting libraries:
    https://github.com/maxint-rd/mxUnifiedIO
    https://github.com/maxint-rd/mxUnifiedPCF8574
    https://github.com/maxint-rd/mxUnified74HC595
- Adafruit GFX Library: 
    https://github.com/adafruit/Adafruit-GFX-Library

For reference:
- Rob Tillaarts PCF8574 library:
    https://github.com/RobTillaart/Arduino/tree/master/libraries/PCF8574
    https://playground.arduino.cc/Main/PCF8574Class
- Adafruit tutorials:
    https://learn.adafruit.com/nokia-5110-3310-monochrome-lcd
    https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
- Arduino I2C scanner sketches:
    https://playground.arduino.cc/Main/I2cScanner
- The original Adafruit PCD8544 Nokia 5110 LCD SPI library:
    https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library


LICENSE
=======
This library inherits the BSD license from the original library which 
limits the usage of this library to specific terms.
Read license.txt for more information.
