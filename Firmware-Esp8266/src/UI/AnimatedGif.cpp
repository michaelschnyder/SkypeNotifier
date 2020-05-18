#include "AnimatedGif.h"

void AnimatedGif::init(TFT_eSPI* tft) {
    AnimatedGif::tft = tft;

    msPerFrame = 1000 / 25;
}

void AnimatedGif::hide() {
    isVisible = false;
}

void AnimatedGif::start() {
    isVisible = true;
    isRunning = true;
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
    AnimatedGif::width = width;
    AnimatedGif::height = height;

    reader = new ProgMemReader(img);
    isSourceReady = true;
}

void AnimatedGif::setImage(String filename, uint32_t width, uint32_t height) {

    AnimatedGif::width = width;
    AnimatedGif::height = height;

    AnimatedGif::filename = filename;
    file = SPIFFS.open(filename, "r");
    if (file.available()) {
        reader = new BufferedReader(&file, SPIFFS_READ_BUFF_SIZE);
        isSourceReady = true;
    }
}

void AnimatedGif::refresh() {

    if (!isVisible || !isRunning) {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdated < msPerFrame) {
        return;
    }

    if (currentFrame >= numberOfFrames) {
        currentFrame = 0;
        reader->restart();
    }

    lastUpdated = millis();

    if (!isSourceReady && filename != "") {
        file = SPIFFS.open(filename, "r");
        if (file.available() && file.size()) {
            reader = new BufferedReader(&file, SPIFFS_READ_BUFF_SIZE);
            isSourceReady = true;
        }
    }

    if(!isSourceReady) {
        return;
    }

    tft->startWrite();
    tft->setAddrWindow(x, y, width, height);
    
    for (uint64_t k = 0; k < width * height; k++ ) {
        uint16_t color = reader->readWord();
        tft->pushColor(color);
    }

    tft->endWrite();

    currentFrame++;

    if (numberOfFrames == 1) {
        isRunning = false;
    }
}