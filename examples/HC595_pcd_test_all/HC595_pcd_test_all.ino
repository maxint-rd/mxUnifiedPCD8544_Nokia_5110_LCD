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

#define LED_BLINK 1       // LED_BUILTIN=GPIO1 (TX) on ESP-01, GPIO2 on ESP-12E, 13 on Uno (should be redefined to 2 to avoid conflict)

#define XPOS 0
#define YPOS 1
#define DELTAY 2

void delayBlink(int nDelay, int nPin=LED_BLINK)
{ // delay some msecs and blink on/off at half delay interval
  unio.digitalWrite(16, HIGH);
  unio.digitalWrite(23, HIGH);    // click the relay on expanded pin 23
  digitalWrite(nPin, HIGH);
  delay(nDelay/4);
  unio.digitalWrite(22, HIGH);    // click the relay on expanded pin 22
  delay(nDelay/4);

  unio.digitalWrite(16, LOW);
  unio.digitalWrite(23, LOW);    // click the relay on expanded pin 23
  digitalWrite(nPin, LOW);
  delay(nDelay/4);
  unio.digitalWrite(22, LOW);    // click the relay on expanded pin 22
  delay(nDelay/4);
}

/**********************************************************************************
 * LED demo
 * The function below demonstrates the sequential switching of a series
 * of LEDs connected to expanded pins 8-15
 * The illuminated LED is switched on/off sequentially and the
 * Nokia display is used to show which LED is lit.
 */

void loopLeds()
{
  Serial.print(F("Leds: "));
  static int nLed=8;
  unio.digitalWrite(nLed, LOW);
  nLed++;
  if(nLed>15) nLed=8;
  unio.digitalWrite(nLed, HIGH);
  Serial.println(nLed-7);

  // show on the Nokia display which LED we turned on
  display.setRotation(2);  
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);

  display.println("Led:");
  display.setTextSize(3);
  display.print(" ");
  display.println(nLed-7);    // Display LED on digitalWrite(8) as number one

  display.setTextSize(1);
  display.println("");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(" Maxint  2017 ");
  display.setTextColor(BLACK, WHITE); // 'normal' text
  display.display();
}

/**********************************************************************************
 * Beep demo
 * 
 * An active buzzer is connected to expanded pin 0.
 * It is shorty activated to make some noise.
  */

void loopBeep(int nDelay=50, int nFreqDelay=1)
{
  uint32_t nStart=millis();
  Serial.println(F("Beep!"));

  while(millis()<nStart+nDelay)
  {
    unio.digitalWrite(0, HIGH);
    delay(nFreqDelay);
    unio.digitalWrite(0, LOW);
    delay(nFreqDelay);
  }
}

/**********************************************************************************
 * Stepper demo
 * This part is used to demonstrate bidirectional operation of a 28BYJ48 stepper 
 * motor using a ULN2003 interface.
 * 
 * The stepper motor is turned one full rotation in one direction and then 
 * reverses. The Nokia display is used to show which direction is active.
 * 
 * The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
 * a factor of 68. One bipolar winding is on motor pins 1 & 3 and
 * the other on motor pins 2 & 4. The step angle is 5.625/64 and the 
 * operating Frequency is 100pps. Current draw is 92mA. 
 */

 int motorPin1 = 4;    // Blue   - 28BYJ48 pin 1
 int motorPin2 = 5;    // Pink   - 28BYJ48 pin 2
 int motorPin3 = 6;    // Yellow - 28BYJ48 pin 3
 int motorPin4 = 7;    // Orange - 28BYJ48 pin 4
                       // Red    - 28BYJ48 pin 5 (VCC)

 int motorSpeed = 1200;  //variable to set stepper speed
 int motorCount = 0;          // count of steps made
 int motorCountsPerRev = 4076/8; // number of steps per full revolution
 int motorStepdelay=1000;
 
 int motorLed1=1;     // LEDs to indicate which direction the motor spins.
 int motorLed2=2;

 int motorLookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

//////////////////////////////////////////////////////////////////////////////
// set pins to ULN2003 high in sequence from 1 to 4
// delay "motorSpeed" between each pin setting (to determine speed)
 void anticlockwise()
 {
   for(int i = 0; i < 8; i++)
   {
     motorSetOutput(i);
     delayMicroseconds(motorSpeed);
   }
 }

void clockwise()
 {
   for(int i = 7; i >= 0; i--)
   {
     motorSetOutput(i);
     delayMicroseconds(motorSpeed);
   }
 }

void motorOff()
{
   unio.digitalWrite(motorPin1, LOW);
   unio.digitalWrite(motorPin2, LOW);
   unio.digitalWrite(motorPin3, LOW);
   unio.digitalWrite(motorPin4, LOW);
}

void motorSetOutput(int out)
{
   unio.digitalWrite(motorPin1, bitRead(motorLookup[out], 0));
   unio.digitalWrite(motorPin2, bitRead(motorLookup[out], 1));
   unio.digitalWrite(motorPin3, bitRead(motorLookup[out], 2));
   unio.digitalWrite(motorPin4, bitRead(motorLookup[out], 3));
}

void motorDisplayNokia(bool fClockwise)
{
  // show on the Nokia display which LED we turned on
  display.setRotation(2);  
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);

  display.println("Stepper");
  display.setTextSize(2);
  display.println(fClockwise?F("Right"):F("Left"));    // Display LED on digitalWrite(8) as number one

  display.setTextSize(1);
  display.println("");
  display.println("");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(" Maxint  2017 ");
  display.setTextColor(BLACK, WHITE); // 'normal' text
  display.display();  
}

void loopStepperMotor()
{
  Serial.println(F("Stepper: "));
  for(int i=0; i<motorCountsPerRev; i++)
  {
    if(motorCount < motorCountsPerRev )
      clockwise();
    else if (motorCount == motorCountsPerRev || motorCount == motorCountsPerRev * 2)
    {
      motorDisplayNokia(unio.digitalRead(motorLed1));
      unio.digitalWrite(motorLed1, !unio.digitalRead(motorLed1));
      unio.digitalWrite(motorLed2, !unio.digitalRead(motorLed1));
      if (motorCount == motorCountsPerRev * 2)
        motorCount = 0;
    }
    else
      anticlockwise();
    motorCount++;
    Serial.print(motorCount);
    if(motorCount%16==0)
      Serial.println("");
    else
      Serial.print(F(" "));
  /*   if(motorCount%(motorCountsperrev/60)==0)
    {
      motorOff();
      delay(motorStepdelay);
    }
  */
     delay(0);   // feed the watchdog (needed for ESP)
  }
  Serial.println(F("."));

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
  Serial.println(F("Some text"));
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
  display.println(" Maxint  2017 ");
  display.display();
  delayBlink(2000);

  // rotation example
  Serial.println(F("Rotate text"));
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
  display.println(" Maxint  2017 ");
  display.setTextColor(BLACK, WHITE); // 'normal' text
  display.display();
  
  Serial.println(F("Done with text"));
  delayBlink(2000);

  // revert back to no rotation
  display.setRotation(0);
}

void loop()
{
  display.invertDisplay(true);
  loopLeds();
  delayBlink(50); 
  loopBeep(100);
  loopStepperMotor();
  display.invertDisplay(false);
  loopLeds();
  delayBlink(200); 
  loopBeep(300,3);
  loopStepperMotor();
}

