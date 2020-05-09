#include "BootScreen.h"
#include "../res/Resources.h"

#define SPINNER_Y 35
#define STATUSTEXT_Y 80

void BootScreen::setup(TFT_eSPI* tft) {
    tft->setRotation(3); 


    tft->fillScreen(0x1082);
    tft->loadFont(AA_FONT_SMALL);

    BootScreen::tft = tft;

    spinner.init(tft);
    spinner.setPosition((160 - 32) / 2, SPINNER_Y);
    spinner.setImage("/spinner-32.bin", 32, 32);
    spinner.setTotalFrames(60);
    spinner.start();

    isReady = true;
}

void BootScreen::refresh() {

    if (!isReady) {
        return;
    }

    if (currentStatusIsDirty) {
        
        tft->fillRect(0, STATUSTEXT_Y, 180, 20, BLACK);

        tft->setTextColor(WHITE, 0x1082);
        tft->setTextWrap(true);

        tft->drawCentreString(BootScreen::currentStatus.c_str(), 80, STATUSTEXT_Y, 1);
        currentStatusIsDirty = false;
    }

    spinner.refresh();
}

void BootScreen::showStatus(String status) {

    currentStatus = status;
    currentStatusIsDirty = true;
    refresh();
}

void BootScreen::hide() {
    spinner.hide();
    tft->fillScreen(BLACK);
}