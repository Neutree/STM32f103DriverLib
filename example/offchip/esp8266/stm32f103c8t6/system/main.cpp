#include "USART.h"
#include "TaskManager.h"
#include "Socket_esp8266.h"
#include "LED.h"

USART com(1,115200,false);
Socket_esp8266 wifi(com);

GPIO ledRedGPIO(GPIOC,13,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red

/*******************configuration****************/
char* mApJoinName  ="lalala";
char* mApJoinPasswd="dd199511";
char* mServerIP    = "192.168.1.104";
uint16_t mServerPort  = 8989;
/***********************************************/

char dataToSend[10]={1,2,3,4,5,6,7,8,9,10};

bool WiFiInit(void);

int main()
{
    TaskManager::DelayS(3);
    if(!WiFiInit())
	{
		while(1)
			ledRed.Blink(0,0.2,false);
	}
    wifi.Write(dataToSend,10);
	while(1)
		ledRed.Blink(0,1,false);
}

bool WiFiInit()
{
    wifi.Init();//初始化时一定要清除串口缓冲区，因为WiFi上电后回显很多调试信息，可能会造成串口缓冲区溢出，上电大于2秒后调用
    if(!wifi.Kick())
        return false;
    wifi.SetEcho(false);//关闭回响
    wifi.SetMode(esp8266::esp8266_MODE_STATION,esp8266::esp8266_PATTERN_DEF);//设置为station模式
    wifi.SetMUX(false);//单连接模式
    wifi.JoinAP(mApJoinName,mApJoinPasswd,esp8266::esp8266_PATTERN_DEF);//加入AP
    TaskManager::DelayS(2);
    //连接服务器
    if(!wifi.Connect(mServerIP,mServerPort,Socket_Type_Stream,Socket_Protocol_IPV4))
        return false;
    return true;
}

