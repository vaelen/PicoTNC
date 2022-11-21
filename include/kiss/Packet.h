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

#pragma once

#include <Arduino.h>

namespace kiss {

    const String KISS_FRAME_END = "\xC0";
    const String KISS_FRAME_ESCAPE = "\xDB";
    const String KISS_ESCAPE_FEND = "\xDB\xDC";
    const String KISS_ESCAPE_FESC = "\xDB\xDD";

    enum Command {
        DataFrame = 0,
        TXDelay = 1,
        Persistence = 2,
        SlotTime = 3,
        TXTail = 4,
        FullDuplex = 5,
        SetHardware = 6,
        Return = 127,
    };

    String encodeData(const String &data);
    String decodeData(const String &data, int beginIndex = 0, int endIndex = -1);

    byte encodeCommand(Command command, byte port = 0);

    class Packet : public Printable {
        public:
            Command command = DataFrame;
            byte port = 0;
            String data;

            // Default to an empty data packet
            Packet(): command(DataFrame), port(0), data("") {}

            // Parse a packet from an input string.
            Packet(const String &s);

            // Constructor for unit tests
            Packet(Command command, byte port, String data): command(command), port(port), data(data) {}

            // Human readable command name
            String commandName() const;

            // Encode a packet to a raw string
            String encode() const;

            // Print a nice version for logging, etc.
            virtual size_t printTo(Print& p) const;
    };

}
