#include "USART.h"
#include "Flash.h"

USART com(1,115200);
flash store(0x08000000+63*MEMORY_PAGE_SIZE,true);

typedef struct {
	int a;
	int b;
	float c;
	double d;
}data_t;
data_t dataToStore={
	.a=10,
	.b=20,
	.c=12.3,
	.d=12.45
},dataToStore2;

int main()
{
	com<<"init complete\n";
	com<<"clear flash block\n";
	store.Clear(0);
	
	store.Read(0,0,(u16*)&dataToStore2,sizeof(dataToStore));
	com<<"value before write:a:"<<dataToStore2.a<<"\tb:"<<dataToStore2.b<<"\tc:"<<dataToStore2.c<<"\td:"<<dataToStore2.d<<"\n";
	
	com<<"write value to flash:a:"<<dataToStore.a<<"\tb:"<<dataToStore.b<<"\tc:"<<dataToStore.c<<"\td:"<<dataToStore.d<<"\n";
	store.Write(0,0,((u16*)&dataToStore),sizeof(dataToStore));
	
	store.Read(0,0,(u16*)&dataToStore2,sizeof(dataToStore));
	com<<"value after write:a:"<<dataToStore2.a<<"\tb:"<<dataToStore2.b<<"\tc:"<<dataToStore2.c<<"\td:"<<dataToStore2.d<<"\n";
	
	if(dataToStore.a==dataToStore2.a &&
		 dataToStore.b==dataToStore2.b &&
		 dataToStore.c==dataToStore2.c &&
		 dataToStore.d==dataToStore2.d)
		com<<"write successs\n";
	else
		com<<"oppps...error occured\n";
	while(1){

	}
	
}

