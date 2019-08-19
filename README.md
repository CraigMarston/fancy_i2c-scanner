# fancy_i2c-scanner
I2C scanner sketch with a TFT screen to build a standalone device

I2C scanner: Maybe elaborate, but I wanted to use an SPI screen to keep the I2C bus free! This size screen is around the same size as the tiny OLED screens anyhow.

I started off using an Arduino Nano clone, but I can’t be sure the cheap TFT screen is 5V tolerant — although it does have a 3.3V regulator on-board for its supply. I would have needed two 4-channel voltage shifters in this case, so just opted to use an ESP8266 device instead. An Arduino Nano Pro (3.3v) would be ideal for this.

The sketch is built from a basic I2C scanner and I’ve simply added the facility for the SPI screen.

The plan is to put this into a box so it becomes a tool that I can just grab.


Colour codes can be found here: https://github.com/esp8266/Basic/blob/master/libraries/Adafruit_ILI9341/Adafruit_ILI9341.h
