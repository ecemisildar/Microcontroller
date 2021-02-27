//configuration header

#include <stm32f10x.h>

//pin numbers port A
#define POT			  GPIO_Pin_0
#define RED    	 	GPIO_Pin_1
//#define YELLOW   	GPIO_Pin_2
//#define GREEN    	GPIO_Pin_3
//#define GREEN_2   GPIO_Pin_4

//functions
void Config_LED(void);
void Config_Pot(void);
void config_Adc(void);
void Config_INTERRUPT(void);
void Config_TIMER(void);
void EXTI0_IRQHandler(void);
void EXTI4_IRQHandler(void);
void TIM2_IRQHandler(void);



int static duty_red=0;//red leds pwm
//int static duty_yellow=0;//yellow leds pwm
//int static duty_green=0;//green leds pwm




