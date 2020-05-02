#ifndef ConnectionManager_h
#define ConnectionManager_h

#include <ESP8266WiFi.h>
#include <Log4Esp.h>

class ConnectionManager {
private:
    log4Esp::Logger logger = log4Esp::Logger("ConnectionManager");

    String ssid;
    String password;
public:
    void setupWifi(String ssid, String password);
    bool connectToWifi();
    bool isConnected();
};
#endif
