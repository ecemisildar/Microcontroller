#include <stm32f10x.h>

//Defining button and leds make easier to read the code
#define BUTTON     GPIO_Pin_0 
#define LED_RED    GPIO_Pin_1
#define LED_YELLOW GPIO_Pin_2
#define LED_GREEN  GPIO_Pin_3

int state 		= 0;				//	states of the fsm
int P_counter = 0;				//	counts how many times pressed to the button
void Delay(uint32_t time);//	Delay function decleration
void LED_ON(void);				//	turn on the leds
void LED_OFF(void);				//	turn off the leds
void LED_BLINK(void);			//	blink led in order red yellow green
void Control(void);				//	to decrease waiting time

int main()
{
	RCC->APB2ENR |= 4; //Enabling PORT A
	
	GPIOA->CRL &= 0xFFFFFFF0; //Reset the Port A PIN 0
	GPIOA->CRL |= 0x0008; //Set port A pin 0 as in Input push pull
	
	GPIOA->CRL &= 0xFFFFFF0F; //Reset the Port A PIN 1
	GPIOA->CRL |= 0x0020; //GP output push pull 2mHz
	
	GPIOA->CRL &= 0xFFFFF0FF; //Reset the Port A PIN 2
	GPIOA->CRL |= 0x0200; //GP output push pull 2mHz
	
	GPIOA->CRL &= 0xFFFF0FFF; //Reset the Port A PIN 3
	GPIOA->CRL |= 0x2000; //GP output push pull 2mHz

	while(1)
	{
		//finite state machine
		switch(state)
		{
			case 1://the case ON
				LED_ON();
			//P_counter counts the press number if it is odd that means led should blinking, 
			//and if it is prime, then leds should turn off.
				if((P_counter%2 == 1) & (!GPIO_ReadInputDataBit(GPIOA, BUTTON)))
				{	
					//if we press to the button 1st time or its mutlipliers and now we release it this condition is provided
					//and state is equal to 2 which is the blinking case.
					state = 2;					
				}
				if((P_counter%2 == 0) & (!GPIO_ReadInputDataBit(GPIOA, BUTTON)))
				{
					//if we press the button 2nd time or its mutlipliers and now release it this condition comes true
					//and state is equal to 0 which is the off case.
					state = 0;
				}
				break;
				
			case 2://the case BLINK
				LED_BLINK();
				if(GPIO_ReadInputDataBit(GPIOA, BUTTON))//Read the button
				{
					P_counter++;//counts how many times we press
					state = 1;//then goes to the first case which is ON
				}
				break;
				
			case 0://the case OFF
				LED_OFF();
				if(GPIO_ReadInputDataBit(GPIOA, BUTTON))//Read the button
				{
					P_counter++;//counts how many times we press
					state = 1;//then goes to the first case which is ON
				}
				break;
		}
	}
}


//set the bits for all leds
void LED_ON(void)
{
	GPIO_SetBits(GPIOA, LED_RED);
	GPIO_SetBits(GPIOA, LED_YELLOW);
	GPIO_SetBits(GPIOA, LED_GREEN);
}
//reset the bits for all leds
void LED_OFF(void)
{
	GPIO_ResetBits(GPIOA, LED_RED);
	GPIO_ResetBits(GPIOA, LED_YELLOW);
	GPIO_ResetBits(GPIOA, LED_GREEN);
}

//provides to blink led in order on red->yellow->green
void LED_BLINK(void)
{
	GPIO_SetBits(GPIOA, LED_RED);
	Control();
	GPIO_ResetBits(GPIOA, LED_RED);
	GPIO_SetBits(GPIOA, LED_YELLOW);
	Control();
	GPIO_ResetBits(GPIOA, LED_YELLOW);
	GPIO_SetBits(GPIOA, LED_GREEN);
	Control();
	GPIO_ResetBits(GPIOA, LED_GREEN);
}

//delay function
void Delay(uint32_t time)
{
	time=time*2318;
	while(time)
	{				
		time--;	
	}
}

//controls if the counter is odd number 100 times and makes delay
void Control(void)
{
	int i = 0;
	for(i=0; i<100; i++)
	{
		if(P_counter%2 == 1)
		Delay(5);
	}
}
