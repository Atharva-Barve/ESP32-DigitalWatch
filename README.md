# ESP32 Digital Watch

## Overview

An ESP32-based digital watch with a real-time clock (RTC) module and display. The watch features:
Timekeeping using an RTC module
OLED/TFT display support for different hardware configurations
Stopwatch mode with pause and reset functionality
Button-controlled mode switching

## Features

Real-Time Clock (RTC): Maintains accurate timekeeping even when powered off.
Display Support: Works with both OLED and TFT displays.
Stopwatch Functionality: Users can switch between clock mode and stopwatch mode.

## Button Controls:

One button to toggle between clock and stopwatch mode.
Another button to pause/reset the stopwatch.

## Hardware Used

ESP32 microcontroller
RTC DS1307 module
OLED Display (SSD1306) / TFT Display (ST7735)
Push buttons for control

## Code Implementation

Two versions of the firmware are available:
OLED Version: Uses an SSD1306 display.
TFT Version: Uses an ST7735 display.
Each version is available in its respective folder.

## Install the required libraries in Arduino IDE:

**Wire.h**
**Adafruit_GFX.h**
**Adafruit_SSD1306.h (for OLED version)**
**Adafruit_ST7735.h (for TFT version)**
**RTClib.h**
**EasyButton.h**

**Upload the corresponding code to your ESP32.
Connect the required hardware as per the schematic.
Press the mode button to toggle between clock and stopwatch.
Use the pause/reset button for stopwatch control.**

