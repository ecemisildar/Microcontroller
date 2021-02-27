//configuration header
//button and traffic lights in port A
//pedestrian ligths in port B

#include <stm32f10x.h>

#define BUTTON     GPIO_Pin_0
#define T_RED    	 GPIO_Pin_1//red traffic light
#define T_YELLOW   GPIO_Pin_2//red traffic light
#define T_GREEN    GPIO_Pin_3//red traffic light
#define P_GREEN    GPIO_Pin_10//green pedestrian light
#define P_RED      GPIO_Pin_1//red pedestrian light

void Config(void);//configuraiton function
static void EXTI0_IRQHandler(void);//interrupt function

