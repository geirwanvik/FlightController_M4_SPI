#include "utilities.h"

int16_t constrain(int16_t input, int16_t minValue, int16_t maxValue)
{
    if (input < minValue)
        return minValue;
    else if (input > maxValue)
        return maxValue;
    else
        return input;
}
