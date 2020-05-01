#include "BootScreen.h"
#include "Colors.h"



void BootScreen::setup(TFT_eSPI tft) {
    tft.setRotation(3); tft.fillScreen(BLACK);
}

void BootScreen::refresh() {



}

void BootScreen::showStatus(String status) {

// tft.setCursor(0, 50);
// tft.setTextColor(WHITE);
// tft.setTextWrap(true);
// tft.println("Application is starting...");

}

/*
  tft.startWrite();
  uint16_t width = 0x20; //32
  uint16_t height = 0x20; //32
  tft.setAddrWindow((160 - width) / 2, 5, width, height);
  tft.startWrite();
  
  for (uint64_t k = 0; k < width * height; k++) {
    uint16_t color = pgm_read_word_near(spinner32 + k);
    tft.pushColor(color); 
  }
  
  tft.endWrite();



*/