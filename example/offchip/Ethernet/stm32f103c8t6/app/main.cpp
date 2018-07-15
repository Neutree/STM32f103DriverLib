#include "USART.h"
#include "socket.h"
#include "TaskManager.h"
#include "w5500.h"

USART log(1,115200,false,0,3,7,1,3);//
uint8_t dataSend[9]={"abcdefg\n"};
u8 hexData[5]={0x12,0x56,0x9A,0xAB,0xEF};
u8 dataReceived[50];

uint8_t sock = 0;
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};
uint8_t local_ip[] = {192,168,31,236};
uint8_t gateway_ip[] = {192,168,31,1};
uint8_t subnetMask[] = {255,255,255,0};

uint16_t local_port = 5000;
uint8_t server_ip[4] = {192,168,31,86};
uint16_t server_port = 6000;
bool flag = false;

uint8_t temp[6];
int main()
{
	log<<"system start, initilize now\n";
	w5500.init();
	w5500.setMACAddress(mac);
	w5500.setIPAddress(local_ip);
	w5500.setGatewayIp(gateway_ip);
	w5500.setSubnetMask(subnetMask);
	log<<"init complete\n";
	
	w5500.getMACAddress(temp);
	log<<"mac:";
	log.PrintHex(temp,6,":")<<"\n";
	
	w5500.getGatewayIp(temp);
	log<<"gateway ip:";
	log.PrintHex(temp,4,".")<<"\n";
	
	w5500.getIPAddress(temp);
	log<<"local ip:";
	log.PrintHex(temp,4,".")<<"\n";
	
	uint8_t ret = socket(sock,SnMR::TCP,local_port,0);
	if(ret == 0)
	{
		log<<"socket fail\n";
	}
	else
	{
		log<<"socket success\n";
		flag = true;
	}
	if(flag)
	{
		ret = connect(sock,(uint8_t*)server_ip,server_port);
		if(ret)
		{
			log<<"connect success\n";
			flag =true;
			while (socketStatus(sock) != SnSR::ESTABLISHED) {
				TaskManager::DelayMs(1);
				if (socketStatus(sock) == SnSR::CLOSED) {
					flag = false;
					break;
				}
			}

		}
		else
		{
			log<<"connect fail\n";
			flag = false;
		}
		
	}
	if(flag)
	{
		log<<"connect server success\n";
	}
	else
	{
		log<<"connect server fail\n";
	}
	while(1)
	{
		if(flag)
		{
			send(sock,(uint8_t*)dataSend,sizeof(dataSend));
			
		}
		TaskManager::DelayS(2);
	}
	
}

