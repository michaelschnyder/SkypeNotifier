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

void AnimatedGif::setImage(String filename, uint32_t width, uint32_t height) {

    AnimatedGif::width = width;
    AnimatedGif::height = height;

    image = 0;
    AnimatedGif::filename = filename;
    file = SPIFFS.open(filename, "r");
}

void AnimatedGif::refresh() {

    if (!isReady) {
        return;
    }

    if (currentFrame >= numberOfFrames) {
        if (numberOfFrames == 1) {
            return;
        }

        currentFrame = 0;
        frameOffset = 0;

        if (file) {
            file.seek(0);
        }
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdated < msPerFrame) {
        return;
    }

    lastUpdated = millis();

    tft->startWrite();
    tft->setAddrWindow(x, y, width, height);
    tft->startWrite();
    
    uint64_t k = 0;

    if (image) {

        for (k = 0; k < width * height; k++) {

            uint16_t color = 0x0;
            color = pgm_read_word_near(image + frameOffset + k);

            tft->pushColor(color);         
        }
    }

    if (filename != "") {
        
        if (!file) {
            file = SPIFFS.open(filename, "r");
        }

        if (file.size() <= 0)  {
            return;
        }

        for (k = 0; k < width * height; k += SPIFFS_READ_BUFF_SIZE / 2 ) {

            char buff[SPIFFS_READ_BUFF_SIZE];
            
            file.readBytes(buff, SPIFFS_READ_BUFF_SIZE);

            for(int i = 0; i < SPIFFS_READ_BUFF_SIZE; i += 2) {
                uint16_t color = (buff[i] << 8) + buff[i+1];
                tft->pushColor(color);
            }
        }
    }

    tft->endWrite();

    currentFrame++;
    frameOffset+= k;
}