#include "BufferedReader.h"

BufferedReader::BufferedReader(Stream *in, size_t buffer_size) {
    inputStream = in;
    bufferSize = buffer_size;
    buffer = new uint8_t[buffer_size];
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