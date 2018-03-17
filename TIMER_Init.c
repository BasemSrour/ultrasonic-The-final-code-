#include "TIMER_Init.h"

void TIMER_Init(void)
{
	//* PF2: T1CCP0 (PCTL=07) both edge (edge-time mode)
	
	volatile unsigned long delay;
	
 /**
	 *To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER or RCGCWTIMER
   *register (see page 338 and page 357). If using any CCP pins, the clock to the appropriate GPIO
   *module must be enabled via the RCGCGPIO register (see page 340). To find out which GPIO port
   *to enable, refer to Table 23-4 on page 1344. Configure the PMCn fields in the GPIOPCTL register to
   *assign the CCP signals to the appropriate pins (see page 688 and Table 23-5 on page 1351).
	**/
	
  // 1 . Enable Clock for TIMER 1 Module
  SYSCTL_RCGCTIMER_R |= 0x02;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	// 2. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	// 3. Set the direction of the GPIO port pins by programming the GPIODIR register. A write of a 1
  // indicates output and a write of a 0 indicates input.
	GPIO_PORTF_DIR_R &= ~0x04;
	
	// 4. Configure the GPIOAFSEL register to program each bit as a GPIO or alternate pin.
	// If you set it to 1, you will turn on the alternate function
	// If you set it to 0, you will turn off the alternate function
	GPIO_PORTF_AFSEL_R |= 0x04;
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & 0xFFFFF0FF) + 0x00000700;
	
	// 5. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register. To
  // enable GPIO pins to their analog function (if available), set the GPIOAMSEL bit in the
  // GPIOAMSEL register.
	GPIO_PORTF_AMSEL_R &= ~0x04;
	GPIO_PORTF_DEN_R |= 0x04;
	
	// A timer is configured to Input Edge Time mode by the following sequence:
  // 1. Ensure the timer is disabled (the TAEN bit is cleared) before making any changes.
  TIMER1_CTL_R &= ~0x101;  // Disable TimerA & TimerB
  
	// 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x00000004.
  TIMER1_CFG_R |= 0x04;    //  this value selects the 16-bit timer configuration.
	
  // 3. Configure Timer n Mode: GPTMTAMR
	// In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x1 and the TnMR
  // field to 0x3 and select a count direction (down) by programming the TnCDIR bit.
  TIMER1_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER1_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	
  // 4. Configure Timer Event Mode: rising-, falling-, or both-edges
	// Configure the type of event that the timer captures by writing the TnEVENT field of the GPTM
  // Control (GPTMCTL) register.
  TIMER1_CTL_R |=  0x0C;  // Both edges
	
  // 5. Configure Load
  // - If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
  //   (GPTMTnPR).
  // - Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
  // - If interrupts are required, set the CnEIM bit in the GPTM Interrupt Mask (GPTMIMR) register.
  TIMER1_TAILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  TIMER1_TAPR_R = 0xFF;
  TIMER1_IMR_R = 0;
	
  // 7. Enable GPTM Timer
	// Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and start counting.
  TIMER1_CTL_R |= 0x01;  // Enable TimerA
}

// Poll the CnERIS bit in the GPTMRIS register or wait for the interrupt to be generated (if enabled).
// In both cases, the status flags are cleared by writing a 1 to the CnECINT bit of the GPTM
// Interrupt Clear (GPTMICR) register. The time at which the event happened can be obtained
// by reading the GPTM Timer n (GPTMTnR) register.
unsigned long long measureD(void)
{
	unsigned long long highEdge,lowEdge;
  unsigned long long deltaT;
 
  /*Capture firstEgde i.e. rising edge*/
  //1. Clear GPTM Timer A Capture Mode Event by writing 1 to
  //   corresponding bit on GPTMICR (GTPM_ICR_R) register
	TIMER1_ICR_R |= 0x04;
 
  //2. Waiting for capture rising edge event by check the GPTM Raw Interrupt Status
  //   GPTMRIS (GTPM_RIS_R) register
	while ((TIMER1_RIS_R & 0x04) == 0);
 
  //3. Read the highEdge from GPTMTAR (GPTM_TAR_R) registers
	highEdge = TIMER1_TAR_R;
 
  /*Capture secondEgde i.e. falling edge*/
  //4. Clear GPTM Timer A Capture Mode Event by writing 1 to
  //   corresponding bit on GPTMICR (TIMER1->ICR) register
	TIMER1_ICR_R |= 0x04;
		
  //5. Waiting for capture falling edge event by check the GPTM Raw Interrupt Status
  //   GPTMRIS (TIMER1->RIS) register
	while ((TIMER1_RIS_R & 0x04) == 0);
		
  //6. Read the lowEdge from GPTMTAR (TIMERa->TAR) registers
	lowEdge = TIMER1_TAR_R;
		
  // 7. Calculate deltaT = highEdge - lowEdge
  // Note: the deltaT must large than zero, cannot be negative value
	deltaT = highEdge - lowEdge;
		
  return deltaT;
}
