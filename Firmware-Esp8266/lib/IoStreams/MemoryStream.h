#include <Arduino.h>

#ifdef ARDUINO
#include <Stream.h>
#else
#include <StreamFake.h>
#endif

class MemoryStream : public Stream {
    uint8_t *buffer;
    uint16_t buffer_size;
    uint16_t pos, size;
public:
    static const uint16_t DEFAULT_SIZE = 64;

    MemoryStream(uint16_t buffer_size = MemoryStream::DEFAULT_SIZE);
    ~MemoryStream();

    void clear(); 

    virtual size_t write(uint8_t);
    virtual int availableForWrite(void);

    virtual int available();
    virtual int read();
    virtual size_t readBytes(char *buffer, size_t length);

    // virtual size_t readBytes(char *buffer, size_t length); // read chars from stream into buffer
    // virtual size_t readBytes(uint8_t *buffer, size_t length) {
    //     return readBytes((char *) buffer, length);
    // }

    virtual int peek();
    virtual void flush();
};