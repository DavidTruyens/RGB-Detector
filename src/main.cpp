#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#include "logging.h"
#include "RGBiRSensor.h"
#include "settings.h"
#include "menu.h"
#include "Wire.h"

#define PowerPIN 11
#define NeoPIN 12
#define SDA_PIN 6
#define SCL_PIN 7
#define OUTPUT_PIN 0

TwoWire I2C_BUS(SDA_PIN, SCL_PIN);
I2Cdev i2c_0(&I2C_BUS);
RGBiRSensor theSensor(I2C_BUS, &i2c_0);

settings theSettings;
menu theMenu;
uLog theLog;
Adafruit_NeoPixel neoled = Adafruit_NeoPixel(1, NeoPIN, NEO_GRB + NEO_KHZ800);

bool outputToSerial(const char* aText);

void setup() {
    Serial.begin(115200);

    theLog.setOutput(0U, outputToSerial);
    theLog.setLoggingLevel(0U, loggingLevel::Debug);
    theLog.setColoredOutput(0U, true);
    theLog.setIncludeTimestamp(0U, true);
    theLog.log(subSystem::general, loggingLevel::Debug, "Starting up");

    theSensor.begin(500);
    //theSettings.begin();
    //theSettings.dump();
}

void loop() {
    //theMenu.run();
    theSensor.run();
}

bool outputToSerial(const char* aText) {
    Serial.print(aText);
    return true;
}

void ledSetup() {
    // put your setup code here, to run once:
    pinMode(PowerPIN, OUTPUT);
    digitalWrite(PowerPIN, HIGH);
    neoled.begin();
}

void ledShow() {
    neoled.setPixelColor(0, neoled.Color(255, 0, 0));
    neoled.show();
    delay(1000);
    neoled.setPixelColor(0, neoled.Color(0, 255, 0));
    neoled.show();
    delay(1000);
    neoled.setPixelColor(0, neoled.Color(0, 0, 255));
    neoled.show();
    delay(1000);
    neoled.setPixelColor(0, neoled.Color(250, 0, 255));
    neoled.show();
    delay(1000);
}