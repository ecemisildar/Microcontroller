#include <stm32f10x.h>
#include "config.h"

int state = 0;//switch case statements
int counter=0;//works with timer
int n=0;
int main()
{
	//enabling peripherals
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	Config();//configurations of leds button timer and interrupt

	while(1)
	{	
		switch(state)
		{
			case 1:	//red traffic light and green pedestrian lights on
			//others turn off
			GPIO_ResetBits(GPIOA, T_YELLOW);
			GPIO_ResetBits(GPIOA, T_GREEN);
			GPIO_ResetBits(GPIOB, P_RED);
			GPIO_SetBits(GPIOA, T_RED);
			GPIO_SetBits(GPIOB, P_GREEN);				
			break;	
			
			case 2:	//yellow traffic light and red pedestrian lights on
			//turn of the lights that are on at 1st case
			GPIO_ResetBits(GPIOA, T_RED);
			GPIO_ResetBits(GPIOB, P_GREEN);
			GPIO_SetBits(GPIOA, T_YELLOW);
			GPIO_SetBits(GPIOB, P_RED);			
			break;
			
			case 3:	//green traffic light and red pedestrian lights on
			//turn of the lights that are on at 2nd case
			GPIO_ResetBits(GPIOA, T_YELLOW);
			GPIO_ResetBits(GPIOB, P_RED);
			GPIO_SetBits(GPIOA, T_GREEN);
			GPIO_SetBits(GPIOB, P_RED);			
			break;
			
			case 4: //yellow traffic light and red pedestrian lights on
			//turn of the lights that are on at 3st case
			GPIO_ResetBits(GPIOA, T_GREEN);
			GPIO_ResetBits(GPIOB, P_RED);
			GPIO_SetBits(GPIOA, T_YELLOW);
			GPIO_SetBits(GPIOB, P_RED);
			counter=1;
			break;			
			default:
			state=1;//in the beginning state is 1
			counter=1;
			break;
		}
	}
}			

		
			
