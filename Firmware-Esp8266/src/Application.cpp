#include "Application.h"
// #include "spinner.01.h"

int const ONE_SECOND_IN_MS = 1000;
int wifiConnectionTimeoutInMs = 10 * ONE_SECOND_IN_MS;

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
  startup.addStep("Finishing",               [this](){ statusScreen.setup(&tft); });

  startup.onBeforeTaskStart([this](String name){ bootscreen.showStatus(name); });

  startup.onCompleted([this](){ 
    bootscreen.hide();
    
    timer.start(30 * 1000);
    timer.onCompleted([this]() {statusScreen.setStatus(Offline); });
  });
}

void Application::loop() {

  startup.work();
  bootscreen.refresh();
  statusScreen.refresh();

  remoteUpdater.handle();
  timer.evaluate();
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

  server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Ready");
    timer.restart();
  });

  server.on("/heartbeat", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    timer.restart();
  });

  server.on("/call/ringing", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    statusScreen.setStatus(IncomingCall);
    timer.restart();
  });

  server.on("/call/missed", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    statusScreen.setStatus(MissedCall);
    timer.restart();
  });

  server.on("/call/active", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    statusScreen.setStatus(ActiveCall);
    timer.restart();
  });

  server.on("/call/clear", HTTP_POST, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain");
    statusScreen.setStatus(Idle);
    timer.restart();
  });
}

/*
  if (!wifiConnection.connectToWifi(config.getWifiSSID(), config.getWifiKey(), wifiConnectionTimeoutInMs)) {

    logger.fatal("Can't connect to WIFI. Restarting ESP in 2s");
    delay(2000);
    ESP.reset();
  }

}
}
*/
