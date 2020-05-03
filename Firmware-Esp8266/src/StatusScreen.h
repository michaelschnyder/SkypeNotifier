#ifndef StatusScreen_h
#define StatusScreen_h

#include <Log4Esp.h>
#include <TFT_eSPI.h>
#include "Colors.h"
#include "UI/AnimatedGif.h"

enum ScreenStatus {
    Idle,
    Offline,
    IncomingCall,
    ActiveCall,
    MissedCall
};

class StatusScreen {
public:
    void setup(TFT_eSPI*);
    void setStatus(ScreenStatus);
    void refresh();
    void hide();

private:
    log4Esp::Logger logger = log4Esp::Logger("StatusScreen");

    TFT_eSPI * tft;
    bool isReady;

    ScreenStatus currentStatus;
    bool currentStatusIsDirty;

    AnimatedGif ringing;
    AnimatedGif heartbeatMissing;

};
#endif