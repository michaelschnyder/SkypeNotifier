#ifndef Application_h
#define Application_h

#include <Log4Esp.h>

#include "RemoteUpdater.h"
#include "ConnectionManager.h"
#include "AppConfig.h"

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

#include <TFT_eSPI.h>
#include "TaskRunner.h"
#include "CountDownTimer.h"

#include "Colors.h"
#include "BootScreen.h"
#include "StatusScreen.h"


class Application {

private:

    TaskRunner startup;
    BootScreen bootscreen;
    StatusScreen statusScreen;
    TFT_eSPI tft = TFT_eSPI();

    AppConfig config;

    RemoteUpdater remoteUpdater;
    CountdownTimer timer;
    CountdownTimer restartTimer;
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
