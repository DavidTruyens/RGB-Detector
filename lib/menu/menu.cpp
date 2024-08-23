#include "menu.h"
#include <Arduino.h>
#include "RGBiRSensor.h"
#include "storage.h"

extern storage theStorage;

// Define the static maps in the corresponding source file
std::map<std::string, Color> menu::colorMap = {
    {"RD", Color::red},
    {"GR", Color::green},
    {"BL", Color::blue},
    {"YW", Color::yellow},
    {"OR", Color::orange},
    {"PR", Color::purple},
    {"BK", Color::black},
    {"WH", Color::white},
    {"PK", Color::pink},
    {"CY", Color::cyan}
    // Add more mappings as needed
};

std::map<std::string, MenuOption> menu::optionMap = {
    {"r", MenuOption::SET_RUNNING_COLOR},
    {"i", MenuOption::SET_IDLE_COLOR},
    {"a", MenuOption::SET_ALARM_COLOR},
    {"w", MenuOption::SET_WARNING_COLOR},
    {"g", MenuOption::SET_GLOBAL_DEVIATION},
    {"R", MenuOption::SET_RED_DEVIATION},
    {"G", MenuOption::SET_GREEN_DEVIATION},
    {"B", MenuOption::SET_BLUE_DEVIATION},
    {"b", MenuOption::SET_BRIGHTNESS},
    {"h", MenuOption::HELP},
    {"t", MenuOption::TOGGLE_RGB_OUTPUT}
    // Add more mappings as needed
};

menu::menu(/* args */) {
}

void menu::run() {
    // Check if data is available on the serial port
    if (Serial.available() > 0) {
        // Read the entire string until a newline character is received
        String receivedStr = Serial.readStringUntil('\n');
        // Remove any trailing newline or spaces
        receivedStr.trim();

        if (receivedStr.length() > 0) {
            std::string key = receivedStr.c_str();

            if (theOption == MenuOption::UNKNOWN) {
                processMenu(key);
            } else if (theOption == MenuOption::SET_IDLE_COLOR || theOption == MenuOption::SET_RUNNING_COLOR || theOption == MenuOption::SET_WARNING_COLOR || theOption == MenuOption::SET_ALARM_COLOR) {
                processColor(key);
            } else if (theOption == MenuOption::SET_GLOBAL_DEVIATION || theOption == MenuOption::SET_RED_DEVIATION || theOption == MenuOption::SET_GREEN_DEVIATION || theOption == MenuOption::SET_BLUE_DEVIATION || theOption == MenuOption::SET_BRIGHTNESS) {
                char* end;
                // Convert string to unsigned long
                unsigned long value = strtoul(key.c_str(), &end, 10);

                // Check if the entire string was converted
                if (*end == '\0') {
                    theLog.snprintf(subSystem::general, loggingLevel::Info, "Conversion successful: %lu", value);
                    executeUnsignedLong(value, theOption);
                } else {
                    theLog.snprintf(subSystem::general, loggingLevel::Error, "Conversion failed: %s", key.c_str());
                }
            } else {
                theLog.snprintf(subSystem::general, loggingLevel::Error, "Unknown option");
            }
        }
    }
}

void menu::processMenu(const std::string& input) {
    // Look up the option in the map
    MenuOption theOption = MenuOption::UNKNOWN;
    auto it              = optionMap.find(input);
    if (it != optionMap.end()) {
        theOption = it->second;
    }
    executeMenuOption(theOption);
}

void menu::processColor(const std::string& colorStr) {
    // Check if the color string is valid
    Color aColor = Color::unknown;
    auto it      = colorMap.find(colorStr);
    if (it != colorMap.end()) {
        // Directly return the Color enum value from the map
        aColor = it->second;
    } else {
        // Return a default value for unknown colors
        aColor = Color::unknown;
    }
    executeColor(aColor);
}

void menu::executeMenuOption(MenuOption option) {
    switch (option) {
        case MenuOption::SET_RUNNING_COLOR:
            Serial.println("Set running color");
            printColorOptions();
            theOption = MenuOption::SET_RUNNING_COLOR;
            break;
        case MenuOption::SET_IDLE_COLOR:
            Serial.println("Set idle color");
            printColorOptions();
            theOption = MenuOption::SET_IDLE_COLOR;
            break;
        case MenuOption::SET_WARNING_COLOR:
            Serial.println("Set warning color");
            printColorOptions();
            theOption = MenuOption::SET_WARNING_COLOR;
            break;
        case MenuOption::SET_ALARM_COLOR:
            Serial.println("Set alarm color");
            printColorOptions();
            theOption = MenuOption::SET_ALARM_COLOR;
            break;
        case MenuOption::SET_GLOBAL_DEVIATION:
            Serial.println("Set global deviation");
            theOption = MenuOption::SET_GLOBAL_DEVIATION;
            break;
        case MenuOption::SET_RED_DEVIATION:
            Serial.println("Set red deviation");
            theOption = MenuOption::SET_RED_DEVIATION;
            break;
        case MenuOption::SET_GREEN_DEVIATION:
            Serial.println("Set green deviation");
            theOption = MenuOption::SET_GREEN_DEVIATION;
            break;
        case MenuOption::SET_BLUE_DEVIATION:
            Serial.println("Set blue deviation");
            theOption = MenuOption::SET_BLUE_DEVIATION;
            break;

        case MenuOption::SET_BRIGHTNESS:
            Serial.println("Set brightness as a value between 0 and 255");
            theOption = MenuOption::SET_BRIGHTNESS;
            break;

        case MenuOption::HELP:
            Serial.println("r - Set running color");
            Serial.println("i - Set idle color");
            Serial.println("a - Set alarm color");
            Serial.println("w - Set warning color");
            Serial.println("g - Set global deviation");
            Serial.println("R - Set red deviation");
            Serial.println("G - Set green deviation");
            Serial.println("B - Set blue deviation");
            Serial.println("b - Set brightness");
            Serial.println("t - Toggle RGB output");
            Serial.println("h - Help");
            theOption = MenuOption::UNKNOWN;
            break;
        case MenuOption::TOGGLE_RGB_OUTPUT:
            theSensor.toggleOutput();
            theOption = MenuOption::UNKNOWN;
            break;
        default:
            Serial.println("Unknown option");
            break;
    }
}

void menu::executeColor(Color aColor) {
    switch (theOption) {
        case MenuOption::SET_RUNNING_COLOR:
            theController.configureState(MainStates::RUNNING, aColor);
            break;

        case MenuOption::SET_IDLE_COLOR:
            theController.configureState(MainStates::IDLE, aColor);
            break;
        case MenuOption::SET_WARNING_COLOR:
            theController.configureState(MainStates::WARNING, aColor);
            break;
        case MenuOption::SET_ALARM_COLOR:
            theController.configureState(MainStates::ALARM, aColor);
            break;
        case MenuOption::SET_GLOBAL_DEVIATION:
        case MenuOption::SET_RED_DEVIATION:
        case MenuOption::SET_GREEN_DEVIATION:
        case MenuOption::SET_BLUE_DEVIATION:
            theLog.snprintf(subSystem::general, loggingLevel::Error, "Invalid state to receive a color");
            break;
        default:
            break;
    }
    theOption = MenuOption::UNKNOWN;
}

void menu::executeUnsignedLong(unsigned long value, MenuOption anOption) {
    switch (theOption) {
        case MenuOption::SET_RUNNING_COLOR:
        case MenuOption::SET_IDLE_COLOR:
        case MenuOption::SET_WARNING_COLOR:
        case MenuOption::SET_ALARM_COLOR:
        case MenuOption::HELP:
        case MenuOption::TOGGLE_RGB_OUTPUT:
            theLog.snprintf(subSystem::general, loggingLevel::Error, "Invalid state to receive a deviation");
            break;
        case MenuOption::SET_GLOBAL_DEVIATION:
            theConfig.RDeviation = value;
            theConfig.GDeviation = value;
            theConfig.BDeviation = value;
            theStorage.saveConfig(theConfig);
            break;
        case MenuOption::SET_RED_DEVIATION:
            theConfig.RDeviation = value;
            theStorage.saveConfig(theConfig);
            break;
        case MenuOption::SET_GREEN_DEVIATION:
            theConfig.GDeviation = value;
            theStorage.saveConfig(theConfig);
            break;
        case MenuOption::SET_BLUE_DEVIATION:
            theConfig.BDeviation = value;
            theStorage.saveConfig(theConfig);
            break;
        case MenuOption::SET_BRIGHTNESS:
            if (value >= 0 && value <= 255) {
                theConfig.brightness = value;
                theStorage.saveConfig(theConfig);
                theController.updateBreatheIncrement(value);
            } else {
                theLog.snprintf(subSystem::general, loggingLevel::Error, "Invalid brightness value");
            }
            break;
    }
    theOption = MenuOption::UNKNOWN;
    theVariables.printConfig(theConfig);
}

void menu::printColorOptions() {
    Serial.println("\tColor options:");
    for (auto const& x : colorMap) {
        Serial.print("\t");
        Serial.print(x.first.c_str());
        Serial.print(" - ");
        Serial.println(theVariables.printColors(x.second));
    }
    // Add more color options as needed
}
