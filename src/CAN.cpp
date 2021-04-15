#include <Arduino.h>
#include "mcp_can.h"
#include "CAN.h"
#include <SPI.h>

/**************************************
PRIVATE
**************************************/

static const int SPI_CS_PIN = 9;
static MCP_CAN CAN(SPI_CS_PIN); // Set CS pin

static unsigned char buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t bufSize = 0;

static uint8_t calcChecksum(unsigned long id, uint8_t legnth, uint8_t ext, uint8_t *data)
{
    int sum = (id % 256) + legnth + ext;

    for (uint8_t i = 0; i < legnth; i++)
    {
        sum += data[i];
        if (sum > 255)
        {
            sum -= 256;
        }
    }

    return sum;
}

/**************************************
PUBLIC
**************************************/

void CANSetup(uint8_t kbps)
{
    /*Provide function with 1-18 to select a baudrate.
#define CAN_5KBPS    1
#define CAN_10KBPS   2
#define CAN_20KBPS   3
#define CAN_25KBPS   4
#define CAN_31K25BPS 5
#define CAN_33KBPS   6
#define CAN_40KBPS   7
#define CAN_50KBPS   8
#define CAN_80KBPS   9
#define CAN_83K3BPS  10
#define CAN_95KBPS   11
#define CAN_100KBPS  12
#define CAN_125KBPS  13
#define CAN_200KBPS  14
#define CAN_250KBPS  15
#define CAN_500KBPS  16
#define CAN_666kbps  17
#define CAN_1000KBPS 18   
    */
    while (CAN_OK != CAN.begin(kbps))
    {
        delay(10);
    }
}

void sendCANTemp(uint8_t min_temp, uint8_t max_temp, uint8_t ave_temp, uint8_t error)
{
    uint8_t tempBuf[8] = {1, min_temp, max_temp, ave_temp, 80, 80, 0, 0};

    if (error)
    {
        tempBuf[4] += 128;
    }

    tempBuf[7] = calcChecksum(tempID, 8, 1, tempBuf);

    CAN.sendMsgBuf(tempID, 1, 8, tempBuf);
}

void setMessageBuffer(uint8_t *data, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = buffer[i];
    }
    bufSize = len;
}

void sendMessage(int id, uint8_t ext)
{
    CAN.sendMsgBuf(id, ext, bufSize, buffer);
}
