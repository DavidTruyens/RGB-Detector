#include "menu.h"
#include <Arduino.h>

menu::menu(/* args */) {
}

void menu::run() {
    // monitor the serial port for commands
    if (Serial.available() > 0) {
        // read the incoming byte
        char receivedChar = Serial.read();
        if (receivedChar == '\n') {  // End of input or max size reached
            validBuffer = true;
        } else if (bufferIndex >= 19) {
            theLog.output(subSystem::general, loggingLevel::Warning, "buffer overflow");
            bufferIndex = 0;
        } else {
            buffer[bufferIndex++] = receivedChar;
            theLog.output(subSystem::general, loggingLevel::Debug, "uncomplete message received");
        }
    }

    if (validBuffer) {
        validBuffer = false;
        buffer[bufferIndex] = '\0';
        bufferIndex = 0;  // Reset bufferIndex for the next input

        // say what you got:
        theLog.snprintf(subSystem::general, loggingLevel::Debug, "I received: %s", buffer);

        switch (theOption) {
            case MenuOption::UNKNOWN:
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
                    default:
                        theOption = MenuOption::UNKNOWN;
                        break;
                }
                break;
            case MenuOption::SET_RUNNING_COLOR:
                Serial.println("Set running color");
                // store the color

                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::SET_IDLE_COLOR:
                Serial.println("Set idle color");
                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::SET_GLOBAL_DEVIATION:
                Serial.println("Set global deviation");
                // check if the incomming byte is a number

                theSettings.theConfig.RDeviation = 10;
                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::SET_RED_DEVIATION:
                Serial.println("Set red deviation");
                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::SET_GREEN_DEVIATION:
                Serial.println("Set green deviation");
                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::SET_BLUE_DEVIATION:
                Serial.println("Set blue deviation");
                theOption = MenuOption::UNKNOWN;
                break;
            case MenuOption::HELP:
                // print all options
                Serial.println("r - Set running color");
                Serial.println("i - Set idle color");
                Serial.println("g - Set global deviation");
                Serial.println("R - Set red deviation");
                Serial.println("G - Set green deviation");
                Serial.println("B - Set blue deviation");
                Serial.println("h - Help");

                theOption = MenuOption::UNKNOWN;
                break;
            default:
                theOption = MenuOption::UNKNOWN;
                break;
        }
    }
}


int menu::checkIncomingByte(char incomingByte) {
    return 0;
}
