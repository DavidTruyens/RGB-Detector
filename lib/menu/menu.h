#pragma once

#include "logging.h"
#include "settings.h"
#include "RGBiRSensor.h"
#include "variables.h"
#include "mainController.h"

#include <map>
#include <string>

extern uLog theLog;
extern RGBiRSensor theSensor;
extern mainController theController;
extern variables theVariables;
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
    void processMenu(const std::string& input);
    void processColor(const std::string& colorStr);
    void executeMenuOption(MenuOption option);
    void executeColor(Color aColor);
    void executeDeviation(int deviation);
    void printColorOptions();

      // Declare static maps
    static std::map<std::string, Color> colorMap;
    static std::map<std::string, MenuOption> optionMap;
};
