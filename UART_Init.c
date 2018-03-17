#include "UART_Init.h"

// Setup UART on 115200 bps
#define UARTIBRD 43            // UARTIBRD = UARTSysClk (80 * 10^6) / clkDiv (16) * Baud rate (115200) = 43
#define UARTFBRD 26            // UARTFBRD = (int) (BRDF (29 / 72) * 64 + 0.5) = 26 

void UART_Init(void)
{
  // 1. Enable clock to UART Module 0 (SYSCTL_RCGCUART)
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
	
	// 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
  // To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	
	// 3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
  // configure, see Table 23-4 on page 1344.
	GPIO_PORTA_AFSEL_R |= 0x03;
	
	// 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
  // page 673 and page 681).
	// Will use the default
	
	// 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
  // pins (see page 688 and Table 23-5 on page 1351).
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
	
	// To use UART follow these steps that are required to use a UART module:
	// 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
  UART0_CTL_R &= ~UART_CTL_UARTEN;
  
	// 2. Write the integer portion of the BRD to the UARTIBRD register
  UART0_IBRD_R = UARTIBRD;
	
	// 3. Write the fractional portion of the BRD to the UARTFBRD register
  UART0_FBRD_R = UARTFBRD;
	
  // 4. Write the desired serial parameters to the UARTLCRH register
  UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	
  // 5. Configure the UART clock source by writing to the UARTCC register.
	// Clock Source from System Clock
	
	// 6. Optionally, configure the µDMA channel (see “Micro Direct Memory Access (µDMA)” on page 585)
  // and enable the DMA option(s) in the UARTDMACTL register.
	
	
  // 7. Enable UART by clearing UARTEN in UARTCTL register
	// Enable the UART by setting the UARTEN bit in the UARTCTL register.
  UART0_CTL_R |= UART_CTL_UARTEN; // Enable UART0, TXE -> U0TX (PA1), RXE -> U0RX (PA0)
	
	// Enable the digital functionality to PA0 and PA1
	GPIO_PORTA_DEN_R |= 0x03;
	
	// Disable the analog functionality to PA0 and PA1
	GPIO_PORTA_AMSEL_R &= ~0x03;
}

char UART_InChar(void)
{
	// as soon as the recieve FIFO is empty do busy wait
	while ((UART0_FR_R & UART_FR_RXFE) != 0);
	// when the reciever FIFO is not empty, return the data (the 1st 8-bit) in the data register
	return ((char) (UART0_DR_R & 0xFF));
}

void UART_OutChar(char data)
{
	// as soon as the recieve FIFO is full do busy wait.
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	// when the recieve FIFO is not full, put the incoming data into the data register.
	UART0_DR_R = data;
}

unsigned long long UART_InDec(void)
{
	unsigned long long number = 0, length = 0;
	char character;
	character = UART_InChar();
	
	while (character != ENTER)
	{
		if ((character >= '0') && (character <= '9'))
		{
			number = 10 * number + (character - '0');
			length++;
			UART_OutChar(character);
		}
		else if ((character == BACKSPACE) && length)
		{
			number /= 10;
			length--;
			UART_OutChar(character);
		}
		character = UART_InChar();
	}
	
	return number;
}

void UART_OutDec (long n)
{
	if (n >= 10)
	{
		UART_OutDec(n / 10);
		n %= 10;
	}
	UART_OutChar(n + 0x30);
}

void UART_InString (char *buffer)
{
	int length = 0;
	char character;
	character = UART_InChar();
	
	while (character != ENTER)
	{
		if ((character == BACKSPACE) && length)
		{
			buffer--;
			length--;
			UART_OutChar(BACKSPACE);
		}
		else
		{
			*buffer = character;
			buffer++;
			length++;
			UART_OutChar(character);
		}
		character = UART_InChar();
	}
	*buffer = 0;
}

void UART_OutString(char *buffer)
{
	int i = 0;
	
	while (*(buffer + i))
	{
		UART_OutChar(*(buffer + i));
		i++;
	}
}

void UART_TEST(void)
{
	char i;
	char string[100];
	long n;
	
	UART_OutString("UART TESTING.....");
	
	UART_OutString("\n\r");
	for (i = 'A'; i <= 'Z'; i++)
	{
		UART_OutChar(i);
	}
		
	UART_OutString("\n\r");
	for (i = 'a'; i <= 'z'; i++)
	{
		UART_OutChar(i);
	}
	
	UART_OutString("\n\r");
	for (i = '0'; i <= '9'; i++)
	{
		UART_OutChar(i);
	}
		
	UART_OutString("\n\r-->");
	UART_OutString("String input: ");
	UART_InString(string);
	UART_OutString(", String output: ");
	UART_OutString(string);
			
	UART_OutString("\n\rDecimal input: ");
	n = UART_InDec();
	UART_OutString(", Decimal output: ");
	UART_OutDec(n);
}
