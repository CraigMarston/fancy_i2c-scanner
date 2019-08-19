
// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not known.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//
//  • This adaptation does not alter the basic I2C reading function
//    It builds on top to run an SPI TFT screen with some graphics.
//    August 2019 Craig Marston.

#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library https://github.com/adafruit/Adafruit_ILI9341
#include <SPI.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define _sclk 14  //  D5
#define _miso 12  //  D6
#define _mosi 13  //  D7
#define _cs 15    //  D8 - slave select
#define _dc 2     //  D4 - data/command mode
#define _rst 0    //  D3

Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);

// x=320 y=240 TFT dimensions
  uint16_t width = 320; // required to be 16-bit variables
  uint16_t height = 240; // for compatability with functions
  uint16_t scalingFactor = 32; // larger number makes smaller squares!
  uint16_t squareSize = (width/scalingFactor); // determined by the above values
  // used for the chequers and to position graphic elements

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 5000;           // interval between scans

// =====================================================================================================
// *°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°◊ Setup ◊°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*
// =====================================================================================================

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
  tft.begin();
  tft.setRotation(1);
  chequers();
  tft.fillRect(2*squareSize, 8*squareSize, width-(4*squareSize), 8*squareSize, ILI9341_WHITE);
  tft.drawRect(2*squareSize, 8*squareSize, width-(4*squareSize), 8*squareSize, ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLUE);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setCursor(10*squareSize, 13*squareSize);
  tft.print("I C Scanner");
  tft.setCursor(10*squareSize, (12*squareSize)-2);
  tft.setFont(&FreeSansBold9pt7b);
  tft.print(" 2");
  delay(3000);
  tft.fillRect(2*squareSize, 2*squareSize, width-(4*squareSize), height-(4*squareSize), ILI9341_WHITE);
  tft.drawRect(2*squareSize, 2*squareSize, width-(4*squareSize), height-(4*squareSize), ILI9341_BLACK);
}


// =====================================================================================================
// *°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°◊ Main Loop ◊°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*°*
// =====================================================================================================

void loop()
{
  unsigned long currentMillis = millis(); // this is instead of using 'delay()'

  if (currentMillis - previousMillis >= interval) // wait between scans…
  {
    previousMillis = currentMillis; // save the last time function ran
  
    byte error, address;
    int nDevices;
  
    Serial.println("Scanning...");
    tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
    tft.setTextColor(ILI9341_ORANGE);
    tft.setFont(&FreeSansBold9pt7b);
    tft.setCursor(4*squareSize, 10*squareSize);
    tft.print("Scanning...");
    delay(150);
  
    nDevices = 0;
    for (address = 1; address < 127; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
  
      if (error == 0) // success
      {
        Serial.print("I2C device found at address 0x");
        tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
        tft.setTextColor(ILI9341_DARKGREEN);
        tft.setFont(&FreeSans9pt7b);
        tft.setCursor(4*squareSize+6, 7*squareSize);
        tft.print("I2C device found at address");
        
        if (address < 16) // for leading zero!!
        {
          Serial.print("0");
          tft.setTextColor(ILI9341_GREEN);
          tft.setFont(&FreeSansBold9pt7b);
          tft.setCursor(4*squareSize, 10*squareSize);
          tft.print("0");
        }
        Serial.print(address, HEX); // actual address
        Serial.println("  !");
        tft.setTextColor(ILI9341_GREEN);
        tft.setFont(&FreeSansBold12pt7b);
        tft.setCursor(11*squareSize, 15*squareSize);
        tft.print("0x");
        tft.setFont(&FreeSansBold24pt7b);
        tft.setCursor(14*squareSize, 15*squareSize);
        tft.print(address, HEX);
        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print("Unknown error at address 0x");
        tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
        tft.setTextColor(ILI9341_GREEN);
        tft.setFont(&FreeSansBold9pt7b);
        tft.setCursor(4*squareSize, 10*squareSize);
        tft.print("Unknown error at address");
        
        if (address < 16)
        {
          Serial.print("0");
          tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
          tft.setTextColor(ILI9341_GREEN);
          tft.setFont(&FreeSansBold9pt7b);
          tft.setCursor(4*squareSize, 10*squareSize);
          tft.print("0");
        } 
        Serial.println(address, HEX);
        tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
        tft.setTextColor(ILI9341_GREEN);
        tft.setFont(&FreeSansBold9pt7b);
        tft.setCursor(4*squareSize, 10*squareSize);
        tft.print(address, HEX); 
      }
    }
    
    if (nDevices == 0)
    {
      Serial.println("No I2C devices found\n");
      tft.fillRect(3*squareSize, 3*squareSize, width-(6*squareSize), height-(6*squareSize), ILI9341_BLACK);
      tft.setTextColor(ILI9341_MAGENTA);
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor(4*squareSize, 10*squareSize);
      tft.print("No I2C devices found\n");
    }
    else
    {
      Serial.println("done\n");
      tft.setTextColor(ILI9341_GREENYELLOW);
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor(21*squareSize, 19*squareSize);
      tft.print("...done!\n");
    }
  }
}



void chequers() // function to generate chequered background
{
  tft.fillScreen(ILI9341_BLACK);
  for ( uint16_t x = 0; x <= width; x = x + (2*squareSize) ) // x-position
  {
    for (uint16_t y = 0; y <= height; y = y + (2*squareSize) )
    {
      tft.fillRect(x, y, squareSize, squareSize, ILI9341_WHITE);
    } 
  }
  
  for ( uint16_t x = squareSize; x <= width; x = x + (2*squareSize) ) // x-position
  {
    for (uint16_t y = squareSize; y <= height; y = y + (2*squareSize) )
    {
      tft.fillRect(x, y, squareSize, squareSize, ILI9341_WHITE);
    } 
  }
}
