#include <pgmspace.h>

#include "Reader.h"

class ProgMemReader : public Reader {

public:
    ProgMemReader(const uint16_t * addr);
    uint8_t readByte();
    uint16_t readWord();

private:
    const uint16_t * baseAddr;
    uint16_t pos = 0;
};