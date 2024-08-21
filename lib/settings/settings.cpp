#include "settings.h"

settings::settings(/* args */) {
}

const char *settings::printColors(Color theColor) {
    switch (theColor) {
        case Color::pink:
            return "pink";
        case Color::red:
            return "red";
        case Color::green:
            return "green";
        case Color::blue:
            return "blue";
        case Color::yellow:
            return "yellow";
        case Color::white:
            return "white";
        case Color::black:
            return "black";
        case Color::orange:
            return "orange";
        case Color::purple:
            return "purple";
        case Color::cyan:
            return "cyan";
        default:
            return "unknown color";
    }
}

void settings::begin() {
    theLog.output(subSystem::filesystem, loggingLevel::Debug, "Start LittleFS_Test on " BOARD_NAME);
    theLog.output(subSystem::filesystem, loggingLevel::Debug, "Version: " LFS_MBED_RP2040_VERSION);

#if defined(LFS_MBED_RP2040_VERSION_MIN)
    if (LFS_MBED_RP2040_VERSION_INT < LFS_MBED_RP2040_VERSION_MIN) {
        theLog.output(subSystem::filesystem, loggingLevel::Error, "Error. Must use this example on Version equal or later than : " LFS_MBED_RP2040_VERSION_MIN_TARGET);
    }
#endif

    myFS = new LittleFS_MBED();

    if (!myFS->init()) {
        theLog.output(subSystem::filesystem, loggingLevel::Error, "LITTLEFS mount Failed");
        return;
    } else {
        theLog.output(subSystem::filesystem, loggingLevel::Debug, "LITTLEFS mount Success");
    }

    FILE *file = fopen(settingsPath, "r");

    if (file) {
        theLog.log(subSystem::general, loggingLevel::Info, "Settings file found");
        readSettings(settingsPath, theConfig);
    } else {
        theLog.log(subSystem::general, loggingLevel::Warning, "Settings file not found");
        writeSettings(settingsPath, theConfig);
    }
}

// a function to print all the settings to the general subsystem with a debug level
void settings::dump() {
    theLog.output(subSystem::general, loggingLevel::Debug, "Settings:");
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "Rdeviation: %d", theConfig.RDeviation);
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "Gdeviation: %d", theConfig.GDeviation);
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "Bdeviation: %d", theConfig.BDeviation);
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "IdleColor: %s", printColors(theConfig.idleColor));
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "Idle RGB: %d %d %d", theConfig.idleRGB[0], theConfig.idleRGB[1], theConfig.idleRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "RunningColor: %s", printColors(theConfig.runningColor));
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "Running RGB: %d %d %d", theConfig.runningRGB[0], theConfig.runningRGB[1], theConfig.runningRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Debug, "RGBoutput: %s", theConfig.RGBoutput ? "true" : "false");
}

void settings::writeSettings(const char *path, Config &aConfig) {
    FILE *file = fopen(path, "w");
    if (file) {
        fwrite(&aConfig, sizeof(Config), 1, file);
        fclose(file);
        Serial.println("Settings written to file");
    } else {
        Serial.println("Failed to open file for writing");
    }
}

void settings::readSettings(const char *path, Config &aConfig) {
    FILE *file = fopen(path, "r");
    if (file) {
        fread(&aConfig, sizeof(Config), 1, file);
        fclose(file);
        Serial.println("Settings read from file");
    } else {
        Serial.println("Failed to open file for reading");
    }
}
