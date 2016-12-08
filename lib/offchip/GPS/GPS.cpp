#include "GPS.h"
#include "stm32f10x_it.h"

u8 Recv_flag = 0;
char RecvBuffer[256] = {0};
u8 pc = 0;

/**
	* @brief GPS constructor 
	* @return None
	*/
void GPS::GPS_IRQHandler(void){
	unsigned char Recv;
  if ( USART_GetITStatus(GPS_USART,USART_IT_RXNE ) != RESET ){
    Recv = USART_ReceiveData(GPS_USART);  
    if ( Recv == '$' ){
      pc = 0;
    }
    else{
      if (pc < sizeof(RecvBuffer) - 1 ){
        pc++;
      }
    }
   RecvBuffer[pc] = Recv;
    if ( Recv == '\r' ) {
      if ( Real_Process_Enabled == Valid ){
        if ( Calc_GPS_Sum( RecvBuffer ) == Valid ){
					Recv_flag = 1;
					strcpy(GPS_Real_buf.data, RecvBuffer);
        }
      }
    }
  }

  if ( USART_GetFlagStatus( GPS_USART, USART_FLAG_ORE ) == SET ) {
    USART_ClearFlag( GPS_USART, USART_FLAG_ORE );
    USART_ReceiveData( GPS_USART );
  }
}

/**
 * @brief GPS DataUpdate
 * @return None
*/
void GPS::Update(void){
	if(Recv_flag){
		Recv_flag = 0;
		Creat_DH_Index(GPS_Real_buf.data );
    Real_GPS_Command_Process();
	}
}
/**
	* @brief GPS DMA Interrupt function
	* @return None
	*/
void GPS::DMA_IRQHandler(void){
	
}
/**
	* @brief GPS constructor (Real_Process_Enabled config invalid)
	* @return None
	*/
GPS::GPS(USART_TypeDef *USART, uint32_t baud, bool useDMA,u8 remap,u8 Prioritygroup,uint8_t preemprionPriority,uint8_t subPriority,u8 dmaPriority
	,uint16_t parity,uint16_t wordLength, uint16_t stopBits):GPS_USART(USART),GPS_UseDma(useDMA){
		GPIO_TypeDef *GPS_GPIO;
		uint16_t TxPin = 0, RxPin = 0;
		uint8_t usartIrqChannel, dmaIrqChannel;//Interrupt Channel , Serial Send Dma Channel
		Real_Process_Enabled = Invalid;					//Real_Process_Disabled 
		GPS_USART_BaudRate = baud;							//GPS_USART_BaudRate(baud)
		SETUSART_BaudRate();										//set gps_usart_baudrate
	if(GPS_USART == USART1){
		pGPS1 = this;
		usartIrqChannel=USART1_IRQn;
		if(GPS_UseDma)
		{
			dmaChannelTx=DMA1_Channel4;
			dmaIrqChannel=DMA1_Channel4_IRQn;
			dmaTcFlagChannel=DMA1_FLAG_TC4;
			dmaGlFlagChannel=DMA1_IT_GL4;
		}
		
		if(remap == 0x01){
			TxPin = GPIO_Pin_6;
			RxPin = GPIO_Pin_7;
			GPS_GPIO=GPIOB;
			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		}else{
			TxPin = GPIO_Pin_9;
			RxPin = GPIO_Pin_10;
			GPS_GPIO = GPIOA;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);
		}
		
	}else if(GPS_USART == USART2){
		pGPS2 = this;
		usartIrqChannel=USART2_IRQn;
		if(GPS_UseDma)
		{
			dmaChannelTx=DMA1_Channel7;
			dmaIrqChannel=DMA1_Channel7_IRQn;
			dmaTcFlagChannel=DMA1_FLAG_TC7;
			dmaGlFlagChannel=DMA1_IT_GL7;
		}
		
		if(remap == 0x01){
			TxPin = GPIO_Pin_5;
			RxPin = GPIO_Pin_6;
			GPS_GPIO=GPIOD;
			GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
		}else{
			TxPin = GPIO_Pin_2;
			RxPin = GPIO_Pin_3;
			GPS_GPIO = GPIOA;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		}
		
	}else if(GPS_USART == USART3){
		pGPS3 = this;
		usartIrqChannel=USART3_IRQn;
		if(GPS_UseDma)
		{
			dmaChannelTx=DMA1_Channel2;
			dmaIrqChannel=DMA1_Channel2_IRQn;
			dmaTcFlagChannel=DMA1_FLAG_TC2;
			dmaGlFlagChannel=DMA1_IT_GL2;
		}
		if(remap == 0x01){
			TxPin = GPIO_Pin_10;
			RxPin = GPIO_Pin_11;
			GPS_GPIO=GPIOC;
			GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
		}else if(remap == 0x11){
			TxPin = GPIO_Pin_8;
			RxPin = GPIO_Pin_9;
			GPS_GPIO=GPIOD;
			GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
		}else {
			TxPin = GPIO_Pin_10;
			RxPin = GPIO_Pin_11;
			GPS_GPIO = GPIOB;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		}
		
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPS_GPIO,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = RxPin;
	GPIO_Init(GPS_GPIO,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = parity;
	USART_InitStructure.USART_StopBits = stopBits;
	USART_InitStructure.USART_WordLength = wordLength;
	USART_Init(GPS_USART,&USART_InitStructure);
	USART_ITConfig(GPS_USART, USART_IT_RXNE, ENABLE);
	USART_Cmd(GPS_USART,ENABLE);
	USART_GetFlagStatus(GPS_USART,USART_FLAG_TC);
	
	//Enable translate DMA
	if(GPS_UseDma)
	{
		USART_DMACmd(GPS_USART, USART_DMAReq_Tx, ENABLE);
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);/*Start Dma Clock*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPS_USART->DR;	//设置DMA源：串口数据寄存器地址   
		//内存地址(要传输的变量的指针)
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)bufferTxDma;			/*dmaChannelTx->CMAR =  (u32)bufferTxDma;*/
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					//方向：从内存到外设
		DMA_InitStructure.DMA_BufferSize = 0;								//传输大小DMA_BufferSize=SENDBUFF_SIZE
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	//外设地址不增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//内存地址自增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据单位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	//内存数据单位 8bit
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;						//DMA mode：not circle
		
		//DMA Priority 
		switch(dmaPriority){
			case 0:
				DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
				break;
			case 1:
				DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
				break;
			case 2:
				DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
				break;
			default:
				DMA_InitStructure.DMA_Priority = DMA_Priority_Low;  
				break;
		} 

		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//forbid M2M transmission
		DMA_Init(dmaChannelTx, &DMA_InitStructure);//configuration DMA1 TX channel
		DMA_Cmd (dmaChannelTx,DISABLE);					//enable dma
		/* DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //configuration DMA transmission complete interrupt */
	}
	else  //use interrupt way to send data
	{
		USART_ITConfig(GPS_USART, USART_IT_TC, DISABLE);//enable the transport complete interrupt
		//USART_GetFlagStatus(usart, USART_FLAG_TC);// clear transmission complete flag
	}
	
	NVIC_InitTypeDef NVIC_InitStructure;
	switch(Prioritygroup)
	{
		case 0:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
			break;
		case 1:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
			break;
		case 2:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			break;
		default:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
			break;
		case 4:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
			break;
	}
	
	NVIC_InitStructure.NVIC_IRQChannel = usartIrqChannel;//configuration usart interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//interrupt start
	NVIC_Init(&NVIC_InitStructure);//interrupt initialization
	
	if(GPS_UseDma)
	{
		NVIC_InitStructure.NVIC_IRQChannel = dmaIrqChannel ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
		NVIC_Init(&NVIC_InitStructure);
		/* Enable DMA TX Channel TCIT  */
		dmaChannelTx->CCR |= DMA_IT_TC;  // Start transmission complete interrupt
		/* Enable DMA TX Channel TEIT  */
		dmaChannelTx->CCR |= DMA_IT_TE; // Start Error Interrupt
		/* Enable DMA TX Channel HTIT  */
		/*DMA1_Channel4->CCR &= ~DMA_IT_HT;*/
	}
}
/**
  *@param  Creat every ","  Index 
	*@param  Svae DH_id_sep[]
	*@return void
	*/
void GPS::Creat_DH_Index( char* buffer ){
	
	unsigned short i, len;
	unsigned char idj;

	memset( DH_id_sep, 0, sizeof( DH_id_sep ) );
	len = strlen( buffer );
	for ( i = 0, idj = 0; i < len; i++ ){
		if ( buffer[i] == ',' ){
			DH_id_sep[idj] = i;
			idj++;
			buffer[i] = 0x00;
		}
	}
}
/**
 * @brife GetInformation
 * @return Information
*/
Information GPS::getInformation(void){
	return GPS_Information;
}
/**	
	*@param  Return content after num "," 
	*@param  
	*@return char *
	*/
char* GPS::Real_Process_DH( char* buffer, unsigned char num ){
	if ( num < 1 )
		return  &buffer[0];
	return  &buffer[ DH_id_sep[num - 1] + 1];
}
/**	
	*@param  GPS_Usart_Buffer Provess 
	*@param  Save to Struct GPS_Information
	*@return void
	*/
void GPS::Real_GPS_Command_Process( void ){
	char* temp;
	unsigned char i, j, zhen;

	if ( strstr( GPS_Real_buf.data, "GGA" ) ){//$GPGGA,112118.000,3743.5044,N,11540.5393,E,1,06,1.6,15.3,M,-9.1,M,,0000*7E
		//************************UTC_Time***********************//
		temp = Real_Process_DH( GPS_Real_buf.data, 1 );
		if ( *temp != 0 )
			memcpy( GPS_Information.UTC_Time, temp, 6 );
		//****************Latitude***********************//
		temp = Real_Process_DH( GPS_Real_buf.data, 2 );
		if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
			memcpy( GPS_Information.Latitude, temp, 9 );
			GPS_Information.Latitude[9] = 0;
		} else {
			GPS_Information.Latitude[0] = '0';
			GPS_Information.Latitude[1] = 0;
		}
		//*****************N/S Indicator*********************************//
		GPS_Information.NS_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 3 ) );
		//******************Longitude************************************//
		temp = Real_Process_DH( GPS_Real_buf.data, 4 );
		if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
			memcpy( GPS_Information.Longitude, temp, 10 );
			GPS_Information.Longitude[10] = 0;
		} else {
			GPS_Information.Longitude[0] = '0';
			GPS_Information.Longitude[1] = 0;
		}
		//*****************E/W Indicator********************************************//
		GPS_Information.EW_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 5 ) );
		GPS_Information.Use_EPH_Sum = atof( Real_Process_DH( GPS_Real_buf.data, 7 ) );
		GPS_Information.HDOP = atof( Real_Process_DH( GPS_Real_buf.data, 8 ) );
		GPS_Information.MSL_Altitude = atof( Real_Process_DH( GPS_Real_buf.data, 9 ) );      //海平面高度 
		GPS_Information.Geoid_Separation = atof( Real_Process_DH( GPS_Real_buf.data, 11 ) ); //Geoid Separation 大地水准面分离
		return;
	}

	if ( strstr( GPS_Real_buf.data, "GSA" ) ) { //$GPGSA,A,3,28,17,11,09,08,07,,,,,,,3.4,1.6,3.0*3B
		GPS_Information.Locate_Mode = * Real_Process_DH( GPS_Real_buf.data, 2 ); 
		temp = Real_Process_DH( GPS_Real_buf.data, 2 ); 
		if ( ( *temp == '2' ) || ( *temp == '3' ) )
			GPS_Information.Locate_Type = *temp;
		else
			GPS_Information.Locate_Type = Invalid;
		for ( i = 0; i < 12; i++ ){
			GPS_Information.User_EPH[i] = atof( Real_Process_DH( GPS_Real_buf.data, i + 3 ) ); 
		}
		GPS_Information.PDOP = atof( Real_Process_DH( GPS_Real_buf.data, 15 ) );
		GPS_Information.HDOP = atof( Real_Process_DH( GPS_Real_buf.data, 16 ) );
		GPS_Information.VDOP = atof( Real_Process_DH( GPS_Real_buf.data, 17 ) );
		return;
	}

	if ( strstr( GPS_Real_buf.data, "RMC" ) ){//$GPRMC,112118.000,A,3743.5044,N,11540.5393,E,0.25,198.81,130613,,,A*67
		//*************************************UTC_Time********************************//
		temp = Real_Process_DH( GPS_Real_buf.data, 1 );
		if ( *temp != 0 )
			memcpy( GPS_Information.UTC_Time, temp, 6 );
		//***********************************Status**************///
		if ( *( Real_Process_DH( GPS_Real_buf.data, 2 ) ) == 'A' ) {
			GPS_Information.Real_Locate = Valid;
			GPS_Information.Located_Status = Valid;
		} else {
			GPS_Information.Real_Locate = Invalid;
		}
		//**********************************Latitude*************************//
		temp = Real_Process_DH( GPS_Real_buf.data, 3 );
		if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
			memcpy( GPS_Information.Latitude, temp, 9 );
			GPS_Information.Latitude[9] = 0;
		} else {
			GPS_Information.Latitude[0] = '0';
			GPS_Information.Latitude[1] = 0;
		}
		GPS_Information.NS_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 4 ) );
		//********************************Longitude********************//
		temp = Real_Process_DH( GPS_Real_buf.data, 5 );
		if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
			memcpy( GPS_Information.Longitude, temp, 10 );
			GPS_Information.Longitude[10] = 0;
		} else {
			GPS_Information.Longitude[0] = '0';
			GPS_Information.Longitude[1] = 0;
		}
		GPS_Information.EW_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 6 ) );
		GPS_Information.Speed = atof( Real_Process_DH( GPS_Real_buf.data, 7 ) );
		GPS_Information.Course = atof( Real_Process_DH( GPS_Real_buf.data, 8 ) );
		//**************************UTC_Date***************//
		temp = Real_Process_DH( GPS_Real_buf.data, 9 );
		if ( *temp != 0 ){
			memcpy( GPS_Information.UTC_Date, temp, 6 );
		}
		//Mode A=Autonomous, D=DGPS, E=DR ( Real_Process_DH( GPS_Real_buf.data, 6 ) );              //Only apply to NMEA version 2.3 (and later) in this NMEA message description.
		return;
	}

	if ( strstr( GPS_Real_buf.data, "GSV" ) ){//$GPGSV,3,1,11,28,73,321,32,17,39,289,43,11,38,053,17,09,37,250,41*78
		// 1,Number of Messages 2 , Message Number
		zhen = atof( Real_Process_DH( GPS_Real_buf.data, 2 ) );
		if ( ( zhen <= 3 ) && ( zhen != 0 ) ){
			for ( i = ( zhen - 1 ) * 4, j = 4; i < zhen * 4; i++ ){
				GPS_Information.EPH_State[i][0] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) );//Satellite ID
				GPS_Information.EPH_State[i][1] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) );//Elevation
				GPS_Information.EPH_State[i][2] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) );//Azimuth
				GPS_Information.EPH_State[i][3] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) );//SNR (C/No) dBHz
			}
		}
		return;
	}
	if(strstr( GPS_Real_buf.data, "VTG" ) ){
		GPS_Information.Course = atof( Real_Process_DH( GPS_Real_buf.data, 1 ) );//True Course
		GPS_Information.Magnetic_Course = atof( Real_Process_DH( GPS_Real_buf.data, 3 ) );//Magnetic Course
		GPS_Information.Knot_Speed = atof( Real_Process_DH( GPS_Real_buf.data, 5 ) );//knots
		GPS_Information.Speed = atof( Real_Process_DH( GPS_Real_buf.data, 6 ) );//km/hr
		return;
	}
}
/**	
	*@param  Calulater GPS_USART_Buffer Sum
	*@param  
	*@return GPS_USART_BUFFER_Sum isValid
	*/
unsigned char GPS::Calc_GPS_Sum( const char* Buffer ){
	unsigned char i, j, k, sum;
	sum = 0;

	for ( i = 1; i < 255; i++ ) {
		if ( ( Buffer[i] != '*' ) && ( Buffer[i] != 0x00 ) ) {
			sum ^= Buffer[i];
		}
		else{
			break;
		}
	}
	j = Buffer[i + 1];
	k = Buffer[i + 2];
	if ( isalpha( j ) ) {
		if ( isupper( j ) ) {
			j -= 0x37;
		}
		else{
			j -= 0x57;
		}
	}
	else{
		if ( ( j >= 0x30 ) && ( j <= 0x39 ) ){
			j -= 0x30;
		}
	}

	if ( isalpha( k ) ) {
		if ( isupper( k ) ) {
			k -= 0x37;
		}
		else{
			k -= 0x57;
		}
	}
	else{
		if ( ( k >= 0x30 ) && ( k <= 0x39 ) ){
			k -= 0x30;
		}
	}

	j = ( j << 4 ) + k; 

	if ( sum == j ){
		return Valid; 
	}
	else{
		return Invalid; 
	}
}
/**
	* @brief enable real process 
	* @return none
	*/
void GPS::EnabledRealProcess(void){
	if(Real_Process_Enabled == Invalid)
		Real_Process_Enabled = Valid;
}
/**
	* @brief Send Buffer to GPS
	* @param Set baudRate
*/
void GPS::GPS_USART_Puts(char *Buffer){
	while(*Buffer != '\0'){	
		USART_SendData(GPS_USART,*Buffer++);
//	USART_SendData(GPS_USART, 0x12);
		while(USART_GetFlagStatus(GPS_USART,USART_FLAG_TXE) == RESET);
	}
}
/**
	* @brief Set USART_BaudRate
	* @return None
*/
void GPS::SETUSART_BaudRate(void){
	USART_ITConfig(GPS_USART, USART_IT_RXNE, DISABLE);
	char SetBaudRateCommand[15];
	switch(GPS_USART_BaudRate){
		case 4800:
			memcpy(SetBaudRateCommand, "$PCAS01,0*1C", 12);
			break;
		case 9600:
			memcpy(SetBaudRateCommand, "$PCAS01,1*1D", 12);
			break;
		case 19200:
			memcpy(SetBaudRateCommand, "$PCAS01,2*1E", 12);
			break;
		case 38400:
			memcpy(SetBaudRateCommand, "$PCAS01,3*1F", 12);
			break;
		case 57600:
			memcpy(SetBaudRateCommand, "$PCAS01,4*18", 12);
			break;
		case 115200:
			memcpy(SetBaudRateCommand, "$PCAS01,5*19", 12);
			break;
	}
	SetBaudRateCommand[12] = '\r';
	SetBaudRateCommand[13] = '\n';
	SetBaudRateCommand[14] = '\0';
//	GPS_USART_Puts(SetBaudRateCommand);								//还有问题，不能将数据发送出，到GPS模块
	USART_ITConfig(GPS_USART, USART_IT_RXNE, ENABLE);
}
/**
	* @brief Set GPSUPDATETIME
	* @return None
*/
void GPS::SETGPSUpdateTime(uint16_t UpdateTime){
	USART_ITConfig(GPS_USART, USART_IT_RXNE, DISABLE);
	char SetUpdateTimeCommand[19];
	switch(UpdateTime){
		case 500:
			memcpy(SetUpdateTimeCommand, "$PCAS02,500*1A", 14);
			SetUpdateTimeCommand[15] = '\r';
			SetUpdateTimeCommand[16] = '\n';
			SetUpdateTimeCommand[17] = '\0';
			break;
		case 1000:
			memcpy(SetUpdateTimeCommand, "$PCAS02,1000*2E", 15);
			SetUpdateTimeCommand[16] = '\r';
			SetUpdateTimeCommand[17] = '\n';
			SetUpdateTimeCommand[18] = '\0';
			break;
		case 2000:
			memcpy(SetUpdateTimeCommand, "$PCAS02,2000*2D", 15);
			SetUpdateTimeCommand[16] = '\r';
			SetUpdateTimeCommand[17] = '\n';
			SetUpdateTimeCommand[18] = '\0';
			break;
		case 3000:
			memcpy(SetUpdateTimeCommand, "$PCAS02,3000*2C", 15);
			SetUpdateTimeCommand[16] = '\r';
			SetUpdateTimeCommand[17] = '\n';
			SetUpdateTimeCommand[18] = '\0';
			break;
	}
//	GPS_USART_Puts(SetUpdateTimeCommand);								//还有问题，不能将数据发送出，到GPS模块
	USART_ITConfig(GPS_USART, USART_IT_RXNE, ENABLE);
}
/**
	* @brief return Locate_Mode
	* @param Attention: A=自动手动2D/3D，M=手动2D/3D
	* @return int
	*/
unsigned char GPS::getLocate_Mode(void){
	return GPS_Information.Locate_Mode;
}
/**
	* @brief return Located_Status
	* @param Attention: Invalid  Valid
	* @return int
	*/
unsigned char GPS::getLocated_Status(void){
	return GPS_Information.Located_Status;
}
/**
	* @brief return Locate_Type
	* @param Attention: "1" ->Invalid  "2"->2D  "3"->3D
	* @return int
	*/
int GPS::getLocate_Type(void){
	if(GPS_Information.Locate_Type == '2')
		return 2;
	else if(GPS_Information.Locate_Type == '3')
		return 3;
	else 
		return 1;
}
/**
	* @brief return Real_Locate Status
	* @param Attention: "1" ->Invalid  "2"->2D  "3"->3D
	* @return int
	*/
unsigned char GPS::getReal_Locate(void){
	return GPS_Information.Real_Locate;
}
/**	
	*@param  Return Day of Month
	*@param  
	*@return uint16_t Day of Month
	*/
int GPS::getDay_Of_Month(){
	char DayofMonth[2];
	DayofMonth[0] = GPS_Information.UTC_Date[0];
	DayofMonth[1] = GPS_Information.UTC_Date[1];
	return atoi(DayofMonth);
}
/**	
	*@param  Return Month
	*@param  
	*@return uint16_t Month
	*/
int GPS::getMonth(){
	char Month[2];
	Month[0] = GPS_Information.UTC_Date[2];
	Month[1] = GPS_Information.UTC_Date[3];
	return atoi(Month);
}
/**	
	*@param  Return Year 
	*@param  
	*@return int Year
	*/
int GPS::getYear(void){
	char Year[2];
	Year[0] = GPS_Information.UTC_Date[4];
	Year[1] = GPS_Information.UTC_Date[5];
	return atoi(Year);
}
/**	
	*@param  Return Hour 
	*@param  
	*@return int Hour
	*/
int GPS::getHour(void){
	char Hour[2];
	Hour[0] = GPS_Information.UTC_Time[0];
	Hour[1] = GPS_Information.UTC_Time[1];
	return atoi(Hour);
}
/**	
	*@param  Return Minute 
	*@param  
	*@return int Minute
	*/
int GPS::getMinute(void){
	char Minute[2];
	Minute[0] = GPS_Information.UTC_Time[2];
	Minute[1] = GPS_Information.UTC_Time[3];
	return atoi(Minute);
}
/**	
	*@param  Return Second 
	*@param  
	*@return int Second
	*/
int GPS::getSecond(void){
	char Second[2];
	Second[0] = GPS_Information.UTC_Time[4];
	Second[1] = GPS_Information.UTC_Time[5];
	return atoi(Second);
}
/**	
	*@param  Return NS 
	*@param  
	*@return char NS
	*/
char GPS::getNS_Indicator(){
	return GPS_Information.NS_Indicator;
}
/**	
	*@param  Return EW 
	*@return char EW
	*/
char GPS::getEW_Indicator(){
	return GPS_Information.EW_Indicator;
}
/**	
	*@param  Return Latitude 
	*@return double Latitude
	*/
double GPS::getLatitude(){
	return atof(GPS_Information.Latitude);
}
/**	
	*@param  Get Longitude 
	*@return double Longitude
	*/
double GPS::getLongitude(){
	return atof(GPS_Information.Longitude);
}
/**	
	*@param  Get Speed Knot
	*@return double Speed
	*/
double GPS::getknotSpeed(void){
	return GPS_Information.Knot_Speed;
}
/**	
	*@param  Get Speed 
	*@return double Speed
	*/
double GPS::getSpeed(){
	return GPS_Information.Speed;
}
/**	
	*@param  Get Course 
	*@return double Course
	*/
double GPS::getCourse(){
	return GPS_Information.Course;
}
/**	
	*@param  Get PDOP 
	*@return double PDOP
	*/
double GPS::getPDOP(){
	return GPS_Information.PDOP;
}
/**	
	*@param  Get HDOP 
	*@return double HDOP
	*/
double GPS::getHDOP(){
	return GPS_Information.HDOP;
}
/**	
	*@param  Get VDOP 
	*@return double VDOP
	*/
double GPS::getVDOP(){
	return GPS_Information.VDOP;
}
/**	
	*@param  Get MSL_Altitude 
	*@return double MSL_Altitude
	*/
double GPS::getMSL_Altitude(){
	return GPS_Information.MSL_Altitude;
}

