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

using arduino::__FlashStringHelper;

namespace ANSI {
    extern const __FlashStringHelper* RESET;
    extern const __FlashStringHelper* BOLD;
    extern const __FlashStringHelper* UNDERLINE;
    extern const __FlashStringHelper* BLINK;
    extern const __FlashStringHelper* INVERSE;
    extern const __FlashStringHelper* BOLD_OFF;
    extern const __FlashStringHelper* UNDERLINE_OFF;
    extern const __FlashStringHelper* BLINK_OFF;
    extern const __FlashStringHelper* INVERSE_OFF;

    extern const __FlashStringHelper* FULL_RESET;
    extern const __FlashStringHelper* SAVE_CURSOR;
    extern const __FlashStringHelper* RESTORE_CURSOR;

    extern const __FlashStringHelper* SHOW_CURSOR;
    extern const __FlashStringHelper* HIDE_CURSOR;

    extern const __FlashStringHelper* CLEAR_SCREEN_BELOW;
    extern const __FlashStringHelper* CLEAR_SCREEN_ABOVE;
    extern const __FlashStringHelper* CLEAR_SCREEN;
    extern const __FlashStringHelper* CLEAR_SCREEN_AND_SCROLLBACK;

    extern const __FlashStringHelper* CLEAR_LINE_AFTER;
    extern const __FlashStringHelper* CLEAR_LINE_BEFORE;
    extern const __FlashStringHelper* CLEAR_LINE;

    extern const __FlashStringHelper* CURSOR_UP;
    extern const __FlashStringHelper* CURSOR_DOWN;
    extern const __FlashStringHelper* CURSOR_FORWARD;
    extern const __FlashStringHelper* CURSOR_BACK;
    extern const __FlashStringHelper* CURSOR_NEXT_LINE;
    extern const __FlashStringHelper* CURSOR_PREV_LINE;

    extern const __FlashStringHelper* SCROLL_UP;
    extern const __FlashStringHelper* SCROLL_DOWN;

    extern const __FlashStringHelper* HOME;
    extern const __FlashStringHelper* DEVICE_STATUS_REPORT;

    extern const __FlashStringHelper* SHIFT_IN;
    extern const __FlashStringHelper* SHIFT_OUT;

    namespace FG {
        extern const __FlashStringHelper* BLACK;
        extern const __FlashStringHelper* RED;
        extern const __FlashStringHelper* GREEN;
        extern const __FlashStringHelper* YELLOW;
        extern const __FlashStringHelper* BLUE;
        extern const __FlashStringHelper* MAGENTA;
        extern const __FlashStringHelper* CYAN;
        extern const __FlashStringHelper* WHITE;

        namespace BRIGHT {
            extern const __FlashStringHelper* BLACK;
            extern const __FlashStringHelper* RED;
            extern const __FlashStringHelper* GREEN;
            extern const __FlashStringHelper* YELLOW;
            extern const __FlashStringHelper* BLUE;
            extern const __FlashStringHelper* MAGENTA;
            extern const __FlashStringHelper* CYAN;
            extern const __FlashStringHelper* WHITE;
        }
    }

    namespace BG {
        extern const __FlashStringHelper* BLACK;
        extern const __FlashStringHelper* RED;
        extern const __FlashStringHelper* GREEN;
        extern const __FlashStringHelper* YELLOW;
        extern const __FlashStringHelper* BLUE;
        extern const __FlashStringHelper* MAGENTA;
        extern const __FlashStringHelper* CYAN;
        extern const __FlashStringHelper* WHITE;

        namespace BRIGHT {
            extern const __FlashStringHelper* BLACK;
            extern const __FlashStringHelper* RED;
            extern const __FlashStringHelper* GREEN;
            extern const __FlashStringHelper* YELLOW;
            extern const __FlashStringHelper* BLUE;
            extern const __FlashStringHelper* MAGENTA;
            extern const __FlashStringHelper* CYAN;
            extern const __FlashStringHelper* WHITE;
        }
    }

    void moveCursor(Print& p, int8_t row, uint8_t column);

}