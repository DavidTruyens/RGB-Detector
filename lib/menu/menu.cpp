#include "menu.h"
#include <Arduino.h>

menu::menu(/* args */) {
}
void menu::run() {
    // Check if data is available on the serial port
    if (Serial.available() > 0) {
        // Read the entire string until a newline character is received
        String receivedStr = Serial.readStringUntil('\n');
        receivedStr.trim();        // Remove any trailing newline or spaces

        if (receivedStr.length() > 0) {
            // Convert String to char array
            receivedStr.toCharArray(buffer, sizeof(buffer));
            bufferIndex = receivedStr.length();

            // Process the buffer content
            processBuffer();
        }
    }
}

void menu::processBuffer() {
    // Clear previous option
    theOption = MenuOption::UNKNOWN;

    // Determine the option based on the first character in the buffer
    switch (buffer[0]) {
        case 'r':
            theOption = MenuOption::SET_RUNNING_COLOR;
            break;
        case 'i':
            theOption = MenuOption::SET_IDLE_COLOR;
            break;
        case 'g':
            theOption = MenuOption::SET_GLOBAL_DEVIATION;
            break;
        case 'R':
            theOption = MenuOption::SET_RED_DEVIATION;
            break;
        case 'G':
            theOption = MenuOption::SET_GREEN_DEVIATION;
            break;
        case 'B':
            theOption = MenuOption::SET_BLUE_DEVIATION;
            break;
        case 'h':
            theOption = MenuOption::HELP;
            break;
        case 't':
            theOption = MenuOption::TOGGLE_RGB_OUTPUT;
            break;
        default:
            theOption = MenuOption::UNKNOWN;
            break;
    }

    // Act based on the option selected
    executeOption();
}

void menu::executeOption() {
    switch (theOption) {
        case MenuOption::SET_RUNNING_COLOR:
            Serial.println("Set running color");
            // Perform actions to set the running color
            break;
        case MenuOption::SET_IDLE_COLOR:
            Serial.println("Set idle color");
            // Perform actions to set the idle color
            break;
        case MenuOption::SET_GLOBAL_DEVIATION:
            Serial.println("Set global deviation");
            // Perform actions to set the global deviation
            break;
        case MenuOption::SET_RED_DEVIATION:
            Serial.println("Set red deviation");
            // Perform actions to set the red deviation
            break;
        case MenuOption::SET_GREEN_DEVIATION:
            Serial.println("Set green deviation");
            // Perform actions to set the green deviation
            break;
        case MenuOption::SET_BLUE_DEVIATION:
            Serial.println("Set blue deviation");
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
            break;
        default:
            Serial.println("Unknown option");
            break;
    }
    // Reset the buffer index after processing
    bufferIndex = 0;
    memset(buffer, 0, sizeof(buffer));        // Clear the buffer
}

int menu::checkIncomingByte(char incomingByte) {
    return 0;
}
