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

//mxUnified74HC595 unio = mxUnified74HC595();                  // use hardware SPI pins, no cascading
//mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK, no cascading (slow, but pin-freedom)
mxUnified74HC595 unio = mxUnified74HC595(3, 2, 0, 3);      // alternative software SPI pins for ESP-01: SS, MOSI, SCLK, no cascading (slow, but pin-freedom)
//mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 1, 2, 3, 4, 5);
mxUnifiedPCD8544 display = mxUnifiedPCD8544(&unio, 17, 18, 19, 20, 21);

#define LED_BLINK 2       // LED_BUILTIN=GPIO1 (TX) on ESP-01, GPIO2 on ESP-12E, 13 on Uno (should be redefined to 2 to avoid conflict)

#define XPOS 0
#define YPOS 1
#define DELTAY 2

void delayBlink(int nDelay, int nPin=LED_BLINK)
{ // delay some msecs and blink on/off at half delay interval
  unio.digitalWrite(12, HIGH);
  unio.digitalWrite(0, HIGH);
  digitalWrite(nPin, HIGH);
  delay(nDelay/2);
  unio.digitalWrite(12, LOW);
  unio.digitalWrite(0, LOW);
  digitalWrite(nPin, LOW);
  delay(nDelay/2);
}

void setup()
{
  pinMode(LED_BLINK, OUTPUT);
  Serial.begin(115200);             // For ESP-01 board: Disable Serial.begin and call display.begin specifying GPIO 1 and 3.
  Serial.println(F("\n\n"));
  Serial.println(F("HC595_pcd_test_text"));

  uint32_t tStart=millis();
  unio.begin(); // regular begin() using default settings
  display.clearDisplay();
  display.begin();  // regular begin() using default settings
  display.clearDisplay();   // clears the screen and buffer
  display.display();
  delayBlink(100);

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  // text display tests
  display.setRotation(2);  // rotate 180 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello");
  display.println("");
  display.setTextSize(2);     // NOTE: wrapped size 2 big text crashes on 168 and blocks all further display! Not on 328, so probably memory related
  display.setTextColor(BLACK);
  display.println("YouTube");
  display.setTextSize(1);
  display.println("");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println("  Maxint 2017 ");
  display.display();
  delayBlink(2000);

  // rotation example
  display.clearDisplay();
  display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(0);  
  display.clearDisplay();   // clears the screen and buffer
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(3);  
  display.clearDisplay();   // clears the screen and buffer
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.display();
  delayBlink(500);

  display.setRotation(2);  
  display.clearDisplay();   // clears the screen and buffer
  display.println("Hello");
  display.setTextSize(2);
  display.println("YouTube");
  display.setTextSize(1);
  display.println("");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println("  Maxint 2017 ");
  display.display();
  
  delayBlink(2000);

  // revert back to no rotation
  display.setRotation(0);
}


void loop()
{
  display.invertDisplay(true);
  delayBlink(1000); 
  display.invertDisplay(false);
  delayBlink(1000); 
}


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

