/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

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

Example sketch adapted by Maxint R&D to demonstrate I2C driven display
https://github.com/maxint-rd/I2C-PCF8574-PCD8544-Nokia-5110-LCD
*********************************************************************/

#include <mxUnified74HC595.h>
#include <mxUnifiedPCD8544.h>

// The 74HC595 shift register requires three pins plus VCC/GND
/* 
Pinout 74HC595 shift register DIP and SMD model:

     +--v--+          PINS 1-8       PINS 9-16
  1 -+     +- 16      1: output Qb   16: VCC
  2 -+     +- 15      2: output Qc   15: output Qa   - expanded pin P0
  3 -+     +- 14      3: output Qd   14: Serial SER  - connect to SPI-MOSI
  4 -+     +- 13      4: output Qe   13: Enable OE   - active low, so connect to GND
  5 -+     +- 12      5: output Qf   12: Latch RCLK  - connect to SPI-SS
  6 -+     +- 11      6: output Qg   11: Clock SRCLK - connect to SPI-SCK
  7 -+     +- 10      7: output Qh   10: Clear SRCLR - active low, so connect to VCC
  8 -+     +- 9       8: GND          9: Cascade SER Qh' - to SER of next chip
     +-----+
*/

// The shift register can be connected to the hardware SPI pin for best speed.
// Default pins for ESP8266:   SS=15, MOSI=13, SCLK=14
// Default pins for ATmega328: SS=10, MOSI=11, SCLK=13
// Note with hardware SPI MISO pins isn't used but will still be read
// and written to during SPI transfer. Be careful when sharing these pins!

// Software SPI pins are slower, but offer pin-freedom
// Suggested pins for ESP-01: SS=3 (RX), MOSI=2, SCLK=1

mxUnified74HC595 unio = mxUnified74HC595();                  // use hardware SPI pins, no cascading
//mxUnified74HC595 unio = mxUnified74HC595(2);               // use hardware SPI pins, two cascaded shift-registers (slightly slower, but more pins)
//mxUnified74HC595 unio = mxUnified74HC595(10, 11, 13);      // alternative software SPI pins: SS, MOSI, SCLK, no cascading (slow, but pin-freedom)
//mxUnified74HC595 unio = mxUnified74HC595(10, 11, 13, 2);   // alternative software SPI pins: SS, MOSI, SCLK, three cascaded shift-registers (slow, but pin-freedom)
//mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK, no cascading (slow, but pin-freedom)
//mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0, 3);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK, three cascaded shift-registers (slow, but pin-freedom)

/* 
Pinout Nokia 5110 PCD8544 monochrome LCD module:
     +------------------+
     |     ........     |   Pin Description
     | +--------------+ |   1.  RST--------- LCD reset
     | +--------------+ |   2.  CE---------- LCD chip selection
     | |              | |   3.  DC---------- Data/Command select
     | |              | |   4.  DIN--------- Serial data line
     | |              | |   5.  CLK--------- Serial Clock
     | |              | |   6.  3.3V-------- VCC
     | +--------------+ |   7.  LIGHT------- Backlight control terminal
     |     ........     |   8.  GND--------- Power negative
     +------------------+
           12345678
*/

// The easiest way to connect the Nokia 5110 to the 74HC595 shift register on a
// breadboard is to use pins P1-P5 on a row and add wires to GND, P0->Back-light and VCC.
mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 1, 2, 3, 4, 5;         // Nokia 5110: CLK=P1, DIN=P2, DC=P3, CE=P4, RST=P5 (Back-light=P0)
//mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 9, 10, 11, 12, 13);
//mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 17, 18, 19, 20, 21);
//mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 25, 26, 27, 28, 29);


// NOTE: Set OPT_TEXTDISPLAY and OPT_DRAWBITMAP to 0 when using an ATmega168 pro-mini.
// This example barely fits in the ATmega168 which has 16K flash and 1K RAM memory.
// (The ATmega328 version of the pro-mini has 32K flash and 2K RAM memory).
// Those settings will skip text display testing and bitmap testing, to make it just.
#define OPT_TEXTDISPLAY 1
#define OPT_DRAWBITMAP 1

#if(OPT_DRAWBITMAP)
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
#endif


void serialTimeSpent(int nCnt=0)
{ // show time spent since previous call
  static uint32_t tStart;

  if(nCnt==0)
  {
    tStart=millis();  
    return;
  }
  Serial.print(F(" ["));
  Serial.print(nCnt);
  Serial.print(F(":"));
  Serial.print(millis()-tStart);
  Serial.print(F("ms] "));
  tStart=millis();
}

void delayBlink(int nDelay, int nPin=2)
{ // delay some msecs and blink on/off at half delay interval
  unio.digitalWrite(12, HIGH);
  unio.digitalWrite(0, HIGH);
  digitalWrite(nPin, HIGH);
  delay(nDelay/2);
  unio.digitalWrite(12, LOW);
  unio.digitalWrite(0, LOW);
  digitalWrite(nPin, LOW);
  delay(nDelay/2);
  serialTimeSpent();
}


void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);             // For ESP-01 board: Disable Serial.begin and call display.begin specifying GPIO 1 and 3.
  Serial.println(F("\n\n"));
  Serial.println(F("HC595_pcd_test"));
  serialTimeSpent();

  uint32_t tStart=millis();
  unio.begin(); // regular begin() using default settings
  display.begin();  // regular begin() using default settings
  serialTimeSpent(1);
  delayBlink(100);
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  serialTimeSpent(2);
  delayBlink(100);
  display.display(); // show splashscreen
  serialTimeSpent(3);
  delayBlink(2000);
  display.clearDisplay();   // clears the screen and buffer

  // draw a single pixel
  display.drawPixel(10, 10, BLACK);
  display.display();
  serialTimeSpent(5);
  delayBlink(2000);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  serialTimeSpent(7);
  display.display();
  serialTimeSpent(8);
  delayBlink(2000);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  serialTimeSpent(9);
  delayBlink(2000);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  serialTimeSpent(11);
  delayBlink(2000);
  display.clearDisplay();

  // draw a circle, 10 pixel radius
  display.fillCircle(display.width()/2, display.height()/2, 10, BLACK);
  display.display();
  serialTimeSpent(12);
  delayBlink(2000);
  display.clearDisplay();

  testdrawroundrect();
  serialTimeSpent(13);
  delayBlink(2000);
  display.clearDisplay();

  testfillroundrect();
  serialTimeSpent(14);
  delayBlink(2000);
  display.clearDisplay();

  testdrawtriangle();
  serialTimeSpent(15);
  delayBlink(2000);
  display.clearDisplay();
   
  testfilltriangle();
  serialTimeSpent(16);
  delayBlink(2000);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  serialTimeSpent(17);
  delayBlink(2000);
  display.clearDisplay();

#if(OPT_TEXTDISPLAY)
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello!");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);     // NOTE: wrapped size 2 big text crashes on 168 and blocks all further display! Not on 328, so probably memory related
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  serialTimeSpent(20);
  delayBlink(2000);

  // rotation example
  display.clearDisplay();
  display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println(F("Rotation"));
  display.setTextSize(2);
  display.println(F("Example!"));
  display.display();
  serialTimeSpent(25);
  delayBlink(2000);
#endif

  // revert back to no rotation
  display.setRotation(0);
  serialTimeSpent(26);

#if(OPT_DRAWBITMAP)
  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  serialTimeSpent(28);
#endif

  display.invertDisplay(true);
  serialTimeSpent(30);
  delayBlink(1000); 
  display.invertDisplay(false);
/* too large for 168
  serialTimeSpent(31);
  delayBlink(1000); 
  display.invertDisplay(true);
  delayBlink(500); 
  display.invertDisplay(false);
  delayBlink(500); 
*/

  // draw many lines
  // Note: calling the display() method after each line takes most time in the I2C library
  // Therefore this test was moved to the end
  testdrawline();
  display.display();
  serialTimeSpent(40);
  delayBlink(2000);
  display.clearDisplay();
  serialTimeSpent(41);

#if(OPT_DRAWBITMAP)
  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);
  serialTimeSpent(50);
#endif

}


void loop() {
  // Nothing to do here...  
}

#if(OPT_DRAWBITMAP)
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  randomSeed(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
/*    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
*/
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
    }
    display.display();
    delayBlink(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, WHITE);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
	icons[f][XPOS] = random(display.width());
	icons[f][YPOS] = 0;
	icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}
#endif

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    //display.writeChar(i);
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, BLACK);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, BLACK);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, BLACK);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, BLACK);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, BLACK);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, BLACK);
    display.display();
  }
  delayBlink(250);
  
  display.clearDisplay();
  //serialTimeSpent(300);
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, BLACK);
    display.display();
  }
  for (int8_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, BLACK);
    display.display();
  }
  delayBlink(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, BLACK);
  }
  display.display();
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, BLACK);
  }
  display.display();
  delayBlink(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, BLACK);
  }
  display.display();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, BLACK); 
  }
  display.display();
  delayBlink(250);
}
