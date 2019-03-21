#include <EnableInterrupt.h>
#include <CapacitiveSensor.h>
#include <TimerOne.h>

#include "touch.h"
#include "system.h"

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */




CapacitiveSensor cs_4_2 = CapacitiveSensor(4, 2); // 2M between pins, Pin 2 is sensing.
unsigned int gateDelay = 0;
char fadeDirection = FADE_UP;
static long lastSync = 0;
static long syncTime = 0;
static long timerSync = 0;
static long lastTimerSync = 0;
static char timerInterruptSkip = 0;

void setup()
{


    pinMode(DIMMER_GATE_pin, OUTPUT);
    digitalWrite(DIMMER_GATE_pin, LOW);

    cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // Autocal period. setting to 0xFFFFFFFF turns it off.
    cs_4_2.set_CS_Timeout_Millis(2000);
    // cs_4_2.reset_CS_AutoCal(); // This can be used to force calibrate capsense function
    enableInterrupt(3, syncInterrupt, RISING);

    //delay(100);
    //while (syncTime == 0);  //wait until we have a sync time

    Timer1.initialize(gateDelay);
    //Timer1.attachInterrupt(timer1Interrupt); // attach t1 interrupt to function

    Serial.begin(115200);

    
}

void loop()
{
    long start = millis();
    long total1 = cs_4_2.capacitiveSensor(30); 
    Serial.print(millis() - start); // check on performance in milliseconds
    Serial.print("\t");             // tab character for debug window spacing
    Serial.println(gateDelay);        // print sensor output 1

    touchDetect(total1);


    // if ((unsigned int)total1 > CAPSENSE_THRESHOLD)
    // {
    //     switch (fadeDirection)
    //     {
    //     case FADE_UP:
    //     gateDelay += 50;    // change this to a fade resolution #define
    //     if (gateDelay > syncTime-800)   //syncTime is measured based on mains freq, darkest light level. subtract 800us to defo prevent glitching. #define this val
    //     {
    //         fadeDirection = FADE_DOWN;
    //     }  
    //     break;

    //     case FADE_DOWN:
    //     gateDelay -= 50;
    //     if (gateDelay < 4000)   // this 4000 defines max brightness.
    //     {
    //         fadeDirection = FADE_UP;
    //     }
    //     break;
    // }
    //}
}

void syncInterrupt(void)
{
    static char binSomeOff = 0;
    if (binSomeOff < 100) // bin a few off to get a good reading
    {
        syncTime = micros() - lastSync;
        lastSync = micros();
        gateDelay = syncTime-800; // keep doing this til we don't run this func amymore, to dim the light to a max level. #define the 800us as it's used in other places.
        binSomeOff++;
    }
    else 
    {
        Timer1.attachInterrupt(timer1Interrupt); // attach t1 interrupt to function. put a small state machine here and do this in the middle. do this here so fnctn doesn't trigger at start
        //noInterrupts();
        Timer1.setPeriod(gateDelay); //Timer1.start(); // restart timer1 from 0.
        Timer1.restart();
        timerInterruptSkip = 1;
        //interrupts();
    }
    //delayMicroseconds(gateDelay);             //accurate delay. TODO: this delay slows the whole loop down, can we use internal interrupts to time this without locking up the loop?
}

void timer1Interrupt(void) 
{
    
    noInterrupts();

    // Timer1.restart(); causes an interrupt to happen that we don't want to trigger on...
    // so for now, bin off 1 / 2 interrupts

    if (timerInterruptSkip == 0)
    {
        digitalWrite(DIMMER_GATE_pin, HIGH); //could use direct port manipulation but timing will be fine with 50/60Hz mains
        delayMicroseconds(200); // short pulse to the triac to make sure it triggers
        digitalWrite(DIMMER_GATE_pin, LOW);

        Timer1.stop(); //always stop the timer after a successful trigger
    }
    else
    {
        // literally do FA to skip over the unwated initial interrupt when Timer1.restart() is called.
        timerInterruptSkip = 0;
    }

    interrupts();
}

void timer2Interrupt(void)
{
    //do gateDelay adjustments here in a regular fashon
}