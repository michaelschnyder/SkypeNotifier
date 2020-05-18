#include "ProgMemReader.h"

ProgMemReader::ProgMemReader(const uint16_t * addr) {
    baseAddr = addr;
}

uint16_t ProgMemReader::readWord() {
    return pgm_read_word_near(baseAddr + pos++);
}

uint8_t ProgMemReader::readByte() {
    return pgm_read_word(baseAddr + pos++);
}

void ProgMemReader::restart() {
    pos = 0;
}