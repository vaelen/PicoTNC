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

#include "aprs/Packet.h"

namespace aprs {
    void _parse(Packet &packet, const String &s) {
        if (packet.data != NULL) delete packet.data;
        switch(packet.data_type) {
            case '!':
                packet.data = new PositionReport(false, s);
                break;
            case '=':
                packet.data = new PositionReport(true, s);
                break;
            case '/':
                packet.data = new PositionReportWithTimestamp(false, s);
                break;
            case '@':
                packet.data = new PositionReportWithTimestamp(true, s);
                break;
            default:
                packet.data = new Data(s);
                break;
        }
    }

    Packet parse(const String s) {
        Packet packet;
        if (s.length() > 0) {
            packet.data_type = s.charAt(0);
            _parse(packet, s.substring(1));
        }
        return packet;
    }

    // Returns a human readable data type
    String Packet::dataTypeName() const {
        String name;
        switch(data_type) {
            case 0x1c:
            case 0x1d:
            case '`':
            case '\'':
                name.concat("Mic-E");
                break;
            case '=':
            case '!':
            case '@':
            case '/':
                name.concat("Position");
                break;
            case ':':
                name.concat("Message");
                break;
            case ';':
                name.concat("Object");
                break;
            case ')':
                name.concat("Item");
                break;
            case '<':
                name.concat("Station Capabilities");
                break;
            case '>':
                name.concat("Status");
                break;
            case 'T':
                name.concat("Telemetry");
                break;
            case '_':
                name.concat("Weather Report");
                break;
            case '$':
                name.concat("Raw GPS Data");
                break;
            case '{':
                name.concat("User-Defined");
                break;
            case '}':
                name.concat("Third-Party Traffic");
                break;
            default:
                name.concat("Unknown");
                break;
        }
        name.concat(" [");
        if (isPrintable(data_type)) {
            name.concat(data_type);
        }
        else {
            name.concat("0x");
            name.concat(String(data_type, HEX));
        }
        name.concat("]");
        return name;
    }

    String Packet::encode() const {
        return data_type + (data == NULL ? String() : data->encode());
    }

    size_t Packet::printTo(Print& p) const {
        return 
            p.print("Type: ") + p.print(dataTypeName()) +
            (data == NULL ? 0 : (p.print(", ") + data->printTo(p)));
    }
}