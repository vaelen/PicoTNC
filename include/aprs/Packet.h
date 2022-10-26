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

#ifndef aprs_packet_h_
#define aprs_packet_h_

#include <Arduino.h>
#include "Data.h"

namespace aprs {
    class Packet : public Printable {
        public:
            char data_type;
            Data data;

            // Default to an empty, user-defined packet
            Packet(): data_type('{'), data() {};

            // A packet with only a data type
            Packet(const char data_type): data_type(data_type), data() {};

            // A packet with a data type and some data
            Packet(const char data_type, const Data data): data_type(data_type), data(data) {};

            // Human readable data type name
            String dataTypeName() const;

            // Encode a packet to a raw string
            String encode() const;

            // Print a nice version for logging, etc.
            virtual size_t printTo(Print& p) const;
    };

    //  Parse a packet from a raw APRS data string
    Packet parse(const String s);
};

#endif