#include "conf.h"
#include "delay.h"
// Step size to solve ODE 
double stepSize = 0.1; 
// System input (step) to be sent if exti triggered 
double inputToSystem = 0; 
// System output to be sent PC
double static outputToPC = 0;


// Difference Equation double 
double Gz(double uk)
{
	static double yk_1=0, yk_2=0, uk_1=0, uk_2=0;
	double yk = 1.8221*yk_1-0.8262*yk_2+0.0021*uk_1+0.0020*uk_2;//coefficient are obtained from matlab
		uk_2 = uk_1; 
		uk_1 = uk; 
		yk_2 = yk_1; 
		yk_1 = yk;
	return yk;
}
                                                 

char data[4];

int main()
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
	| RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);//gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);//usart
	
	CONFIG_TIM2();
	CONFIG_NVIC();
  CONFIG_UART1();
  Config_INTERRUPT();

	delayInit();
	while(1)
	{				
		sprintf(data, "%2.3f\r", (float)(outputToPC));//Uart sends the data to pc
		UART_Transmit(data);
		delayMs(200);//some delay	
	}
}
// Trapezoidal integration 
double Integral(double ek, double ek_1) 
{
	return (ek + ek_1) * (stepSize / 2);//area calculation
}

// PID parameters 
volatile double Kp = 2.8, Ki = 0.8;//proportional and integrator coefficients
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

// Timer 2 IRQ 
void TIM2_IRQHandler(void)//10Hz timer
{
static double y1=0; // Initial value
if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // Periodic IRQ 
	{
		//y1=Gz(inputToSystem);//open loop system
		y1 = Gz(PID(inputToSystem,y1));//controller+system
		
		outputToPC = y1;//send that data to pc
			
		// Clear interrupt flag 		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
	}
}

int press=0;

void EXTI4_IRQHandler(void)//interrupt function
{
if(EXTI_GetITStatus(EXTI_Line4) != RESET)//when pressed the button
{
	press++;//counts the number of presses
	if(press%2==1)
		inputToSystem = 1.00;//even numbers
	else
		inputToSystem = 0;//odd numbers
	EXTI_ClearITPendingBit(EXTI_Line4); //Clear interrupt flag
}
}



	
	
