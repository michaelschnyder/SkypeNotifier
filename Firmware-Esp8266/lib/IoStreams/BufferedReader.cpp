#include "BufferedReader.h"

BufferedReader::BufferedReader(Stream *in, uint16_t buffer_size) {
    inputStream = in;
}

char BufferedReader::readByte() {
    return inputStream->read();
}