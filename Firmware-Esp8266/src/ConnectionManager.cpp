#include "ConnectionManager.h"

void ConnectionManager::setupWifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);    // Station Mode, i.e. connect to a WIFI and don't serve as AP
  WiFi.persistent(false); // Do not store WIFI information in EEPROM.

  ConnectionManager::ssid = ssid;
  ConnectionManager::password = password;
}

bool ConnectionManager::connectToWifi() { 

  logger.trace("Connecting to WLAN with SSID '%s'. This may take some time...", ssid.c_str());

  WiFi.begin(ssid, password); 
}

bool ConnectionManager::isConnected() {
    if (WiFi.status() != WL_CONNECTED) {
      return false;
    }

  logger.trace("Connected, IP address: %s", WiFi.localIP().toString().c_str());
  return true;
}



// void retryConnect() {
//       if(WiFi.status() != WL_CONNECTED) {
//     logger.warning("Wifi connection was interrupted. Trying to re-astablish connection.");
    
//     for(int i = 0; i < 12; i++) {
//       if (connectToWifi()) {
//         logger.trace("Wifi connection successfully re-established.");
//         return;
//       }
//     }

//     logger.fatal("Wifi connection failed and unable to reconnect after %d trials during %ds. Resetting.", 12, 12 * wifiConnectionTimeoutInMs / 1000);
//     ESP.reset();
//   }


// }
