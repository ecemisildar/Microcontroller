#include "stm32f10x.h"
#include "delay.h"

#define POT   	 		 GPIO_Pin_0
#define pwm					 GPIO_Pin_1	
#define RED_ref    	 GPIO_Pin_5
#define YELLOW_ref   GPIO_Pin_2
#define GREEN_ref    GPIO_Pin_3
#define RED_os    	 GPIO_Pin_4
#define YELLOW_os    GPIO_Pin_7
#define GREEN_os     GPIO_Pin_6

#define LM75_REG_TEMP    0x00 // Temperature
#define LM75_REG_CONF    0x01 // Configuration


void Config_GPIO(void);
void CONFIG_TIM2(void);
void CONFIG_NVIC(void);
void CONFIG_UART1(void);
void UART_Transmit(char *string);
int READ_ADC(void);
void Config_INTERRUPT(void);
void Config_Adc(void);
void CONFIG_I2C(void);

float readTemperature(void);
uint16_t LM75_ReadReg(uint8_t reg);
const char* USART_ReceiveChar(USART_TypeDef* USARTx);





double Gz(double uk);
double PID(double r, double y);
double Derivative(double ek, double ek_1);
double Integral(double ek, double ek_1) ;




