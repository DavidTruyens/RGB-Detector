#pragma once

#include "logging.h"
#include "variables.h"
// #include "settings.h"
#include "Adafruit_NeoPixel.h"
#include "RGBiRSensor.h"

extern uLog theLog;

enum class MainStates {
    IDLE,
    RUNNING,
    WARNING,
    ALARM,
    UNKNOWN
};

class mainController {
  public:
    mainController(/* args */);
    void begin();
    void run(RGBiRData theData);
    void configureState(MainStates aState, Color aColor);
    void configureDeviation(Color aColor, int deviation);

  private:
    MainStates theMainState = MainStates::UNKNOWN;
    MainStates theNewState  = MainStates::UNKNOWN;
    Adafruit_NeoPixel neoled;
    void standbyLED();
    void setLEDcolor(Color theColor);
    void checkRGBvalues(RGBiRData theData);
    bool compareRGB(RGBiRData theData, unsigned long targetRGB[3]);
    void setOutput(bool aState);
    void printAMainStates(MainStates aState);

    const ColorValues theColorValues;

    unsigned long lastMillis      = 0;
    unsigned long standbyInterval = 1000;
    bool ledState                 = false;
};
