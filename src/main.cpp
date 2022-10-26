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
#include <Ethernet.h>
#include <SPI.h>

#include <APRS_IS.h>
#include <tnc2.h>

const pin_size_t PIN_ETH_RST = 20;
const pin_size_t PIN_ETH_INT = 21;

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

void setup() {
  Serial.begin(9600);

  delay(3000);

  Serial.println("Starting...");

  Serial.print("Initializing Pins.. ");
  pinMode(PIN_ETH_RST, OUTPUT);
  Serial.print("1");
  digitalWrite(PIN_ETH_RST, HIGH);
  Serial.print("2");
  pinMode(PIN_ETH_INT, INPUT);
  Serial.print("3");
  Serial.println(" Done.");
 
  Serial.print("Initializing SPI.. ");
  SPI.begin(false);
  Serial.println(" Done.");

  Serial.print("Initializing Ethernet.. ");
  Ethernet.init(PIN_SPI0_SS);
  Ethernet.begin(mac, emptyIP);
  Serial.println(" Done.");

  // Serial.print("Starting NTP Client.. ");
  // NTP.begin("pool.ntp.org", "time.nist.gov");
  // Serial.println(" Done.");
}

void setup1() {
  delay(5000);
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

void loop() {
  if (networkConnected()) {
    if (!client.connected()) {
      Serial.print("Starting APRS-IS Client.. ");
      bool connected = client.connect(APRS_IS_SERVER, APRS_IS_PORT, APRS_IS_FILTER);
      Serial.println(connected ? " Done." : "Failed.");
    } else {
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

void loop1() {

  while(networkConnected()) {
    Ethernet.maintain();
  }
  Serial.println("Network disconnected");
  delay(1000);
}