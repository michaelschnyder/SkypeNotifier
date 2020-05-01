#include "Application.h"
// #include "spinner.01.h"

int const ONE_SECOND_IN_MS = 1000;
int wifiConnectionTimeoutInMs = 10 * ONE_SECOND_IN_MS;
unsigned long lastHeartbeat = 0;

bool heartBeatOk = true;

enum DisplayState { idle, offline, ringing, active, missed };

DisplayState desiredDisplayState = idle;
DisplayState lastprocessedDisplayState = idle;

Application::Application() { }

void Application::bootstrap() {

  startup.addStep("Application is starting", [this](){ tft.init(); bootscreen.setup(&tft); });
  startup.addStep("Setting up device",       [this](){ setGeneratedDeviceId(); });
  startup.addStep("Initialize File System",  [this](){ initializeFileSystem(); });
  startup.addStep("Load Configuration",      [this](){ config.load(); });
  startup.addStep("Initializing Wifi",       [this](){ wifiConnection.setupWifi(); });
  startup.addStep("Connecting to Wifi",      [this](){ wifiConnection.connectToWifi(config.getWifiSSID(), config.getWifiKey(), wifiConnectionTimeoutInMs); });
  startup.addStep("Configure OTA-Updater",   [this](){ remoteUpdater.setup(deviceId); });
  startup.addStep("Start OTA-Updater",       [this](){ remoteUpdater.start(); });
  startup.addStep("Configure WebServer",     [this](){ setupWebServer(); });
  startup.addStep("Start WebServer",         [this](){ server.begin(); });

  startup.onBeforeTaskStart([this](String name){ bootscreen.showStatus(name); });

  startup.onCompleted([this](){ 
    bootscreen.hide();
    lastHeartbeat = millis();
    heartBeatOk = true; 
  });
}

void Application::loop() {

  startup.work();
  bootscreen.refresh();

  remoteUpdater.handle();

}

void Application::setGeneratedDeviceId() {
  uint8_t mac[6]; 
  WiFi.macAddress(mac);

  sprintf(deviceId, "SkypeNotifier-%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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

void Application::setupWebServer() {
    // respond to GET requests on URL /heap
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Ready");
  });

  server.on("/heartbeat", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    lastHeartbeat = millis();    
  });

  server.on("/call/ringing", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    desiredDisplayState = ringing;
    lastHeartbeat = millis();    
  });

  server.on("/call/missed", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    desiredDisplayState = missed;
    lastHeartbeat = millis();    
  });

  server.on("/call/active", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    desiredDisplayState = active;
    lastHeartbeat = millis();    
  });

  server.on("/call/clear", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    lastHeartbeat = millis();    
    desiredDisplayState = idle;
  });

}

/*
  if (!wifiConnection.connectToWifi(config.getWifiSSID(), config.getWifiKey(), wifiConnectionTimeoutInMs)) {

    logger.fatal("Can't connect to WIFI. Restarting ESP in 2s");
    delay(2000);
    ESP.reset();
  }

}

void Application::loop() {

  remoteUpdater.handle();

  unsigned long currentMillis = millis();

  bool previousStatus = heartBeatOk;

  if (currentMillis - lastHeartbeat >= 30 * ONE_SECOND_IN_MS) {
    heartBeatOk = false;
  }
  else {
    heartBeatOk = true;
  }

  if (heartBeatOk != previousStatus) {

    if (!heartBeatOk) {
      desiredDisplayState = offline;
      logger.error("Connection lost to host.");
    }
    else {
      desiredDisplayState = idle;

      logger.trace("Connection restored.");
    }
  }

  if (lastprocessedDisplayState != desiredDisplayState) {

    if (desiredDisplayState == offline) {
        logger.trace("device is offline");
        tft.fillScreen(DARK_RED);
    }

    if (desiredDisplayState == idle) {
        logger.trace("device is idle");
        tft.fillScreen(BLACK);
    }

    if (desiredDisplayState == ringing) {
        logger.trace("incoming call");
        tft.fillScreen(DARK_BLUE);
    }

    if (desiredDisplayState == active) {
        logger.trace("active call");
        tft.fillScreen(DARK_GREEN);
    }

    if (desiredDisplayState == missed) {
        logger.trace("missed call");
        tft.fillScreen(ORANGE);
    }

    lastprocessedDisplayState = desiredDisplayState;
  }

}

}
*/
