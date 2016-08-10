
#ifndef __SENSER_H
#define __SENSER_H

class Senser{
	public: 
		Senser(){};
		virtual bool Updata()=0;
	//	virtual unsigned short int GetU16Data()=0;
		virtual float GetFloatData()=0;
		virtual unsigned char Data_Hight_8() =0; 
		virtual unsigned char Data_Low_8() =0;
};

#endif
