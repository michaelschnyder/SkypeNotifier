#ifndef Application_h
#define Application_h

#include <ESP8266WiFi.h>
#include <Log4Esp.h>

#include "RemoteUpdater.h"
#include "AppConfig.h"



class Application {

private:

    AppConfig config;
    WiFiClient wifiClient;
    WiFiClientSecure wifiClientSecure;

    RemoteUpdater remoteUpdater;

    log4Esp::Logger logger = log4Esp::Logger("Application");

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
