#pragma once

#include <LittleFS_Mbed_RP2040.h>
#include "logging.h"
#include "variables.h"

extern uLog theLog;
extern variables theVariables;
extern Config theConfig;

#define LFS_MBED_RP2040_VERSION_MIN_TARGET "LittleFS_Mbed_RP2040 v1.1.0"
#define LFS_MBED_RP2040_VERSION_MIN 1001000

#define _LFS_LOGLEVEL_ 4
#define LFS_MBED_RP2040_DEBUG 1
#define RP2040_FS_SIZE_KB 32

#define FORCE_REFORMAT false


class settings {
  public:
    settings(/* args */);
    void begin();

  private:
    LittleFS_MBED *myFS;
    FILE *file;
    const char settingsPath[sizeof(MBED_LITTLEFS_FILE_PREFIX "/settings.bin")] = MBED_LITTLEFS_FILE_PREFIX "/settings.bin";

    void writeSettings(const char *path, Config &aConfig);
    void readSettings(const char *path, Config &aConfig);

    void writeFile(const char *path, const char *message, size_t messageSize);
    void readFile(const char *path, char *message, size_t messageSize);
    void readFile(const char *path);
    int lfs_ls(lfs_t *lfs, const char *path);
};
