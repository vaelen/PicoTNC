/*
Copyright (c) 2022 Andrew C. Young (JJ1OKA / NU8W)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <api/Common.h>
#include <pins_arduino.h>

// Serial / GPS
#define PIN_GPS_TX      (0u)
#define PIN_GPS_RX      (1u)

// Serial / KISS
#define PIN_KISS_TX     (4u)
#define PIN_KISS_RX     (5u)

// Serial / Console
#define PIN_CONSOLE_TX  (2u)
#define PIN_CONSOLE_RX  (3u)

// SPI / Ethernet
#define PIN_ETH_MISO    (16u)
#define PIN_ETH_MOSI    (19u)
#define PIN_ETH_SCK     (18u)
#define PIN_ETH_SS      (17u)
#define PIN_ETH_RST     (20u)
#define PIN_ETH_INT     (21u)

// I2C
#define PIN_SDA         (8u)
#define PIN_SCL         (9u)

// Serial // ESP Wifi
#define PIN_ESP_TX      (8u)
#define PIN_ESP_RX      (9u)