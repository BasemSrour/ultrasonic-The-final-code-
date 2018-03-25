#include "TIMER_Init.h"

// Initialization for TIMER (echo) pins
// "ECHO" pins are PC4, PC5, PF0, PF1, PF2, PF3, PB0, PB1, PB2 & PB3
void TIMER_Init(void)
{
	// WideTimer0
	//* PC4: WT0CCP0 (PCTL=07) both edges (edge-time mode)
	//* PC5: WT0CCP1 (PCTL=07) both edges (edge-time mode)
	// TIMER0
	//* PF0: T0CCP0 (PCTL=07) both edges (edge-time mode)
	//* PF1: T0CCP1 (PCTL=07) both edges (edge-time mode)
	// TIMER1
	//* PF2: T1CCP0 (PCTL=07) both edges (edge-time mode)
	//* PF3: T1CCP1 (PCTL=07) both edges (edge-time mode)
	// TIMER2
	//* PB0: T2CCP0 (PCTL=07) both edges (edge-time mode)
	//* PB1: T2CCP1 (PCTL=07) both edges (edge-time mode)
	// TIMER3
	//* PB2: T3CCP0 (PCTL=07) both edges (edge-time mode)
	//* PB3: T3CCP1 (PCTL=07) both edges (edge-time mode)
	
	volatile unsigned long delay;
	
 /**
	 *To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER or RCGCWTIMER
   *register (see page 338 and page 357). If using any CCP pins, the clock to the appropriate GPIO
   *module must be enabled via the RCGCGPIO register (see page 340). To find out which GPIO port
   *to enable, refer to Table 23-4 on page 1344. Configure the PMCn fields in the GPIOPCTL register to
   *assign the CCP signals to the appropriate pins (see page 688 and Table 23-5 on page 1351).
	**/
	
  // 1 . Enable Clock for WTimer 0, TIMER 0, 1, 2 & 3 Modules
	// For WTIMER0 (PC4: WT0CCP0 & PC5: WT0CCP1)
	SYSCTL_RCGCWTIMER_R |= 0x01;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	// For TIMER0 (PF0: T0CCP0, PF1: T0CCP1)
	SYSCTL_RCGCTIMER_R |= 0x01;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	// For TIMER1 (PF2: T1CCP0, PF3: T1CCP1)
	SYSCTL_RCGCTIMER_R |= 0x02;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	// For TIMER2 (PB0: T2CCP0, PB1: T2CCP1)
	SYSCTL_RCGCTIMER_R |= 0x04;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	// For TIMER3 ((PB2: T3CCP0, PB3: T3CCP1))
	SYSCTL_RCGCTIMER_R |= 0x08;
  // Waiting till stabilizing
	delay = SYSCTL_RCGCTIMER_R;
	
	
	// 2. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	// For PC4 & PC5
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	// For PF0, PF1, PF2 & PF3
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	// For PB0, PB1, PB2 & PB3
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	
	// 3. Set the direction of the GPIO port pins by programming the GPIODIR register.
  // A write of a 1 indicates output and a write of a 0 indicates input.
	// I want them all to be input (write 0)
	// For PC4 & PC5
	GPIO_PORTC_DIR_R &= ~0x30;
	
	// For PF0, PF1, PF2 & PF3
	GPIO_PORTF_DIR_R &= ~0x0F;
	
	// For PB0, PB1, PB2 & PB3
	GPIO_PORTB_DIR_R &= ~0x0F;
	
	
	// 4. Configure the GPIOAFSEL register to program each bit as a GPIO or alternate pin.
	// If you set it to 1, you will turn on the alternate function
	// If you set it to 0, you will turn off the alternate function
	// I want to turn the alternate function on so i will write 1 to the approperiate pin.
	// For PC4 & PC5
	GPIO_PORTC_AFSEL_R |= 0x30;
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0x00770000;
	
	// For PF0, PF1, PF2 & PF3
	GPIO_PORTF_AFSEL_R |= 0x0F;
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & 0xFFFF0000) + 0x00007777;
	
	// For PB0, PB1, PB2 & PB3
	GPIO_PORTB_AFSEL_R |= 0x0F;
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0000) + 0x00007777;
	
	
	// 5. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register. To
  // enable GPIO pins to their analog function (if available), set the GPIOAMSEL bit in the
  // GPIOAMSEL register.
	// For PC4 & PC5
	GPIO_PORTC_AMSEL_R &= ~0x30;
	GPIO_PORTC_DEN_R |= 0x30;
	
	// For PF0, PF1, PF2 & PF3
	GPIO_PORTF_AMSEL_R &= ~0x0F;
	GPIO_PORTF_DEN_R |= 0x0F;
	
	// For PB0, PB1, PB2 & PB3
	GPIO_PORTB_AMSEL_R &= ~0x0F;
	GPIO_PORTB_DEN_R |= 0x0F;
	
	
	// 6. Optionally, software can lock the configurations of the NMI and JTAG/SWD pins on the GPIO
  // port pins, by setting the LOCK bits in the GPIOLOCK register.
	// For PC4 & PC5
	GPIO_PORTC_LOCK_R = 0x4C4F434B;   // unlock GPIO Port C
  GPIO_PORTC_CR_R = 0x30;           // allow changes to PC4 & PC5
	
	// For PF0, PF1, PF2 & PF3
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port E
  GPIO_PORTF_CR_R = 0x0F;           // allow changes to PF0, PF1, PF2 & PF3
	
	// For PB0, PB1, PB2 & PB3
	GPIO_PORTB_LOCK_R = 0x4C4F434B;   // unlock GPIO Port B
  GPIO_PORTB_CR_R = 0x0F;           // allow changes to PB0, PB1, PB2 & PB3
	
	
	
	// A timer is configured to Input Edge Time mode by the following sequence:
  // 1. Ensure the timer is disabled (the TAEN bit is cleared) before making any changes.
	// For WTIMER0, TIMER0, 1, 2 & 3 (Disable TimerA & TimerB)
	WTIMER0_CTL_R &= ~0x101;
	TIMER0_CTL_R &= ~0x101; 
	TIMER1_CTL_R &= ~0x101;
  TIMER2_CTL_R &= ~0x101;
	TIMER3_CTL_R &= ~0x101;
  
	// 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x00000004.
	// For WTIMER0, TIMER0, 1, 2 & 3
	//  this value selects the 32-bit timer configuration as it wide timer.
	WTIMER0_CFG_R |= 0x04;
	//  this value selects the 16-bit timer configuration as it normal timer.
  TIMER0_CFG_R |= 0x04;
	TIMER1_CFG_R |= 0x04;
	TIMER2_CFG_R |= 0x04;
	TIMER3_CFG_R |= 0x04;
	
  // 3. Configure Timer n Mode: GPTMTAMR
	// In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x1 and the TnMR
  // field to 0x3 and select a count direction (down) by programming the TnCDIR bit.
	// For WTIMER0, TIMER0, 1, 2 & 3
	// For WT0CCP0 (A) & WT0CCP1 (B)
	WTIMER0_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	WTIMER0_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	WTIMER0_TBMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	WTIMER0_TBMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	
	// For T0CCP0 (A) & T0CCP1 (B)
	TIMER0_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER0_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	TIMER0_TBMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER0_TBMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
  
	// For T1CCP0 (A) & T1CCP1 (B)
	TIMER1_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER1_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	TIMER1_TBMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER1_TBMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	
	// For T2CCP0 (A) & T2CCP1 (B)
	TIMER2_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER2_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	TIMER2_TBMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER2_TBMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	
	// For T3CCP0 (A) & T3CCP1 (B)
	TIMER3_TAMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER3_TAMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	TIMER3_TBMR_R |= 0x07;  // write write the TnCMR field to 0x1 and the TnMR field to 0x3
	TIMER3_TBMR_R &= ~0x10; // select a count direction (down -> write 0) by programming the TnCDIR bit
	
	
  // 4. Configure Timer Event Mode: rising-, falling-, or both-edges
	// Configure the type of event that the timer captures by writing the TnEVENT field of the GPTM
  // Control (GPTMCTL) register.
	// For WTIMER0, TIMER0, 1, 2 & 3
	// This value is for selecting Both edges.
	WTIMER0_CTL_R |=  0x0C;
	TIMER0_CTL_R |=  0x0C;
	TIMER1_CTL_R |=  0x0C;
	TIMER2_CTL_R |=  0x0C;
  TIMER3_CTL_R |=  0x0C;
	
  // 5. Configure Load
  // - If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
  //   (GPTMTnPR).
  // - Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
  // - If interrupts are required, set the CnEIM bit in the GPTM Interrupt Mask (GPTMIMR) register.
	// For WTIMER0, TIMER0, 1, 2 & 3
	// For WT0CCP0 (A) & WT0CCP1 (B)
  WTIMER0_TAILR_R = 0xFFFF;
	WTIMER0_TBILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  WTIMER0_TAPR_R = 0xFF;
	WTIMER0_TBPR_R = 0xFF;
	// disable the interrupt.
  WTIMER0_IMR_R = 0;
	
	// For T0CCP0 (A) & T0CCP1 (B)
  TIMER0_TAILR_R = 0xFFFF;
	TIMER0_TBILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  TIMER0_TAPR_R = 0xFF;
	TIMER0_TBPR_R = 0xFF;
	// disable the interrupt.
  TIMER0_IMR_R = 0;
	
	// For T1CCP0 (A) & T1CCP1 (B)
  TIMER1_TAILR_R = 0xFFFF;
	TIMER1_TBILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  TIMER1_TAPR_R = 0xFF;
	TIMER1_TBPR_R = 0xFF;
	// disable the interrupt.
  TIMER1_IMR_R = 0;
	
	// For T2CCP0 (A) & T2CCP1 (B)
  TIMER2_TAILR_R = 0xFFFF;
	TIMER2_TBILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  TIMER2_TAPR_R = 0xFF;
	TIMER2_TBPR_R = 0xFF;
	// disable the interrupt.
  TIMER2_IMR_R = 0;
	
	// For T3CCP0 (A) & T3CCP1 (B)
  TIMER3_TAILR_R = 0xFFFF;
	TIMER3_TBILR_R = 0xFFFF;
  // Set the prescaler to 0xFF
  TIMER3_TAPR_R = 0xFF;
	TIMER3_TBPR_R = 0xFF;
	// disable the interrupt.
  TIMER3_IMR_R = 0;
	
  // 7. Enable GPTM Timer
	// Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and start counting.
	// For WTIMER0, TIMER0, 1, 2 & 3 (Enable TimerA & B)
  WTIMER0_CTL_R |= 0x101;
	TIMER0_CTL_R |= 0x101;
	TIMER1_CTL_R |= 0x101;
	TIMER2_CTL_R |= 0x101;
	TIMER3_CTL_R |= 0x101;
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
