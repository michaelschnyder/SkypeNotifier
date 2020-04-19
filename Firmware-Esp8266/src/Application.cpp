#include "Application.h"

int const ONE_SECOND_IN_MS = 1000;
int wifiConnectionTimeoutInMs = 10 * ONE_SECOND_IN_MS;

Application::Application() {
  
}

void Application::bootstrap() {
  
  tft.initR(INITR_GREENTAB);

  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.println("Application is starting...");

  setGeneratedDeviceId();
  startupBanner();

  tft.println("Init FS and load config...");
  initializeFileSystem();
  config.load();

  tft.print("Connecting to Wifi... ");
  setupWifi();

  if (!connectToWifi()) {
    logger.fatal("Can't connect to WIFI. Restarting ESP in 2s");
    delay(2000);
    ESP.reset();
  }
  
  tft.println("OK");
  
  tft.println("IP: " + WiFi.localIP().toString());
  remoteUpdater.setup(deviceId);

  delay(1000);
  tft.fillScreen(ST7735_BLACK);
}

void Application::loop() {

  remoteUpdater.handle();

  if(WiFi.status() != WL_CONNECTED) {
    logger.warning("Wifi connection was interrupted. Trying to re-astablish connection.");
    
    for(int i = 0; i < 12; i++) {
      if (connectToWifi()) {
        logger.trace("Wifi connection successfully re-established.");
        return;
      }
    }

    logger.fatal("Wifi connection failed and unable to reconnect after %d trials during %ds. Resetting.", 12, 12 * wifiConnectionTimeoutInMs / 1000);
    ESP.reset();
  }
}

void Application::setupWifi() {
  WiFi.mode(WIFI_STA);    // Station Mode, i.e. connect to a WIFI and don't serve as AP
  WiFi.persistent(false); // Do not store WIFI information in EEPROM.
}

bool Application::connectToWifi() { 

  logger.trace("Connecting to WLAN with SSID '%s'. This may take some time...", config.getWifiSSID().c_str());

  WiFi.begin(config.getWifiSSID(), config.getWifiKey());
  
  long lastAttemptt = millis();
  bool isTimeout = false;

  while (WiFi.status() != WL_CONNECTED && !isTimeout)
  {
    delay(50);
    
    unsigned long currentMillis = millis();
    isTimeout = (currentMillis - lastAttemptt) >= wifiConnectionTimeoutInMs;
  }

  if(isTimeout) {
    logger.error("Could not connect to Wifi with SSID '%s' after %ds", config.getWifiSSID().c_str(), wifiConnectionTimeoutInMs / 1000);
    return false;
  }

  logger.trace("Connected, IP address: %s", WiFi.localIP().toString().c_str());
  return true;
}

void Application::setGeneratedDeviceId() {
  byte mac[6]; 
  WiFi.macAddress(mac);

  sprintf(deviceId, "SkypeNotifier-%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void Application::startupBanner() {
  logger.trace("Device Started");
  logger.trace("-------------------------------------");
  logger.trace("Device Id: %s", deviceId);
  logger.trace(__FILE__ " " __DATE__ " " __TIME__);
  logger.trace("-------------------------------------");
}

void Application::initializeFileSystem() {
  if (!SPIFFS.begin()) {
      logger.error("Unable to start filesystem. Formatting now as remediation action...");
      if (!SPIFFS.format()) {
        logger.error("Formatting failed. Unable to recover.");
      }
      else {
        logger.trace("Fomatting succeded. Restarting now.");
        ESP.reset();
      }
  }
  else {
      logger.trace("Filesystem ready for usage.");    
  }
}