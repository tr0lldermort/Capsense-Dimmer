/*
 * Define & control software timers
 * Author:  dTb
 * Date:    13/03/19
 */

#include "softwareTimers.h"

softwareTimer::softwareTimer(uint8_t* timerId)
{
    static uint8_t myTimerId = 0; // tracks the number of software timers being used
    myTimerId++; // increment the number
    //initSoftwareTimer(&myTimerId);
    *timerId = myTimerId;
    return;
}

// void initSoftwareTimer(uint8_t* myTimerId)
// {
//     //probably make this function private
//     //make a software timer that will respond to the id passed here
// }

void softwareTimer::startSoftwareTimer(uint8_t timerId, unsigned int durationMillis)
{
    static unsigned long _timerEndings[0xFF];
    _timerEndings[timerId] = millis() + durationMillis;
}

bool softwareTimer::checkTimer(uint8_t timerId)
{
    if (millis() < _timerEndings[timerId])
    {
        return 0;
    }
    else
    {
        return 1;   //timer has elapsed
    }
}