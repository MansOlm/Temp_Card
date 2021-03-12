#include <Arduino.h>
#include <SPI.h>
#include "mcp_can.h"

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

const uint8_t analog_0 = 0;
const uint8_t analog_1 = 1;
const uint8_t analog_2 = 2;
const uint8_t analog_3 = 3;
const uint8_t analog_4 = 4;
const uint8_t analog_5 = 5;
const uint8_t analog_6 = 6;

float analog_volt(int x)
{
  return x * (5.0 / 1023.0);
}

void read_voltage(uint8_t array[])
{
  for (uint8_t i = 0; i < 7; i++)
  {
    array[i] = analogRead(i);
  }
}

void debug_volt(char array[])
{
  for (int i = 0; i < 7; i++)
  {
    Serial.println(array[i]);
  }
}

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN); // Set CS pin

void setup()
{
  SERIAL.begin(115200);
  while (!Serial)
  {
  };

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  { // init can bus : baudrate = 500k
    SERIAL.println("CAN BUS Shield init fail");
    SERIAL.println(" Init CAN BUS Shield again");
    delay(100);
  }
  SERIAL.println("CAN BUS Shield init ok!");
}

uint8_t temperature[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop()
{
  // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
  read_voltage(temperature);

  CAN.sendMsgBuf(0x00, 0, 8, temperature);
  delay(100); // send data per 100ms
  SERIAL.println("CAN BUS sendMsgBuf ok!");
}