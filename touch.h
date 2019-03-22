
#ifndef TOUCH_H
#define TOUCH_H

#include "system.h"
#include "softwareTimers.h"

#define DIR_UP 0
#define DIR_DOWN 1

#define MIN_BRIGHTNESS 25

void touchDetect(long touchVal);

typedef struct
{
    int en;
    bool fadeDirection;
    int brightness;
}LIGHTSTATE;

extern LIGHTSTATE lightState;

#endif