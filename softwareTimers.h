/*
 * Define & control software timers
 * Author:  dTb
 * Date:    13/03/19
 */

#ifndef SOFTWARETIMERS_H
#define SOFTWARETIMERS_H

#include "system.h"

class softwareTimer
{
  public:             //funcs and vars that can be accessed from outside the instance of class
    softwareTimer(); // constructor, used to create an instance of this class. no return type, must match name of class, called once per instance of class.
    void startTimer(uint8_t* timerId, unsigned int durationMillis);
    bool checkTimer(uint8_t timerId);

    private:
    //void initSoftwareTimer(uint8_t* myTimerId);
    // float _voltage; // underscores are used to differentiate between priv and pub variables
    // float _pressure;
    unsigned long _timerEndings[0xFF];
};

#endif