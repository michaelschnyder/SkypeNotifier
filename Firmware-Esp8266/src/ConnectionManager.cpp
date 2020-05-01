#include "ConnectionManager.h"

void ConnectionManager::setupWifi() {
  WiFi.mode(WIFI_STA);    // Station Mode, i.e. connect to a WIFI and don't serve as AP
  WiFi.persistent(false); // Do not store WIFI information in EEPROM.
}

bool ConnectionManager::connectToWifi(String ssid, String password, long timeout) { 

  logger.trace("Connecting to WLAN with SSID '%s'. This may take some time...", ssid.c_str());

  WiFi.begin(ssid, password);
  
  long lastAttemptt = millis();
  bool isTimeout = false;

  while (WiFi.status() != WL_CONNECTED && !isTimeout)
  {
    delay(50);
    
    unsigned long currentMillis = millis();
    isTimeout = (currentMillis - lastAttemptt) >= timeout;
  }

  if(isTimeout) {
    logger.error("Could not connect to Wifi with SSID '%s' after %ds", ssid.c_str(), timeout / 1000);
    return false;
  }

  logger.trace("Connected, IP address: %s", WiFi.localIP().toString().c_str());
  return true;
}

bool ConnectionManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
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
