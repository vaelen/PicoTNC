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

#include "aprs/Data.h"

namespace aprs {

    String pad(const String &s, size_t length) {
        if (s.length() >= length) {
            return s.substring(0, length);
        }
        String out(s);
        while (out.length() < length) {
            out.concat(PADDING_CHAR);
        }
        return out;
    }

    ///// Data

    String Data::encode() const {
        return raw_data;
    }

    size_t Data::printTo(Print& p) const {
        return 
            p.print("Data: ") + p.print(raw_data);
    }

    ///// PositionReport

    PositionReport::PositionReport(bool messaging, const String s):
        messaging(messaging), 
        latitude(s.substring(0,8)), 
        longitude(s.substring(9,18)), 
        symbol(s.substring(8,9) + s.substring(18,19)), 
        comment(s.substring(19)) {}

    String PositionReport::encode() const {
        return pad(latitude, 8) + symbol[0] + pad(longitude, 9) + symbol[1] + comment.substring(0,43);
    }

    size_t PositionReport::printTo(Print& p) const {
        return
            p.print("Lat: ") + p.print(latitude.substring(0,8)) +
            p.print(", Long: ") + p.print(longitude.substring(0,9)) +
            p.print(", Symbol: ") + p.print(symbol) +
            p.print(", Msg: ") + p.print(messaging ? 'Y' : 'N') +
            p.print(", Comment: ") + p.print(comment.substring(0,43));
    }

    ///// PositionReportWithTimestamp

    PositionReportWithTimestamp::PositionReportWithTimestamp(bool messaging, const String s): 
        PositionReport(messaging, s.substring(7)), timestamp(s.substring(0,7)) {}

    String PositionReportWithTimestamp::encode() const {
        return pad(timestamp, 7) + PositionReport::encode();
    }

    size_t PositionReportWithTimestamp::printTo(Print& p) const {
        return
            p.print("Time: ") + p.print(timestamp) + p.print(", ") +PositionReport::printTo(p);
    }
}
