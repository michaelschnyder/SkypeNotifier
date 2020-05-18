#include <BufferedReader.h>
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
    memStream->write(0xff);

    BufferedReader *reader = new BufferedReader(memStream);

    uint8_t singleByte = reader->readByte();
    TEST_ASSERT_EQUAL(0xff, singleByte);
}

void multiple_bytes_can_be_read(void) {
    memStream = new MemoryStream(10);
    memStream->write(0xfa);
    memStream->write(0xfb);
    memStream->write(0xfc);

    BufferedReader *reader = new BufferedReader(memStream);

    uint8_t first = reader->readByte();
    uint8_t second = reader->readByte();
    uint8_t third = reader->readByte();
    TEST_ASSERT_EQUAL(0xfa, first);
    TEST_ASSERT_EQUAL(0xfb, second);
    TEST_ASSERT_EQUAL(0xfc, third);
}

void small_buffer_multiple_reads() {
    memStream = new MemoryStream(4);
    memStream->write(0xfa);
    memStream->write(0xfb);
    memStream->write(0xfc);

    BufferedReader *reader = new BufferedReader(memStream, 2);

    // read through first buffered size
    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
    TEST_ASSERT_EQUAL(0xfb, reader->readByte());

    // Did the buffer refresh?
    TEST_ASSERT_EQUAL(0xfc, reader->readByte());
}

void small_buffer_dont_overcopy() {
    memStream = new MemoryStream(4);
    memStream->write(0xfa);
    memStream->write(0xfb);

    BufferedReader *reader = new BufferedReader(memStream, 2);

    // read through first buffered size
    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
    TEST_ASSERT_EQUAL(0xfb, reader->readByte());

    // Did the buffer refresh?
    memStream->write(0xfc);
    TEST_ASSERT_EQUAL(0xfc, reader->readByte());
}

void restart_start_from_beginning() {
    memStream = new MemoryStream(16);
    memStream->write(0xfa);
    memStream->write(0xfb);
    memStream->write(0xfc);
    memStream->write(0xfd);

    Reader* reader = new BufferedReader(memStream, 2);
    reader->readByte();
    reader->readByte();

    reader->restart();

    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
    TEST_ASSERT_EQUAL(0xfb, reader->readByte());
}

void access_as_reader_works() {
    memStream = new MemoryStream(4);
    memStream->write(0xfa);

    Reader* reader = new BufferedReader(memStream, 1);
    TEST_ASSERT_EQUAL(0xfa, reader->readByte());
}

void infrastruture_setup_test(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(infrastruture_setup_test);
    RUN_TEST(singlebyte_can_be_read);
    RUN_TEST(multiple_bytes_can_be_read);
    RUN_TEST(small_buffer_multiple_reads);
    RUN_TEST(small_buffer_dont_overcopy);
    RUN_TEST(restart_start_from_beginning);
    RUN_TEST(access_as_reader_works);
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