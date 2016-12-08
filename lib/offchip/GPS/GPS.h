/**
	* GPS(USART_TypeDef *USART = USART2, uint32_t baud = 9600, bool useDMA = false,u8 remap = 0x00,u8 Prioritygroup = 1,
	*	uint8_t preemprionPriority = 0,uint8_t subPriority = 1,u8 dmaPriority = 3
	*	,uint16_t parity = USART_Parity_No,uint16_t wordLength = USART_WordLength_8b, uint16_t stopBits = USART_StopBits_1);
	* @brief example GPS gps (USART2, 9600, false, 0x00, 1, 0, 1);
	*	目前没有找到设置此GPS串口波特率的方法（或者程序有问题还没有解决），只能用9600波特率， 
	*	DMA中断服务函数还没有内容
*/
#ifndef _GPS_H

#define _GPS_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>

#define NOP               0xFF  		 // Define No Operation, might be used to read status register
#define GPS_BUF_LENGTH    128
#define Valid							0x01
#define Invalid 					0x00
#define GPS_DMA_TX_BUFFER_SIZE 20    //USART DMA BUFFER SIZE
extern u8 Recv_flag;

typedef struct _GPS_Real_buf{
	char data[256];            				 //GPS Data Buffer
	volatile unsigned short rx_pc;     //GPS Data Pointer
}Real_buf;

typedef struct _GPS_Information{
	unsigned char Real_Locate;  				//Real Locate Valid
	unsigned char Located_Status;       //Locate Valid 有效定位，无效定位
	unsigned char Locate_Mode;        	//Locate Mode A=自动手动2D/3D，M=手动2D/3D
	unsigned char Locate_Type;					//Locate Type 1=未定位，2=2D定位，3=3D定位
	char UTC_Time[7];                   //UTC TIme  hhmmss.sss(时分秒.毫秒)格式
	char UTC_Date[7];                   //UTC Date
	char Latitude[10];                  //Latitude  ddmm.mmmm(度分)格式(前面的0也将被传输)
	char NS_Indicator;                  //NS
	char Longitude[11];                 //Longtitude
	char EW_Indicator;                  //EW
	double Knot_Speed;                  //Knot_Speed 地面速率(000.0~999.9节，前面的0也将被传输)、
	double Speed;												//地面速率  (0000.0~1851.8公里/小时，前面的0也将被传输)
	double Course;                      //Course  以真北为参考基准，沿顺时针方向至航向的角度。
	double Magnetic_Course;							//Magnetic Course 以磁北为参考基准，沿顺时针方向至航向的角度。
	
	double PDOP;                        //PDOP综合位置精度因子（0.5 - 99.9）
	double HDOP;                        //HDOP水平精度因子（0.5 - 99.9）
	double VDOP;                        //VDOP垂直精度因子（0.5 - 99.9）
	
	double MSL_Altitude;                //High 海平面高度
	double Geoid_Separation;            //Geoid Separation大地水准面分离
	unsigned char Use_EPH_Sum;       		//Use_EPH_Sum
	unsigned char User_EPH[12];         //User_EPH   contain:Satellite Used1
	unsigned short EPH_State[12][4]; 		//EPH_State  contain:Satellite ID , Elevation , Azimuth , SNR (C/No)
}Information;

class GPS{
	private:
		unsigned char Real_Process_Enabled;
		unsigned char DH_id_sep[32];							//"," Index
		USART_TypeDef *GPS_USART; 								//GPIO_USART
		uint32_t GPS_USART_BaudRate;							//USART_BaudRate
		DMA_Channel_TypeDef* dmaChannelTx;				//DMA Channel
		u8 bufferTxDma[GPS_DMA_TX_BUFFER_SIZE];
		bool GPS_UseDma;
		uint32_t dmaTcFlagChannel;						//DMA Interrupt TC Flag
		uint32_t dmaGlFlagChannel;						//DMA Interrupt Flag
	public:
		Real_buf GPS_Real_buf;							 //GPS Buffer Struct
		
		void GPS_USART_Puts(char *Buffer);	
		void Creat_DH_Index( char* buffer );											//Creat "," Index
		char* Real_Process_DH( char* buffer, unsigned char num ); //Return Data after ","
		void Real_GPS_Command_Process( void );										//Data Process
		unsigned char Calc_GPS_Sum( const char* Buffer );					//Calculater GPS Data Sum

		GPS(USART_TypeDef *USART = USART2, uint32_t baud = 9600, bool useDMA = false,u8 remap = 0x00,u8 Prioritygroup = 1,uint8_t preemprionPriority = 0,uint8_t subPriority = 1,u8 dmaPriority = 3
		,uint16_t parity = USART_Parity_No,uint16_t wordLength = USART_WordLength_8b, uint16_t stopBits = USART_StopBits_1);
		
		void Update(void);
		Information GPS_Information;	//GPS Information Struct
		Information getInformation(void);
		void SETUSART_BaudRate(void);
		void SETGPSUpdateTime(uint16_t UpdateTime);
		void EnabledRealProcess(void);

		int getDay_Of_Month(void);
		int	getMonth(void);
		int getYear(void);
		int getHour(void);
		int getMinute(void);
		int getSecond(void);
		
		unsigned char getLocate_Mode(void);
		unsigned char getReal_Locate(void);
		int getLocate_Type(void);
		unsigned char getLocated_Status(void);
		
		char getNS_Indicator(void);
		char getEW_Indicator(void);
		double getLatitude(void);
		double getLongitude(void);
		double getSpeed(void);
		double getknotSpeed(void);
		double getCourse(void);
		double getPDOP(void);
		double getHDOP(void);
		double getVDOP(void);
		double getMSL_Altitude(void);
	
		void GPS_IRQHandler(void);
		void DMA_IRQHandler(void);
};

#endif
