#include "stm32f10x.h"
#include "config.h"
#include "delay.h"
#include "math.h"
#include <stdio.h>

int state = 0;
int value_1 = 0;//read first button 
int value_2 = 0;//read second button
int max = 4095;//max value of pot
int speed = 0;//sum of frequences
//debug control variables
int a=0;
int k=0;

static int data=0;//data to sent PC
char spd[20];//char array
int Sent_data=0;


void EXTI1_IRQHandler(void)//interrupt function for the 1. button
{		
if(EXTI_GetITStatus(EXTI_Line1) != RESET)//when pressed the button
{
	value_1 = (GPIO_ReadInputData(GPIOA) & (BUTTON_1))/2;//reads the 1. button		
	EXTI_ClearITPendingBit(EXTI_Line1); //Clear interrupt flag
}	
}

void EXTI4_IRQHandler(void)//interrupt function for the 2. button
{
if(EXTI_GetITStatus(EXTI_Line4) != RESET)//when pressed the button
{
	value_2 = (GPIO_ReadInputData(GPIOA) & (BUTTON_2))/16;//reads the second button	
EXTI_ClearITPendingBit(EXTI_Line4); //Clear interrupt flag
}
}

int main()
{
	//enabling peripherals
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
	| RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);//gpio
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//timer
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);//usart
	//functions
	conf_LED();
	conf_PWM();
	conf_INTERRUPT();
	conf_ADC1();
	conf_UART1();
	delayInit();	
	
	while(1)
	{	
		//reads the value from the potentiometer and divided by the max value of it.
	//I used the pot max value as 300 for the calculation of the speed, I divided it by 3.
		limiter=ceil((float)(ADC_GetConversionValue(ADC1))/max *300+1);
		a=fmod(limiter,10);//I took the mode of the limiter with 10
		speed= (freq_1+freq_2+freq_3)/3;//green+yellow+red freqs/3		
		
		//I have 5 cases including default
		switch(state)
		{			 
			case 1://green led: 0-33km/h
				//if I press 1. button freq of green is smaller than limiter and its max value(100-10)
				if(value_1==1 && (freq_1+10)<=limiter && freq_1<=90)
				{	
					freq_2=0;//yellow led freq 0
					freq_3=0;//red led freq 0
					TIM2->CCR2=freq_2;//off yellow led
					TIM2->CCR3=freq_3;//off red led
					delayMs(250);//0.25 seconds of the press the button
					freq_1 = freq_1 + 10;//freq of green led increases by 10							
			  }
				//if I press 2. button and freq of green is greater than zero 			
				if(value_2==1 && freq_1>0)
				{
					freq_2=0;//yellow freq 0
					freq_3=0;//green freq 0
					delayMs(250);//0.25 seconds of the press the button
					freq_1 = freq_1 - 10;//freq of green led decreases by 10
				}
				//if green led freq is equal to 100 or greater than pot's value
				//I mean, pot restricts the frequency
				else if(freq_1 == 100 | (freq_1+10)>=limiter)
				{
					state = 2;//go to the next state turn on yellow led
				}
				else if((freq_1+freq_2+freq_3)>=limiter)//if total frequency is greater than pot's value
				{
					state = 4;//this means pot is decreasing//go to the state 4
				}
				TIM2->CCR4=freq_1;//turn on the green led					
				break;		
						
			case 2://yellow led: 34-66km/h
				//if I press 1. button freq of yellow is smaller than limiter and its max value(100-10)
				if(value_1==1 && (freq_1+freq_2+10)<=limiter && freq_2<=90)
				{	
					freq_3=0;//red led freq 0
					TIM2->CCR3=freq_3;//off red led
					delayMs(250);
					freq_2 = freq_2 + 10;//freq of yellow led increases by 10					
			  }	
				//if I press 2. button and freq of yellow is greater than zero				
				if(value_2==1 && freq_2>0)
				{
					freq_3=0;//red led freq 0
					delayMs(250);
					freq_2 = freq_2 - 10;//freq of yellow led decreases by 10					
				}
				//if yellow led freq is equal to 100 or greater than pot's value
				else if(freq_2 == 100 | (freq_1+freq_2+10)>= limiter)
				{
					state = 3;//go to the next state turn on red led
				}
				//if yellow led turned off
				if(freq_2 == 0)
				{
					state = 1;//go to the previous case 
				}
				if((freq_1+freq_2+freq_3)>=limiter)//if total frequency is greater than pot's value
				{
					state = 4;//this means pot is decreasing//go to the state 4
				}
						TIM2->CCR2=freq_2;//turn on the yellow led
				break;
			
			case 3://yellow led: 67-100km/h
				//if I press 1. button freq of red is smaller than limiter
				if(value_1==1 && (freq_1+freq_2+freq_3+10)<=limiter)
				{	
					delayMs(250);
					freq_3 = freq_3 + 10;//freq of red led increases by 10						
			  }	
				//if I press 2. button and freq of red is greater than zero				
				if(value_2==1 && freq_3>0)
				{
					delayMs(250);
					freq_3 = freq_3 - 10;//freq of red led decreases by 10						
				}
				//if red led freq is equal to 100 or greater than pot's value
				else if(freq_3 == 100 | (freq_1+freq_2+freq_3+10)>= limiter)
				{
					state = 3;//wait inside this case
				}
				//if red led turned off
				if(freq_3 == 0)
				{
					state = 2;//go to the previous case
				}
				//if total frequency is greater than pot's value
				else if((freq_1+freq_2+freq_3)>=limiter)
				{
					state = 4;//this means pot is decreasing//go to the state 4
				}
						TIM2->CCR3=freq_3;//turn on the red led	
				break;
				
			case 4://potantiometer restriction
					//there are 3 possibility
					//limiter greater than 200,between 100 and 200,smaller than 100
					if(limiter>=200)
					{
						k=1;
						freq_1=100;//turn on first two leds at full freq
						freq_2=100;
						freq_3=limiter-200-a;//if green and yellow leds are on it makes minimum 200 freq and, 
						//"a" is for subtraction pot values remainder part 	
						state = 3;//go to the 3. case
					}					
					else if(limiter>=100 & limiter<200)
					{
						k=2;
						freq_1=100;//turn on the first led at full freq
						freq_2=limiter-100-a;//if green led is on it makes minimum 100 freq and, 
						//"a" is for subtraction pot values remainder part
						state = 2;//go to the 2. case
					}
					else if(limiter<100)
					{
						k=3;
						freq_3=0;//turn of red and yellow leds
						freq_2=0;
						freq_1=limiter-a;//"a" is for subtraction pot values remainder part
						state= 1;//go to the 1. case
					}
					
			break;
				
			default://off state
				if(value_1==1)//if I press the first button
					state = 1;//go to the 1. case and start to journey
				break;
			
	}
}
}
//control purpose variables
int counter=0;
int x=0;

void TIM2_IRQHandler(void)//timer for serial comm 
	{ 
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//Periodic IRQ 
			{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)//I used pulse=1 
				{	
					counter++;
					if(counter%500==0)//my timer is 1000Hz, so I divided it by 500 to obtain 2Hz
					{
						x++;
					USART_SendData(USART1, speed);//send my speed data to PC	
						while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);//reset the flag
					}
					TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);						
				}						
			}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
