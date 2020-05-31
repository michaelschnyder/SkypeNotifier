#include "RunLengthDecoder.h"

RunLengthDecoder::RunLengthDecoder(Stream *in, size_t repeat_size, size_t code_size): inputStream(in), repeatSize(repeat_size), codeSize(code_size) {
    currentCode = new uint8_t[code_size];
    repeatSizeBuf = new uint8_t[repeat_size];
    streamType = Unknown;
}

RunLengthDecoder::RunLengthDecoder(fs::File *in, size_t repeat_size, size_t code_size): inputStream(in), repeatSize(repeat_size), codeSize(code_size) {
    currentCode = new uint8_t[code_size];
    repeatSizeBuf = new uint8_t[repeat_size];
    streamType = File;
}

RunLengthDecoder::RunLengthDecoder(MemoryStream *in, size_t repeat_size, size_t code_size): inputStream(in), repeatSize(repeat_size), codeSize(code_size) {
    currentCode = new uint8_t[code_size];
    repeatSizeBuf = new uint8_t[repeat_size];
    streamType = Memory;
}

uint8_t RunLengthDecoder::readByte() {
    if (remainingCount == 0) {
        
        // Read the number of repetition
        inputStream->readBytes(&repeatSizeBuf[0], repeatSize);
        for (size_t i = 0; i < repeatSize; i++)
        {
            remainingCount = (remainingCount << 8) + repeatSizeBuf[i];
        }
        
        remainingCount*=codeSize;
        // Read next code 
        inputStream->readBytes(&currentCode[0], codeSize);

        currentCodePos = 0;
    }

    if (!(currentCodePos < codeSize)) {
        currentCodePos = 0;
    }

    remainingCount -= 1;
    return currentCode[currentCodePos++];
}

uint16_t RunLengthDecoder::readWord() {
    
    uint8_t byte0 = readByte();
    uint8_t byte1 = readByte();

    return (byte0 << 8) + byte1;
}

void RunLengthDecoder::restart() {
    if (streamType == Memory) {
        MemoryStream *memStream = (MemoryStream *)inputStream;
        memStream->seek(0);
    }

    if (streamType == File) {
        fs::File *fileStream = (fs::File *)inputStream;
        fileStream->seek(0);
    }
}