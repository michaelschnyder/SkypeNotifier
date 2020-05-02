#if !defined(AnimatedGif_h)
#define AnimatedGif_h

#include <TFT_eSPI.h>

class AnimatedGif {

public: 
    void setup(TFT_eSPI* tft);
    void setImage(const uint16_t[]);
    void refresh();

private:
    TFT_eSPI * tft;

    const uint16_t * image;

    bool isReady;
    long currentFrame;
    long msPerFrame;
};

#endif // AnimatedGif_h
