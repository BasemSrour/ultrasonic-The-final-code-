#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>

// The CPAC register specifies the access privileges for coprocessors
#define SCB_CPAC          (*((volatile unsigned long*)0xE000ED88))

#include "PLL_Init.h"
#include "SysTick.h"
#include "UART_Init.h"
#include "GPIO_Init.h"
#include "TIMER_Init.h"

// send trigger pulse for 10 micro seconds to "Trig" pins (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7)
void send_Trigger_pulse(void);

int main(void)
{
	unsigned long long pulseWidth; // For calculating the period of the pulse input to the echo pin of the ultrasonic
	unsigned int distanceInCm = 0;     // For the distance calculation
	const double eightyMHzOneClockCycle = 0.0000000125;
	const int ultraSoundSpeedInCmPerS = 34000;
	unsigned long long i = 0;
	
	PLL_Init();     // initializes the PLL to the desired frequency which is 80 MHz
	SysTick_Init(); // initialize the SysTick with busy wait running at bus clock
	UART_Init();    // initialization UART on PA0 and PA1
	GPIO_Init();    // initialization the GPIOs
	TIMER_Init(); // initialization Timer Input Capture on PORTF[2] "echo" pin
	
	UART_TEST();
	UART_OutString("\n\rUltrasonic output: \n\r");
	
	while(1)
	{
		// send 10 micro seconds pulse from the "trigger" pin (PF1)
		send_Trigger_pulse();
		
		// Call measureD() to get the delta t on the "echo" pin (PF2)
		pulseWidth = measureD();
		
		// Calculate distance
		distanceInCm = ((pulseWidth * eightyMHzOneClockCycle) / 2) * ultraSoundSpeedInCmPerS;
		
		// send the data through UART
		UART_OutDec(++i);
    UART_OutString(": width: ");
		UART_OutDec(pulseWidth);
		UART_OutString(", distance: ");
		UART_OutDec(distanceInCm);
		UART_OutString(" cm\n\r");
	}
}

// send trigger pulse for 10 micro seconds
// "Trig" pins are (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7)
void send_Trigger_pulse(void)
{
	// set "Trig" pin (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7) to low
	GPIO_PORTA_DATA_R &= ~0xFC;
	GPIO_PORTE_DATA_R &= ~0x0F;
		
	// Waiting for 60 ms
	SysTick_wait1miliS(60);
		
	// Set "Trig" pin (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7) to High for 10 micro seconds
	GPIO_PORTA_DATA_R |= 0xFC;
	GPIO_PORTE_DATA_R |= 0x0F;
	// Waiting for 10 micro S
	SysTick_wait1microS(10);
		
	// Set "Trig" pin (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7) to Low
	GPIO_PORTA_DATA_R &= ~0xFC;
	GPIO_PORTE_DATA_R &= ~0x0F;
}

void SystemInit(void)
{
	SCB_CPAC |= 0x00F00000;
}
