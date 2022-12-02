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

#define ROW_TIME 2
#define COL_TIME 8
#define ROW_SATELLITES 4
#define COL_SATELLITES 14
#define ROW_LOCATION 4
#define COL_LOCATION 30
#define ROW_LINK 6
#define COL_LINK 10
#define ROW_START_LOG 8

const size_t LEN_TIME = 70;
const size_t LEN_LOC = 48;
const size_t LEN_SAT = 3;
const size_t LEN_LINK = 68;

Console console;

Console::Console(): Print(), 
    _rows(24), _columns(80), _lastRefresh(0),
    stream(NULL) 
{
    _satellites = new char[LEN_SAT + 1];
     for (int i = 0; i < LEN_SAT + 1; i++) {
        _satellites[i] = 0;
    }

    _time = new char[LEN_TIME + 1];
    for (int i = 0; i < LEN_TIME + 1; i++) {
        _time[i] = 0;
    }

    _location = new char[LEN_LOC + 1];
    for (int i = 0; i < LEN_LOC + 1; i++) {
        _location[i] = 0;
    }
}

Console::~Console() {
    if (_time != NULL) delete _time;
    if (_location != NULL) delete _location;
    if (_satellites != NULL) delete _satellites;
}


// Lifecycle Methods

void Console::begin(Stream &s) {
    stream = &s;
    setLogAttributes();
    stream->print(ANSI::CLEAR_SCREEN);
    refreshScreen();
    ANSI::moveCursor(*stream, 6, 1);
    setLogAttributes();
}

void Console::update() {
    if (stream == NULL) return;
    if (millis() - _lastRefresh > 30000) {
        refreshScreen();
    }
}

// Rows
uint8_t Console::rows() { 
    return _rows;
}

void Console::rows(uint8_t r) {
    _rows = r;
}

// Columns
uint8_t Console::columns() {
    return _columns;
}

void Console::columns(uint8_t c) {
    _columns = c;
}

void Console::setTime(const char *time) {
    if (_time == NULL || time == NULL) return;
    if (strncmp(_time, time, LEN_TIME) != 0) {
        strncpy(_time, time, LEN_TIME);
        updateTime();
    }
}

void Console::setSatellites(const char *satellites) {
    if (_satellites == NULL || satellites == NULL) return;
    if (strncmp(satellites, _satellites, LEN_SAT) != 0) {
        strncpy(_satellites, satellites, LEN_SAT);
        updateSatellites();
    }
}

void Console::setLocation(const char *location) {
    if (_location == NULL || location == NULL) return;
    if (strncmp(_location, location, LEN_LOC) != 0) {
        strncpy(_location, location, LEN_LOC);
        updateLocation();
    }
}

void Console::setLinkStatus(const char *linkStatus) {
    if (_linkStatus == NULL || linkStatus == NULL) return;
    if (strncmp(_linkStatus, linkStatus, LEN_LINK) != 0) {
        strncpy(_linkStatus, linkStatus, LEN_LINK);
        updateLinkStatus();
    }
}

// Print Implementation (writes to log)

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

// Internal Methods

void Console::updateLocation() {
    if (stream == NULL) return;
    stream->print(ANSI::SAVE_CURSOR);
    ANSI::moveCursor(*stream, ROW_LOCATION, COL_LOCATION);
    setGPSAttributes();
    for (int i = 0; i < LEN_LOC; i++) {
        stream->print(' ');
    }
    ANSI::moveCursor(*stream, ROW_LOCATION, COL_LOCATION);
    stream->print(_location);
    stream->print(ANSI::RESTORE_CURSOR);
}

void Console::updateSatellites() {
    if (stream == NULL) return;
    stream->print(ANSI::SAVE_CURSOR);
    ANSI::moveCursor(*stream, ROW_SATELLITES, COL_SATELLITES);
    setGPSAttributes();
    for (int i = 0; i < LEN_LOC; i++) {
        stream->print(' ');
    }
    ANSI::moveCursor(*stream, ROW_SATELLITES, COL_SATELLITES);
    stream->print(_location);
    stream->print(ANSI::RESTORE_CURSOR);
}

void Console::updateTime() {
    if (stream == NULL) return;
    stream->print(ANSI::SAVE_CURSOR);
    ANSI::moveCursor(*stream, ROW_TIME, COL_TIME);
    setGPSAttributes();
    for (int i = 0; i < LEN_TIME; i++) {
        stream->print(' ');
    }
    ANSI::moveCursor(*stream, ROW_TIME, COL_TIME);
    stream->print(_time);
    stream->print(ANSI::RESTORE_CURSOR);
}

void Console::updateLinkStatus() {
    if (stream == NULL) return;
    stream->print(ANSI::SAVE_CURSOR);
    ANSI::moveCursor(*stream, ROW_LINK, COL_LINK);
    setLinkAttributes();
    int fieldWidth = _columns - COL_LINK - 1;
    for (int i = 0; i < fieldWidth; i++) {
        stream->print(' ');
    }
    ANSI::moveCursor(*stream, ROW_LINK, COL_LINK);
    stream->print(_linkStatus);
    stream->print(ANSI::RESTORE_CURSOR);
}

void Console::setBorderAttributes() {
    if (stream == NULL) return;
    stream->print(ANSI::BLINK_OFF);
    stream->print(ANSI::BOLD_OFF);
    stream->print(ANSI::INVERSE_OFF);
    stream->print(ANSI::BG::BLACK);
    stream->print(ANSI::FG::GREEN);
}

void Console::setLogAttributes() {
    if (stream == NULL) return;
    stream->print(ANSI::BLINK_OFF);
    stream->print(ANSI::BOLD_OFF);
    stream->print(ANSI::INVERSE_OFF);
    stream->print(ANSI::BG::BLACK);
    stream->print(ANSI::FG::WHITE);
}

void Console::setGPSAttributes() {
    if (stream == NULL) return;
    stream->print(ANSI::BLINK_OFF);
    stream->print(ANSI::BOLD_OFF);
    stream->print(ANSI::INVERSE_OFF);
    stream->print(ANSI::BG::BLACK);
    stream->print(ANSI::FG::WHITE);
}

void Console::setLinkAttributes() {
    if (stream == NULL) return;
    stream->print(ANSI::BLINK_OFF);
    stream->print(ANSI::BOLD_OFF);
    stream->print(ANSI::INVERSE_OFF);
    stream->print(ANSI::BG::BLACK);
    stream->print(ANSI::FG::WHITE);
}

void Console::refreshScreen() {
    _lastRefresh = millis();
    if (stream == NULL) return;

    stream->print(ANSI::SAVE_CURSOR);

    // Set scrolling region
    stream->print(F("\033[8;255r"));

    ANSI::moveCursor(*stream, ROW_TIME, 2);
    setGPSAttributes();
    stream->print(ANSI::CLEAR_LINE);
    stream->print(F("Time: "));

    ANSI::moveCursor(*stream, ROW_SATELLITES, 2);
    setGPSAttributes();
    stream->print(ANSI::CLEAR_LINE);
    stream->print(F("Satellites:     Location: "));

    ANSI::moveCursor(*stream, ROW_LINK, 2);
    setLinkAttributes();
    stream->print(ANSI::CLEAR_LINE);
    stream->print(F("Address: "));

    ANSI::moveCursor(*stream, 1, 1);
    setBorderAttributes();
    stream->print('+');
    for (int i = 0; i < _columns - 2; i++) {
        stream->print('-');
    }
    stream->print('+');

    ANSI::moveCursor(*stream, 2, 1);
    stream->print('|');

    ANSI::moveCursor(*stream, 2, _columns);
    stream->print('|');

    ANSI::moveCursor(*stream, 3, 1);
    stream->print('+');
    for (int i = 0; i < _columns - 2; i++) {
        stream->print('-');
    }
    stream->print('+');

    ANSI::moveCursor(*stream, 4, 1);
    stream->print('|');
    ANSI::moveCursor(*stream, 4, _columns);
    stream->print('|');

    ANSI::moveCursor(*stream, 5, 1);
    stream->print('+');
    for (int i = 0; i < _columns - 2; i++) {
        stream->print('-');
    }
    stream->print('+');

    ANSI::moveCursor(*stream, 6, 1);
    stream->print('|');
    ANSI::moveCursor(*stream, 6, _columns);
    stream->print('|');

    ANSI::moveCursor(*stream, 7, 1);
    stream->print('+');
    for (int i = 0; i < _columns - 2; i++) {
        stream->print('-');
    }
    stream->print('+');

    stream->print(ANSI::RESTORE_CURSOR);

    updateTime();
    updateSatellites();
    updateLocation();
    updateLinkStatus();
}
