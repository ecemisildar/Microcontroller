#include "stm32f10x.h"
#include "delay.h"
#include "conf.h"
#include "string.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>

#define LM75_REG_TEMP    0x00 // Temperature
#define LM75_REG_CONF    0x01 // Configuration
int a=0;
int16_t temp;//temperature
int bound=0;//instantenious pot value
int Sent_data=0;//data came from PC
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
		
	conf_GPIO();
	conf_ADC1();
	conf_UART1();
	READ_ADC();
	delayInit();
	
	while(1)
	{	
		a++;	
		bound=READ_ADC();//reads pot data
		temp=LM75_Temperature();//takes the value from sensor and converts to temp value
		Sent_data=USART_ReceiveData(USART1);//send temp data to PC via uart
    sprintf(adc,"%d\r",temp);//data sent from PC 
		//if(bound<temp & Sent_data=='1')//temp is bigger than threshold and I send 1 from PC
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
		if(bound>temp | Sent_data=='0')
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		UART_Transmit(adc);
			delayMs(1000);
		
	}
}
uint16_t LM75_ReadReg(uint8_t reg) {
	uint16_t value;
	
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
	value = (I2C_ReceiveData(I2C2) << 8); // Receive high byte
	I2C_AcknowledgeConfig(I2C2,DISABLE); // Disable I2C acknowledgment
	I2C_GenerateSTOP(I2C2,ENABLE); // Send STOP condition
	while (!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for EV7 (Byte received from slave)
	value |= I2C_ReceiveData(I2C2); // Receive low byte

	return value;
}


int16_t LM75_Temperature(void) {
	uint16_t raw;
	
	raw = LM75_ReadReg(LM75_REG_TEMP) >> 7;
		// Positive temperature
		temp = ((raw & 0xFE) >> 1) * 10 + (raw & 0x01) * 5;

	return temp*0.125;
}



