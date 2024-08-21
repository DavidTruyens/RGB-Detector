#pragma once
#include <Wire.h>
#include "SPI.h"
#include "APDS9253.h"
#include "I2Cdev.h"
#include "logging.h"

extern uLog theLog;
extern TwoWire I2C_BUS;

struct RGBiRData {
    uint32_t R;
    uint32_t G;
    uint32_t B;
    uint32_t IR;
};

class RGBiRSensor {
  public:
    RGBiRSensor(TwoWire& i2cBus, I2Cdev* i2cDev);
    void begin(unsigned long interval);
    void run();
    uint8_t getChipID();
    void enable();
    void toggleOutput();
    // create a function to return the RGBiRData struct
    
    RGBiRData getRGBiRData();

  private:
    APDS9253 theRGBSensor;
    RGBiRData theData;
    unsigned long lastTime;
    unsigned long _interval;
    uint32_t RGBiRValues[4];
    I2Cdev* i2c;
    TwoWire& I2C_BUS;

    bool RGBoutput = true;

    uint8_t RGB_mode = RGBiR;        // Choice is ALSandIR (green and IR channels only) or RGBiR for all four channels
    // rate has to be slower than ADC settle time defines by resolution
    //  Bright sunlight is maximum ~25 klux so choose gain of 6x and minimum resolution (16 bits)
    //  that allows ~24 klux maximum to be measured;  a 1 Hz rate costs ~114 uA * 25/1000 ~ 3 uA
    uint8_t LS_res  = res18bit;        // Choices are res20bit (400 ms), res19bit, res18bit (100 ms, default), res17bit, res16bit (25 ms).
    uint8_t LS_rate = rate1Hz;         // Choices are rate40Hz (25 ms), rate20Hz, rate10Hz (100 ms, default), rate5Hz, rate2_5Hz (400 ms), rate1Hz, rate0_5Hz
    uint8_t LS_gain = gain6;           // Choices are gain1, gain3 (default), gain6, gain9, gain18
};
