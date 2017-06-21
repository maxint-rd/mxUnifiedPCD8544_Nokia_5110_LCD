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
All text above, and the splash screen below must be included in any redistribution

Library adapted by Maxint R&D to drive Nokia 5110 display via PCF8574 I2C I/O expander
https://github.com/maxint-rd/I2C-PCF8574-PCD8544-Nokia-5110-LCD
*********************************************************************/

#if defined(ESP8266)
#include <pgmspace.h>
#endif
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR__
  #include <util/delay.h>
#endif

#ifndef _BV
  #define _BV(x) (1 << (x))
#endif

#include <stdlib.h>

#include "mxUnifiedPCD8544.h"

// the memory buffer for the LCD
uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF,
0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE,
0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xC0, 0xE0, 0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x7E, 0x3F, 0x3F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

mxUnifiedPCD8544::mxUnifiedPCD8544(mxUnifiedIO *pUniOut, int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t CS, int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{
	_pUniOut=pUniOut;

  //_bl=BL;
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
}

mxUnifiedPCD8544::mxUnifiedPCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t CS, int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{
	_pUniOut=NULL;
	_din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
  //_bl=-1;
}

mxUnifiedPCD8544::mxUnifiedPCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{
	_pUniOut=NULL;
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = -1;
  //_bl=-1;
}

mxUnifiedPCD8544::mxUnifiedPCD8544(int8_t DC, int8_t CS, int8_t RST):
  Adafruit_GFX(LCDWIDTH, LCDHEIGHT)
{
	_pUniOut=NULL;
  // -1 for din and sclk specify using hardware SPI
  _din = -1;
  _sclk = -1;
  _dc = DC;
  _rst = RST;
  _cs = CS;
  //_bl=-1;
}


// the most basic function, set a single pixel
void mxUnifiedPCD8544::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  int16_t t;
  switch(rotation){
    case 1:
      t = x;
      x = y;
      y =  LCDHEIGHT - 1 - t;
      break;
    case 2:
      x = LCDWIDTH - 1 - x;
      y = LCDHEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = LCDWIDTH - 1 - y;
      y = t;
      break;
  }

  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);  
  else
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t mxUnifiedPCD8544::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  return (pcd8544_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;  
}

void mxUnifiedPCD8544::begin(uint8_t contrast, uint8_t bias)
{
	//Serial.println(F("mxUnifiedPCD8544 begin"));
	if (isUniOut()) {
		// optimize by always setting CS low (active) and starting by DC in command mode
		// for the remainder of the session CS is not changed as the PCD8544 can be assumed the only SPI device on this I2C i/o expander
		if(_cs > 0)
			digitWriteTwo(_dc, LOW, _cs, LOW);
		else
			digitWrite(_dc, LOW);
	}
  else if (isHardwareSPI()) {
    // Setup hardware SPI.
    SPI.begin();
#ifdef ESP8266
		// see https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library/pull/27/commits
    // Datasheet says 4 MHz is max SPI clock speed
    SPI.setFrequency(4000000);
#else
    SPI.setClockDivider(PCD8544_SPI_CLOCK_DIV);
#endif
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
  }
  else {
    // Setup software SPI.

    // Set software SPI specific pin outputs.
    pnMode(_din, OUTPUT);
    pnMode(_sclk, OUTPUT);

// see https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library/pull/27/commits
#ifndef ESP8266
    // Set software SPI ports and masks.
    clkport     = portOutputRegister(digitalPinToPort(_sclk));
    clkpinmask  = digitalPinToBitMask(_sclk);
    mosiport    = portOutputRegister(digitalPinToPort(_din));
    mosipinmask = digitalPinToBitMask(_din);
#endif
  }

  // Set common pin outputs.
  pnMode(_dc, OUTPUT);
  if (_rst > 0)
      pnMode(_rst, OUTPUT);
  if (_cs > 0)
      pnMode(_cs, OUTPUT);

  // toggle RST low to reset
  if (_rst > 0) {
    digitWrite(_rst, LOW);
    delay(500);
    digitWrite(_rst, HIGH);
  }

  // get into the EXTENDED mode!
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

  // LCD bias select (4 is optimal?)
  command(PCD8544_SETBIAS | bias);

  // set VOP
  if (contrast > 0x7f)
    contrast = 0x7f;

  command( PCD8544_SETVOP | contrast); // Experimentally determined


  // normal mode
  command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  // Push out pcd8544_buffer to the Display (will show the AFI logo)
  display();
}

inline void mxUnifiedPCD8544::spiWrite(uint8_t d) {
  if (isHardwareSPI()) {
    // Hardware SPI write.
    SPI.transfer(d);
  }
  else {
    // Software SPI write with bit banging.
#ifdef ESP8266
    // see https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library/pull/27/commits
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      ::digitalWrite(_sclk, LOW);
      if (d & bit) ::digitalWrite(_din, HIGH);
      else         ::digitalWrite(_din, LOW);
      ::digitalWrite(_sclk, HIGH);
    }
 #else
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      *clkport &= ~clkpinmask;
      if(d & bit) *mosiport |=  mosipinmask;
      else        *mosiport &= ~mosipinmask;
      *clkport |=  clkpinmask;
    }
#endif
  }
}

bool mxUnifiedPCD8544::isHardwareSPI() {
  return (_din == -1 && _sclk == -1);
}

bool mxUnifiedPCD8544::isUniOut() {
  return (_pUniOut != NULL);
}

void mxUnifiedPCD8544::command(uint8_t c)
{
  if(isUniOut())
  {	// for speed optimisation changing the _dc and _cs pins are combined and the transmission is bundled
			//Serial.print(F("I2C command(0x"));
			//Serial.print(c, HEX);
			//Serial.println(F(")"));
    _pUniOut->startTransmission();
//    _pUniOut->setBit(_cs, LOW);
//		_pUniOut->sendBits();		// not much optimization: DC and CS can be set combined with first CLK and DIN, but that doesn't give much gain
	  _pUniOut->setBit(_dc, LOW);
	  unioWrite(c, false);		// writing of DC is combined with writing the command
//    _pUniOut->setBit(_cs, HIGH);
//    _pUniOut->sendBits();
	  _pUniOut->endTransmission();
  }
  else
  {
	  digitWrite(_dc, LOW);
	  if (_cs > 0)
	    digitWrite(_cs, LOW);
		  spiWrite(c);
	  if (_cs > 0)
	    digitWrite(_cs, HIGH);
  }
}

void mxUnifiedPCD8544::data(uint8_t c)
{	// The call to data() seems not used by GFX library. (So not optimized for speed). 
  // Serial prints are left to confirm this assumption.
  digitWrite(_dc, HIGH);
  if (_cs > 0)
    digitWrite(_cs, LOW);
  if(isUniOut())
  {
		Serial.print(F("I2C data("));
		Serial.print(c);
		Serial.println(F(")"));
  	unioWrite(c);
  }
  else
	  spiWrite(c);
  if (_cs > 0)
    digitWrite(_cs, HIGH);
}

void mxUnifiedPCD8544::setContrast(uint8_t val)
{
  if (val > 0x7f) {
    val = 0x7f;
  }
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  command( PCD8544_SETVOP | val); 
  command(PCD8544_FUNCTIONSET);
}


void mxUnifiedPCD8544::display(void) {
  uint8_t col, maxcol, p;
  
  for(p = 0; p < 6; p++) {
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    command(PCD8544_SETYADDR | p);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    command(PCD8544_SETXADDR | col);

		if(isUniOut())
		{	// for speed optimisation changing the _dc and _cs pins are combined and the display data transmission is bundled
			//digitWriteTwo(_dc, HIGH, _cs, LOW);
			//digitWrite(_dc, HIGH);

	    _pUniOut->startTransmission();
	    _pUniOut->setBit(_dc, HIGH);		// writing is combined when unioWrite is called
	    uint16_t uCnt=0;
	    for(; col <= maxcol; col++)
	    {
	  		//Serial.print(F("W"));
		  	unioWrite(pcd8544_buffer[(LCDWIDTH*p)+col], false);
    		uCnt++;
    		if(uCnt%2==0 && col<maxcol-1)
   			{	// can't send too much in one go because I2C has limited buffer (32 bytes) on both ATmega and ESP8266
   				// Combining more than two column-bytes per transmission causes data to be lost, even when running at lower speeds.
   				// Note that for each byte 16 clk changes are required, meaning 16 I2C bytes are send per byte.
   				// No error (e.g. for buffer overflow) is given, but data is definitely not complete.
   				// (ending every 4th column gives half complete columns, every 8th gives quarter screen)
   				// possibility to combine depends on number of _pUniOut->write() calls in unioWrite().
 		  		//Serial.print(F("E"));
				  _pUniOut->endTransmission();
 		  		//Serial.print(F("B"));
			    _pUniOut->startTransmission();
			  }
	    }
		  _pUniOut->endTransmission();

	    //if (_cs > 0)
	    //  digitWrite(_cs, HIGH);
#ifdef ESP8266
			// Since the display method takes the longest time on the relative slow I2C transmission,
			// the watchdog of the ESP may bite after repetitive calls
			// This is a good moment to feed that dog and keep it happy!
			yield();
#endif
		}
		else
		{
	    digitWrite(_dc, HIGH);
	    if (_cs > 0)
	      digitWrite(_cs, LOW);
	    for(; col <= maxcol; col++) {
	      spiWrite(pcd8544_buffer[(LCDWIDTH*p)+col]);
	    }
	    if (_cs > 0)
	      digitWrite(_cs, HIGH);
    }
  }

  if(!isUniOut())	// MMOLE: also no idea, lets optimize and skip this.
  	command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}

void mxUnifiedPCD8544::invertDisplay(boolean i)
{
  //if (isHardwareSPI()) spi_begin();
  command(PCD8544_FUNCTIONSET);
  command(PCD8544_DISPLAYCONTROL | (i ? PCD8544_DISPLAYINVERTED : PCD8544_DISPLAYNORMAL));
  //if (isHardwareSPI()) spi_end();
}

// clear everything
void mxUnifiedPCD8544::clearDisplay(void) {
  memset(pcd8544_buffer, 0, LCDWIDTH*LCDHEIGHT/8);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  cursor_y = cursor_x = 0;
}


void mxUnifiedPCD8544::unioWrite(uint8_t d, bool fClosedTransmission)
{		// Use I2C to (slowly) write a bit of data. It resembles bit banging 
    // software SPI to the pins of the I2C interface, but is slow as every 
    // change of the same pin requires an I2C transmission.
    // fClosedTransmission can be set false to combine setting of multiple bits in the same I2C transmission
    if(fClosedTransmission)
	    _pUniOut->startTransmission();

		_pUniOut->shiftOut(_din, _sclk, MSBFIRST, d);
/*
		uint8_t aI2C_bytestream[16];
		uint8_t uCnt=0;
    for(uint8_t bit = 0x80; bit; bit >>= 1)
    {
  		// Using the PCF8574 library for bulk writes is WAY TOO SLOW! (1400 ms to display the screen)
   		// Each SPI DIN databyte transfer requires 16 CLK toggles, i.e. 16 I2C bytes to write.
   		// Therefor bundle the writing of each byte into one _pUniOut->write() call and minimize the begin/end calls.
   		// This saves writing the address for each bit and speeds screen display up to 380 ms.
   		// Still slow, but acceptable when display() function is not called after drawing each primitive. 
			// BTW. Setting bits is more readable using the _pUniOut->setBit function and doesn't cost much performance.
    	_pUniOut->setBit(_sclk, LOW);
    	_pUniOut->setBit(_din, (d & bit));
	    aI2C_bytestream[uCnt++]=_pUniOut->getBits();
	    //_pUniOut->sendBits();
    	_pUniOut->setBit(_sclk, HIGH);
	    aI2C_bytestream[uCnt++]=_pUniOut->getBits();
	    //_pUniOut->sendBits();
    }
		//Serial.print(F("W"));
    _pUniOut->sendBytes(aI2C_bytestream, sizeof(aI2C_bytestream));		// quest writing the bytes of all 16 bits in one go (still takes 302-480ms)
*/
		if(fClosedTransmission)
	    _pUniOut->endTransmission();
}

void mxUnifiedPCD8544::digitWrite(uint8_t nPin, uint8_t nValue)
{	// set a regular pin or one of the I2C I/O expander when using I2C
	if(isUniOut())
	{
	  _pUniOut->startTransmission();
	  _pUniOut->setBit(nPin, nValue);
	  _pUniOut->sendBits();
	  _pUniOut->endTransmission();
	}
	else
		::digitalWrite(nPin, nValue);
}

void mxUnifiedPCD8544::digitWriteTwo(uint8_t nPin1, uint8_t nValue1, uint8_t nPin2, uint8_t nValue2)
{	// set the status of two pins and write them
	if(isUniOut())
	{
	  _pUniOut->startTransmission();
	  _pUniOut->setBit(nPin1, nValue1);
	  _pUniOut->setBit(nPin2, nValue2);
	  _pUniOut->sendBits();
	  _pUniOut->endTransmission();
	}
 	else
 	{
		::digitalWrite(nPin1, nValue1);
		::digitalWrite(nPin2, nValue2);
	}
}

void mxUnifiedPCD8544::pnMode(uint8_t nPin, uint8_t nMode)
{
	if(isUniOut())
	{	// currently only output pins are supported when using I2C
	}
	else
		::pinMode(nPin, nMode);
}
