#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>

// The CPAC register specifies the access privileges for coprocessors
#define SCB_CPAC          (*((volatile unsigned long*)0xE000ED88)) 
#define TRIG_PF1          (*((volatile unsigned long*)0x40025008))

#include "PLL_Init.h"
#include "SysTick.h"
#include "UART_Init.h"
#include "GPIO_Init.h"
#include "TIMER_Init.h"

void send_Trigger_pulse(void);  // send trigger pulse for 10 micro seconds

int main(void)
{
	unsigned long long pulseWidth; // For calculating the period of the pulse input to the echo pin of the ultrasonic
	unsigned long long distance = 0;     // For the distance calculation
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
		//distance = (pulseWidth * 340) / 2;
		//distance = distance / 1000000;
		
		// send the data through UART
		UART_OutDec(++i);
    UART_OutString(": width: ");
		UART_OutDec(pulseWidth);
		UART_OutString(", distance: ");
		UART_OutDec(distance);
		UART_OutString("\n\r");
	}
}

// send trigger pulse for 10 micro seconds
void send_Trigger_pulse(void)
{
	// set "Trig" pin (PF1) to low
	TRIG_PF1 = 0;
		
	// Waiting for 60 ms
	SysTick_wait1miliS(60);
		
	// Set "Trig" pin (PF1) to High for 10 micro S
	TRIG_PF1 = 0x02;
	// Waiting for 10 micro S
	SysTick_wait1microS(10);
		
	// Set "Trig" pin (PF1) to Low
	TRIG_PF1 = 0;
}

void SystemInit(void)
{
	SCB_CPAC |= 0x00F00000;
}
