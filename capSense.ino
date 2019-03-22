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
char getSamp = 0;

void setup()
{
    Serial.begin(115200);

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

    lightState.en = 0;
    lightState.fadeDirection = FADE_UP;
    lightState.brightness = 0;

}











void loop()
{
    long senseVal = 0;


    senseVal = cs_4_2.capacitiveSensor(30); 

    touchDetect(senseVal);

    // //calculate gateDelay based on brightness
    // if(lightState.brightness == 0)
    //     gateDelay = syncTime - 800;

    // if (lightState.brightness == 100)
    //     gateDelay = 100;

    gateDelay = ((100 - lightState.brightness) * syncTime) / 100;
    if (gateDelay < 100)
        gateDelay = 100;
    if (gateDelay > syncTime - 500)
        gateDelay = syncTime - 500;
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
}



void timer1Interrupt(void) // this function gets called at the right time, depending on ligfhtState.brightness level.
{
    
    //noInterrupts();

    // Timer1.restart(); causes an interrupt to happen that we don't want to trigger on...
    // so for now, bin off 1 / 2 interrupts

    if (timerInterruptSkip == 0)
    {
        if(lightState.en == 1)
        {
            digitalWrite(DIMMER_GATE_pin, HIGH); //could use direct port manipulation but timing will be fine with 50/60Hz mains
            delayMicroseconds(200);              // short pulse to the triac to make sure it triggers
            digitalWrite(DIMMER_GATE_pin, LOW);
        }
        else
        {
            digitalWrite(DIMMER_GATE_pin, LOW); // off
        }

        Timer1.stop(); //always stop the timer after a successful trigger
    }
    else
    {
        // literally do FA to skip over the unwated initial interrupt when Timer1.restart() is called.
        timerInterruptSkip = 0;
    }

    //interrupts();
}

