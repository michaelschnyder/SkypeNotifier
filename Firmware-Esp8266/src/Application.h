#ifndef Application_h
#define Application_h

#include <ESP8266WiFi.h>
#include <Log4Esp.h>

#include "RemoteUpdater.h"
#include "AppConfig.h"

#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_ST7735.h>   // include Adafruit ST7735 TFT library

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

// ST7735 TFT module connections
#define TFT_RST   2     // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    0     // TFT CS  pin is connected to NodeMCU pin D4 (GPIO0)
#define TFT_DC    4     // TFT DC  pin is connected to NodeMCU pin D4 (GPIO4)

class Application {

private:

    AppConfig config;
    WiFiClient wifiClient;
    WiFiClientSecure wifiClientSecure;

    RemoteUpdater remoteUpdater;

    log4Esp::Logger logger = log4Esp::Logger("Application");
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    AsyncWebServer server = AsyncWebServer(80);  

    char deviceId[10];
    void setupWifi();
    boolean connectToWifi();
    void setGeneratedDeviceId();
    void startupBanner();
    void initializeFileSystem();
    void initializeOTAUpdater();
public:
    Application();   
    void bootstrap();
    void loop();    
};

#endif
