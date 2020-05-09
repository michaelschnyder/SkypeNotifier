#include "StatusScreen.h"
#include "res/Resources.h"

void StatusScreen::setup(TFT_eSPI* tft) {
    tft->setRotation(3); 
    tft->fillScreen(BLACK);

    StatusScreen::tft = tft;

    ringing.init(tft);
    ringing.setPosition((160 - 64) / 2, 20);
    ringing.setImage("/ringing-64.bin", 64, 64);
    ringing.setTotalFrames(37);


    heartbeatMissing.init(tft);
    heartbeatMissing.setPosition((160 - 120) / 2, (128 - 90) / 2);
    heartbeatMissing.setImage("/heartbeat-single-120x90.bin", 120, 90);
    heartbeatMissing.setTotalFrames(1);
    heartbeatMissing.setFramesPerSecond(1);

    isReady = true;
}

void StatusScreen::refresh() {

    if (!isReady) {
        return;
    }

    ringing.refresh();
    heartbeatMissing.refresh();

    if (currentStatusIsDirty) {
        
        if (currentStatus == Offline) {
            logger.trace("device is offline");
            tft->fillScreen(0xda8a);
            heartbeatMissing.start();
        }
        else {
            heartbeatMissing.hide();
        }

        if (currentStatus == Idle) {
            logger.trace("device is idle");
            tft->fillScreen(BLACK);
        }

        if (currentStatus == IncomingCall) {
            logger.trace("incoming call");
            tft->fillScreen(0x040c);
            ringing.start();
        }
        else {
            ringing.hide();
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
    //refresh();
}

void StatusScreen::hide() {
    tft->fillScreen(BLACK);
}