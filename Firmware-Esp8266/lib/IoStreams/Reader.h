#if !defined(Reader_h)
#define Reader_h

enum StreamTypeEnum {
    Unknown,
    Memory,
    File
};

class Reader {
    
public:    
    virtual uint8_t readByte() = 0;
    virtual uint16_t readWord() = 0;
    virtual void restart() = 0;
};

#endif // Reader_h


