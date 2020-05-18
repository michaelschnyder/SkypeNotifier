#include <ProgMemReader.h>
#include <unity.h>

#ifdef ARDUINO

#include <Arduino.h>
#include <pgmspace.h>
const PROGMEM uint16_t ProgMemWordAddrLocation [] = { 0xfafa, 0xfbfb, 0xfcfc, 0xfdfd };
const PROGMEM uint16_t ProgMemByteAddrLocation [] = { 0xfa, 0xfb, 0xfc, 0xfd };
#endif

void word_read(void) {
    ProgMemReader* reader = new ProgMemReader(ProgMemWordAddrLocation);
    TEST_ASSERT_EQUAL(0xfafa, reader->readWord());
    TEST_ASSERT_EQUAL(0xfbfb, reader->readWord());
    TEST_ASSERT_EQUAL(0xfcfc, reader->readWord());
    TEST_ASSERT_EQUAL(0xfdfd, reader->readWord());
}

void word_byte(void) {
    ProgMemReader* reader = new ProgMemReader(ProgMemByteAddrLocation);
    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
    TEST_ASSERT_EQUAL(0xfb, reader->readByte());
    TEST_ASSERT_EQUAL(0xfc, reader->readByte());
    TEST_ASSERT_EQUAL(0xfd, reader->readByte());
}

void access_as_reader_works() {
    Reader* reader = new ProgMemReader(ProgMemByteAddrLocation);
    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(word_read);
    RUN_TEST(word_byte);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

#include <Arduino.h>
#include <StreamFake.h>

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif