#include "SysTick.h"

// Initialize the SysTick with busy wait running at bus clock
void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;              // disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;   // maximum reload value
	NVIC_ST_CURRENT_R = 0;           // any write to current clears it 
	NVIC_ST_CTRL_R = 0x00000005;     // enable SysTick with core clock
}

// Time dela using busy wait.
// The delay parameter is in units of core clock.
void SysTick_wait(unsigned long delay)
{
	volatile unsigned long elapsedTime;
	unsigned long startTime = NVIC_ST_CURRENT_R;
	do
	{
		elapsedTime = (startTime - NVIC_ST_CURRENT_R) & 0x00FFFFFF;
	}
	while(elapsedTime <= delay);
}

// Time delay using busy wait.
// This assumes 80 MHz sysytem clock
void SysTick_wait1microS(unsigned long delay)
{
	unsigned long i;
	for (i = 0; i < delay; i++)
	{
		SysTick_wait(80); // wait 1 micro seconds (assumes 80 MHz clock)
	}
}

// Time delay using busy wait.
// This assumes 80 MHz sysytem clock
void SysTick_wait1miliS(unsigned long delay)
{
	unsigned long i;
	for (i = 0; i < delay; i++)
	{
		SysTick_wait(80000); // wait 1 mili seconds (assumes 80 MHz clock)
	}
}

// Time delay using busy wait.
// This assumes 80 MHz sysytem clock
void SysTick_wait10miliS(unsigned long delay)
{
	unsigned long i;
	for (i = 0; i < delay; i++)
	{
		SysTick_wait(800000); // wait 10 mili seconds (assumes 80 MHz clock)
	}
}


void Delay1micros ( unsigned long microsec )
{
	unsigned long i;
  while ( microsec > 0 )
	{
    i = 16;  // this number means 1 micro s
    while ( i > 0 )
		{
      i = i - 1;
    }
    microsec = microsec - 1; // decrements every 1 micro s
  }
}

void Delay1ms ( unsigned long msec )
{
	unsigned long i;
  while ( msec > 0 )
	{
    i = 16000;  // this number means 1 mili s
    while ( i > 0 )
		{
      i = i - 1;
    }
    msec = msec - 1; // decrements every 1 mili s
  }
}

void Delay1s ( unsigned long sec )
{
	unsigned long i;
  while ( sec > 0 )
	{
    i = 16000000;  // this number means 1 s
    while ( i > 0 )
		{
      i = i - 1;
    }
    sec = sec - 1; // decrements every 1 s
  }
}
