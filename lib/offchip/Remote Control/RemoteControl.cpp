#include "RemoteControl.h"


RemoteControl::RemoteControl(InputCapture_TIM *hunter1,u8 Pit,u8 Thr,u8 Yaw,u8 Rol)
{
	State=REMOTECONTROL_LOCK;
	
	Limit_MAX=RC_DEFAULT_LIMIT_MAX;	
	Limit_MIN=RC_DEFAULT_LIMIT_MIN;
	
	OriginalPitMax=FUTABA_ORIGINAL_MAX;
	OriginalPitMin=FUTABA_ORIGINAL_MIN;

	OriginalThrMax=FUTABA_ORIGINAL_MAX;
	OriginalThrMin=FUTABA_ORIGINAL_MIN;

	OriginalYawMax=FUTABA_ORIGINAL_MAX;
	OriginalYawMin=FUTABA_ORIGINAL_MIN;

	OriginalRolMax=FUTABA_ORIGINAL_MAX;
	OriginalRolMin=FUTABA_ORIGINAL_MIN;
	
	mHunter1 = hunter1;
	mPitchCh1=Pit;
	mThrottleCh2=Thr;
	mYawCh3=Yaw;
	mRollCh4=Rol;
	
	mHunter2=0;
	
  NightState =0;
	LeftState =0;
	
}
//More than four channels(超过四个通道的构造函数)
RemoteControl::RemoteControl(InputCapture_TIM *hunter1,u8 Pit,u8 Thr,u8 Yaw,u8 Rol,InputCapture_TIM *hunter2,u8 Ch5,u8 Ch6,u8 Ch7,u8 Ch8)
{
	State=REMOTECONTROL_LOCK;
	
	Limit_MAX=RC_DEFAULT_LIMIT_MAX;	
	Limit_MIN=RC_DEFAULT_LIMIT_MIN;
	
	OriginalPitMax=FUTABA_ORIGINAL_MAX;
	OriginalPitMin=FUTABA_ORIGINAL_MIN;

	OriginalThrMax=FUTABA_ORIGINAL_MAX;
	OriginalThrMin=FUTABA_ORIGINAL_MIN;

	OriginalYawMax=FUTABA_ORIGINAL_MAX;
	OriginalYawMin=FUTABA_ORIGINAL_MIN;

	OriginalRolMax=FUTABA_ORIGINAL_MAX;
	OriginalRolMin=FUTABA_ORIGINAL_MIN;
	
	OriginalModelMax=FUTABA_ORIGINAL_MAX;
	OriginalModelMin=FUTABA_ORIGINAL_MIN;

	OriginalAdd6Max=FUTABA_ORIGINAL_MAX;
	OriginalAdd6Min=FUTABA_ORIGINAL_MIN;

	OriginalAdd7Max=FUTABA_ORIGINAL_MAX;
	OriginalAdd7Min=FUTABA_ORIGINAL_MIN;

	OriginalAdd8Max=FUTABA_ORIGINAL_MAX;
	OriginalAdd8Min=FUTABA_ORIGINAL_MIN;
	
	mHunter1 = hunter1;
	mPitchCh1=Pit;
	mThrottleCh2=Thr;
	mYawCh3=Yaw;
	mRollCh4=Rol;
	
	mHunter2 = hunter2;
	mModelCh5=Ch5;
	mCh6=Ch6;
	mCh7=Ch7;
	mCh8=Ch8;
	
}

RemoteControl::~RemoteControl()
{}
	
//Scope of transformation(将从硬件捕获得到的值（油门、偏航、横滚、俯仰）转换为上层可以理解的值)
uint16_t RemoteControl::LimitsConversion(uint16_t OriginalData, uint16_t OriginalMax,uint16_t OriginalMin)
{
	return (OriginalData-OriginalMin)*1.0/(OriginalMax-OriginalMin)*(Limit_MAX-Limit_MIN)+Limit_MIN;
}



//Set the output range
void RemoteControl::Set_Limit_MAX_MIN(u16 max,u16 min)
{
	 Limit_MAX=max;
	 Limit_MIN=min;
}


//Data update and state judgment
u8 RemoteControl::Updata(u16 time_ms,u16 Time_Of_Duration_ms)
{
	static u16 StateCnt=0;
	u16 cnt=(u16)(Time_Of_Duration_ms/time_ms);//次数=触发命令需要保持舵量为特定值持续的时长 / 每两次之间进本函数的间隔

	//Save after processing of the original value, in the range 0 to 1000(保存处理后的原始值，范围在0到1000)
	  //保存原始值
	 mOriginalPITCH = (*mHunter1)[mPitchCh1]*1000;
	 mOriginalTHROTTLE = (*mHunter1)[mThrottleCh2]*1000;
	 mOriginalYAW = (*mHunter1)[mYawCh3]*1000;
	 mOriginalROLL = (*mHunter1)[mRollCh4]*1000;
		//规范化原始数据
	if(State!=REMOTECONTROL_CALIBRATION)
	{
		mPITCH = LimitsConversion(mOriginalPITCH,OriginalPitMax,OriginalPitMin);
		mTHROTTLE = LimitsConversion(mOriginalTHROTTLE,OriginalThrMax,OriginalThrMin);
		mYAW = LimitsConversion(mOriginalYAW,OriginalYawMax,OriginalYawMin);
		mROLL = LimitsConversion(mOriginalROLL,OriginalRolMax,OriginalRolMin);
			
		//Prevent the output of the original value is too small and beyond(防止由于输出原始值过小而超出范围)
		if(mPITCH>Limit_MAX)
			mPITCH=Limit_MAX;
		if(mTHROTTLE>Limit_MAX)
			mTHROTTLE=Limit_MAX;
		if(mYAW>Limit_MAX)
			mYAW=Limit_MAX;
		if(mROLL>Limit_MAX)
			mROLL=Limit_MAX;	
	}

	//如果存在5678通道 暂时就这么写了
	if(mHunter2)
	{
		if(mModelCh5)
		mModel=LimitsConversion((*mHunter2)[mModelCh5]*1000,OriginalModelMax,OriginalModelMin);
		if(mCh6)
		mAdditional6=LimitsConversion((*mHunter2)[mCh6]*1000,OriginalAdd6Max,OriginalAdd6Min);
		if(mCh7)
		mAdditional7=LimitsConversion((*mHunter2)[mCh7]*1000,OriginalAdd7Max,OriginalAdd7Min);
		if(mCh8)
		mAdditional8=LimitsConversion((*mHunter2)[mCh8]*1000,OriginalAdd8Max,OriginalAdd8Min);	
	}
	
	LeftState = GetLeftState();
	NightState= GetNightState();
	
	//state judgment
	if(LeftState ==9 && NightState ==7)  //如果是内八字 上锁
	{
		StateCnt++;   //累计
//		Unlock_cnt=0; //清空其他量		
		
		if(cnt==StateCnt) //如果达到阀值
		{
			StateCnt=0;   //清空
			if(State!=REMOTECONTROL_CALIBRATION)//如果不是在校准
				State=REMOTECONTROL_LOCK; //标识上锁状态
		  return State;  //返回已经上锁
		}
		else
		  return State;
	}
	else if(LeftState ==7 && NightState==9)  //如果是外八字 解锁
	{
		StateCnt++;   //累计
		
		if(cnt==StateCnt)
		{
			StateCnt=0;
			if(State!=REMOTECONTROL_CALIBRATION)
			 State= REMOTECONTROL_UNLOCK;
			return State;
		}
		else
			return  State;
	}
	else 		     //norm
	{		
		StateCnt=0;
		State = State | 0x10 ;
		return State ;
	}
}


#ifdef 	USE_THE_RANGE_GIVEN_OUTPUT
uint16_t RemoteControl::operator[](u8 chNum)
{
	if(chNum==1)
		return mPITCH;
	else if(chNum==2)
		return mTHROTTLE;
    else if(chNum==3)
		return mYAW;
	else if(chNum==4)
		return mROLL;
	else if(chNum==5)
		return mModel;
	else if(chNum==6)
		return mAdditional6;
	else if(chNum==7)
		return mAdditional7;
	else if(chNum==8)
		return mAdditional8;
	else
		return 0;
}
	#endif

#ifdef 	USE_PERCENTAGE_OUTPUT
 double RemoteControl::operator[](u8 chNum)
 {
	if(chNum==1)
		return (mPITCH-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==2)
		return (mTHROTTLE-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==3)
		return (mYAW-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==4)
		return (mROLL-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;		
	else if(chNum==5)
		return (mModel-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==6)
		return (mAdditional6-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==7)
		return (mAdditional7-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else if(chNum==8)
		return (mAdditional8-Limit_MIN)*1.0/(Limit_MAX-Limit_MIN)*100.0;
	else
		return 0;
 }
#endif



//Return to the original value(返回原始值)
uint16_t RemoteControl::GetOriginalValue(u8 ChNum)
{
//	 mOriginalPITCH = (*mHunter1)[mPitchCh1]*1000;
//	 mOriginalTHROTTLE = (*mHunter1)[mThrottleCh2]*1000;
//	 mOriginalYAW = (*mHunter1)[mYawCh3]*1000;
//	 mOriginalROLL = (*mHunter1)[mRollCh4]*1000;
	
	if (ChNum==1)
		return mOriginalPITCH;
	else if(ChNum==2)
		return mOriginalTHROTTLE;
	else if(ChNum==3)
		return mOriginalYAW;
	else if(ChNum==4)
		return mOriginalROLL;
	else 
		return 0;
	
//	if (ChNum==1)
//		return (*mHunter1)[mPitchCh1]*1000;
//	else if(ChNum==2)
//		return (*mHunter1)[mThrottleCh2]*1000;
//	else if(ChNum==3)
//		return (*mHunter1)[mYawCh3]*1000;
//	else if(ChNum==4)
//		return (*mHunter1)[mRollCh4]*1000;
//	else if(ChNum==5)
//		return (*mHunter2)[mModelCh5]*1000;
//	else if(ChNum==6)
//		return (*mHunter2)[mCh6]*1000;
//	else if(ChNum==7)
//		return (*mHunter2)[mCh7]*1000;
//	else if(ChNum==8)
//		return (*mHunter2)[mCh8]*1000;
//	else
//		return 0;
}



//这里要注意的是遥控器的左右的大小变化可能不一样
//油门 下小上大  偏航 左大右小   俯仰 上大下小 横滚 左大右小

u8 RemoteControl::GetLeftState() //左上向右数的九宫格
{
	u8 Xtemp,Ytemp;
	
	if(mOriginalYAW ==0)
		return 0;
	else if(mOriginalYAW<=(OriginalYawMin+OriginalYawMax/10))
		Xtemp = 6;
	else if(mOriginalYAW>=(OriginalYawMax-OriginalYawMax/10))
		Xtemp = 4;
	else
		Xtemp = 5;
	
	if(mOriginalTHROTTLE<=(OriginalThrMin+OriginalThrMax/10))
		Ytemp = 8;
	else if(mOriginalTHROTTLE>=(OriginalThrMax-OriginalThrMax/10))
		Xtemp = 2;
	else
		Ytemp = 5;
	
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
	return 0;
}


u8 RemoteControl::GetNightState() //左上向右数的九宫格
{
	u8 Xtemp,Ytemp;
	
	if(mOriginalROLL ==0)
		return 0;
	else if(mOriginalROLL<=(OriginalRolMin+OriginalRolMax/10))
		Xtemp = 6;
	else if(mOriginalROLL>=(OriginalRolMax-OriginalRolMax/10))
		Xtemp = 4;
	else
		Xtemp = 5;
	
	if(mOriginalPITCH<=(OriginalPitMin+OriginalPitMax/10))
		Ytemp = 8;
	else if(mOriginalPITCH>=(OriginalPitMax-OriginalPitMax/10))
		Xtemp = 2;
	else
		Ytemp = 5;
	
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
	return 0;
}
