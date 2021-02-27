#include <stm32f10x.h>
#include "config.h"
#include "delay.h"

int state=0;//states of switch case 
int button_1=0;//interrupt for button 1
int button_2=0;//interrupt for button 2

void EXTI0_IRQHandler(void)//interrupt function for the 1. button
{	

if(EXTI_GetITStatus(EXTI_Line0) != RESET)//when pressed the button
{
	button_1=1;//when interrupt occurs button_1=1

EXTI_ClearITPendingBit(EXTI_Line0); //Clear interrupt flag
}
}

void EXTI4_IRQHandler(void)//interrupt function for the 2. button
{
if(EXTI_GetITStatus(EXTI_Line4) != RESET)//when pressed the button
{
	button_2=1;//when interrupt occurs button_1=1
	
EXTI_ClearITPendingBit(EXTI_Line4); //Clear interrupt flag
}
}

int main()
{	
	//enabling peripherals
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//functions
	Config_LED();
	Config_BUTTON();
	Config_INTERRUPT();
	Config_TIMER();
	delayInit();
	
	while(1)
	{				
		switch(state)//long fsm with 10 states
		{
			//For low case duty cycle is 1000
			//Medium case duty cycle is 5000
			//High case duty cycle is 10000
			case 1://RED LOW
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to YELLOW LOW case (case 4)
				//only the yellow led turns on with low opacity and the others are turned off
				duty_yellow=1000;
				 TIM2->CCR3 = duty_yellow;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
				state=4;//after that goes to case 4
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to the RED MEDIUM case (case 2)
				 duty_red=5000;
				//only the red led turns on with medium opacity and the others are turned off
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=2;//goes to the case 2
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}			
			break;
			
			case 2://RED MEDIUM
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to YELLOW LOW case (case 4)
				//only the yellow led turns on with low opacity and the others are turned off
				duty_yellow=1000;
				 TIM2->CCR3 = duty_yellow;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=4;//after that goes to case 4
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to RED HIGH case (case 3)
				//only the red led turns on with high opacity and the others are turned off
				duty_red=10000;
				 TIM2->CCR2 = duty_red;	
				duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=3;//after that goes to case 3
				button_1=0;//and makes the buttons zero again
				button_2=0;			
			}
			break;
			
			case 3://RED HIGH
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to YELLOW LOW case (case 4)
				//only the yellow led turns on with low opacity and the others are turned off
				duty_yellow=1000;
				 TIM2->CCR3 = duty_yellow;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=4;//after that goes to case 4
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to RED LOW case (case 1)
				//only the red led turns on with low opacity and the others are turned off
				duty_red=1000;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=1;//after that goes to case 1
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			break;
			
			case 4://YELLOW LOW				
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to GREEN LOW case (case 7)
				//only the green led turns on with low opacity and the others are turned off
				duty_green=1000;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=7;//after that goes to case 7
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to YELLOW MEDIUM case (case 5)
				//only the yellow led turns on with medium opacity and the others are turned off
			duty_yellow=5000;
				 TIM2->CCR3 = duty_yellow;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
				state=5;//after that goes to case 7
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			break;
						
			case 5://YELLOW MEDIUM
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to GREEN LOW case (case 7)
				//only the green led turns on with low opacity and the others are turned off
				duty_green=1000;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=7;//after that goes to case 7
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to YELLOW HIGH case (case 6)
				//only the yellow led turns on with high opacity and the others are turned off
				duty_yellow=10000;
				 TIM2->CCR3 = duty_yellow;
				duty_green=0;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
				state=6;//after that goes to case 6
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			break;			
			case 6://YELLOW HIGH
			delayMs(200);		
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to GREEN LOW case (case 7)
				//only the green led turns on with low opacity and the others are turned off
				duty_green=1000;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=7;//after that goes to case 7
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to YELLOW LOW case (case 7)
				//only the yellow led turns on with low opacity and the others are turned off
				duty_yellow=1000;
				 TIM2->CCR3 = duty_yellow;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_green=0;
				 TIM2->CCR4 = duty_green;
				state=4;//after that goes to case 4
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			 break;
						
			case 7://GREEN LOW
			delayMs(200);		
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to DEFAULT case (case 0)
				//the all leds are turned off
				duty_red=0;
			TIM2->CCR2 = duty_red;
				duty_yellow=0;
			TIM2->CCR3 = duty_red;
				duty_green=0;
			TIM2->CCR4 = duty_red;
				state=0;//after that goes to case 0
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to GREEN MEDIUM case (case 8)
				//only the green led turns on with medium opacity and the others are turned off
				duty_green=5000;
				 TIM2->CCR4 = duty_green;
				duty_red=0;
				 TIM2->CCR2 = duty_red;
				duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=8;//after that goes to case 8
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			break;
			
			case 8://GREEN MEDIUM
			delayMs(200);			
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to DEFAULT case (case 0)
				//The all leds are turned off
				duty_red=0;
			TIM2->CCR2 = duty_red;
				duty_yellow=0;
			TIM2->CCR3 = duty_red;
				duty_green=0;
			TIM2->CCR4 = duty_red;
				state=0;//after that goes to case 0
				button_1=0;//and makes the buttons zero again				
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to GREEN HIGH case (case 9)
				//only the green led turns on with high opacity and the others are turned off
				duty_green=10000;
				 TIM2->CCR4 = duty_green;	
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=9;//after that goes to case 9
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			 break;
			
			case 9://GREEN HIGH
			delayMs(200);
			if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to DEFAULT case (case 0)
				//The all leds are turned off
				duty_red=0;
			TIM2->CCR2 = duty_red;
				duty_yellow=0;
			TIM2->CCR3 = duty_red;
				duty_green=0;
			TIM2->CCR4 = duty_red;
				state=0;//after that goes to case 0
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}				
			else if(button_2==1 && button_1==0)//when the second button pressed
			{
				//goes to GREEN LOW case (case 7)
				//only the green led turns on with low opacity and the others are turned off
				duty_green=1000;
				 TIM2->CCR4 = duty_green;
			duty_red=0;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
				state=7;//after that goes to case 7
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			break;			 
	    default:
			delayMs(200);
	    if(button_1==1 && button_2==0)//when the first button pressed
			{
				//goes to RED LOW case (case 1)
				//only the red led turns on with low opacity and the others are turned off
				duty_red=1000;
				 TIM2->CCR2 = duty_red;
			duty_yellow=0;
				 TIM2->CCR3 = duty_yellow;
			duty_green=0;
				 TIM2->CCR4 = duty_green;	
				state=1;//after that goes to case 1
				button_1=0;//and makes the buttons zero again
				button_2=0;
			}
			else if(button_1==0 && button_2==0)
			{
				//goes to DEFAULT case (case 0)
				//The all leds are turned off
					duty_red=0;
			TIM2->CCR2 = duty_red;
				duty_yellow=0;
			TIM2->CCR3 = duty_red;
				duty_green=0;
			TIM2->CCR4 = duty_red;
				state=0;//after that goes to case 0
				button_1=0;//and makes the buttons zero again
				button_2=0;				
			}
			break;
		}		
	}
}

