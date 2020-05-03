#include "AnimatedGif.h"

void AnimatedGif::init(TFT_eSPI* tft) {
    AnimatedGif::tft = tft;

    msPerFrame = 1000 / 25;
}

void AnimatedGif::hide() {
    isReady = false;
}

void AnimatedGif::start() {
    isReady = true;
    lastUpdated = millis();
    currentFrame = 0;
}

void AnimatedGif::setPosition(uint32_t x, uint32_t y) {
    AnimatedGif::x = x;
    AnimatedGif::y = y;
}

void AnimatedGif::setFramesPerSecond(int frames) {
    msPerFrame = 1000 / 25;
}

void AnimatedGif::setTotalFrames(int numberOfFrames) {
    AnimatedGif::numberOfFrames = numberOfFrames;
}

void AnimatedGif::setImage(const uint16_t * img, uint32_t width, uint32_t height) {
    image = img;
    AnimatedGif::width = width;
    AnimatedGif::height = height;
}

void AnimatedGif::refresh() {

    if (!isReady) {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdated < msPerFrame) {
        return;
    }

    lastUpdated = millis();

    tft->startWrite();
    tft->setAddrWindow(x, y, width, height);
    tft->startWrite();
    
    uint64_t frameOffset = (height * width) * currentFrame;

    for (uint64_t k = 0; k < width * height; k++) {
        uint16_t color = pgm_read_word_near(image + frameOffset + k);
        tft->pushColor(color); 
    }
    
    tft->endWrite();

    if (currentFrame < numberOfFrames - 1) {
        currentFrame++;
    }
    else {
        currentFrame = 0;
    }
}