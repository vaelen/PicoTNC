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

void setup() {
  Serial.begin(9600); // Console
  Serial1.begin(9600); // GPS

  delay(3000);

  Serial.println("Starting...");

  Serial.print("Initializing I2C.. ");
  Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.begin();
  Serial.println(" Done.");

  Serial.print("Initializing LCD.. ");
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setBacklight(128);
  lcd.home();
  lcd.clear();
  lcd.print("Loading...");
  Serial.println(" Done.");
 
  Serial.print("Initializing SPI.. ");
  SPI.begin(false);
  Serial.println(" Done.");
 
  Serial.print("Initializing Ethernet Pins.. ");
  pinMode(PIN_ETH_RST, OUTPUT);
  digitalWrite(PIN_ETH_RST, HIGH);
  pinMode(PIN_ETH_INT, INPUT);
  Serial.println(" Done.");

  Serial.print("Initializing Ethernet.. ");
  Ethernet.init(PIN_SPI0_SS);
  Ethernet.begin(mac, emptyIP);
  Serial.println(" Done.");

  // Serial.print("Starting NTP Client.. ");
  // NTP.begin("pool.ntp.org", "time.nist.gov");
  // Serial.println(" Done.");
}

void printLinkStatus() {
  Serial.print("Controller: ");

  switch (Ethernet.hardwareStatus()) {
    case EthernetNoHardware: Serial.print("None"); break;
    case EthernetW5100: Serial.print("W5100"); break;
    case EthernetW5200: Serial.print("W5200"); break;
    default: Serial.print("W5500"); break;
  }

  Serial.print(", Link: ");
  switch (Ethernet.linkStatus()) {
    case LinkON: Serial.print("On"); break;
    case LinkOFF: Serial.print("Off"); break;
    default: Serial.print("Unknown"); break;
  }

  Serial.print(", IP: ");
  Serial.println(Ethernet.localIP());
}

bool networkConnected() {
  return Ethernet.linkStatus() == LinkON && Ethernet.localIP() != IPAddress(emptyIP);
}

void gpsInfo()
{
  Serial.print(F("Satellites: "));
  Serial.print(gps.satellites.value());
  Serial.print(F("  "));
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void updateLcd() {

}

unsigned long lastMillis = 0;

void loop() {
  // Update GPS
  while (Serial1.available()) {
    int c = Serial1.read();
    gps.encode(c);
    //Serial.write(c);
  }

  // Display GPS info
  unsigned long m = millis();
  if (m % 5000 == 0 && m != lastMillis) {
    gpsInfo();
    lastMillis = m;
  }

  // Check for network connection
  if (networkConnected()) {
    Ethernet.maintain();
    // Check for APRS-IS connection
    if (!client.connected()) {
      Serial.print("Starting APRS-IS Client.. ");
      bool connected = client.connect(APRS_IS_SERVER, APRS_IS_PORT, APRS_IS_FILTER);
      Serial.println(connected ? " Done." : "Failed.");
    } else {
      // Network and APRS-IS client connected
      if (client.connected() && client.available()) {
        String message = client.getMessage();
        if (message.startsWith("#")) {
          // Comment
          Serial.println(message);
        } else {
          // Packet
          tnc2::Packet packet(message);
          Serial.println(packet);
        }
      }
    }
  } else {
    Serial.print("Obtaining IP Address.. ");
    int ethStatus = Ethernet.begin(mac);
    Serial.println(ethStatus == 1 ? " Done." : "Failed.");
    if (ethStatus == 0) {
      Serial.print("Using Default IP Address.. ");
      Ethernet.begin(mac, ip);
      Serial.println(" Done.");
    }
    printLinkStatus();
  }
}

void setup2() {
  Serial2.begin(9600); // KISS TNC
}

void loop2() {
  while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
}