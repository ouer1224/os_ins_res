#include "include.h"

vu32 TimingDelay;

void delay_ms(u32 nTime)
{ 
 TimingDelay = nTime;
 while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
 if (TimingDelay != 0x00)
 { 
   TimingDelay--;
 }
}

void delay_us(u32 nTime)
{ 
  while(nTime--);
}
