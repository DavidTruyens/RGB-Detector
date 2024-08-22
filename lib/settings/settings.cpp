#include "settings.h"

settings::settings(/* args */) {
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

    theLog.snprintf(subSystem::filesystem, loggingLevel::Debug, "path : %s", settingsPath);

    char fileName1[] = MBED_LITTLEFS_FILE_PREFIX "/hello1.txt";
    char message[]   = "Hello from " BOARD_NAME "\n";

    writeFile(fileName1, message, sizeof(message));
    readFile(fileName1);

    file = fopen(settingsPath, "r");

    if (file) {
        theLog.output(subSystem::general, loggingLevel::Info, "Settings file found");
        fclose(file);
        readSettings(settingsPath, theConfig);
    } else {
        theLog.output(subSystem::general, loggingLevel::Warning, "Settings file not found");
        writeSettings(settingsPath, theConfig);
    }
}

void settings::writeSettings(const char *path, Config &aConfig) {
    file = fopen(path, "w+");
    if (file) {
        fwrite(&aConfig, sizeof(Config), 1, file);
        fclose(file);
        theLog.output(subSystem::filesystem, loggingLevel::Debug, "Settings written to file");
    } else {
        theLog.output(subSystem::filesystem, loggingLevel::Error, "Failed to open file for writing");
    }
}

void settings::readSettings(const char *path, Config &aConfig) {
    file = fopen(path, "r");
    if (file) {
        fread(&aConfig, sizeof(Config), 1, file);
        fclose(file);
        theLog.output(subSystem::filesystem, loggingLevel::Debug, "Settings read from file");
    } else {
        theLog.output(subSystem::filesystem, loggingLevel::Error, "Failed to open file for reading");
    }
}

void settings::writeFile(const char *path, const char *message, size_t messageSize) {
    Serial.print("Writing file: ");
    Serial.print(path);

    FILE *file = fopen(path, "w");

    if (file) {
        Serial.println(" => Open OK");
    } else {
        Serial.println(" => Open Failed");
        return;
    }

    if (fwrite((uint8_t *)message, 1, messageSize, file)) {
        Serial.println("* Writing OK");
    } else {
        Serial.println("* Writing failed");
    }

    fclose(file);
}

void settings::readFile(const char *path) {
    Serial.print("Reading file: ");
    Serial.print(path);

    FILE *file = fopen(path, "r");

    if (file) {
        Serial.println(" => Open OK");
    } else {
        Serial.println(" => Open Failed");
        return;
    }

    char c;
    uint32_t numRead = 1;

    while (numRead) {
        numRead = fread((uint8_t *)&c, sizeof(c), 1, file);

        if (numRead)
            Serial.print(c);
    }

    fclose(file);
}

int settings::lfs_ls(lfs_t *lfs, const char *path) {
    lfs_dir_t dir;
    int err = lfs_dir_open(lfs, &dir, path);
    if (err) {
        return err;
    }

    struct lfs_info info;
    while (true) {
        int res = lfs_dir_read(lfs, &dir, &info);
        if (res < 0) {
            return res;
        }

        if (res == 0) {
            break;
        }

        switch (info.type) {
            case LFS_TYPE_REG:
                printf("reg ");
                break;
            case LFS_TYPE_DIR:
                printf("dir ");
                break;
            default:
                printf("?   ");
                break;
        }

        static const char *prefixes[] = {"", "K", "M", "G"};
        for (int i = sizeof(prefixes) / sizeof(prefixes[0]) - 1; i >= 0; i--) {
            if (info.size >= (1 << 10 * i) - 1) {
                printf("%*u%sB ", 4 - (i != 0), info.size >> 10 * i, prefixes[i]);
                break;
            }
        }

        printf("%s\n", info.name);
    }

    err = lfs_dir_close(lfs, &dir);
    if (err) {
        return err;
    }

    return 0;
}
