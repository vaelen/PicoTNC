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

#include <ansi.h>

using arduino::__FlashStringHelper;

namespace ANSI {
    const __FlashStringHelper* RESET = F("\033[0m");
    const __FlashStringHelper* BOLD = F("\033[1m");
    const __FlashStringHelper* UNDERLINE = F("\033[4m");
    const __FlashStringHelper* BLINK = F("\033[5m");
    const __FlashStringHelper* INVERSE = F("\033[7m");
    const __FlashStringHelper* BOLD_OFF = F("\033[22m");
    const __FlashStringHelper* UNDERLINE_OFF = F("\033[24m");
    const __FlashStringHelper* BLINK_OFF = F("\033[25m");
    const __FlashStringHelper* INVERSE_OFF = F("\033[27m");

    const __FlashStringHelper* FULL_RESET = F("\033c");
    const __FlashStringHelper* SAVE_CURSOR = F("\0337");
    const __FlashStringHelper* RESTORE_CURSOR = F("\0338");

    const __FlashStringHelper* SHOW_CURSOR = F("\033[?25h");
    const __FlashStringHelper* HIDE_CURSOR = F("\033[?25l");

    const __FlashStringHelper* CLEAR_SCREEN_BELOW = F("\033[0J");
    const __FlashStringHelper* CLEAR_SCREEN_ABOVE = F("\033[1J");
    const __FlashStringHelper* CLEAR_SCREEN = F("\033[2J");
    const __FlashStringHelper* CLEAR_SCREEN_AND_SCROLLBACK = F("\033[4J");

    const __FlashStringHelper* CLEAR_LINE_AFTER = F("\033[0K");
    const __FlashStringHelper* CLEAR_LINE_BEFORE = F("\033[1K");
    const __FlashStringHelper* CLEAR_LINE = F("\033[2K");

    const __FlashStringHelper* CURSOR_UP = F("\033[A");
    const __FlashStringHelper* CURSOR_DOWN = F("\033[B");
    const __FlashStringHelper* CURSOR_FORWARD = F("\033[C");
    const __FlashStringHelper* CURSOR_BACK = F("\033[D");
    const __FlashStringHelper* CURSOR_NEXT_LINE = F("\033[E");
    const __FlashStringHelper* CURSOR_PREV_LINE = F("\033[F");

    const __FlashStringHelper* SCROLL_UP = F("\033[S");
    const __FlashStringHelper* SCROLL_DOWN = F("\033[T");

    const __FlashStringHelper* HOME = F("\033[H");
    const __FlashStringHelper* DEVICE_STATUS_REPORT = F("\033[H");

    const __FlashStringHelper* SHIFT_OUT = F("\x0E");
    const __FlashStringHelper* SHIFT_IN = F("\x0F");

    namespace FG {
        const __FlashStringHelper* BLACK = F("\033[30m");
        const __FlashStringHelper* RED = F("\033[31m");
        const __FlashStringHelper* GREEN = F("\033[32m");
        const __FlashStringHelper* YELLOW = F("\033[33m");
        const __FlashStringHelper* BLUE = F("\033[34m");
        const __FlashStringHelper* MAGENTA = F("\033[35m");
        const __FlashStringHelper* CYAN = F("\033[36m");
        const __FlashStringHelper* WHITE = F("\033[37m");

        namespace BRIGHT {
            const __FlashStringHelper* BLACK = F("\033[90m");
            const __FlashStringHelper* RED = F("\033[91m");
            const __FlashStringHelper* GREEN = F("\033[92m");
            const __FlashStringHelper* YELLOW = F("\033[93m");
            const __FlashStringHelper* BLUE = F("\033[94m");
            const __FlashStringHelper* MAGENTA = F("\033[95m");
            const __FlashStringHelper* CYAN = F("\033[96m");
            const __FlashStringHelper* WHITE = F("\033[97m");
        }
    }

    namespace BG {
        const __FlashStringHelper* BLACK = F("\033[40m");
        const __FlashStringHelper* RED = F("\033[41m");
        const __FlashStringHelper* GREEN = F("\033[42m");
        const __FlashStringHelper* YELLOW = F("\033[43m");
        const __FlashStringHelper* BLUE = F("\033[44m");
        const __FlashStringHelper* MAGENTA = F("\033[45m");
        const __FlashStringHelper* CYAN = F("\033[46m");
        const __FlashStringHelper* WHITE = F("\033[47m");

        namespace BRIGHT {
            const __FlashStringHelper* BLACK = F("\033[100m");
            const __FlashStringHelper* RED = F("\033[101m");
            const __FlashStringHelper* GREEN = F("\033[102m");
            const __FlashStringHelper* YELLOW = F("\033[103m");
            const __FlashStringHelper* BLUE = F("\033[104m");
            const __FlashStringHelper* MAGENTA = F("\033[105m");
            const __FlashStringHelper* CYAN = F("\033[106m");
            const __FlashStringHelper* WHITE = F("\033[107m");
        }
    }

    void moveCursor(Print& p, int8_t row, uint8_t column) {
        p.print(F("\033["));
        p.print(row);
        p.print(';');
        p.print(column);
        p.print('H');
    }

}