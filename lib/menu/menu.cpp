#include "menu.h"
#include <Arduino.h>
#include "RGBiRSensor.h"

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
    {"g", MenuOption::SET_GLOBAL_DEVIATION},
    {"R", MenuOption::SET_RED_DEVIATION},
    {"G", MenuOption::SET_GREEN_DEVIATION},
    {"B", MenuOption::SET_BLUE_DEVIATION},
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
            } else {
                processColor(key);
            }

            // Process the buffer content
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
            // Perform actions to set the running color
            break;
        case MenuOption::SET_IDLE_COLOR:
            Serial.println("Set idle color");
            theOption = MenuOption::SET_IDLE_COLOR;
            // Perform actions to set the idle color
            break;
        case MenuOption::SET_GLOBAL_DEVIATION:
            Serial.println("Set global deviation");
            theOption = MenuOption::SET_GLOBAL_DEVIATION;
            // Perform actions to set the global deviation
            break;
        case MenuOption::SET_RED_DEVIATION:
            Serial.println("Set red deviation");
            theOption = MenuOption::SET_RED_DEVIATION;
            // Perform actions to set the red deviation
            break;
        case MenuOption::SET_GREEN_DEVIATION:
            Serial.println("Set green deviation");
            theOption = MenuOption::SET_GREEN_DEVIATION;
            // Perform actions to set the green deviation
            break;
        case MenuOption::SET_BLUE_DEVIATION:
            Serial.println("Set blue deviation");
            theOption = MenuOption::SET_BLUE_DEVIATION;
            // Perform actions to set the blue deviation
            break;
        case MenuOption::HELP:
            Serial.println("r - Set running color");
            Serial.println("i - Set idle color");
            Serial.println("g - Set global deviation");
            Serial.println("R - Set red deviation");
            Serial.println("G - Set green deviation");
            Serial.println("B - Set blue deviation");
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
        case MenuOption::SET_GLOBAL_DEVIATION:
        case MenuOption::SET_RED_DEVIATION:
        case MenuOption::SET_GREEN_DEVIATION:
        case MenuOption::SET_BLUE_DEVIATION:


        default:
            break;
    }
    theOption = MenuOption::UNKNOWN;
}

void menu::executeDeviation(int deviation) {
    switch (theOption) {
        case MenuOption::SET_GLOBAL_DEVIATION:
            theController.configureDeviation(Color::unknown, deviation);
            break;
        case MenuOption::SET_RED_DEVIATION:
            theController.configureDeviation(Color::red, deviation);
            break;
        case MenuOption::SET_GREEN_DEVIATION:    
            theController.configureDeviation(Color::green, deviation);
            break;
        case MenuOption::SET_BLUE_DEVIATION:    
            theController.configureDeviation(Color::blue, deviation);
            break;
    }
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