#include "USART.h"
#include "TaskManager.h"
#include "Socket_esp8266.h"
#include "LED.h"

USART com(2,115200,false);
USART debug(1,115200);
Socket_esp8266 wifi(com);

GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red

/*******************configuration****************/
char* mApJoinName  ="Neucrack";
char* mApJoinPasswd="15023490062...";
char* mServerIP    = "192.168.168.183";
uint16_t mServerPort  = 5080;
/***********************************************/

char dataToSend[100]="1234567890";

bool WiFiInit(void);

int main()
{
	TaskManager::DelayS(3);
	debug<<"start initialize...\n";
	if(!WiFiInit())
	{
		debug<<"WiFi initialize fail!!!\n";
		while(1)
			ledRed.Blink(0,200,false);
	}
	debug<<"initialize complete!\n";
	debug<<"write:"<<dataToSend<<"\n";
  wifi.Write(dataToSend,10);
	while(1)
	{
		if(com.ReceiveBufferSize()>7)
		{
			debug<<"received data\n";
			int size = wifi.Read(dataToSend);
			debug<<"size:"<<size<<"\n";;
			if(size > 0)
				wifi.Write(dataToSend,size);
			debug<<"data:";
			debug.SendData((uint8_t*)dataToSend,size);
			debug<<"\n";
		}
		ledRed.Blink(0,500,false);
	}
}

bool WiFiInit()
{
    wifi.Init();//初始化时一定要清除串口缓冲区，因为WiFi上电后回显很多调试信息，可能会造成串口缓冲区溢出，上电大于2秒后调用
    if(!wifi.Kick())
        return false;
    wifi.SetEcho(false);//关闭回响
    wifi.SetMode(esp8266::esp8266_MODE_STATION,esp8266::esp8266_PATTERN_DEF);//设置为station模式
    wifi.SetMUX(false);//单连接模式
    if(!wifi.JoinAP(mApJoinName,mApJoinPasswd,esp8266::esp8266_PATTERN_DEF))//加入AP
		{
			debug<<"join ap error!!!\n";
			return false;
		}
    TaskManager::DelayS(2);
    //连接服务器
    if(!wifi.Connect(mServerIP,mServerPort,Socket_Type_Stream,Socket_Protocol_IPV4))
        return false;
    return true;
}

