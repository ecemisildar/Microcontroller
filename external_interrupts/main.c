#include <stm32f10x.h>
#include "delay.h"

//Defining button and leds make easier to read the code
#define BUTTON     GPIO_Pin_0 
#define LED_RED    GPIO_Pin_1
#define LED_YELLOW GPIO_Pin_2
#define LED_GREEN  GPIO_Pin_3

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

//function definitions
void Config(void);
void GREEN(void);
void EXTI0_IRQHandler(void);

static int state = 0;//switch case statements
int i=0;

void EXTI0_IRQHandler(void)//interrupt function
{
if(EXTI_GetITStatus(EXTI_Line0) != RESET)//when pressed the button
{
if(state==3)//state is equal green
{
	GPIO_ResetBits(GPIOA, LED_GREEN);//turn of the green led
	state=1;//go to the state 1 this means red led is on
}

EXTI_ClearITPendingBit(EXTI_Line0); //Clear interrupt flag
}
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
 
 if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
 {
	 	 duty_1=7200;
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);	
 }
}

int main()
{
	//enabling gpio and afio periph 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	Config();//configurations of leds button and interrupt
	delayInit();//calling delay function
	
	while(1)
	{	
		switch(state)
		{
			case 1://red led blink 10s
				GPIO_SetBits(GPIOA, LED_RED);
				delayMs(10000);
				GPIO_ResetBits(GPIOA, LED_RED);
				state=2;//goes to the yellow state
			break;
			
			case 2://yellow led blink 2s
				GPIO_SetBits(GPIOA, LED_YELLOW);
				delayMs(2000);
				GPIO_ResetBits(GPIOA, LED_YELLOW);
				state=3;//goes to the green state
			break;
			
			case 3://green led blink 2s
				GREEN();
				if(state!=1)//if state didnt become 1, I mean we dont press the button
				state=4;//continue with yellow state again		
				else
				state=1;//however if pressed to the button, go to state 1 which turns on the red led
			break;
			
			case 4://yellow led blink 2s
				GPIO_SetBits(GPIOA, LED_YELLOW);
				delayMs(2000);
				GPIO_ResetBits(GPIOA, LED_YELLOW);
				state=1;//goes to the red state
			break;
			
			default:
				state=1;//in the beginning state is 1 
			break;
		}				
	}	
}


void GREEN(void)//Green led function
{
	GPIO_SetBits(GPIOA, LED_GREEN);
	for(i=0;i<1000;i++)//I divided delay into the pieces to prevent delay after interrupt
	{
	delayMs(10);
		if(state==1)//if state became that means there is an interrupt
			break;//stop delaying and go to the next state
	}
	GPIO_ResetBits(GPIOA, LED_GREEN);
}
void Config(void)
{
	//LED_RED 
	GPIO_InitStruct.GPIO_Pin = LED_RED;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//LED_YELLOW
	GPIO_InitStruct.GPIO_Pin = LED_YELLOW;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//LED_GREEN
	GPIO_InitStruct.GPIO_Pin = LED_GREEN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//BUTTON
	GPIO_InitStruct.GPIO_Pin = BUTTON;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	//INTERRUPT
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//Configure nested vector interrupt controller
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
