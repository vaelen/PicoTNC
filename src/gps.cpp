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

String gpsTime() {
    char buf[60];
    buf[0] = 0;
    if (!isGPSEnabled()) {
        char *b = buf;
        strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
        b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
        strcpy_P(b, (const char *) F("DISABLED"));
        b += strlen_P((const char *) F("DISABLED"));
        strcpy_P(b, (const char *) ANSI::RESET);
    } else if (gps.date.isValid()) {
        sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d.%03d", 
            gps.date.month(), gps.date.day(), gps.date.year(),
            gps.time.hour(), gps.time.minute(), gps.time.second(), gps.time.centisecond());       
    } else {
        char *b = buf;
        strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
        b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
        strcpy_P(b, (const char *) F("INVALID"));
        b += strlen_P((const char *) F("INVALID"));
        strcpy_P(b, (const char *) ANSI::RESET);
    }
    return String(buf);
}

int gpsSatellites() {
    if (!isGPSEnabled()) return 0;
    return gps.satellites.value();
}

String gpsLocation() {
    char buf[60];
    buf[0] = 0;
    if (!isGPSEnabled()) {
        char *b = buf;
        strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
        b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
        strcpy_P(b, (const char *) F("DISABLED"));
        b += strlen_P((const char *) F("DISABLED"));
        strcpy_P(b, (const char *) ANSI::RESET);
    } else if (gps.location.isValid()) {
            sprintf(buf, "%3.6f,%3.6f", gps.location.lat(), gps.location.lng());      
    } else {
        char *b = buf;
        strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
        b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
        strcpy_P(b, (const char *) F("INVALID"));
        b += strlen_P((const char *) F("INVALID"));
        strcpy_P(b, (const char *) ANSI::RESET);
    }
    return String(buf);
}