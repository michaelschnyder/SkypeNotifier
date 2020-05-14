

#include <Arduino.h>
#ifdef ARDUINO
#include <Stream.h>
#else
#include <StreamFake.h>
#endif

class BufferedReader {

public:
    static const uint16_t DEFAULT_SIZE = 64;

    BufferedReader(Stream *in, uint16_t buffer_size = BufferedReader::DEFAULT_SIZE);
    char readByte();

private:
    Stream *inputStream;
};