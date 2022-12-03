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

#include <Arduino.h>
#include <console.h>
#include <ansi.h>

Console console;

Console::Console(): Print(), stream(NULL) {}
Console::~Console() {}

// Lifecycle Methods

void Console::begin(Stream &s) {
    stream = &s;
    stream->print(ANSI::CLEAR_SCREEN);
}

void Console::update() {
    if (stream == NULL) return;
}

// Print Implementation

size_t Console::write(uint8_t b) {
    if (stream == NULL) return 0;
    return stream->write(b);
}

size_t Console::write(const uint8_t *buffer, size_t size) {
    if (stream == NULL) return 0;
    return stream->write(buffer, size);
}

int Console::availableForWrite() {
    if (stream == NULL) return 0;
    return stream->availableForWrite();
}

void Console::flush() {
    if (stream == NULL) return;
    stream->flush();
}
