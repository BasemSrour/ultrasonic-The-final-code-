#include "GPIO_Init.h"

// GPIO Initialization and Configuration for the trigger pin (PF1)
void GPIO_Init(void)
{
	volatile unsigned long delay;
	
	// 1. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	// Waiting till stabilizing
  delay = SYSCTL_RCGC2_R;
	
	// 2. Set the direction of the GPIO port pins by programming the GPIODIR register. A write of a 1
  // indicates output and a write of a 0 indicates input.
	GPIO_PORTF_DIR_R |= 0x02;
	
	// 3. Configure the GPIOAFSEL register to program each bit as a GPIO or alternate pin.
	// If you set it to 1, you will turn on the alternate function
	// If you set it to 0, you will turn off the alternate function
	GPIO_PORTF_AFSEL_R &= ~0x02;
	// select GPIO functionality by clearing the bits in the GPIO Port Control (GPIOPCTL)
	GPIO_PORTF_PCTL_R &= 0xFFFFFF0F;
	
	// 4. Set the drive strength for each of the pins through the GPIODR2R, GPIODR4R, and GPIODR8R
  // registers.
	// select it to default.
	
	// 5. Program each pad in the port to have either pull-up, pull-down, or open drain functionality through
  // the GPIOPUR, GPIOPDR, GPIOODR register. Slew rate may also be programmed, if needed,
	// through the GPIOSLR register.
	
	// 6. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register. To
  // enable GPIO pins to their analog function (if available), set the GPIOAMSEL bit in the
  // GPIOAMSEL register.
	GPIO_PORTF_AMSEL_R &= ~0x02;
	GPIO_PORTF_DEN_R |= 0x02;
	
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
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x06;           // allow changes to PF1 and PF2
}
