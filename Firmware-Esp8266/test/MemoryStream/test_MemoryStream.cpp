#include <BufferedReader.h>
#include <MemoryStream.h>
#include <unity.h>

#ifdef ARDUINO

#include <Arduino.h>
#include <Stream.h>
#else

#endif

void singlebyte_can_be_read(void) {
    MemoryStream* memStream = new MemoryStream(1);
    memStream->write(0xff);

    TEST_ASSERT_EQUAL(0xff, memStream->read());
}

void singlebyte_can_be_read_continous(void) {
    MemoryStream* memStream = new MemoryStream(2);
    memStream->write(0xfa);
    memStream->write(0xfb);

    TEST_ASSERT_EQUAL(0xfa, memStream->read());
    TEST_ASSERT_EQUAL(0xfb, memStream->read());
}

void multiple_bytes_can_be_read(void) {
    MemoryStream* memStream = new MemoryStream(10);
    memStream->write(0xff);
    memStream->write(0xfa);
    memStream->write(0xfb);

    char buff[3];
    size_t len = memStream->readBytes(&buff[0], 3);

    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0xff, buff[0]);
    TEST_ASSERT_EQUAL(0xfa, buff[1]);
    TEST_ASSERT_EQUAL(0xfb, buff[2]);
}

void multiple_read_multiple_starts_at_position() {
    MemoryStream* memStream = new MemoryStream(10);
    memStream->write(0xfa);
    memStream->write(0xfb);
    memStream->write(0xfc);
    memStream->write(0xfd);

    char buff[2];
    memStream->readBytes(&buff[0], 2);
    memStream->readBytes(&buff[0], 2);

    TEST_ASSERT_EQUAL(0xfc, buff[0]);
}

void singlebyte_read_multiple_starts_at_position() {
    MemoryStream* memStream = new MemoryStream(10);
    memStream->write(0xfa);
    memStream->write(0xfb);
    memStream->write(0xfc);

    memStream->read();
    memStream->read();

    char buff[2];
    memStream->readBytes(&buff[0], 2);

    TEST_ASSERT_EQUAL(0xfc, buff[0]);
}

void seek_read_again() {

    MemoryStream* memStream = new MemoryStream(10);
    memStream->write(0xfa);
    memStream->write(0xfb);

    memStream->read();
    memStream->seek(0);

    TEST_ASSERT_EQUAL(0xfa, memStream->read());
}

void cast_virtual_readByte(void) {

    Stream* stream = new MemoryStream(10);
    stream->write(0xff);
    TEST_ASSERT_EQUAL(0xff, stream->read());
}

void cast_virtual_readBytes(void) {

    Stream* stream = new MemoryStream(10);
    stream->write(0xfa);
    stream->write(0xfb);

    char buff[3];
    size_t len = stream->readBytes(&buff[0], 2);

    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xfa, buff[0]);
    TEST_ASSERT_EQUAL(0xfb, buff[1]);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(singlebyte_can_be_read);
    RUN_TEST(singlebyte_can_be_read_continous);
    RUN_TEST(multiple_bytes_can_be_read);
    RUN_TEST(cast_virtual_readByte);
    RUN_TEST(cast_virtual_readBytes);
    RUN_TEST(seek_read_again);
    RUN_TEST(multiple_read_multiple_starts_at_position);
    RUN_TEST(singlebyte_read_multiple_starts_at_position);
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