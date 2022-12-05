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
#include <pins.h>
#include <console.h>
#include <ansi.h>

SerialUART shellUART(uart1, PIN_CONSOLE_TX, PIN_CONSOLE_RX);
Console console;
struct ush_object ush;

// working buffers allocations (size could be customized)
#define BUF_IN_SIZE    32
#define BUF_OUT_SIZE   32
#define PATH_MAX_SIZE  32

static char ush_in_buf[BUF_IN_SIZE];
static char ush_out_buf[BUF_OUT_SIZE];

// root directory handler
static struct ush_node_object root;

// non-blocking read interface
static int ush_read(struct ush_object *self, char *ch)
{
    // should be implemented as a FIFO
    if (shellUART.available() > 0) {
        *ch = shellUART.read();
        return 1;
    }
    return 0;
}

// non-blocking write interface
static int ush_write(struct ush_object *self, char ch)
{
    // should be implemented as a FIFO
    return (shellUART.write(ch) == 1);
}

// I/O interface descriptor
static const struct ush_io_interface ush_iface = {
    .read = ush_read,
    .write = ush_write,
};

// microshell descriptor
static const struct ush_descriptor ush_desc = {
    .io = &ush_iface,                           // I/O interface pointer
    .input_buffer = ush_in_buf,                 // working input buffer
    .input_buffer_size = sizeof(ush_in_buf),    // working input buffer size
    .output_buffer = ush_out_buf,               // working output buffer
    .output_buffer_size = sizeof(ush_out_buf),  // working output buffer size
    .path_max_length = PATH_MAX_SIZE,           // path maximum length (stack)
    .hostname = const_cast<char *>("PicoTNC"),  // hostname (in prompt)
};

void initShell() {
    // initialize I/O interface
    shellUART.begin(115200UL);
    
    // initialize microshell instance
    ush_init(&ush, &ush_desc);

    // mount root directory (root must be first)
    ush_node_mount(&ush, "/", &root, NULL, 0);
}

void updateShell() {
    // non-blocking microshell service
    ush_service(&ush);
}

size_t Console::write(uint8_t c) {
    return ush_write(&ush, c);
}

size_t Console::write(const uint8_t *buffer, size_t size) {
    if (buffer == NULL) return 0;
    for (size_t i = 0; i < size; i++ ) {
        ush_write(&ush, buffer[i]);
    }
    return size;
}

void Console::flush() {
    ush_flush(&ush);
}