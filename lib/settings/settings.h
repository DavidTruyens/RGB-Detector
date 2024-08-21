#pragma once

#include <LittleFS_Mbed_RP2040.h>
#include "logging.h"

extern uLog theLog;

#define LFS_MBED_RP2040_VERSION_MIN_TARGET "LittleFS_Mbed_RP2040 v1.1.0"
#define LFS_MBED_RP2040_VERSION_MIN 1001000

#define _LFS_LOGLEVEL_ 0
#define LFS_MBED_RP2040_DEBUG 1
#define RP2040_FS_SIZE_KB 64

#define FORCE_REFORMAT true

enum Color {
    pink,
    red,
    green,
    blue,
    yellow,
    white,
    black,
    orange,
    purple,
    cyan
};

struct Config {
    int RDeviation     = 10;
    int GDeviation     = 10;
    int BDeviation     = 10;
    Color runningColor = Color::pink;
    Color idleColor    = Color::black;
    int runningRGB[3]  = {255, 0, 255};
    int idleRGB[3]     = {0, 0, 0};
    bool RGBoutput     = true;
};
;

class settings {
  public:
    settings(/* args */);
    void begin();
    void dump();
    const char *printColors(Color theColor);
    Config theConfig;

  private:
    LittleFS_MBED *myFS;
    const char settingsPath[sizeof(MBED_LITTLEFS_FILE_PREFIX "/settings.bin")] = MBED_LITTLEFS_FILE_PREFIX "/settings.bin";

    void writeSettings(const char *path, Config &aConfig);
    void readSettings(const char *path, Config &aConfig);
};

struct ColorValues {
    // store rgb values for different colors
    int pink[3]   = {255, 0, 255};
    int red[3]    = {255, 0, 0};
    int green[3]  = {0, 255, 0};
    int blue[3]   = {0, 0, 255};
    int yellow[3] = {255, 255, 0};
    int white[3]  = {255, 255, 255};
    int black[3]  = {0, 0, 0};
    int orange[3] = {255, 165, 0};
    int purple[3] = {128, 0, 128};
    int cyan[3]   = {0, 255, 255};
};
