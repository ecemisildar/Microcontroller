#include <stm32f10x.h>

#define POT 		  GPIO_Pin_0//port A
#define BUTTON_1  GPIO_Pin_1//port A
#define BUTTON_2  GPIO_Pin_4//port A
#define RED    	 	GPIO_Pin_2//port A
#define YELLOW   	GPIO_Pin_3//port A
#define GREEN    	GPIO_Pin_3//port B

static double limiter = 1;//potentiometer value
static double freq_1 = 0;//duty cycle of green led
static double freq_2 = 0;//duty cycle of yellow led
static double freq_3 = 0;//duty cycle of red led
//configuration functions
void conf_LED(void);
void conf_PWM(void);
void conf_INTERRUPT(void);
void conf_ADC1(void);
void conf_TIMER(void);
void conf_UART1(void);
void UART_Transmit(char *string);



