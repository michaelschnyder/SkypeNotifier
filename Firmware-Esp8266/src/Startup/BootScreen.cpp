#include "BootScreen.h"
#include "spinner.01.h"

void BootScreen::setup(TFT_eSPI* tft) {
    tft->setRotation(3); 
    tft->fillScreen(BLACK);

    BootScreen::tft = tft;

    spinner.setup(tft);
    spinner.setImage(arrayspinner);

    isReady = true;
}

void BootScreen::refresh() {

    if (!isReady) {
        return;
    }

    if (currentStatusIsDirty) {
        
        tft->fillRect(0, 50, 180, 20, BLACK);
        tft->setCursor(0, 50);
        tft->setTextColor(WHITE, BLACK);
        tft->setTextWrap(true);
        tft->println(BootScreen::currentStatus.c_str());
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
    tft->fillScreen(BLACK);
}