#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#define PowerPIN 11
#define NeoPIN 12

Adafruit_NeoPixel neoled = Adafruit_NeoPixel(1, NeoPIN, NEO_GRB + NEO_KHZ800);
void setup()
{
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
