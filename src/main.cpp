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

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <config.h>
#include <pins.h>
#include <console.h>

#include <gps.h>
#include <network.h>

#include <APRS_IS.h>
#include <tnc2.h>

APRS_IS client(APRS_IS_USER, APRS_IS_PASSCODE, APRS_IS_TOOL_NAME, APRS_IS_TOOL_VERSION);

SerialPIO tncSerial(PIN_KISS_TX,PIN_KISS_RX, 256u);

void initTNC() {
  tncSerial.begin(38400); // KISS TNC
}

void checkTNC() {
  if (!tncSerial) return;
  // Check TNC
  if (tncSerial.available() > 0) {
    console.print("KISS: ");
    while (tncSerial.available()) {
      int c = tncSerial.read();
      if (c < 0x20 || c > 0x7e ) {
        console.print("<0x");
        if (c < 16) {
          console.print("0");
        }
        console.print(c, 16);
        console.print(">");
      } else {
        console.print((char)c);
      }
    }
    console.print("\r\n");
  }
}

void setup() {
  initConsole();
  initGPS();
  initTNC();

  delay(3000);

  console.println("Starting...");

  if (isI2CEnabled()) {
    console.print("Initializing I2C.. ");
    Wire.setSDA(PIN_SDA);
    Wire.setSCL(PIN_SCL);
    Wire.begin();
    console.println(" Done.");
  }

  if (isEthEnabled()) {
    console.print("Initializing Ethernet.. ");
    initEth();
    console.println(" Done.");
  }

  if (isWifiEnabled()) {
    console.print("Initializing Wifi.. ");
    initWifi();
    console.println(" Done.");
  }

  // console.print("Starting NTP Client.. ");
  // NTP.begin("pool.ntp.org", "time.nist.gov");
  // console.println(" Done.");

  console.println("Ready");
}

unsigned long lastMillis = 0;

void loop() {

  checkTNC();
  
  // Update GPS
  if (isGPSEnabled()) {
    updateGPS();
  }

  // Display debug info
  unsigned long m = millis();
  if (DEBUG_INTERVAL_MS > 0 && m % DEBUG_INTERVAL_MS == 0 && m != lastMillis) {
    // console.print("\xde\xad\xbe\xef\xde\xad\xbe\xef");
    if (isGPSEnabled() && debugGPS()) {
      printGPSInfo();
    }
    if (isEthEnabled() && debugEth()) {
      printLinkStatus();
    }
    lastMillis = m;
  }

  updateNetwork();

  // Check for network connection
  if (isEthEnabled()) {
    if (networkConnected()) {
      Ethernet.maintain();
      // Check for APRS-IS connection
      if (!client.connected()) {
        console.print("Starting APRS-IS Client.. ");
        bool connected = client.connect(APRS_IS_SERVER, APRS_IS_PORT, APRS_IS_FILTER);
        console.println(connected ? " Done." : "Failed.");
      } else {
        // Network and APRS-IS client connected
        if (client.connected() && client.available()) {
          String message = client.getMessage();
          if (message.startsWith("#")) {
            // Comment
            console.println(message);
          } else {
            // Packet
            tnc2::Packet packet(message);
            console.println(packet);
          }
        }
      }
    } else {
      beginEth();
      printLinkStatus();
    }
  }
}

// void setup1() {
//   initTNC();
// }

// void loop1() {
//   if (tncSerial) checkTNC();
// }