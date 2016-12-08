#include "GPS.h"
//#include "USART.h"
void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}

void RCC_HSE_Configuration(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	if(RCC_WaitForHSEStartUp() == SUCCESS){
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	 
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08); 
	}
}

int main(void){
//	USART com2(2,115200);
	u8 i = 0,j=0;
	SystemInit();
	GPS GPS1(USART2, 9600, false, 0x00, 1, 0, 1);
	GPS1.EnabledRealProcess();
//	GPS1.SETGPSUpdateTime(500);
	USART_Config();
	while(1){
		for(i = 0;i<255;i++)
			for(j = 0;j<255;j++)
				;
		GPS1.Update();
		printf("Longitude=%lf\r\n", GPS1.getLongitude());
		printf("Latitude=%lf\r\n", GPS1.getLatitude());
		printf("Speed=%lf\r\n", GPS1.getSpeed());
		printf("Course=%lf\r\n", GPS1.getCourse());
		printf("MSL_Al=%lf\r\n", GPS1.getMSL_Altitude());
		printf("PDOP=%lf\r\n", GPS1.getPDOP());
		printf("HDOP=%lf\r\n", GPS1.getHDOP());
		printf("VDOP=%lf\r\n", GPS1.getVDOP());
		printf("NS_Ind=%c\r\n", GPS1.getNS_Indicator());
		printf("EW_Ind=%c\r\n", GPS1.getEW_Indicator());
	}
}
extern "C"{
	int fputc(int ch,FILE *f)
	{
		USART_SendData(USART1,(u8)ch);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return ch;
	}
}

