#pragma once

#include "logging.h"
#include "settings.h"

extern uLog theLog;
// extern settings theSettings;

enum class MenuOption {
    SET_RUNNING_COLOR,
    SET_IDLE_COLOR,
    SET_GLOBAL_DEVIATION,
    SET_RED_DEVIATION,
    SET_GREEN_DEVIATION,
    SET_BLUE_DEVIATION,
    HELP,
    TOGGLE_RGB_OUTPUT,
    UNKNOWN
};

class menu {
  public:
    menu(/* args */);
    void run();

  private:
    MenuOption theOption = MenuOption::UNKNOWN;
    int checkIncomingByte(char incomingByte);
    char buffer[20];
    bool validBuffer = false;
    int bufferIndex  = 0;
    void processBuffer();
    void executeOption();
};
