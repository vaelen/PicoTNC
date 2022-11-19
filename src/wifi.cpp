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

/*
// Defines where Wifi config is stored in the EEPROM
#define EEPROM_START            0
// Change this to true to ask for custom parameters during Wifi config
#define USE_DYNAMIC_PARAMETERS  false
// Scan for wifi networks
#define SCAN_WIFI_NETWORKS      true
// Use the builtin LED for showing wifi status
#define USING_CONFIG_MODE_LED   true
#define CONFIG_MODE_LED         LED_BUILTIN
#define HOST_NAME "PicoTNC"
*/

#include <wifi.h>
//#include <ESP_AT_Lib.h>
#include <console.h>

SerialUART espSerial(uart1, PIN_ESP_TX, PIN_ESP_RX);
//ESP8266 wifi(&espSerial);

void initWifi() {
    if (!isWifiEnabled()) return;
    espSerial.begin(115200);
    while(!espSerial);
}

void updateWifi() {
    if (!isWifiEnabled()) return;
}

bool wifiConnected() {
    if (!isWifiEnabled()) return false;
    return false;
}