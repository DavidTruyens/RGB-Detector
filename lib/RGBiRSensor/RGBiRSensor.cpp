#include "RGBiRSensor.h"
#include "Arduino.h"

RGBiRSensor::RGBiRSensor(TwoWire& i2cBus, I2Cdev* i2cDev) : I2C_BUS(i2cBus), i2c(i2cDev), theRGBSensor(i2cDev) {
}

void RGBiRSensor::begin(unsigned long interval) {
    _interval = interval;
    I2C_BUS.begin();
    delay(1000);
    I2C_BUS.setClock(400000);
    delay(1000);

    theLog.output(subSystem::general, loggingLevel::Debug, "Scan for I2C devices:");
    i2c->I2Cscan();        // Assuming there's a method for scanning I2C devices
    delay(1000);

    uint8_t chipID = theRGBSensor.getChipID();
    if (chipID == 0xC2) {
        theLog.output(subSystem::general, loggingLevel::Info, "APDS9253 is online...");
        delay(10);
        theRGBSensor.init(RGB_mode, LS_res, LS_rate, LS_gain);
        theRGBSensor.enable();
        theRGBSensor.getStatus();        // clear interrupt before main loop
        theLog.output(subSystem::general, loggingLevel::Info, "APDS9253 setup finished");
    } else {
        theLog.output(subSystem::general, loggingLevel::Error, "APDS9253 not functioning!");
    }
}

void RGBiRSensor::run() {
    if (millis() - lastTime > _interval) {
        lastTime       = millis();
        uint8_t status = theRGBSensor.getStatus();
        if (status & 0x08) {
            theRGBSensor.getRGBiRdata(RGBiRValues);
            theData.R = RGBiRValues[0];
            theData.G = RGBiRValues[1];
            theData.B = RGBiRValues[2];
            theData.IR = RGBiRValues[3];

            if (RGBoutput) {
                theLog.snprintf(subSystem::general, loggingLevel::Info, "R %d, G %d, B %d, IR %d", RGBiRValues[0], RGBiRValues[1], RGBiRValues[2], RGBiRValues[3]);
            }
        }
    }
}

RGBiRData RGBiRSensor::getRGBiRData() {
    return theData;
}

uint8_t RGBiRSensor::getChipID() {
    return theRGBSensor.getChipID();
}

void RGBiRSensor::enable() {
    theRGBSensor.enable();
}

void RGBiRSensor::toggleOutput() {
    RGBoutput = !RGBoutput;
}
