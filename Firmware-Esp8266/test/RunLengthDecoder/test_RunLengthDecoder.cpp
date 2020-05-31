#include <RunLengthDecoder.h>
#include <MemoryStream.h>
#include <unity.h>

#ifdef ARDUINO

#include <Arduino.h>
#include <Stream.h>
#else

#endif

MemoryStream *memStream;

void singlebyte_can_be_read(void) {
    memStream = new MemoryStream(10);
    memStream->write(0x01);
    memStream->write(0xff);

    RunLengthDecoder *decoder = new RunLengthDecoder(memStream);

    uint8_t singleByte = decoder->readByte();
    TEST_ASSERT_EQUAL(0xff, singleByte);
}

void singleword_can_be_read(void) {
    memStream = new MemoryStream(10);
    memStream->write(0x01);
    memStream->write(0xfa);
    memStream->write(0xfb);

    RunLengthDecoder *decoder = new RunLengthDecoder(memStream, 1, 2);

    uint16_t word = decoder->readWord();
    TEST_ASSERT_EQUAL(0xfafb, word);
}

void infrastruture_setup_test(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(singlebyte_can_be_read);
    RUN_TEST(singleword_can_be_read);
    RUN_TEST(infrastruture_setup_test);
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