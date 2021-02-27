#include "config.h"

//exti interrupt
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//timer
TIM_TimeBaseInitTypeDef
TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
//adc
ADC_InitTypeDef ADC_InitStructure;
//usart
USART_InitTypeDef USART_InitStructure;

void conf_LED(void)
{
	//RED 
	GPIO_InitStructure.GPIO_Pin = RED;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//YELLOW
	GPIO_InitStructure.GPIO_Pin = YELLOW;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE );//remapping
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GREEN
	GPIO_InitStructure.GPIO_Pin = GREEN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE );//remapping
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void conf_INTERRUPT(void)
{
	//BUTTON 1
	GPIO_InitStructure.GPIO_Pin = BUTTON_1;//PA_1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//input
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//BUTTON 2
	GPIO_InitStructure.GPIO_Pin = BUTTON_2;//PA_4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//input
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//INTERRUPT 1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//PA_1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//counts rising and falling edges of the pulses
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//Configure nested vector interrupt controller
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	//INTERRUPT 2 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//PA_4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//Configure nested vector interrupt controller
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void conf_PWM(void)
{
	//Timer Configurations for pwm	
	//72.000.000/(1000*72)=1000Hz
	//ideal brightness for the led 
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//three pwm timers for the 3 leds
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	//timer for the uart comm	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 
	TIM_OCInitStructure.TIM_Pulse = 1; 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		
	TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	//Configure nested vector timer controller
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//ADC configuration
void conf_ADC1(void)
{
	GPIO_InitStructure.GPIO_Pin = POT; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//pot as an analog input
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1); 
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1)); 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}

void conf_UART1(void)
{
	// Configue UART RX - UART module's TX should be connected to this pin
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Configue UART TX - UART module's RX should be connected to this pin
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	// USART settings 
	USART_InitStructure.USART_BaudRate = 9600; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(USART1, &USART_InitStructure); 
	// Enable data receive interrupt & USART1 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART_Transmit(char *string)
{
	while(*string)
	{
		while(!(USART1->SR & 0x00000040));//if uart is available
		USART_SendData(USART1,*string);//send data
		*string++;
	}
}