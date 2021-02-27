#include "stm32f10x.h"
#include "math.h"
# define PI 3.14

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
ADC_InitTypeDef ADC_InitStructure;

//defining the functions 
void conf_GPIO(void);
void conf_TIM2(void);
void conf_ADC1(void);
void TIM3_IRQHandler(void);

//defining the values
static double b1 = 0;
static double b2 = 0;
static double b3 = 0;
static double b4 = 0;
static double freq = 0;
//time value
static int t = 0; 

int main(void)
{	
	
	//enabling peripherals
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
	| RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//calling funtions
	conf_GPIO();
	conf_ADC1();
	conf_TIM2();
	
	while(1)
	{
		//duty cycles
		b1=(sin(2*PI*freq*t-1.5))/2;
		b2=(sin(2*PI*freq*t-0.5))/2;
		b3=(sin(2*PI*freq*t+0.5))/2;
		b4=(sin(2*PI*freq*t+1.5))/2;
		
		freq=(int)(ADC_GetConversionValue(ADC1)*0.01);//reads the potentiometer
		t++;//changes the time
		
		//writes the duty cycle values to the LED pins
		TIM2->CCR1 = b1*10000;
		TIM2->CCR2 = b2*10000;
		TIM2->CCR3 = b3*10000;
		TIM2->CCR4 = b4*10000;
			
	}
}
//LEDs and Potantiometer configurations
void conf_GPIO(void)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//pot as an analog input
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//I used PA_15 because the timer_2 have 4 channels
	//and my PA_0 pin is used for potentiometer. So, I remap the channel_1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//LED as an alternate function
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE );//remapping
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//I used PB_3 and I remap the timer_2 second channel
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE );//remapping
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PA_2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA_3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void conf_TIM2(void)
{
	//5ms timer
	TIM_TimeBaseStructure.TIM_Period = 35999;
	TIM_TimeBaseStructure.TIM_Prescaler = 9;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//PWM mode
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = freq;//pulse values are changing depends on the potentiometer
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//OC1->ch1
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = freq;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//OC2->ch2
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = freq;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);//OC3->ch3
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = freq;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);//OC4->ch4
	TIM_Cmd(TIM2, ENABLE);
}

//ADC configuration
void conf_ADC1(void)
{
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


