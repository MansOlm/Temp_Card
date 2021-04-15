#include <Arduino.h>
#include "tempMeasure.h"
#include <math.h>

/***************************************************** 
************** PRIVATE METHODS ***********************
******************************************************/

static int binToTemp(uint16_t bin)
{
    float volt = (bin * (5.0 / 1023.0));
    return (int)ceil((-122.446 * pow(volt, 3)) + (709.397 * pow(volt, 2)) - (1445.35 * volt) + 1046.85);
}

static void choosePort(uint8_t port)
{
    //Writes A,B,C to the given port for the HC4851. Port == 0-7.
    digitalWrite(A, (port & (1 << 0)) >> 0);
    digitalWrite(B, (port & (1 << 1)) >> 1);
    digitalWrite(C, (port & (1 << 2)) >> 2);
}

static void readPort(int port, int *temps)
{
    //Reads A0-A3 and puts their value in temps
    uint8_t i = port * 4;
    choosePort(port);
    temps[i] = binToTemp(analogRead(A0));
    temps[i + 1] = binToTemp(analogRead(A1));
    temps[i + 2] = binToTemp(analogRead(A2));
    temps[i + 3] = binToTemp(analogRead(A3));
}

static void calculate(int *temps, uint8_t *stats)
{
    //Sets stats[0] to max value from temps, stats[1] to min value and stats[2] to average value

    int ave = 0;
    int max = 0;
    int min = 5;
    uint8_t err = 0;

    for (uint8_t i = 0; i < 28; i++)
    {
        ave += temps[i];
        temps[i] > max ? max = temps[i] : max;
        temps[i] < min ? min = temps[i] : min;
    }
    stats[0] = max;
    stats[1] = min;
    stats[2] = (int)ceil(ave / 28);
}

static uint8_t errCheck(int *temps, uint8_t *stats)
{
    for (uint8_t i = 0; i < 28; i++)
    {
        if (temps[i] < stats[2] * 0.3)
        {
            return 1;
        }
        if (temps[i] < 0)
        {
            return 1;
        }
    }
    return 0;
}

/***************************************************** 
************** PUBLIC METHODS ************************
******************************************************/

uint8_t measureTemp(uint8_t *stats)
{
    //Returns an array with the max, min and average temperature of the measurements of all 28 modules.
    int temps[28];

    for (uint8_t i = 0; i < 8; i++)
    {
        readPort(i, temps);
    }
    calculate(temps, stats);

    if (errCheck(temps, stats))
    {
        return 1;
    }
    return 0;
}

void measureSetup()
{
    //Sets digital pins A,B,C to output and A0,A1,A2,A3 to input.
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
}