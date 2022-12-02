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
#include <ansi.h>

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
    if (gpsSerial.available()) {
        while (gpsSerial.available()) {
            int c = gpsSerial.read();
            gps.encode(c);
        }
    }
}

void printGPSInfo() {
    char buf[60];
    for (int i = 0; i < 60; i++) {
        buf[i] = 0;
    }

    if (!isGPSEnabled()) {
        sprintf(buf, "%3d", 0);
        console.setSatellites(buf);

        char *b = buf;
        strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
        b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
        strcpy_P(b, (const char *) F("GPS DISABLED"));
        console.setLocation(buf);
        console.setTime(buf);
    } else {
        sprintf(buf, "%3d", gps.satellites.value());
        console.setSatellites(buf);

        if (gps.location.isValid()) {
            sprintf(buf, "%3.60f,%3.60f", gps.location.lat(), gps.location.lng());
            console.setLocation(buf);
        } else {
            char *b = buf;
            strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
            b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
            strcpy_P(b, (const char *) F("INVALID"));
            console.setLocation(buf);
        }

        if (gps.date.isValid()) {
            sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d.%03d", 
                gps.date.month(), gps.date.day(), gps.date.year(),
                gps.time.hour(), gps.time.minute(), gps.time.second(), gps.time.centisecond());
            console.setTime(buf);
        } else {
            char *b = buf;
            strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
            b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
            strcpy_P(b, (const char *) F("INVALID"));
            console.setTime(buf);
        }
    }
}