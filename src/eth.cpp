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

#include <config.h>
#include <eth.h>

#include <pins.h>
#include <console.h>
#include <config.h>

#include <api/Compat.h>
#include <Ethernet_Generic.h>

uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
uint8_t ip[] = { 192, 168, 1, 200 };
uint8_t emptyIP[] = { 0, 0, 0, 0};

void initEth() {
  if (!isEthEnabled()) return;

  SPI.setRX(PIN_ETH_MISO);
  SPI.setTX(PIN_ETH_MOSI);
  SPI.setSCK(PIN_ETH_SCK);
  SPI.setCS(PIN_ETH_SS);
  SPI.begin(false);

  pinMode(PIN_ETH_RST, OUTPUT);
  digitalWrite(PIN_ETH_RST, HIGH);
  pinMode(PIN_ETH_INT, INPUT);

  Ethernet.setRstPin(PIN_ETH_RST);
  Ethernet.init(PIN_ETH_SS);
  Ethernet.begin(mac, emptyIP);
}

void beginEth() {
  if (!isEthEnabled()) return;
  console.print("Obtaining IP Address.. ");
  int ethStatus = Ethernet.begin(mac);
  console.println(ethStatus == 1 ? " Done." : "Failed.");
  if (ethStatus == 0) {
      console.print("Using Default IP Address.. ");
      Ethernet.begin(mac, ip);
      console.println(" Done.");
  }
}

bool ethConnected() {
  if (!isEthEnabled()) return false;
  return Ethernet.localIP() != IPAddress(emptyIP);
}

void printEthLinkStatus() {
  console.print("Ethernet: ");

  if (isEthEnabled()) {
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
  } else {
    console.println("Disabled");
  }
}

void updateEth() {
  if (!isEthEnabled()) return;
  if (ethConnected()) {
    Ethernet.maintain();
  }
}