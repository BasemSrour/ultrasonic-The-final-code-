// defining the registers of the systick
#define NVIC_ST_CTRL_R         (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R       (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R      (*((volatile unsigned long *)0xE000E018))

// SysTick delay functions using busy wait
void SysTick_Init(void);
void SysTick_wait(unsigned long delay);
void SysTick_wait1microS(unsigned long delay);
void SysTick_wait1miliS(unsigned long delay);
void SysTick_wait10miliS(unsigned long delay);

// Delay functions using do nothing for loops
void Delay1ms ( unsigned long msec );
void Delay1micros ( unsigned long microsec );
void Delay1s ( unsigned long sec );
