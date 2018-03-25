#include "GPIO_Init.h"

// GPIO Initialization and Configuration for the trigger pin (PE0, PE1, PE2, PE3, PA2, PA3, PA4, PA5, PA6 & PA7)
void GPIO_Init(void)
{
	volatile unsigned long delay;
	
	// 1. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	// For PORT A
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	// For PORT E
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	
	// 2. Set the direction of the GPIO port pins by programming the GPIODIR register.
  // A write of a 1 indicates output and a write of a 0 indicates input.
	// Make them output (write of a 1) for each pin.
	GPIO_PORTA_DIR_R |= 0xFC; // PA2, PA3, PA4, PA5, PA6 & PA7
	GPIO_PORTE_DIR_R |= 0x0F; // PE0, PE1, PE2 & PE3
	
	
	// 3. Configure the GPIOAFSEL register to program each bit as a GPIO or alternate pin.
	// If you set it to 1, you will turn on the alternate function.
	// If you set it to 0, you will turn off the alternate function.
	// I want to turn off the alternate function (set it to 0).
	GPIO_PORTA_AFSEL_R &= ~0xFC; // PA2, PA3, PA4, PA5, PA6 & PA7
	GPIO_PORTE_AFSEL_R &= ~0x0F; // PE0, PE1, PE2 & PE3
	
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTA_PCTL_R &= 0x000000FF; // PA2, PA3, PA4, PA5, PA6 & PA7
	GPIO_PORTE_PCTL_R &= 0xFFFF0000; // PE0, PE1, PE2 & PE3
	
	
	// 4. Set the drive strength for each of the pins through the GPIODR2R, GPIODR4R, and GPIODR8R
  // registers.
	// keep it in default.
	
	
	// 5. Program each pad in the port to have either pull-up, pull-down, or open drain functionality through
  // the GPIOPUR, GPIOPDR, GPIOODR register. Slew rate may also be programmed, if needed,
	// through the GPIOSLR register.
	// keep it in default.
	
	
	// 6. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register. To
  // enable GPIO pins to their analog function (if available), set the GPIOAMSEL bit in the
  // GPIOAMSEL register.
	// PA2, PA3, PA4, PA5, PA6 & PA7
	GPIO_PORTA_AMSEL_R &= ~0xFC;
	GPIO_PORTA_DEN_R |= 0xFC;
	
	// PE0, PE1, PE2 & PE3
	GPIO_PORTE_AMSEL_R &= ~0x0F;
	GPIO_PORTE_DEN_R |= 0x0F;
	
	
	// 7. Program the GPIOIS, GPIOIBE, GPIOEV, and GPIOIM registers to configure the type, event,
  // and mask of the interrupts for each port.
	/*
	 * Note: To prevent false interrupts, the following steps should be taken when re-configuring
	 * GPIO edge and interrupt sense registers:
	 * a. Mask the corresponding port by clearing the IME field in the GPIOIM register.
	 * b. Configure the IS field in the GPIOIS register and the IBE field in the GPIOIBE
	 * register.
	 * c. Clear the GPIORIS register.
	 * d. Unmask the port by setting the IME field in the GPIOIM register.
	 */
	
	
	// 8. Optionally, software can lock the configurations of the NMI and JTAG/SWD pins on the GPIO
  // port pins, by setting the LOCK bits in the GPIOLOCK register.
	// For PORT A
	GPIO_PORTA_LOCK_R = 0x4C4F434B;   // unlock GPIO Port A
  GPIO_PORTA_CR_R = 0xFC;           // allow changes to PA2, PA3, PA4, PA5, PA6 & PA7
	
	// For PORT E
	GPIO_PORTE_LOCK_R = 0x4C4F434B;   // unlock GPIO Port E
  GPIO_PORTE_CR_R = 0x0F;           // allow changes to PE0, PE1, PE2 & PE3
}
