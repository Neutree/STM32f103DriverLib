
#include "Assert.h"

//#define DEBUG_USE_USART

#ifdef DEBUG_USE_USART
#include "USART.h"
extern USART log;
#define LOG(str) log<<str

#else
#define LOG(str)  
#endif

void assert_failed(uint8_t* file, uint32_t line)
{
	LOG("!!!!!assert fail!!!!!");
	LOG("\n==================\nfile:");
	LOG((const char*)file);
	LOG("\nline:");
	LOG((const char*)line);
	LOG("\n==================\n");
	while(1);
}
