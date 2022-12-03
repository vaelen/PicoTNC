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
#include <ansi.h>

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
  console.print(F("Obtaining IP Address.. "));
  int ethStatus = Ethernet.begin(mac);
  console.print(ethStatus == 1 ? F(" Done.\n") : F("Failed.\n"));
  if (ethStatus == 0) {
      console.print(F("Using Default IP Address.. "));
      Ethernet.begin(mac, ip);
      console.print(F(" Done.\n"));
  }
}

bool ethConnected() {
  if (!isEthEnabled()) return false;
  return Ethernet.localIP() != IPAddress(emptyIP);
}

String ethIPAddress() {
  char buf[40];
  buf[0] = 0;
  if (isEthEnabled()) {
    sprintf(buf, "%3d.%3d.%3d.%3d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
  } else {
    char *b = buf;
    strcpy_P(b, (const char *) ANSI::FG::BRIGHT::RED);
    b += strlen_P((const char *) ANSI::FG::BRIGHT::RED);
    strcpy_P(b, (const char *) F("DISABLED"));
  }
  return String(buf);
}

void updateEth() {
  if (!isEthEnabled()) return;
  if (ethConnected()) {
    Ethernet.maintain();
  }
}