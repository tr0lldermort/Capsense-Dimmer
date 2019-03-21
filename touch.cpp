#include "touch.h"

/*
 * This function manipulates the global "gateDelay" based on capsense input.
 * We get here when the system is idle, and stay until the touch input has stopped
 * would be better to not lock the loop whilst inputs are being detected but meh
 */
void touchDetect(long touchVal)
{
    static char touchState = 0;

    if ((unsigned int)touchVal > CAPSENSE_THRESHOLD)
    {
        switch (touchState)
        {
            case 0:
            break;

            case 1:
            break;

            case 2:
            break;

            case 3:
            break;

            case 4:
            break;
            
        }
    }
}