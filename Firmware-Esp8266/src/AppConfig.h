#ifndef AppConfig_h
#define AppConfig_h

#include <stdint.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <Log4Esp.h>

class AppConfig {
private:
    const String filename = "/config.json";

    char wifiSSID[33];
    char wifiKey[33]; 

    log4Esp::Logger logger = log4Esp::Logger("AppConfig");
public:
    AppConfig();   
    void load();

    String getWifiSSID();
    String getWifiKey();
};

#endif