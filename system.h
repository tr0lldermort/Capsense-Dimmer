/*
 * ...
 * Author:  dTb
 * Date:    13/03/19
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

#define DIMMER_GATE_pin 5
#define FADE_UP 0
#define FADE_DOWN 1

#define CAPSENSE_THRESHOLD 2000

extern unsigned int gateDelay;

void syncInterrupt(void);
void timer1Interrupt(void);
void timer2Interrupt(void);

#endif