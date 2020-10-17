#ifndef MCSU_UTIL_H
#define MCSU_UTIL_H

#include <stdlib.h>

#define ARRAY_SIZE(static_array) (sizeof(static_array) / sizeof(static_array[0]))

#define RAND_RANGE(min, max) (rand() % (max - min) + min)

float fast_sqrtf(float number);

float distance(float x0, float y0, float x1, float y1);

#endif //MCSU_UTIL_H
