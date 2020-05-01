#include "StatusScreen.h"

void StatusScreen::setup(TFT_eSPI* tft) {
    tft->setRotation(3); 
    tft->fillScreen(BLACK);

    StatusScreen::tft = tft;
    isReady = true;
}

void StatusScreen::refresh() {

    if (!isReady) {
        return;
    }

    if (currentStatusIsDirty) {
        
        if (currentStatus == Offline) {
            logger.trace("device is offline");
            tft->fillScreen(DARK_RED);
        }

        if (currentStatus == Idle) {
            logger.trace("device is idle");
            tft->fillScreen(BLACK);
        }

        if (currentStatus == IncomingCall) {
            logger.trace("incoming call");
            tft->fillScreen(DARK_BLUE);
        }

        if (currentStatus == ActiveCall) {
            logger.trace("active call");
            tft->fillScreen(DARK_GREEN);
        }

        if (currentStatus == MissedCall) {
            logger.trace("missed call");
            tft->fillScreen(ORANGE);
        }

        currentStatusIsDirty = false;
    }

}

void StatusScreen::setStatus(ScreenStatus status) {

    currentStatus = status;
    currentStatusIsDirty = true;
    refresh();
}

void StatusScreen::hide() {
    tft->fillScreen(BLACK);
}