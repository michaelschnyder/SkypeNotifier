

#include <Arduino.h>
#ifdef ARDUINO
#include <Stream.h>
#else
#include <StreamFake.h>
#endif

#include "Reader.h"
#include <MemoryStream.h>
#include <FS.h>


class BufferedReader : public Reader {

public:
    static const size_t DEFAULT_SIZE = 64;

    BufferedReader(Stream *in, size_t buffer_size = BufferedReader::DEFAULT_SIZE);
    BufferedReader(fs::File *in, size_t buffer_size = BufferedReader::DEFAULT_SIZE);
    BufferedReader(MemoryStream *in, size_t buffer_size = BufferedReader::DEFAULT_SIZE);
    uint8_t readByte();
    uint16_t readWord();
    void restart();

private:
    bool hasBufferData = false;
    uint16_t pos = 0;
    StreamTypeEnum streamType;
    Stream *inputStream;
    size_t bufferSize;
    uint8_t* buffer = NULL;
};