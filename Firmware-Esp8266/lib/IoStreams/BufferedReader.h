

#include <Arduino.h>
#ifdef ARDUINO
#include <Stream.h>
#else
#include <StreamFake.h>
#endif

#include "Reader.h"

class BufferedReader : public Reader {

public:
    static const size_t DEFAULT_SIZE = 64;

    BufferedReader(Stream *in, size_t buffer_size = BufferedReader::DEFAULT_SIZE);
    uint8_t readByte();
    uint16_t readWord();

private:
    size_t bufferSize = DEFAULT_SIZE;
    uint8_t* buffer = NULL;
    bool hasBufferData = false;
    uint16_t pos = 0;
    Stream *inputStream;
};