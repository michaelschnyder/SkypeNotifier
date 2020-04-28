#ifndef Application_h
#define Application_h

#include <ESP8266WiFi.h>
#include <Log4Esp.h>

#include "RemoteUpdater.h"
#include "AppConfig.h"

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

#include <TFT_eSPI.h>

class Application {

private:

    TFT_eSPI tft = TFT_eSPI();

    AppConfig config;
    WiFiClient wifiClient;
    WiFiClientSecure wifiClientSecure;

    RemoteUpdater remoteUpdater;

    log4Esp::Logger logger = log4Esp::Logger("Application");

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
