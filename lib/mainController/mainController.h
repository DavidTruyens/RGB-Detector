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
    void updateBreatheIncrement(int newBrightness);

  private:
    MainStates theMainState = MainStates::UNKNOWN;
    MainStates theNewState  = MainStates::UNKNOWN;
    Adafruit_NeoPixel neoled;
    void breatheLED();
    void setLEDcolor(Color theColor);
    void checkRGBvalues(RGBiRData theData);
    bool compareRGB(RGBiRData theData, unsigned long targetRGB[3]);
    void setOutput(bool aState);
    void printAMainStates(MainStates aState);

    const ColorValues theColorValues;

    unsigned long breatheInterval = 30;
    int steps                     = 100;        // Steps between 0 and 255
    int breatheBrightness         = 0;          // Current breatheBrightness level
    int fadeAmount                = 5;          // Amount by which breatheBrightness changes
    int currentFadeAmount         = fadeAmount;
    unsigned long lastMillis      = 0;        // Last update time
};
