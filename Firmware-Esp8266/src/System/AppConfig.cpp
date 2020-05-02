#include "AppConfig.h"

AppConfig::AppConfig() {
}

void AppConfig::load() {

    logger.verbose("Attempting to load configuration from '%s'", AppConfig::filename.c_str());

    if (!SPIFFS.exists(AppConfig::filename)) {
        
        logger.warning("Configuration file '%s' does not exist.", AppConfig::filename.c_str());
        return;
    }

    File jsonFile;
    jsonFile = SPIFFS.open(AppConfig::filename, "r");

    if (!jsonFile) {
        logger.error("Cannot open configuration file '%s'", AppConfig::filename.c_str());
        return;
    }

    // Allocate a buffer to store contents of the file.
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(jsonFile);
    jsonFile.close();

    if (!root.success()) {
        logger.error("failed to load application configuration.");
        return;
    }

    strcpy(AppConfig::wifiSSID, root["wifiSSID"]);
    strcpy(AppConfig::wifiKey, root["wifiKey"]);

    logger.trace("Application configuration loaded.");
}

String AppConfig::getWifiSSID() {
    return AppConfig::wifiSSID;
}

String AppConfig::getWifiKey() {
    return AppConfig::wifiKey;
}