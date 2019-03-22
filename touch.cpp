#include "touch.h"

/*
 * This function manipulates the global "gateDelay" based on capsense input.
 * We get here when the system is idle, and stay until the touch input has stopped
 * would be better to not lock the loop whilst inputs are being detected but meh
 */

LIGHTSTATE lightState;


void touchDetect(long touchVal)
{
    static softwareTimer softwareTimer_c;
    static char touchState = 0, tapTimeoutState = 0;
    static unsigned char deBounceTimer, tapTimer, tapTimeout, fadeInterval;
    static char tapCount = 0;

    if ((unsigned int)touchVal < CAPSENSE_THRESHOLD) // timeout for tap.
    {
        switch (tapTimeoutState) 
        {
        case 0:
            if (tapCount > 0)
            {
                softwareTimer_c.startTimer(&tapTimeout, 300); //300); //debounce timer start
                tapTimeoutState++;
                Serial.println("timeout begun");
            }
            break;

        case 1:
            if (softwareTimer_c.checkTimer(&tapTimeout))
            {
                tapCount = 0;
                tapTimeoutState = 0;
                Serial.println("timeout ended");
            }
            break;
        }
    }
        

    switch (touchState)
    {
        case 0:
        if ((unsigned int)touchVal > CAPSENSE_THRESHOLD)    // if we detect a touch, start the timer and go to next step after debounce
        {
            softwareTimer_c.startTimer(&deBounceTimer, 10); //debounce timer start
            Serial.println("timer start");
            touchState++;
        }
        break;

        case 1:
        if ((unsigned int)touchVal > CAPSENSE_THRESHOLD)
        {
            if (softwareTimer_c.checkTimer(&deBounceTimer))
            {
                Serial.println("hi");
                touchState++;
            }
            else Serial.println("timer not elapsed");

        } 
        else 
        {
            touchState = 0; // go back home if touch stops
            tapCount = 0; // wipe the taps too
            Serial.println("cya");
        }
        break;

        case 2: // now figure out if this is as tap or a hold using a tap duration timer
        // if release within tap duratino timer, it's a tap. go to tap behaviour
        // if hold, move to hold behavior
        softwareTimer_c.startTimer(&tapTimer, 200); //tap timer start
        touchState++;
        break;

        case 3:
        if ((unsigned int)touchVal > CAPSENSE_THRESHOLD)
        {
            if (softwareTimer_c.checkTimer(&tapTimer))
            {
                // longer than a tap, do the fading manipulation here
                Serial.println("fade duration detected");
                touchState++;
                break;
            }
        } 
        else 
        {
            tapCount++;
            Serial.println("tap detected");
            Serial.print("en: ");
            Serial.print(lightState.en);
            Serial.print(" brightness: ");
            Serial.println(lightState.brightness);
            if (tapCount > 1) // double tap (or more for safety)
            {
                Serial.println("double tap detected");
                //turn light off unless already off, then turn on.
                if (lightState.en == 0 && lightState.brightness == 0)
                {
                    lightState.en = 1;
                    lightState.brightness = 100;
                    Serial.println("enable with 100 brightness");
                }
                else if (lightState.en == 1 && lightState.brightness != 0)
                {
                    //do more stuff
                    lightState.en = 0;
                    lightState.brightness = 0;
                    Serial.println("disable with 0 brightness");
                }
                //end of state go to beginning
                tapCount = 0;
                touchState = 0;
                break;
            }
            touchState = 0;
        }
        break;

        case 4: // we're fading, do this here
        // check we're at tapCount 1 or ignore input
        {   //scoping for defines
            static char fadeInhibit = 0;

            if (tapCount != 1)
            {
                Serial.println("trying to fade without prior tap not allowed");
                touchState = 0;
                break;
            }
            if ((unsigned int)touchVal > CAPSENSE_THRESHOLD) // make sure we're still holding the input
            {
                // these tests/direction changes actually cause light to fade up and down automatically by holding on
                if (lightState.brightness >= MAX_BRIGHTNESS)
                    lightState.fadeDirection = DIR_DOWN;
                if (lightState.brightness <= MIN_BRIGHTNESS)
                    lightState.fadeDirection = DIR_UP;

                if (lightState.fadeDirection == DIR_UP) // up
                {
                    if (!fadeInhibit)
                    {
                        if (lightState.en == 0)
                            lightState.en = 1;
                        //Serial.print("Fading up. Brightness: ");
                        lightState.brightness += 1;
                        //softwareTimer_c.startTimer(&fadeInterval, 1);
                        fadeInhibit = 1;
                    }
                }
                else //down
                {
                    if (!fadeInhibit)
                    {
                        //Serial.print("Fading down. Brightness: ");
                        lightState.brightness -= 1;
                        //softwareTimer_c.startTimer(&fadeInterval, 1);
                        fadeInhibit = 1;
                    }
                }

                if (lightState.brightness > MAX_BRIGHTNESS)
                    lightState.brightness = MAX_BRIGHTNESS;
                if (lightState.brightness < MIN_BRIGHTNESS)
                    lightState.brightness = MIN_BRIGHTNESS;
                //Serial.println(lightState.brightness);
            }
            else
            {
                lightState.fadeDirection = !lightState.fadeDirection; // flip the direction ready for the next fade cycle, can only do this coz fadeDirection is a bool
                touchState = 0;
                tapCount = 0;
            }
            if (fadeInhibit)    //this is the delay timer checker
                if(softwareTimer_c.checkTimer(&fadeInterval))
                    fadeInhibit = 0;

            break;
        }
    }
}
