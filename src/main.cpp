#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "APDS9253.h"
#include "RTC.h"
#include "I2Cdev.h"
// #include "Adafruit_AS726x.h"
#include "logging.h"
#include "SPI.h"

#define PowerPIN 11
#define NeoPIN 12
#define SDA_PIN 6
#define SCL_PIN 7
#define OUTPUT_PIN 0

uLog theLog;

// #define I2C_BUS          Wire               // Define the I2C bus (Wire instance) you wish to use
// set I2C pins
TwoWire I2C_BUS(SDA_PIN, SCL_PIN);
I2Cdev i2c_0(&I2C_BUS);        // Instantiate the I2Cdev object and point to the desired I2C bus

uint8_t RGB_mode = RGBiR;        // Choice is ALSandIR (green and IR channels only) or RGBiR for all four channels
// rate has to be slower than ADC settle time defines by resolution
//  Bright sunlight is maximum ~25 klux so choose gain of 6x and minimum resolution (16 bits)
//  that allows ~24 klux maximum to be measured;  a 1 Hz rate costs ~114 uA * 25/1000 ~ 3 uA
uint8_t LS_res        = res16bit;            // Choices are res20bit (400 ms), res19bit, res18bit (100 ms, default), res17bit, res16bit (25 ms).
uint8_t LS_rate       = rate1Hz;             // Choices are rate40Hz (25 ms), rate20Hz, rate10Hz (100 ms, default), rate5Hz, rate2_5Hz (400 ms), rate1Hz, rate0_5Hz
uint8_t LS_gain       = gain6;               // Choices are gain1, gain3 (default), gain6, gain9, gain18
uint32_t RGBiRData[4] = {0, 0, 0, 0};        // red, green, blue, ir counts

float ALSluxTable[25] = {        // lux per count for ALS depends on gain and resolution chosen
    0.136, 0.273, 0.548, 1.099, 2.193,
    0.045, 0.090, 0.180, 0.359, 0.722,
    0.022, 0.045, 0.090, 0.179, 0.360,
    0.015, 0.030, 0.059, 0.119, 0.239,
    0.007, 0.015, 0.029, 0.059, 0.117};

// Assume all channels have the same lux per LSB scaling
float luxScale = ALSluxTable[LS_gain * 5 + LS_res];

APDS9253 APDS9253(&i2c_0);        // Instantiate APDS9253 light sensor

Adafruit_NeoPixel neoled = Adafruit_NeoPixel(1, NeoPIN, NEO_GRB + NEO_KHZ800);

bool outputToSerial(const char* aText);

void setup() {
    Serial.begin(115200);

    theLog.setOutput(0U, outputToSerial);
    theLog.setLoggingLevel(0U, loggingLevel::Debug);
    theLog.setColoredOutput(0U, true);
    theLog.setIncludeTimestamp(0U, true);
    theLog.log(subSystem::mainController, loggingLevel::Debug, "Starting up");

    I2C_BUS.begin();
    delay(1000);
    I2C_BUS.setClock(400000);
    delay(1000);

    Serial.println("Scan for I2C devices:");
    i2c_0.I2Cscan();        // should detect APDS9253 at 0x4C
    delay(1000);

    // Check device IDs
    // Read the APDS9253 Part ID register, this is a good test of communication
    Serial.println("APDS9253 RGBiR Light Sensor...");
    byte APDS9253_ID = APDS9253.getChipID();        // Read PART_ID register for APDS9253
    Serial.print("APDS9253 ");
    Serial.print("chipID = 0x");
    Serial.print(APDS9253_ID, HEX);
    Serial.print(", Should be 0x");
    Serial.println(0xC2, HEX);
    Serial.println(" ");
    delay(1000);

    if (APDS9253_ID == 0xC2)        // check if all I2C sensors with WHO_AM_I have acknowledged
    {
        Serial.println("APDS9253 is online...");
        Serial.println(" ");

        Serial.print(" lux per count = ");
        Serial.println(luxScale, 3);

        // APDS9253.reset();
        delay(10);
        APDS9253.init(RGB_mode, LS_res, LS_rate, LS_gain);
        APDS9253.enable();
        APDS9253.getStatus();        // clear interrupt before main loop

        Serial.println("APDS9253 setup finished");

    } else {
        if (APDS9253_ID != 0xC2) Serial.println(" APDS9253 not functioning!");
    }

    // put your setup code here, to run once:
    pinMode(PowerPIN, OUTPUT);
    digitalWrite(PowerPIN, HIGH);
    neoled.begin();
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

    theLog.output(subSystem::mainController, loggingLevel::Debug, "Setup done");
    theLog.output(subSystem::mainController, loggingLevel::Error, "Example of an error");
    theLog.output(subSystem::mainController, loggingLevel::Warning, "Example of a warning");
}

int i = 0;

void loop() {
    /* APDS9253 Data Handling */
    APDS9253.enable();        // enable APDS9253 sensor
    // while (!(APDS9253.getStatus() & 0x08)) {
    // };                                       // wait for data ready
    APDS9253.getRGBiRdata(RGBiRData);        // read light sensor data
    APDS9253.disable();                      // disable APDS9253 sensor

    Serial.print("run ");
    Serial.println(i);
    i++;

    Serial.print("Red raw counts = ");
    Serial.println(RGBiRData[0]);
    Serial.print("Green raw counts = ");
    Serial.println(RGBiRData[1]);
    Serial.print("Blue raw counts = ");
    Serial.println(RGBiRData[2]);
    Serial.print("IR raw counts = ");
    Serial.println(RGBiRData[3]);
    Serial.println("  ");

    // Serial.print("Red intensity = ");
    // Serial.print(((float)RGBiRData[0]) * luxScale);
    // Serial.println(" lux");
    // Serial.print("Green intensity = ");
    // Serial.print(((float)RGBiRData[1]) * luxScale);
    // Serial.println(" lux");
    // Serial.print("Blue intensity = ");
    // Serial.print(((float)RGBiRData[2]) * luxScale);
    // Serial.println(" lux");
    // Serial.print("IR intensity = ");
    // Serial.print(((float)RGBiRData[3]) * luxScale);
    // Serial.println(" lux");
    // Serial.println("  ");

    delay(2000);
}

bool outputToSerial(const char* aText) {
    Serial.print(aText);
    return true;
}