#include "conf.h"

void Config_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA 
	| RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//PWM GPIO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//Alternate push pull
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Configure pins (SDA->PB11, SCL->PB10) Sensor
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Configure input PA0 (Analog Input) 
	GPIO_InitStruct.GPIO_Pin = POT; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	//RED LED Reference
	GPIO_InitStruct.GPIO_Pin = RED_ref;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//YELLOW LED Reference
	GPIO_InitStruct.GPIO_Pin = YELLOW_ref;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GREEN LED Reference
	GPIO_InitStruct.GPIO_Pin = GREEN_ref;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	//RED Overshoot
	GPIO_InitStruct.GPIO_Pin = RED_os;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//General output
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//YELLOW Overshoot
	GPIO_InitStruct.GPIO_Pin = YELLOW_os;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//General output
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GREEN Overshoot
	GPIO_InitStruct.GPIO_Pin = GREEN_os;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//General output
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
}	

void Config_Adc(void)
{
	//ADC pin
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	ADC_InitTypeDef ADC_InitStructure;
	// Configure ADC mode (Independent | RegSimult | FastInterl | etc.) 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	// Enable/disable external conversion trigger (EXTI | TIM | etc.) 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	// Configure data alignment (Right | Left) 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	// Set the number of channels to be used and initialize ADC
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1); 
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1)); 
	// Start the conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}



void CONFIG_TIM2(void)
{
	int duty;
	TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// Configure time base generation
  //Timer Configurations for pwm	
	TIM_TimeBaseStructure.TIM_Period = 3599;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	//TIM_OCInitStructure.TIM_Pulse = 36000;//for pwm
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	
	
	TIM_Cmd(TIM2, ENABLE);//enable timer2
		
}
void CONFIG_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void CONFIG_UART1(void)
{
	USART_InitTypeDef USART_InitStructure;
//  USART_ClockInitTypeDef USART_ClockInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configue UART RX - UART module's TX should be connected to this pin
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Configue UART TX - UART module's RX should be connected to this pin
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void UART_Transmit(char *string)
{
	while(*string)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,*string++);
	}
}
int READ_ADC(void)
{
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
   while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
   return (float)(ADC_GetConversionValue(ADC1));//pot's value is now 0-90;
}


void CONFIG_I2C(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
while (I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)); // Wait until I2C free
}

float readTemperature(void)
{
	uint8_t LM75A_DataBuffer[2] = {0,0};
	uint16_t temperature;
	// Wait if busy
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	// Enable ACK
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	// Generate start condition
	I2C_GenerateSTART(I2C2, ENABLE);
	while (!I2C_GetFlagStatus(I2C2, I2C_FLAG_SB)); 
	// Send device address for write
	I2C_Send7bitAddress(I2C2, 0x91, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;
	// Write to device (temperature request)
	I2C_SendData(I2C2, 0x00);  
  while ((!I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(I2C2,I2C_FLAG_BTF)));
	// Generate start condition again (Re-start)
	I2C_GenerateSTART(I2C2, ENABLE);   
  while (!I2C_GetFlagStatus(I2C2,I2C_FLAG_SB));
	// Send device address for read this time
	I2C_Send7bitAddress(I2C2, 0x91, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED));
	LM75A_DataBuffer[0] = I2C_ReceiveData(I2C2);	
	// No ACK by master & Stop condition
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE); 	
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED));
	LM75A_DataBuffer[1] = I2C_ReceiveData(I2C2);	 
	// Calculate temp data
	temperature = (uint16_t)((LM75A_DataBuffer[0] << 8) | (LM75A_DataBuffer[1]));
	
	return ((float)(temperature >> 7) * 0.5);
}


uint16_t LM75_ReadReg(uint8_t reg) {

	uint8_t LM75A_DataBuffer[2] = {0,0};
	uint16_t temperature;
	I2C_AcknowledgeConfig(I2C2,ENABLE); // Enable I2C acknowledgment
	I2C_GenerateSTART(I2C2,ENABLE);
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
	I2C_Send7bitAddress(I2C2,0x90,I2C_Direction_Transmitter); // Send slave address
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // Wait for EV6
	I2C_SendData(I2C2,reg); // Send register address
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // Wait for EV8
	I2C_GenerateSTART(I2C2,ENABLE); // Send repeated START condition (aka Re-START)
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
	I2C_Send7bitAddress(I2C2,0x90,I2C_Direction_Receiver); // Send slave address for READ
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); // Wait for EV6
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for EV7 (Byte received from slave)
	//value = (I2C_ReceiveData(I2C2) << 8); // Receive high byte
	LM75A_DataBuffer[0] = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2,DISABLE); // Disable I2C acknowledgment
	I2C_GenerateSTOP(I2C2,ENABLE); // Send STOP condition
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for EV7 (Byte received from slave)
	//value |= I2C_ReceiveData(I2C2); // Receive low byte
	LM75A_DataBuffer[1] = I2C_ReceiveData(I2C2);
	 
	// Calculate temp data
	temperature = (uint16_t)((LM75A_DataBuffer[0] << 8) | (LM75A_DataBuffer[1]));
	
	return (float)((float)(temperature >> 7) * 0.5);

}



//sampling time
volatile double stepSize=0.2;
// Backward differentiation 
double Derivative(double ek, double ek_1) 
{
	return (ek - ek_1) / stepSize;
}
// Trapezoidal integration 
double Integral(double ek, double ek_1) 
{
	return (ek + ek_1) * (stepSize / 2);
}
// PID parameters 
volatile double Kp = 15, Ki = 0.8, Kd;//proportional and integrator coefficients
// PID Controller 
double PID(double r, double y)//r->reference(system input)  y->instantanious output
{
	static double ek_1 = 0, integralSum = 0; 
	double outputPID; 
	double ek = r - y;//error is difference between reference and output
	integralSum += Integral(ek, ek_1);//errors and old errors are summing 
	outputPID = Kp * ek + Ki * integralSum;//Kp times error and Ki times all errors 
	ek_1 = ek;//error is now old error
	return outputPID;//pid is ready 
}
