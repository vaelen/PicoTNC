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

#define ETHERNET_LARGE_BUFFERS 1

#include <Ethernet_Generic.h>

#include <APRS_IS.h>
#include <tnc2.h>

#include <TinyGPS++.h>

#include <LiquidCrystal_PCF8574.h>

const pin_size_t PIN_CONSOLE_TX = 6;
const pin_size_t PIN_CONSOLE_RX = 7;

const pin_size_t PIN_ETH_RST = 20;
const pin_size_t PIN_ETH_INT = 21;
const pin_size_t PIN_SDA = 8;
const pin_size_t PIN_SCL = 9;

const uint8_t LCD_ADDR = 0x27;
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

const uint8_t OLED_ADDR = 0x78;

const String APRS_IS_USER = "JJ1OKA"; 
const String APRS_IS_PASSCODE = "17318"; 
const String APRS_IS_TOOL_NAME = "NanoTNC";
const String APRS_IS_TOOL_VERSION = "1.0"; 
const String APRS_IS_SERVER = "asia.aprs2.net"; 
const int APRS_IS_PORT = 14580; 
const String APRS_IS_FILTER = "m/50 o/JJ1OKA* g/JJ1OKA* b/JJ1OKA*"; 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 200 };
byte emptyIP[] = { 0, 0, 0, 0};

APRS_IS client(APRS_IS_USER, APRS_IS_PASSCODE, APRS_IS_TOOL_NAME, APRS_IS_TOOL_VERSION);

TinyGPSPlus gps;

LiquidCrystal_PCF8574 lcd(LCD_ADDR);

// Serial ports
SerialPIO console(PIN_CONSOLE_TX, PIN_CONSOLE_RX);
SerialUART &gpsSerial = Serial1;
SerialUART &tncSerial = Serial2;

void setup() {
  console.begin(9600); // Console
  gpsSerial.begin(9600); // GPS
  tncSerial.begin(38400); // KISS TNC

  delay(3000);

  console.println("Starting...");

  console.print("Initializing I2C.. ");
  Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.begin();
  console.println(" Done.");

  console.print("Initializing LCD.. ");
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setBacklight(128);
  lcd.home();
  lcd.clear();
  lcd.print("Loading...");
  console.println(" Done.");
 
  console.print("Initializing SPI.. ");
  SPI.begin(false);
  console.println(" Done.");
 
  console.print("Initializing Ethernet Pins.. ");
  pinMode(PIN_ETH_RST, OUTPUT);
  digitalWrite(PIN_ETH_RST, HIGH);
  pinMode(PIN_ETH_INT, INPUT);
  console.println(" Done.");

  console.print("Initializing Ethernet.. ");
  Ethernet.init(PIN_SPI0_SS);
  Ethernet.begin(mac, emptyIP);
  console.println(" Done.");

  // console.print("Starting NTP Client.. ");
  // NTP.begin("pool.ntp.org", "time.nist.gov");
  // console.println(" Done.");
}

void printLinkStatus() {
  console.print("Controller: ");

  switch (Ethernet.hardwareStatus()) {
    case EthernetNoHardware: console.print("None"); break;
    case EthernetW5100: console.print("W5100"); break;
    case EthernetW5200: console.print("W5200"); break;
    default: console.print("W5500"); break;
  }

  console.print(", Link: ");
  switch (Ethernet.linkStatus()) {
    case LINK_ON: console.print("On"); break;
    case LINK_OFF: console.print("Off"); break;
    default: console.print("Unknown"); break;
  }

  console.print(", IP: ");
  console.println(Ethernet.localIP());
}

bool networkConnected() {
  return Ethernet.localIP() != IPAddress(emptyIP);
}

void gpsInfo()
{
  console.print(F("Satellites: "));
  console.print(gps.satellites.value());
  console.print(F("  "));
  console.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    console.print(gps.location.lat(), 6);
    console.print(F(","));
    console.print(gps.location.lng(), 6);
  }
  else
  {
    console.print(F("INVALID"));
  }

  console.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    console.print(gps.date.month());
    console.print(F("/"));
    console.print(gps.date.day());
    console.print(F("/"));
    console.print(gps.date.year());
  }
  else
  {
    console.print(F("INVALID"));
  }

  console.print(F(" "));
  if (gps.time.isValid())
  {
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
  }
  else
  {
    console.print(F("INVALID"));
  }

  console.println();
}

void updateLcd() {

}

unsigned long lastMillis = 0;

void loop() {
  // Update GPS
  while (gpsSerial.available()) {
    int c = gpsSerial.read();
    gps.encode(c);
    //console.write(c);
  }

  // Display GPS info
  unsigned long m = millis();
  if (m % 5000 == 0 && m != lastMillis) {
    gpsInfo();
    lastMillis = m;
  }

  // Check TNC
  if (tncSerial.available()) {
    console.print("KISS: ");
    while (tncSerial.available()) {
      int c = tncSerial.read();
      if (c < 0x20 || c > 0x7e ) {
        console.print("<0x");
        console.print(c, 16);
        console.print(">");
      } else {
        console.print((char)c);
      }
    }
    console.print("\r\n");
  }

  // Check for network connection
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
    console.print("Obtaining IP Address.. ");
    int ethStatus = Ethernet.begin(mac);
    console.println(ethStatus == 1 ? " Done." : "Failed.");
    if (ethStatus == 0) {
      console.print("Using Default IP Address.. ");
      Ethernet.begin(mac, ip);
      console.println(" Done.");
    }
    printLinkStatus();
  }
}

// void setup2() {
//   tncSerial.begin(38400); // KISS TNC
// }

// void loop2() {
//   while (tncSerial.available() > 0) {
//     console.write(tncSerial.read());
//   }
// }