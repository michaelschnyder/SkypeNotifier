#if !defined(RUNLENGHTDECODER_H)

#include <Arduino.h>
#ifdef ARDUINO
#include <Stream.h>
#else
#include <StreamFake.h>
#endif

#include "Reader.h"
#include <MemoryStream.h>
#include <FS.h>

class RunLengthDecoder : public Reader {

public:
    RunLengthDecoder(Stream *in, size_t repeat_size = 1, size_t code_size = 1);
    RunLengthDecoder(fs::File *in, size_t repeat_size = 1, size_t code_size = 1);
    RunLengthDecoder(MemoryStream *in, size_t repeat_size = 1,size_t code_size = 1);

    uint8_t readByte();
    uint16_t readWord();
    void restart();
private:
    Stream *inputStream;
    StreamTypeEnum streamType;
    uint64_t remainingCount = 0;

    size_t repeatSize;
    uint8_t* repeatSizeBuf = NULL;

    size_t codeSize;
    uint8_t* currentCode = NULL;
    size_t currentCodePos = 0;
};

#endif // RUNLENGHTDECODER_H
