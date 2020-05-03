#ifndef BootScreen_h
#define BootScreen_h

#include <TFT_eSPI.h>
#include "AnimatedGif.h"
#include "../Colors.h"
#include "NotoSansBold15.h"

#define AA_FONT_SMALL NotoSansBold15

class BootScreen {
public:
    void setup(TFT_eSPI*);
    void refresh();
    void showStatus(String);
    void hide();

private:
    TFT_eSPI * tft;
    String currentStatus;
    bool currentStatusIsDirty;
    bool isReady;

    AnimatedGif spinner;
};
#endif