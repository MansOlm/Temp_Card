#ifndef TEMP_MEASURE
#define TEMP_MEASURE

#include <Arduino.h>

#define A 2
#define B 3
#define C 4

typedef struct Tab
{
    uint8_t temp;
    float volt;

} tab;

// PUBLIC METHODS
uint8_t measureTemp(uint8_t *stats);
void measureSetup();

#endif