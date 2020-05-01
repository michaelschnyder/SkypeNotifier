#ifndef Application_h
#define Application_h

#include <Log4Esp.h>

#include "BootSequence.h"
#include "BootScreen.h"
#include "RemoteUpdater.h"
#include "ConnectionManager.h"
#include "AppConfig.h"

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

#include <TFT_eSPI.h>

class Application {

private:

    BootSequence startup;
    BootScreen bootscreen;
    TFT_eSPI tft = TFT_eSPI();

    AppConfig config;

    RemoteUpdater remoteUpdater;

    log4Esp::Logger logger = log4Esp::Logger("Application");

    AsyncWebServer server = AsyncWebServer(80);  
    ConnectionManager wifiConnection;

    char deviceId[10];
    void setupWifi();
    boolean connectToWifi();
    void setGeneratedDeviceId();
    void initializeFileSystem();
    void setupWebServer();
public:
    Application();   
    void bootstrap();
    void loop();    
};

#endif
