/*********************************************************************
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

Library adapted by Maxint R&D to drive Nokia 5110 display via a PCF8574
I2C I/O expander or the 74HC595 shift register using the mxUnifiedIO API.
https://github.com/maxint-rd/mxUnifiedPCD8544_Nokia_5110_LCD
*********************************************************************/
#ifndef _MXUNIFIEDPCD8544_H
#define _MXUNIFIEDPCD8544_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
#endif

//#include <Wire.h>
#include <SPI.h>
#include <mxUnifiedIO.h>
#include <Adafruit_GFX.h>

#ifdef __SAM3X8E__
  typedef volatile RwReg PortReg;
  typedef uint32_t PortMask;
#elif defined(ESP8266)
  typedef volatile uint32_t PortReg;
  typedef uint32_t PortMask;
#else
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#endif


#define BLACK 1
#define WHITE 0

#define PCD8544_LCDWIDTH 84
#define PCD8544_LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

// Default to max SPI clock speed for PCD8544 of 4 mhz (16mhz / 4) for normal Arduinos.
// This can be modified to change the clock speed if necessary (like for supporting other hardware).
#define PCD8544_SPI_CLOCK_DIV SPI_CLOCK_DIV4

class mxUnifiedPCD8544 : public Adafruit_GFX
//	, public PCF8574
{
 public:
  // I2C to SPI via mxUnifiedIO (such as I2X PCF8574) with explicit definition of mxUnifiedIO pins P0-P7.
  mxUnifiedPCD8544(mxUnifiedIO *pUniOut, int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST);
  // Software SPI with explicit CS pin.
  mxUnifiedPCD8544(int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST);
  // Software SPI with CS tied to ground.  Saves a pin but other pins can't be shared with other hardware.
  mxUnifiedPCD8544(int8_t SCLK, int8_t DIN, int8_t DC, int8_t RST);
  // Hardware SPI based on hardware controlled SCK (SCLK) and MOSI (DIN) pins. CS is still controlled by any IO pin.
  // NOTE: MISO and SS will be set as an input and output respectively, so be careful sharing those pins!
  mxUnifiedPCD8544(int8_t DC, int8_t CS, int8_t RST);

  void begin(uint8_t contrast = 40, uint8_t bias = 0x04);

  void command(uint8_t c);
  void data(uint8_t c);
  
  void setContrast(uint8_t val);
	void invertDisplay(boolean i);
  void display();
	void clearDisplay(void);
  
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  uint16_t getPixel(int16_t x, int16_t y);

 private:
  int8_t _din, _sclk, _dc, _rst, _cs;
  mxUnifiedIO *_pUniOut;
  volatile PortReg  *mosiport, *clkport;
  PortMask mosipinmask, clkpinmask;

  void spiWrite(uint8_t c);
  void unioWrite(uint8_t c, bool fClosedTransmission=true);

  void digitWrite(uint8_t, uint8_t);
	void digitWriteTwo(uint8_t nPin1, uint8_t nValue1, uint8_t nPin2, uint8_t nValue2);
	void pnMode(uint8_t, uint8_t);
  bool isHardwareSPI();
  bool isUniOut();
};

#endif
