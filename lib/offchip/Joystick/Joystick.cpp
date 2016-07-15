#include "Joystick.h"



Joystick::Joystick(ADC &adc,u8 LX,u8 LY,u8 NX,u8 NY):mAdc(adc)
{
	mCH_LX=LX;
	mCH_LY=LY;
	mCH_NX=NX;
	mCH_NY=NY;
}


bool Joystick::Updata()
{
	
		mLX_Val=mAdc[mCH_LX];
		mLY_Val=mAdc[mCH_LY];
		mNX_Val=mAdc[mCH_NX];
		mNY_Val=mAdc[mCH_NY];
	
	return true;
	
}


u8 Joystick::getLeftState() //左上向右数的九宫格
{
	u8 Xtemp,Ytemp;
	
	//左右判断
	if(mLX_Val ==0)
		return 0;
	else if(mLX_Val<LX_NIGHT_THRESHOLDS) //向右搬动了
		Xtemp=6;
	else if(mLX_Val>LX_LEFT_THRESHOLDS) //向左搬动了
		Xtemp=4;
	else
		Xtemp=5;
	
  //上下判断
	if(mLY_Val ==0)
		return 0;
  else if(mLY_Val>LX_UP_THRESHOLDS)//向上搬动了
		Ytemp=2;
	else if(mLY_Val<LX_DOWN_THRESHOLDS)//向下搬动了
		Ytemp=8;
	else
		Ytemp=5;
	
	//综合返回
	if(Xtemp==4 && Ytemp==2) //左上
		return 1;
	if(Xtemp==5 && Ytemp==2) //上
		return 2;
	if(Xtemp==6 && Ytemp==2) //右上
		return 3;
	if(Xtemp==4 && Ytemp==5) //左
		return 4;
	if(Xtemp==5 && Ytemp==5) //中
		return 5;			
	if(Xtemp==6 && Ytemp==5) //右
		return 6;
	if(Xtemp==4 && Ytemp==8) //左下
		return 7;
	if(Xtemp==5 && Ytemp==8) //下
		return 8;
	if(Xtemp==6 && Ytemp==8) //右下
		return 9;
			
	return 0;//错误退出
}


//右摇杆X 在小于0.7的时候判定为向左搬动了,在大于1的时候判定为向右搬动了
//右摇杆Y 在小于2的时候判定为向上搬动了,在大于3的时候判定为向下搬动了

u8 Joystick::getNightState()
{
		u8 Xtemp,Ytemp;
	
	//左右判断
	if(mNX_Val ==0)
		return 0;
	else if(mNX_Val>NX_NIGHT_THRESHOLDS) //向右搬动了
		Xtemp=6;
	else if(mNX_Val<NX_LEFT_THRESHOLDS) //向左搬动了
		Xtemp=4;
	else
		Xtemp=5;
	
  //上下判断
	if(mNY_Val==0)
		return 0;
  else if(mNY_Val<NX_UP_THRESHOLDS)//向上搬动了
		Ytemp=2;
	else if(mNY_Val>NX_DOWN_THRESHOLDS)//向下搬动了
		Ytemp=8;
	else
		Ytemp=5;
	
	//综合返回
	if(Xtemp==4 && Ytemp==2) //左上
		return 1;
	if(Xtemp==5 && Ytemp==2) //上
		return 2;
	if(Xtemp==6 && Ytemp==2) //右上
		return 3;
	if(Xtemp==4 && Ytemp==5) //左
		return 4;
	if(Xtemp==5 && Ytemp==5) //中
		return 5;			
	if(Xtemp==6 && Ytemp==5) //右
		return 6;
	if(Xtemp==4 && Ytemp==8) //左下
		return 7;
	if(Xtemp==5 && Ytemp==8) //下
		return 8;
	if(Xtemp==6 && Ytemp==8) //右下
		return 9;
	
	return 0;//错误退出
}
