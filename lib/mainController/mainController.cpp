#include "mainController.h"

#define PowerPIN 11
#define NeoPIN 12
#define OUTPUT_PIN 0

extern uLog theLog;

mainController::mainController() : neoled(1, NeoPIN, NEO_GRB + NEO_KHZ800) {

}

void mainController::begin() {
    // put your setup code here, to run once:
    pinMode(PowerPIN, OUTPUT);
    digitalWrite(PowerPIN, HIGH);
    neoled.begin();
    neoled.setPixelColor(0, neoled.Color(255, 255, 255));
    neoled.show();
    theLog.output(subSystem::mainController, loggingLevel::Debug, "mainController started");
}

void mainController::run(RGBiRData theData) {
    switch (theMainState) {
        case MainStates::IDLE:
        case MainStates::RUNNING:
        case MainStates::WARNING:
        case MainStates::ALARM:
        case MainStates::UNKNOWN:
            standbyLED();
            break;

        default:
            theLog.output(subSystem::mainController, loggingLevel::Error, "Unknown state");
            break;
    }
}

void mainController::standbyLED() {
    if (millis() - lastMillis > standbyInterval) {
        lastMillis = millis();
        ledState   = !ledState;
        if (ledState) {
            neoled.setPixelColor(0, neoled.Color(255, 0, 0));
        } else {
            neoled.setPixelColor(0, neoled.Color(0, 0, 0));
        }
    }
    neoled.show();
}

void mainController::setLEDcolor(Color theColor) {
}