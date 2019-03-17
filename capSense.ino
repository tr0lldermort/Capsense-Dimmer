#include <EnableInterrupt.h>

#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */

#define DIMMER_GATE_pin 5
#define FADE_UP 0
#define FADE_DOWN 1

#define CAPSENSE_THRESHOLD 2000


CapacitiveSensor cs_4_2 = CapacitiveSensor(4, 2); // 2M between pins, Pin 2 is sensing.
unsigned int usDelay = 4000;
bool fadeDirection = FADE_UP;

void setup()
{

    pinMode(DIMMER_GATE_pin, OUTPUT);
    digitalWrite(DIMMER_GATE_pin, LOW);

    cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // Autocal period. setting to 0xFFFFFFFF turns it off.
    cs_4_2.set_CS_Timeout_Millis(2000);
    // cs_4_2.reset_CS_AutoCal(); // This can be used to force calibrate capsense function

    enableInterrupt(3, syncInterrupt, RISING);

    Serial.begin(115200);
}

void loop()
{
    // long start = millis();
    long total1 = cs_4_2.capacitiveSensor(30); 
    // Serial.print(millis() - start); // check on performance in milliseconds
    // Serial.print("\t");             // tab character for debug window spacing
    Serial.println(usDelay);        // print sensor output 1

    if ((unsigned int)total1 > CAPSENSE_THRESHOLD) 
    {
        switch (fadeDirection)
        {
            case FADE_UP:
            usDelay += 100;
            if (usDelay > 8500) 
            {
                fadeDirection = FADE_DOWN;
            }  
            break;

            case FADE_DOWN:
            usDelay -= 100;
            if (usDelay < 4000)
            {
                fadeDirection = FADE_UP;
            }
            break;
        }
    }
}

void syncInterrupt(void){
    // static long oldTime = millis();
    // Serial.print("We've interrupted baby!");
    // Serial.print("\t");
    // Serial.print("Time between interrupts: ");
    // Serial.println(millis()-oldTime);
    // oldTime = millis();

    delayMicroseconds(usDelay);             //accurate delay. TODO: this delay slows the whole loop down, can we use internal interrupts to time this without locking up the loop?
    digitalWrite(DIMMER_GATE_pin, HIGH);    //could use direct port manipulation but timing will be fine with 50/60Hz mains
    digitalWrite(DIMMER_GATE_pin, LOW);
}