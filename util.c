#include <stdio.h>

#include "util.h"

float fast_sqrtf(float number)
{
    const float x2 = number * 0.5F;
    const float threehalfs = 1.5F;

    union {
        float f;
        int i; /* assume sizeof(float) == sizeof(int) */
    } conv = { number }; /* member 'f' set to value of 'number'. */
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= (threehalfs - (x2 * conv.f * conv.f ));
    return 1.0f / conv.f;
}

float distance(float x0, float y0, float x1, float y1)
{
    return fast_sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}
