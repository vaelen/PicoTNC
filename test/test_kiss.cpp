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

#include <kiss/Packet.h>

#include <unity.h>

const String DECODED_DATA = "TEST\xC0\xDB\xDB\xC0";
const String ENCODED_DATA = "TEST\xDB\xDC\xDB\xDD\xDB\xDD\xDB\xDC";
const kiss::Packet DECODED_DATA_PACKET(kiss::DataFrame, 5, "TEST\xC0\xDB\xDB\xC0");
const String ENCODED_DATA_PACKET = "\xC0\x50TEST\xDB\xDC\xDB\xDD\xDB\xDD\xDB\xDC\xC0";
const String ENCODED_DATA_PACKET_WITH_PADDING = "\xC0\xC0\xC0\x50TEST\xDB\xDC\xDB\xDD\xDB\xDD\xDB\xDC\xC0\xC0\xC0\xC0";

void test_encodeData() {
  String encoded = kiss::encodeData(DECODED_DATA);
  TEST_ASSERT_EQUAL_STRING(ENCODED_DATA.c_str(), encoded.c_str());
}

void test_decodeData() {
  String decoded = kiss::decodeData(ENCODED_DATA);
  TEST_ASSERT_EQUAL_STRING(DECODED_DATA.c_str(), decoded.c_str());
}

void test_encode_data_packet() {
  String encoded = DECODED_DATA_PACKET.encode();
  TEST_ASSERT_EQUAL_STRING(ENCODED_DATA_PACKET.c_str(), encoded.c_str());
}

void test_decode_data_packet() {
  TEST_IGNORE();
  // kiss::Packet p(ENCODED_DATA_PACKET);
  // TEST_ASSERT_EQUAL(DECODED_DATA_PACKET.port, p.port);
  // TEST_ASSERT_EQUAL(DECODED_DATA_PACKET.command, p.command);
  // TEST_ASSERT_EQUAL_STRING(DECODED_DATA_PACKET.data.c_str(), p.data.c_str());
}

void test_decode_padded_data_packet() {
  TEST_IGNORE();
  // kiss::Packet p(ENCODED_DATA_PACKET_WITH_PADDING);
  // TEST_ASSERT_EQUAL(DECODED_DATA_PACKET.port, p.port);
  // TEST_ASSERT_EQUAL(DECODED_DATA_PACKET.command, p.command);
  // TEST_ASSERT_EQUAL_STRING(DECODED_DATA_PACKET.data.c_str(), p.data.c_str());
}

int runTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_encodeData);
    RUN_TEST(test_decodeData);
    RUN_TEST(test_encode_data_packet);
    RUN_TEST(test_decode_data_packet);
    RUN_TEST(test_decode_padded_data_packet);
    return UNITY_END();
}

void start() {
  runTests();
}

void loop() {}

int main() {
  return runTests();
}