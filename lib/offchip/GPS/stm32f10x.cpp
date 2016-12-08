#include "stm32f10x_it.h"

GPS *pGPS1 = 0;
GPS *pGPS2 = 0;
GPS *pGPS3 = 0;
void USART1_IRQHandler(void){
	if(pGPS1)
		pGPS1->GPS_IRQHandler();
}

void USART2_IRQHandler(void){
	if(pGPS2)
		pGPS2->GPS_IRQHandler();
}

void USART3_IRQHandler(void){
	if(pGPS3)
		pGPS3->GPS_IRQHandler();
}

void DMA1_Channel1_IRQHandler(void){

}

void DMA1_Channel2_IRQHandler(void){

}

void DMA1_Channel3_IRQHandler(void){

}

void DMA1_Channel4_IRQHandler(void){

}

void DMA1_Channel5_IRQHandler(void){

}

void DMA1_Channel6_IRQHandler(void){

}

void DMA1_Channel7_IRQHandler(void){

}
