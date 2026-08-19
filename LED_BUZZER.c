#include <lpc21xx.h>
#include "delay.h"
void RED_LED_ON(void)
{
   IOSET0 = 1<<23;//failure passcode mismatch
}

void RED_LED_OFF(void)
{
   IOCLR0 = 1<<23;//7- read led off
}




