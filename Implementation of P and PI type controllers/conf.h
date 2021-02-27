#include "stm32f10x.h"

void CONFIG_TIM2(void);
void CONFIG_NVIC(void);
void CONFIG_UART1(void);
void UART_Transmit(char *string);
int READ_ADC(void);
void Config_INTERRUPT(void);
void Config_Adc(void);
