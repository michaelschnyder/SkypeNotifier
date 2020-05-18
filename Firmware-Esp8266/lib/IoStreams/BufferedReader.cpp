#include "BufferedReader.h"

BufferedReader::BufferedReader(Stream *in, size_t buffer_size): inputStream(in), bufferSize(buffer_size) {
    buffer = new uint8_t[buffer_size];
    streamType = Unknown;
}

BufferedReader::BufferedReader(fs::File *in, size_t buffer_size): inputStream(in), bufferSize(buffer_size) {
    buffer = new uint8_t[buffer_size];
    streamType = File;
}

BufferedReader::BufferedReader(MemoryStream *in, size_t buffer_size): inputStream(in), bufferSize(buffer_size) {
    buffer = new uint8_t[buffer_size];
    streamType = Memory;
}

uint8_t BufferedReader::readByte() {

    if (!hasBufferData || pos == bufferSize) {
        inputStream->readBytes(&buffer[0], bufferSize);
        pos = 0;
        hasBufferData = true;
    }
    
    uint8_t retVal = buffer[pos];
    pos++;
    return retVal;
}

uint16_t BufferedReader::readWord() {
    
    uint8_t byte0 = readByte();
    uint8_t byte1 = readByte();

    return (byte0 << 8) + byte1;
}

void BufferedReader::restart() {
    pos = 0;
    hasBufferData = false;

    if (streamType == Memory) {
        MemoryStream *memStream = (MemoryStream *)inputStream;
        memStream->seek(0);
    }

    if (streamType == File) {
        fs::File *fileStream = (fs::File *)inputStream;
        fileStream->seek(0);
    }
}