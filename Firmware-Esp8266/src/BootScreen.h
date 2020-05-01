#ifndef BootScreen_h
#define BootScreen_h

#include <TFT_eSPI.h>

class BootScreen {
public:
    void setup(TFT_eSPI);
    void refresh();
    void showStatus(String);
};
#endif