#if !defined(AnimatedGif_h)
#define AnimatedGif_h

#include <TFT_eSPI.h>

class AnimatedGif {

public: 
    void init(TFT_eSPI* tft);
    void setPosition(uint32_t x, uint32_t y);
    void setImage(const uint16_t[], uint32_t width, uint32_t height);
    void setFramesPerSecond(int frames);
    void setTotalFrames(int numberOfframes);
    void start();
    void refresh();
    void hide();

private:
    TFT_eSPI * tft;

    const uint16_t * image;

    uint32_t x;
    uint32_t y;
    
    uint32_t width;
    uint32_t height;

    bool isReady;
    long currentFrame;
    long msPerFrame;

    unsigned long lastUpdated;
    long numberOfFrames = 0;
};

#endif // AnimatedGif_h
