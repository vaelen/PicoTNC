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

#include <api/String.h>
#include <api/Print.h>
#include <api/Stream.h>
#include <stdint.h>

class Console: public Print {
    public:
        Console();
        ~Console();

        // Lifecycle methods
        void begin(Stream &stream);
        void update();

        // Print implementation
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buffer, size_t size);
        virtual int availableForWrite();
        virtual void flush();

    protected:
        Stream *stream;
};

extern Console console;