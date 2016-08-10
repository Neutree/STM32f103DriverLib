/*

*/



#include "USART.h"
#include "TaskManager.h"

class MHZ14 {

	private:
		USART &COM;
		u8 Command_getvalue[9]; //发送命令
		u8 rev_buffer[9];
		u16 CO2_Concentration;
		bool SumCheck(u8 data[9]);
	public:
		u8 DATA_H;
	  u8 DATA_L;
		MHZ14(USART &com);
		bool Updata();//更新数据，返回是否更新成功 true  false
		u16 GetValue(); //得到一个浓度数据
		
};
