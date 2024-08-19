#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "APDS9253.h"
#include "RTC.h"
#include "I2Cdev.h"
// #include "Adafruit_AS726x.h"
#include "logging.h"
#include "SPI.h"
#include "settings.h"
#include "menu.h"

#define PowerPIN 11
#define NeoPIN 12
#define SDA_PIN 6
#define SCL_PIN 7
#define OUTPUT_PIN 0

settings theSettings;
menu theMenu;
uLog theLog;
Adafruit_NeoPixel neoled = Adafruit_NeoPixel(1, NeoPIN, NEO_GRB + NEO_KHZ800);
TwoWire I2C_BUS(SDA_PIN, SCL_PIN);
I2Cdev i2c_0(&I2C_BUS);               // Instantiate the I2Cdev object and point to the desired I2C bus
APDS9253 theRGBSensor(&i2c_0);        // Instantiate APDS9253 light sensor

bool outputToSerial(const char* aText);

uint8_t RGB_mode = RGBiR;        // Choice is ALSandIR (green and IR channels only) or RGBiR for all four channels
// rate has to be slower than ADC settle time defines by resolution
//  Bright sunlight is maximum ~25 klux so choose gain of 6x and minimum resolution (16 bits)
//  that allows ~24 klux maximum to be measured;  a 1 Hz rate costs ~114 uA * 25/1000 ~ 3 uA
uint8_t LS_res        = res18bit;            // Choices are res20bit (400 ms), res19bit, res18bit (100 ms, default), res17bit, res16bit (25 ms).
uint8_t LS_rate       = rate1Hz;             // Choices are rate40Hz (25 ms), rate20Hz, rate10Hz (100 ms, default), rate5Hz, rate2_5Hz (400 ms), rate1Hz, rate0_5Hz
uint8_t LS_gain       = gain6;               // Choices are gain1, gain3 (default), gain6, gain9, gain18
uint32_t RGBiRData[4] = {0, 0, 0, 0};        // red, green, blue, ir counts

void setup() {
    Serial.begin(115200);

    theLog.setOutput(0U, outputToSerial);
    theLog.setLoggingLevel(0U, loggingLevel::Debug);
    theLog.setColoredOutput(0U, true);
    theLog.setIncludeTimestamp(0U, true);
    theLog.log(subSystem::general, loggingLevel::Debug, "Starting up");

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
    byte APDS9253_ID = theRGBSensor.getChipID();        // Read PART_ID register for APDS9253
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

        delay(10);
        theRGBSensor.init(RGB_mode, LS_res, LS_rate, LS_gain);
        theRGBSensor.enable();
        theRGBSensor.getStatus();        // clear interrupt before main loop

        Serial.println("APDS9253 setup finished");

    } else {
        if (APDS9253_ID != 0xC2) Serial.println(" APDS9253 not functioning!");
    }

    theRGBSensor.enable();        
   // theSettings.begin();
   // theSettings.dump();
}


unsigned long lastTime = 0;
unsigned long interval = 100;

void loop() {
    //theMenu.run();

    if (millis() - lastTime > interval) {
        lastTime       = millis();
        uint8_t status = theRGBSensor.getStatus();
        // theLog.snprintf(subSystem::mainController, loggingLevel::Debug, "Status = 0x%02X", status);

        if (status & 0x08) {
            theRGBSensor.getRGBiRdata(RGBiRData);        // read light sensor data
            theLog.snprintf(subSystem::mainController, loggingLevel::Info, "R %d, G %d, B %d, IR %d", RGBiRData[0], RGBiRData[1], RGBiRData[2], RGBiRData[3]);
        }
    }
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