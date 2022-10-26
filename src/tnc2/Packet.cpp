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

#include "tnc2/Packet.h"

// TNC2 Format:
// SOURCE>DESTINATION,PATH:PACKET

namespace tnc2 {
    Packet::Packet(const String s): source(), destination(), path(), data() {
        enum sections { SOURCE, DEST, PATH, DATA };
        int section = SOURCE;
        for (int i = 0; i < s.length(); i++) {
            const char c = s.charAt(i);
            if (c == '\n') {
                break;
            }
            if (c == '\r') {
                continue;
            }
            switch(section) {
                case SOURCE:
                    if (c == '>') {
                        section = DEST;
                    } else {
                        source.concat(c);
                    }
                    break;
                case DEST:
                    if (c == ',') {
                        section = PATH;
                    } else if (c == ':') {
                        section = DATA;
                    } else {
                        destination.concat(c);
                    }
                    break;
                case PATH:
                    if (c == ':') {
                        section = DATA;
                    } else {
                        path.concat(c);
                    }
                    break;
                default:
                    data.concat(c);
                    break;
            }
        }
    }

    aprs::Packet Packet::aprsData() const {
        return aprs::parse(data);
    }

    String Packet::encode() const {
        return source + ">" + destination + "," + path + ":" + data;
    }

    size_t Packet::printTo(Print& p) const {
        return 
            p.print("Source: ") + p.print(source) +
            p.print(", Destination: ") + p.print(destination) +
            p.print(", Path: ") + p.print(path) +
            //p.print(", Data: ") + p.print(data) +
            p.print(", Data: ") + p.print(aprsData());
    }
}