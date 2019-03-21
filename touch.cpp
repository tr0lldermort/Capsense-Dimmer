#include "touch.h"

/*
 * This function manipulates the global "gateDelay" based on capsense input.
 * We get here when the system is idle, and stay until the touch input has stopped
 * would be better to not lock the loop whilst inputs are being detected but meh
 */
void touchDetect(long touchVal)
{
    static char touchState = 0;
    static unsigned char swTimer1, swTimer2, swTimer3;
    static softwareTimer softwareTimer_c;

    switch (touchState)
    {
        case 0:
        if ((unsigned int)touchVal > CAPSENSE_THRESHOLD)    // if we detect a touch, start the timer and go to next step after debounce
        {
            softwareTimer_c.startTimer(&swTimer1, 500); //debounce timer
         touchState++;
        }
        else
        {
            //stop timer
        }
        
        
        

        break;



        case 1:
            if (softwareTimer_c.checkTimer(swTimer1))
            {
                Serial.println("poo face");
                touchState=0;
            }
            break;

        case 2:
        break;

        case 3:
        break;

        case 4:
        break;
        
    }

}