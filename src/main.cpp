#include <Arduino.h>

#include "mainController.h"
#include "logging.h"
#include "RGBiRSensor.h"
//#include "settings.h"
#include "menu.h"
#include "Wire.h"

#define SDA_PIN 6
#define SCL_PIN 7

TwoWire I2C_BUS(SDA_PIN, SCL_PIN);
I2Cdev i2c_0(&I2C_BUS);
RGBiRSensor theSensor(I2C_BUS, &i2c_0);

mainController theController;
//settings theSettings;
menu theMenu;
uLog theLog;
variables theVariables;

Config theConfig;

bool outputToSerial(const char* aText);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting up");

    theLog.setOutput(0U, outputToSerial);
    theLog.setLoggingLevel(0U, loggingLevel::Info);
    theLog.setColoredOutput(0U, true);
    theLog.setIncludeTimestamp(0U, true);
    theLog.output(subSystem::general, loggingLevel::Debug, "Starting up");

    Serial.println("Starting up");

    theSensor.begin(500);
    theController.begin();
    //theSettings.begin();
    //theSettings.dump();
}

void loop() {
    theMenu.run();
    theSensor.run();
    theController.run(theSensor.getRGBiRData());
}

bool outputToSerial(const char* aText) {
    Serial.print(aText);
    return true;
}
