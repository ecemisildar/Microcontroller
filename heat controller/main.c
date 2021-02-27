#include "conf.h"
#include "delay.h"
#include "math.h"

int state = 0;//for switch case
float temp;//temperature value
float reference=0;//instantenious pot value
char adc[20];//temp value will be sent
int main()
{
	I2C_AcknowledgeConfig(I2C2,ENABLE); // Enable I2C acknowledgment
	// Enable clocks for I2C2, PORTB and AFIO 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);//i2c
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//enabling peripherals
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
	| RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);//gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);//usart	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
	| RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);//gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);//usart
	//configuration functions
	Config_GPIO();
	Config_Adc();
	CONFIG_TIM2();
	CONFIG_NVIC();
	CONFIG_UART1();
	READ_ADC();
	CONFIG_I2C();
	delayInit();				
	while(1)
	{
		reference=READ_ADC();//reads pot data		
		switch(state)
		{			 
			case 1://30 degree
				if(reference<1365)// 4095/3
				{
					GPIO_ResetBits(GPIOA, RED_ref);//red off
					GPIO_ResetBits(GPIOA, YELLOW_ref);//yellow off
					GPIO_SetBits(GPIOA, GREEN_ref);//green reference led is on
					if(temp<27.5)//full power until difference is less than 2.5 degree 
						TIM2->CCR2=0;					
					if(temp>27.5)//zero power after 2.5 degree difference
					{
						TIM2->CCR2=3600;
						if(temp==29 | temp==28.5)	
							TIM2->CCR2=1800;
						if(temp>30 & temp<30.6)//overshoot smaller than 2%
						{
							GPIO_ResetBits(GPIOA, RED_os);//red off
							GPIO_ResetBits(GPIOA, YELLOW_os);//yellow off
							GPIO_SetBits(GPIOA, GREEN_os);//green overshoot led is on
						}
						else if(temp>33)//overshoot greater than 10%
						{
							GPIO_ResetBits(GPIOA, GREEN_os);
							GPIO_ResetBits(GPIOA, YELLOW_os);
							GPIO_SetBits(GPIOA, RED_os);//red overshoot led is on
						}
						else if(temp>30.6 & temp<33)//overshoot between 2%-10%
						{
							GPIO_ResetBits(GPIOA, RED_os);
							GPIO_ResetBits(GPIOA, GREEN_os);
							GPIO_SetBits(GPIOA, YELLOW_os);//yellow overshoot led is on
						}
					}		
				}
				if(reference>1365 && reference<2730)//if reference changes
				state=2;//go to case 2
				break;
			
			case 2://35 degree
				if(reference>1365 && reference<2730)// 4095*2/3
				{
					GPIO_ResetBits(GPIOA, RED_ref);//red off
					GPIO_ResetBits(GPIOA, GREEN_ref);//green off
					GPIO_SetBits(GPIOA, YELLOW_ref);//yellow reference led is on
					if(temp<32.5)//full power until difference is less than 2.5 degree
						TIM2->CCR2=0;
					if(temp>32.5)//zero power after 2.5 degree difference
					{
						TIM2->CCR2=3600;
						if(temp==34 | temp==33.5)	
							TIM2->CCR2=1800;
						if(temp>35 & temp<35.7)//overshoot smaller than 2%
						{
							GPIO_ResetBits(GPIOA, RED_os);//red off
							GPIO_ResetBits(GPIOA, YELLOW_os);//yellow off
							GPIO_SetBits(GPIOA, GREEN_os);//green overshoot led is on
						}
						else if(temp>38.5)//overshoot greater than 10%
						{
							GPIO_ResetBits(GPIOA, GREEN_os);//green off
							GPIO_ResetBits(GPIOA, YELLOW_os);//yellow off
							GPIO_SetBits(GPIOA, RED_os);//green overshoot led is on
						}
						else if(temp>35.7 & temp<38.5)//overshoot between 2%-10%
						{
							GPIO_ResetBits(GPIOA, RED_os);//red off
							GPIO_ResetBits(GPIOA, GREEN_os);//green off
							GPIO_SetBits(GPIOA, YELLOW_os);//yellow overshoot led is on
						}
					}	
					
				}
				if(reference<1365)//if reference value is decreased
					state=1;//go to the case 1
				if(reference>2730)//if reference value is increased
					state=3;//go to the case 3
				break;
			
			case 3://40 degree
				if(reference>2730)// third reference
				{
					GPIO_SetBits(GPIOA, RED_ref);//red reference led is on	
					GPIO_ResetBits(GPIOA, YELLOW_ref);//yellow is off
					GPIO_ResetBits(GPIOA, GREEN_ref);//green is off
					if(temp<37.5)//full power until difference is less than 2.5 degree
					{
						TIM2->CCR2=0;
					}
					if(temp>37.5)//zero power after 2.5 degree difference
					{
						TIM2->CCR2=36000;
						if(temp==39 | temp==38.5)
							TIM2->CCR2=18000;
						else if(temp>40 & temp<40.8)//overshoot smaller than 2%
						{
							GPIO_ResetBits(GPIOA, RED_os);//red off
							GPIO_ResetBits(GPIOA, YELLOW_os);//yellow off
							GPIO_SetBits(GPIOA, GREEN_os);//green overshoot led is on
						}
						else if(temp>44)//overshoot greater than 10%
						{
							GPIO_ResetBits(GPIOA, GREEN_os);//green off
							GPIO_ResetBits(GPIOA, YELLOW_os);//yellow off
							GPIO_SetBits(GPIOA, RED_os);//red overshoot led is on
						}
						else if(temp>40.8 & temp<44)//overshoot between 2%-10%
						{
							GPIO_ResetBits(GPIOA, RED_os);//red off
							GPIO_ResetBits(GPIOA, GREEN_os);//yellow off
							GPIO_SetBits(GPIOA, YELLOW_os);//yellow overshoot led is on
						}
					}	
				}	
				if(reference>1365 && reference<2730)//reference decreases
				state=2;//go back to the second case
				break;
				
			default:
				state=1;//at the beginning
		}
		
		temp=readTemperature();//takes the value from sensor and converts to temp value
    sprintf(adc,"%2.2f\r",(temp));//float data sent to PC 

		UART_Transmit(adc);//UART sends the temp value
				
		delayMs(200);//some delay
	}
	
}



