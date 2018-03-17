#include "tm4c123gh6pm.h"
#define ENTER 0x0D
#define BACKSPACE 0x08
#define LINEFEED 0x0A 

void UART_Init (void);
char UART_InChar (void);
void UART_OutChar (char data);
unsigned long long UART_InDec (void);
void UART_OutDec (long n);
void UART_InString (char *buffer);
void UART_OutString(char *buffer);
void UART_TEST (void);
