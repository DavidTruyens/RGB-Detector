#include "storage.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#define FLASH_TARGET_OFFSET (512 * 1024) // Example offset, adjust as needed
#define FLASH_PAGE_SIZE 256              // Flash page size

storage::storage() {
    // Constructor can initialize any necessary variables, if needed
}

void storage::begin() {
    // Load existing config from flash or use default if not found
    if (!isConfigSaved()) {
        config = getDefaultConfig();  // Load the default config from variables.h
        writeConfigToFlash(config);
        Serial.println("No config found. Default config saved.");
    } else {
        readConfigFromFlash(config);
        Serial.println("Config loaded from flash.");
    }
}

Config storage::getConfig() {
    return config;
}

void storage::saveConfig(const Config &newConfig) {
    config = newConfig;
    writeConfigToFlash(config);
    Serial.println("Config saved to flash.");
}

bool storage::isConfigSaved() {
    Config tempConfig;
    readConfigFromFlash(tempConfig);  // Read the existing data from flash

    // Check if the marker matches the expected value
    return tempConfig.marker == CONFIG_MARKER;
}

void storage::writeConfigToFlash(const Config &config) {
    Config configWithMarker = config;
    configWithMarker.marker = CONFIG_MARKER;  // Set the marker

    uint32_t ints = save_and_disable_interrupts();
    
    // Erase the sector (flash memory is typically written in sectors)
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_PAGE_SIZE);
    
    // Write the config data to the flash memory
    flash_range_program(FLASH_TARGET_OFFSET, (const uint8_t*)&configWithMarker, sizeof(Config));
    
    restore_interrupts(ints);
}

void storage::readConfigFromFlash(Config &config) {
    const uint8_t *flash_data = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    memcpy(&config, flash_data, sizeof(Config));

    // If the config isn't saved (marker is wrong), return default config
    if (config.marker != CONFIG_MARKER) {
        config = getDefaultConfig();
    }
}

Config storage::getDefaultConfig() {
    Config defaultConfig;  // Create a default config using the values from variables.h
    defaultConfig = Config();  // Use default initializer from variables.h

    defaultConfig.marker = CONFIG_MARKER;  // Ensure marker is set

    return defaultConfig;
}
