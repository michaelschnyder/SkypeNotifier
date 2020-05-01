#ifndef ConnectionManager_h
#define ConnectionManager_h

#include <ESP8266WiFi.h>
#include <Log4Esp.h>

class ConnectionManager {
private:
    log4Esp::Logger logger = log4Esp::Logger("ConnectionManager");

public:
    void setupWifi();
    bool connectToWifi(String ssid, String password, long timeout);
    bool isConnected();
};
#endif
