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

#include "kiss/Packet.h"

namespace kiss {
    String encodeData(const String &data) {
        String encoded(data);
        encoded.replace(KISS_FRAME_ESCAPE, KISS_ESCAPE_FESC);
        encoded.replace(KISS_FRAME_END, KISS_ESCAPE_FEND);
        return encoded;
    }

    String decodeData(const String &data, int beginIndex, int endIndex) {
        if (endIndex < 0) {
            endIndex = data.length();
        }
        String decoded = data.substring(beginIndex, endIndex);
        return decoded;
        
        decoded.replace(KISS_ESCAPE_FEND, KISS_FRAME_END);
        decoded.replace(KISS_ESCAPE_FESC, KISS_FRAME_ESCAPE);
        return decoded;
    }

    byte encodeCommand(Command command, byte port) {
        return (port << 4) | command;
    }

    Packet::Packet(const String &s): command(DataFrame), port(0), data("") {
        int i = 0;
        while (i < s.length() && s.charAt(i) == KISS_FRAME_END.charAt(0)) {
            i++;
        }
        if (i < data.length()) {
            char c = data.charAt(i);
            port = (byte) (c >> 4);
            command = (Command) (c & 0x0F);

            i++;

            int data_start = i;
            
            while (i < s.length() && s.charAt(i) != KISS_FRAME_END.charAt(0)) {
                i++;
            }

            int data_end = i;

            if (data_start < s.length()) {
                data = decodeData(s, data_start, data_end);
            }
        }
    }

    String Packet::encode() const {
        return KISS_FRAME_END + encodeCommand(command, port) + encodeData(data) + KISS_FRAME_END;
    }

    String Packet::commandName() const {
        switch (command) {
            case DataFrame: return "Data Frame"; break;
            case TXDelay: return "TX Delay"; break;
            case Persistence: return "Persistence"; break;
            case SlotTime: return "Slot Time"; break;
            case TXTail: return "TX Tail"; break;
            case FullDuplex: return "Full Duplex"; break;
            case SetHardware: return "Set Hardware"; break;
            case Return: return "Exit KISS Mode"; break;
            default: return "Unknown"; break;
        }
    }

    size_t Packet::printTo(Print& p) const {
        return 
            p.print("Port: ") + p.print(port) +
            p.print(", Command: ") + p.print(commandName()) +
            p.print(", Data: ") + p.print(data);
    }
}