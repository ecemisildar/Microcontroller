#include <stm32f10x.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include <stdbool.h>

#define Button_1_PIN (1)	//First button is set as pin 1
#define Button_2_PIN (0)	//First button is set as pin 0

void Delay(uint32_t time);	// Defining dealy function

int i = 0;
int m = 0;
int k = 0;	//Added to check which loop the code is in
int p = 0;	//When button 1 is pressed two times, breaks the while loop

static int signal_1 = 0;	//Button 1 press
static int signal_2 = 0;	//Button 2 press
 
 int value_1 = 0;	//To read button 1
 int value_2 = 0;	//To read button 2

//GPIO_InitTypeDef GPIO_InitStruct;

int main()
{
	RCC->APB2ENR |= 4; //Enabling PORT A
	RCC->APB2ENR |= 0x10; //Enabling PORT C
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIOA->CRL &= 0xFFFFFFF0; //Reset the Port A PIN 0
	GPIOA->CRL |= 0x0008; //Set port A pin 0 as in Input push pull
	
	GPIOA->CRL &= 0xFFFFFF0F; //Reset the Port A PIN 1
	GPIOA->CRL |= 0x0080; //Set port A pin 1 as in Input push pull
	
	GPIOC->CRH &= 0xFF0FFFFF; //Reset the pin 13
	GPIOC->CRH |= 0x00200000; //Set PIN 13 as output
	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStruct);

//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIOC->ODR |= 0x2000;	//Turn off the led

	
	while(1)	//General while for infinite loop
	{		
		GPIOC->ODR |= 0x2000;	//Turn off the led
		value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	//Checks if 1. button pressed
//		value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
		if(value_1 == 2)
					{
						value_1 = true; //If 1.button is pressed, makes this variable TRUE						
					}
					
		if(value_1 == true)
					{
						signal_1 = 1;	//When 1. button pressed, this variable is equal to 1
						signal_2 = 0;	//When 2. button not pressed, this variable is equal to 0
						p = 1; 
					}

		while(signal_1==1) //When we press 1-time to first button, led is blinking with 1000 ms frequency
		{	
			//value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	
			//value_2 = (GPIOA->IDR & (1<<Button_2_PIN));
			value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
			value_2 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_0));
				if(p == 1)
				{
					value_1 = false; //If we pressed 1. button 2 times or 2k times value is equal to false and breaks loop
					p = 0;
				}
					if(value_1 == 2)
					{
						value_1 = true;	//If 1.button is pressed, it assigns TRUE					
					}	
					

			//These if and else blocks shows all possible situations for button 1	
			//If value_1 and signal_1 values are TRUE, this means there is one pulse
			//When the pulse appears on the system, current values of signal 1 are changed		
			if(value_1 == false & signal_1 == 0)
			{
				signal_1 = 0;  
			}
			else if(value_1 == true & signal_1	== 0)
			{
				signal_1 = 1;
			}
			else if(value_1 == true & signal_1	==1)
			{
				signal_1 = 0;
			}
			else if(value_1 == false & signal_1	==1)
			{
				signal_1 = 1;
			}
			
			//These if and else blocks are show all possible situations for button 2	
			//If value_2 and signal_2 values are TRUE, this means there is one pulse.
			//When the pulse appears on the system, current values of signal 2 are changed
			if(value_2 == false & signal_2 == 0)
			{
				signal_2 = 0;
			}
			else if(value_2 == true & signal_2	== 0)
			{
				signal_2 = 1;
			}
			else if(value_2 == true & signal_2	==1)
			{
				signal_2 = 0;
			}
			else if(value_2 == false & signal_2	==1)
			{
				signal_2 = 1;
			}
			
			if(signal_2 == 1)
			{k=2;	//To check which loop the code is in
				GPIOC->ODR |= 0x2000; //Led is off
//			GPIO_ResetBits(GPIOC, GPIO_Pin_13);				
				for(i = 0; i<100; i++)
				{
				//value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	
				//value_2 = (GPIOA->IDR & (1<<Button_2_PIN));
			  value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
			  value_2 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_0));
				if(value_1 == 2)
				{
					value_1 = true;	//If 1.button is pressed, it assigns TRUE		
					break;
				}		
				Delay(5); //Delay for 500 ms
				}
				GPIOC->ODR &= ~0x2000; //Led is on
//			GPIO_SetBits(GPIOC, GPIO_Pin_13);
					for(i = 0; i<100; i++)
				{
				//value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	
				//value_2 = (GPIOA->IDR & (1<<Button_2_PIN));
			  value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
			  value_2 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_0));
				if(value_1 == 2)
				{
					value_1 = true;	//If 1.button is pressed, it assigns TRUE	
					break;
				}	
				Delay(5); //Delay for 500 ms
				}
			}				

			if(signal_2 == 0)	//If we pressed just 1-time first button, this loop begins
			{k=1; //To check which loop the code is in
			GPIOC->ODR |= 0x2000; //Led is off
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);				
				for(i = 0; i<10; i++)
				{
				//value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	
				//value_2 = (GPIOA->IDR & (1<<Button_2_PIN));
			  value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
			  value_2 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_0));
				if(value_1 == 2)
				{
					value_1 = true;	//If 1.button is pressed, it assigns TRUE		
					break;					
				}		
				Delay(100); //Delay for 500 ms
				}
				GPIOC->ODR &= ~0x2000; //Led is on
//			GPIO_SetBits(GPIOC, GPIO_Pin_13);				
					for(i = 0; i<10; i++)
				{
				//value_1 = (GPIOA->IDR & (1<<Button_1_PIN));	
				//value_2 = (GPIOA->IDR & (1<<Button_2_PIN));
			  value_1 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_1));
			  value_2 = (GPIO_ReadInputData(GPIOA) & (GPIO_Pin_0));
				if(value_1 == 2)
				{
					value_1 = true;	//If 1.button is pressed, it assigns TRUE		
					break;
				}	
				Delay(100); //Delay for 500 ms
				}
			}
			
				if(signal_1 == 0)
				{k=4;
					GPIOC->ODR |= 0x2000;
		      GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				}

		}

			GPIOC->ODR |= 0x2000;	//If code is quit the while loop, led off	
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
}

//Delay function
void Delay(uint32_t time)
{
	time=time*2318;
	while(time)
	{				
		time--;	
	}
}
