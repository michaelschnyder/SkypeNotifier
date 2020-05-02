#include "AnimatedGif.h"

void AnimatedGif::setup(TFT_eSPI* tft) {
    AnimatedGif::tft = tft;

    isReady = true;

}

void AnimatedGif::setImage(const uint16_t * img) {
    image = img;
}

unsigned long lastUpdated = millis();
long intervalMs = 50;
long numberOfFrames = 8;

void AnimatedGif::refresh() {

    if (!isReady) {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdated < intervalMs) {
        return;
    }

    Serial.printf("Show frame with id %d\n", currentFrame);

    lastUpdated = millis();

    tft->startWrite();
    uint16_t width = 0x20; //32
    uint16_t height = 0x20; //32
    tft->setAddrWindow((160 - width) / 2, 5, width, height);
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