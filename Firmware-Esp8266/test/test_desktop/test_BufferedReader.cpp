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
    memStream = new MemoryStream(1);
    memStream->write('a');

    BufferedReader reader = BufferedReader(memStream);

    TEST_ASSERT_EQUAL('a', reader.readByte());
}

void infrastruture_setup_test(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(infrastruture_setup_test);
    RUN_TEST(singlebyte_can_be_read);
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