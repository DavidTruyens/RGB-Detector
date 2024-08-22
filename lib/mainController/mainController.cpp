#include "mainController.h"
#include "RGBiRSensor.h"

#define PowerPIN 11
#define NeoPIN 12
#define OUTPUT_PIN 0

extern uLog theLog;
extern variables theVariables;
extern Config theConfig;
extern RGBiRSensor theSensor;

mainController::mainController() : neoled(1, NeoPIN, NEO_GRB + NEO_KHZ800) {
}

void mainController::begin() {
    // put your setup code here, to run once:
    pinMode(PowerPIN, OUTPUT);
    digitalWrite(PowerPIN, HIGH);
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);
    neoled.begin();
    neoled.setPixelColor(0, neoled.Color(255, 255, 255));
    neoled.show();
    theLog.output(subSystem::mainController, loggingLevel::Debug, "mainController started");
}

void mainController::run(RGBiRData theData) {
    checkRGBvalues(theData);
    switch (theMainState) {
        case MainStates::IDLE:
        case MainStates::RUNNING:
        case MainStates::WARNING:
        case MainStates::ALARM:
            break;
        case MainStates::UNKNOWN:
            standbyLED();
            break;
        default:
            theLog.output(subSystem::mainController, loggingLevel::Error, "Unknown state");
            break;
    }
}

void mainController::checkRGBvalues(RGBiRData theData) {
    // loop through all mainstates and check if the RGB data fits the state
    bool output    = false;
    Color newColor = Color::unknown;
    if (compareRGB(theData, theConfig.runningRGB)) {
        theNewState = MainStates::RUNNING;
        newColor    = theConfig.runningColor;
        output      = true;
    } else if (compareRGB(theData, theConfig.idleRGB)) {
        theNewState = MainStates::IDLE;
        newColor    = theConfig.idleColor;
    } else if (compareRGB(theData, theConfig.warningRGB)) {
        theNewState = MainStates::WARNING;
        newColor    = theConfig.warningColor;
    } else if (compareRGB(theData, theConfig.alarmRGB)) {
        theNewState = MainStates::ALARM;
        newColor    = theConfig.alarmColor;
    } else {
        theNewState = MainStates::UNKNOWN;
    }

    if (theNewState != theMainState) {
        printAMainStates(theNewState);
        theMainState = theNewState;
        setLEDcolor(newColor);
        setOutput(output);
    }
}

bool mainController::compareRGB(RGBiRData theData, unsigned long targetRGB[3]) {
    unsigned long RDiff = 0;
    unsigned long GDiff = 0;
    unsigned long BDiff = 0;

    if (theData.R > targetRGB[0]) {
        RDiff = theData.R - targetRGB[0];
    } else {
        RDiff = targetRGB[0] - theData.R;
    }

    if (theData.G > targetRGB[1]) {
        GDiff = theData.G - targetRGB[1];
    } else {
        GDiff = targetRGB[1] - theData.G;
    }

    if (theData.B > targetRGB[2]) {
        BDiff = theData.B - targetRGB[2];
    } else {
        BDiff = targetRGB[2] - theData.B;
    }

    return (RDiff <= theConfig.RDeviation && GDiff <= theConfig.GDeviation && BDiff <= theConfig.BDeviation);
}

void mainController::configureState(MainStates aState, Color aColor) {
    theLog.snprintf(subSystem::mainController, loggingLevel::Debug, "Configuring state arrived with %s color", theVariables.printColors(aColor));
    RGBiRData theData = theSensor.getRGBiRData();
    switch (aState) {
        case MainStates::RUNNING:
            theConfig.runningRGB[0] = theData.R;
            theConfig.runningRGB[1] = theData.G;
            theConfig.runningRGB[2] = theData.B;
            theConfig.runningColor  = aColor;
            break;

        default:
            break;
    }
    theVariables.printConfig(theConfig);
}

void mainController::configureDeviation(Color aColor, int deviation) {
    theLog.output(subSystem::mainController, loggingLevel::Debug, "Configuring deviation arrived");
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
    switch (theColor) {
        case Color::pink:
            neoled.setPixelColor(0, theColorValues.pink[0], theColorValues.pink[1], theColorValues.pink[2]);
            break;
        case Color::red:
            neoled.setPixelColor(0, theColorValues.red[0], theColorValues.red[1], theColorValues.red[2]);
            break;
        case Color::green:
            neoled.setPixelColor(0, theColorValues.green[0], theColorValues.green[1], theColorValues.green[2]);
            break;
        case Color::blue:
            neoled.setPixelColor(0, theColorValues.blue[0], theColorValues.blue[1], theColorValues.blue[2]);
            break;
        case Color::yellow:
            neoled.setPixelColor(0, theColorValues.yellow[0], theColorValues.yellow[1], theColorValues.yellow[2]);
            break;
        case Color::white:
            neoled.setPixelColor(0, theColorValues.white[0], theColorValues.white[1], theColorValues.white[2]);
            break;
        case Color::black:
            neoled.setPixelColor(0, theColorValues.black[0], theColorValues.black[1], theColorValues.black[2]);
            break;
        case Color::orange:
            neoled.setPixelColor(0, theColorValues.orange[0], theColorValues.orange[1], theColorValues.orange[2]);
            break;
        case Color::purple:
            neoled.setPixelColor(0, theColorValues.purple[0], theColorValues.purple[1], theColorValues.purple[2]);
            break;
        case Color::cyan:
            neoled.setPixelColor(0, theColorValues.cyan[0], theColorValues.cyan[1], theColorValues.cyan[2]);
            break;
        case Color::unknown:
            neoled.setPixelColor(0, 0, 0, 0);
            break;
        default:
            theLog.output(subSystem::mainController, loggingLevel::Error, "Unknown color");
            break;
    }
    neoled.show();
}

void mainController::setOutput(bool aState) {
    digitalWrite(OUTPUT_PIN, aState);
    theLog.snprintf(subSystem::mainController, loggingLevel::Debug, "Output set to %s", aState ? "true" : "false");
}

void mainController::printAMainStates(MainStates aState) {
    switch (aState) {
        case MainStates::IDLE:
            theLog.output(subSystem::mainController, loggingLevel::Info, "IDLE");
            break;
        case MainStates::RUNNING:
            theLog.output(subSystem::mainController, loggingLevel::Info, "RUNNING");
            break;
        case MainStates::WARNING:
            theLog.output(subSystem::mainController, loggingLevel::Info, "WARNING");
            break;
        case MainStates::ALARM:
            theLog.output(subSystem::mainController, loggingLevel::Info, "ALARM");
            break;
        case MainStates::UNKNOWN:
            theLog.output(subSystem::mainController, loggingLevel::Info, "UNKNOWN");
            break;
        default:
            theLog.output(subSystem::mainController, loggingLevel::Error, "Unknown state");
            break;
    }
}
