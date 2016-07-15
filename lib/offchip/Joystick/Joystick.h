#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include "stm32f10x.h"
#include "ADC.h"




//左摇杆X 在小于2的时候判定为向右搬动了，在大于3的时候判定为左搬动
//左摇杆Y 在大于1的时候判定为向上搬动了, 在小于0.5的时候判定为向下搬动了

//右摇杆X 在小于0.7的时候判定为向左搬动了,在大于1的时候判定为向右搬动了
//右摇杆Y 在小于2的时候判定为向上搬动了,在大于3的时候判定为向下搬动了
#define LX_NIGHT_THRESHOLDS 2
#define LX_LEFT_THRESHOLDS 3
#define LX_UP_THRESHOLDS 1
#define LX_DOWN_THRESHOLDS 0.5


#define NX_NIGHT_THRESHOLDS 1
#define NX_LEFT_THRESHOLDS 0.7
#define NX_UP_THRESHOLDS 2
#define NX_DOWN_THRESHOLDS 3


class Joystick{
	
	private:
		ADC &mAdc;
		u8 mCH_LX,mCH_LY,mCH_NX,mCH_NY;	
		float mLX_Val;
		float mLY_Val;
		float mNX_Val;
		float mNY_Val;
		
	public:
		Joystick(ADC &adc,u8 LX,u8 LY,u8 NX,u8 NY);
		bool Updata();
		u8 getLeftState(); //左上向右数的九宫格
		u8 getNightState();
	
};


#endif


//X1（5）: 中间值：2.56        最小：（右移）0.008   最大（左移） 3.299
//Y1（6）:         0.87              （下移）0.005		   （上移） 2.98
//X2（8）:         0.83              （左移）0.002       （右移） 2.89
//Y2（9）:         2.27              （上移）0.010       （下移） 3.299  

//左摇杆X 在小于2的时候判定为向右搬动了，在大于3的时候判定为左搬动
//左摇杆Y 在大于1的时候判定为向上搬动了, 在小于0.5的时候判定为向下搬动了

//右摇杆X 在小于0.7的时候判定为向左搬动了,在大于1的时候判定为向右搬动了
//右摇杆Y 在小于2的时候判定为向上搬动了,在大于3的时候判定为向下搬动了

