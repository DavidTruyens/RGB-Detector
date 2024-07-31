#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Adafruit_AS726x.h"
#include "logging.h"
#include "SPI.h"

uLog theLog;

#define PowerPIN 11
#define NeoPIN 12

Adafruit_NeoPixel neoled = Adafruit_NeoPixel(1, NeoPIN, NEO_GRB + NEO_KHZ800);

bool outputToSerial(const char* aText);

void setup()
{
  Serial.begin(115200);
  theLog.setOutput(0U, outputToSerial);
  theLog.setLoggingLevel(0U, loggingLevel::Debug);
  theLog.setColoredOutput(0U, true);
  theLog.setIncludeTimestamp(0U, true);
  theLog.log(subSystem::mainController, loggingLevel::Debug, "Starting up");

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
}

void loop()
{
  // put your main code here, to run repeatedly:
}
