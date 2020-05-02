#include "Application.h"
// #include "spinner.01.h"

Application::Application() { }

void Application::bootstrap() {

  startup.setTaskTimeoutInMs(5 * 1000);
  startup.setTaskRetryCount(10);

  startup.addStep("Application is starting", [this](){ tft.init(); bootscreen.setup(&tft); });
  startup.addStep("Setting up device",       [this](){ setGeneratedDeviceId(); });
  startup.addStep("Initialize File System",  [this](){ initializeFileSystem(); });
  startup.addStep("Load Configuration",      [this](){ config.load(); });
  startup.addStep("Initializing Wifi",       [this](){ wifiConnection.setupWifi(config.getWifiSSID(), config.getWifiKey()); });
  startup.addStep("Connecting to Wifi",      [this](){ wifiConnection.connectToWifi(); }, [this]() { return wifiConnection.isConnected(); });
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

  startup.onTaskExpired([this](String name) { 
    startup.cancel();

    logger.fatal("Failed to run '%s'. Restarting ESP in 2s", name.c_str());
    bootscreen.showStatus("Failed. Restart in 2s...");

    restartTimer.start(2 * 1000);
    restartTimer.onCompleted([this]() { ESP.reset();  });
  });
}

void Application::loop() {

  startup.work();
  bootscreen.refresh();
  statusScreen.refresh();

  remoteUpdater.handle();
  timer.evaluate();
  restartTimer.evaluate();
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