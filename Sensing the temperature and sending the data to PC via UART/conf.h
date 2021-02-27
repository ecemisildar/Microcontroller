#include <stm32f10x.h>

#define POT 	GPIO_Pin_0//port A
#define LED   GPIO_Pin_4//port A
#define UART_PORT         USART1
#define LM75_REG_TEMP       0x00 // Temperature
#define LM75_REG_CONF       0x01 // Configuration
//configuration functions
void conf_GPIO(void);
void conf_ADC1(void);
void conf_UART1(void);
void UART_Transmit(char *string);
int READ_ADC(void);
int16_t LM75_Temperature(void);
uint16_t LM75_ReadReg(uint8_t reg);

#define LM75A_ADDR		    0x90
#define LM75A_REG_TEMP    0x00  
#define LM75A_REG_CONF    0x01  
#define LM75A_REG_THYS    0x02  
#define LM75A_REG_TOS     0x03  
