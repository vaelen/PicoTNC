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

#ifndef aprs_data_h_
#define aprs_data_h_

#include <Arduino.h>

namespace aprs {
    // Ambiguous (Big Question mark)
    const String DEFAULT_SYMBOL = "\\.";

    const char PADDING_CHAR = '.';
    String pad(const String &s, size_t length);

    class Data: public Printable {
        public:
        String raw_data;

        Data(): raw_data() {};
        Data(const String s): raw_data(s) {};

        virtual String encode() const;
        virtual size_t printTo(Print& p) const;       
    };

    /*
    Examples:
    !4903.50N/07201.75W-Test 001234 no timestamp, no APRS messaging, with comment.
    !4903.50N/07201.75W-Test /A=001234 no timestamp, no APRS messaging, altitude = 1234 ft.
    !49VV.VVN/072VV.VVW- no timestamp, no APRS messaging, location to nearest degree.
    */
    class PositionReport: public Data {
        public:
        bool messaging;
        String latitude;
        String longitude;
        String symbol;
        String comment;

        PositionReport(): 
            messaging(false), latitude(), longitude(), symbol(DEFAULT_SYMBOL), comment() {};
        PositionReport(bool messaging): 
            messaging(messaging), latitude(), longitude(), symbol(DEFAULT_SYMBOL), comment() {};
        PositionReport(bool messaging, const String s);

        virtual String encode() const;
        virtual size_t printTo(Print& p) const;
    };

        /*
    Examples:
    /092345z4903.50N/07201.75W>Test1234 with timestamp, no APRS messaging, zulu time, with comment.
    @092345/4903.50N/07201.75W>Test1234 with timestamp, with APRS messaging, local time, with comment.
    */
    class PositionReportWithTimestamp: public PositionReport {
        public:
        String timestamp;

        PositionReportWithTimestamp(): 
            PositionReport(), timestamp() {};
        PositionReportWithTimestamp(bool messaging): 
            PositionReport(messaging), timestamp() {};
        PositionReportWithTimestamp(bool messaging, const String s);

        virtual String encode() const;
        virtual size_t printTo(Print& p) const;
    };
}

#endif