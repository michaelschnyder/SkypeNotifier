#if !defined(AnimatedGif_h)
#define AnimatedGif_h

#include <TFT_eSPI.h>
#include <FS.h>
#include <BufferedReader.h>
#include <ProgMemReader.h>

#define SPIFFS_READ_BUFF_SIZE 512

class AnimatedGif {

public: 
    void init(TFT_eSPI* tft);
    void setPosition(uint32_t x, uint32_t y);
    void setImage(const uint16_t[], uint32_t width, uint32_t height);
    void setImage(String filename, uint32_t width, uint32_t height);
    void setFramesPerSecond(int frames);
    void setTotalFrames(int numberOfframes);
    void start();
    void refresh();
    void hide();

private:
    TFT_eSPI * tft;

    String filename;
    fs::File file;
    
    bool isSourceReady = false;
    Reader *reader = NULL;

    uint32_t x;
    uint32_t y;
    
    uint32_t width;
    uint32_t height;

    bool isVisible;
    bool isRunning;
    long currentFrame;

    unsigned long msPerFrame;

    unsigned long lastUpdated;
    long numberOfFrames = 0;
};

#endif // AnimatedGif_h
