#include <Arduino.h>
#include <SPI.h>
#include "tempMeasure.h"
#include "CAN.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  };

  measureSetup();
}

void loop()
{
  uint8_t err = 0;
  uint8_t temps[3];
  err = measureTemp(temps);

  sendCANTemp(temps[1], temps[0], temps[2], err);

  delay(100); // send data per 100ms
}