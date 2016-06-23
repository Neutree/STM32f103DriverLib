/**
  *@file Delay.h
  *@author Neutree
  *@breif Delay Class
  *@copyright CQUT IOT Lib all rights reserved
  */


#ifndef __DELAY_H
#define __DELAY_H

/*************************configuration******************************/
//			#define       DELAY_USE_SYSTICK
			#define       DELAY_USE_SOFTDELAY

/********************************************************************/

#include "stm32f10x.h"

class Delay
{
	public:
		/////////////////////////
		///delay static function
		///use Delay::Ms() or use object to invoke
		///@param nMs the time will delay Unit:ms
		/////////////////////////
		static void Ms(uint16_t nMs);
	
		/////////////////////////
		///delay static function
		///use Delay::Us() or use object to invoke
		///@param nUs the time will delay Unit:us
		/////////////////////////
		static void Us(uint32_t nUs);
		/////////////////////////
		///delay static function
		///use Delay::nS() or use object to invoke
		///@param nS the time will delay Unit:nS
		/////////////////////////
		static void S(uint32_t nS);
};
	

#endif
