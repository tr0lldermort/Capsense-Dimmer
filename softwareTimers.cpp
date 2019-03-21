/*
 * Define & control software timers
 * Author:  dTb
 * Date:    13/03/19
 */

#include "softwareTimers.h"

softwareTimer::softwareTimer()
{
    Serial.print("Constructor Time... ");
    Serial.println(millis());
    return;
}

void softwareTimer::startTimer(uint8_t* timerId, unsigned int durationMillis)
{
    static uint8_t myTimerId = 0; // tracks the number of software timers being used
    myTimerId++;                  // increment the number
    //initSoftwareTimer(&myTimerId);
    *timerId = myTimerId;

    //static unsigned long _timerEndings[0xFF];
    _timerEndings[myTimerId] = millis() + durationMillis;
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