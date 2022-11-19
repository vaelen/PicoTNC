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

#include <pins.h>
#include <config.h>
#include <gps.h>
#include <console.h>

#include <SerialUART.h>

TinyGPSPlus gps;
SerialUART gpsSerial(uart0, PIN_GPS_TX, PIN_GPS_RX);

void initGPS() {
    if (!isGPSEnabled()) return;
    gpsSerial.begin(9600);
}

void updateGPS() {
    if (!isGPSEnabled()) return;
    // Update GPS
    while (gpsSerial.available()) {
        int c = gpsSerial.read();
        gps.encode(c);
    }
}

void printGPSInfo() {
    console.print(F("GPS Status: "));
    if (isGPSEnabled()) {
        console.print(F("Satellites: "));
        console.print(gps.satellites.value());
        console.print(F("  "));
        console.print(F("Location: ")); 
        if (gps.location.isValid()) {
            console.print(gps.location.lat(), 6);
            console.print(F(","));
            console.print(gps.location.lng(), 6);
        } else {
            console.print(F("INVALID"));
        }

        console.print(F("  Date/Time: "));
        if (gps.date.isValid()) {
            console.print(gps.date.month());
            console.print(F("/"));
            console.print(gps.date.day());
            console.print(F("/"));
            console.print(gps.date.year());
        } else {
            console.print(F("INVALID"));
        }

        console.print(F(" "));
        if (gps.time.isValid()) {
            if (gps.time.hour() < 10) console.print(F("0"));
            console.print(gps.time.hour());
            console.print(F(":"));
            if (gps.time.minute() < 10) console.print(F("0"));
            console.print(gps.time.minute());
            console.print(F(":"));
            if (gps.time.second() < 10) console.print(F("0"));
            console.print(gps.time.second());
            console.print(F("."));
            if (gps.time.centisecond() < 10) console.print(F("0"));
            console.print(gps.time.centisecond());
        } else {
            console.print(F("INVALID"));
        }
    } else {
        console.print(F("Disabled"));
    }

    console.println();
}