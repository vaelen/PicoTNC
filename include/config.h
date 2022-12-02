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

#include <Arduino.h>

const String APRS_IS_USER = "JJ1OKA"; 
const String APRS_IS_PASSCODE = "17318"; 
const String APRS_IS_TOOL_NAME = "NanoTNC";
const String APRS_IS_TOOL_VERSION = "1.0"; 
const String APRS_IS_SERVER = "asia.aprs2.net"; 
const int APRS_IS_PORT = 14580; 
const String APRS_IS_FILTER = "m/5 o/JJ1OKA* g/JJ1OKA* b/JJ1OKA*"; 

bool isGPSEnabled();
bool isEthEnabled();
bool isWifiEnabled();
bool isI2CEnabled();