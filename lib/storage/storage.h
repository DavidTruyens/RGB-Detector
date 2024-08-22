#pragma once

#include <Arduino.h>
#include "logging.h"
#include "variables.h"

extern uLog theLog;

// Magic number to identify valid configuration
const unsigned long CONFIG_MARKER = 0xDEADBEEF;

class storage {
  public:
    storage();

    void begin();
    Config getConfig();
    void saveConfig(const Config &newConfig);

  private:
    Config config;
    bool isConfigSaved();
    void writeConfigToFlash(const Config &config);
    void readConfigFromFlash(Config &config);
    Config getDefaultConfig();        // If needed, can return the default Config from variables.h
};