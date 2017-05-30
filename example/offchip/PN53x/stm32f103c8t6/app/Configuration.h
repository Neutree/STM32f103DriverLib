#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "stm32f10x.h"

//~~~~~~~~~~~~~~~USART Congfigure~~~~~~~~~~~//
#define USE_USART              //---USART---//
#define USE_USART1             //---USART1--//
#define USE_USART2           //---USART2--//
#define USE_USART3           //---USART3--//

//~~~~~~~~~~~~~~~~I2C Congfigure~~~~~~~~~~~~//
//#define USE_I2C                //----IIC----//
//#define USE_I2C1             //----IIC1---//
//#define USE_I2C2               //----IIC2---//

//~~~~~~~~~~~~~~~Timer Congfigure~~~~~~~~~~~//
//#define USE_TIMER              //---Timer---//
//#define USE_TIMER1              //---Timer1--//
//#define USE_TIMER2             //---Timer2--//
//#define USE_TIMER3            //---Timer3--//
//#define USE_TIMER4            //---Timer4--//

//~~~~~~~~~~~~~~Capture Congfigure~~~~~~~~~~//
//#define USE_CAPTURE_TIM         //--Capture- With Timer-//
//#define USE_CAPTURE_TIM1       //--ICP-TIM1-//
//#define USE_CAPTURE_TIM2       //--ICP-TIM2-//
//#define USE_CAPTURE_TIM3       //--ICP-TIM3-//
//#define USE_CAPTURE_TIM4       //--ICP-TIM4-//

//#define USE_CAPTURE_EXIT        //--Capture-With EXIT-//
//#define USE_CAPTURE_EXIT0       //--ICP-EXIT0//
//#define USE_CAPTURE_EXIT1       //--ICP-EXIT1//
//#define USE_CAPTURE_EXIT2       //--ICP-EXIT2//
//#define USE_CAPTURE_EXIT3       //--ICP-EXIT3//
//#define USE_CAPTURE_EXIT4       //--ICP-EXIT4//
//#define USE_CAPTURE_EXIT5       //--ICP-EXIT5//
//#define USE_CAPTURE_EXIT6       //--ICP-EXIT6//
//#define USE_CAPTURE_EXIT7       //--ICP-EXIT7//
//#define USE_CAPTURE_EXIT8       //--ICP-EXIT8//
//#define USE_CAPTURE_EXIT9       //--ICP-EXIT9//
//#define USE_CAPTURE_EXIT10      //--ICP-EXIT10//
//#define USE_CAPTURE_EXIT11      //--ICP-EXIT11//
//#define USE_CAPTURE_EXIT12      //--ICP-EXIT12//
//#define USE_CAPTURE_EXIT13      //--ICP-EXIT13//
//#define USE_CAPTURE_EXIT14      //--ICP-EXIT14//
//#define USE_CAPTURE_EXIT15      //--ICP-EXIT15//


//~~~~~~~~~~~~~~~~EXTI Congfigure~~~~~~~~~~~//
//#define USE_EXTI0            //---EXTI0---//
//#define USE_EXTI1            //---EXTI1---//
//#define USE_EXTI2            //---EXTI2---//
//#define USE_EXTI3            //---EXTI3---//
//#define USE_EXTI4            //---EXTI4---//
//#define USE_EXTI5            //---EXTI5---//
//#define USE_EXTI6            //---EXTI6---//
//#define USE_EXTI7            //---EXTI7---//
//#define USE_EXTI8            //---EXTI8---//
//#define USE_EXTI9            //---EXTI9---//
//#define USE_EXTI10           //---EXTI10--//
//#define USE_EXTI11           //---EXTI11--//
//#define USE_EXTI12           //---EXTI12--//
//#define USE_EXTI13           //---EXTI13--//
//#define USE_EXTI14           //---EXTI14--//
//#define USE_EXTI15           //---EXTI15--//

//~~~~~~~~~~~~~Ultrasonic Congfigure~~~~~~~~~//
//#define USE_ULTRASONIC         //-Ultrasonic-//
//#define USE_ULTRASONIC_EXTI0   //-Ultr-EXTI0-//
//#define USE_ULTRASONIC_EXTI1   //-Ultr-EXTI1-//
//#define USE_ULTRASONIC_EXTI2   //-Ultr-EXTI2-//
//#define USE_ULTRASONIC_EXTI3   //-Ultr-EXTI3-//
//#define USE_ULTRASONIC_EXTI4   //-Ultr-EXTI4-//
//#define USE_ULTRASONIC_EXTI5   //-Ultr-EXTI5-//
//#define USE_ULTRASONIC_EXTI6   //-Ultr-EXTI6-//
//#define USE_ULTRASONIC_EXTI7   //-Ultr-EXTI7-//
//#define USE_ULTRASONIC_EXTI8   //-Ultr-EXTI8-//
//#define USE_ULTRASONIC_EXTI9   //-Ultr-EXTI9-//
//#define USE_ULTRASONIC_EXTI10  //-Ultr-EXTI10-//
//#define USE_ULTRASONIC_EXTI11  //-Ultr-EXTI11-//
//#define USE_ULTRASONIC_EXTI12  //-Ultr-EXTI12-//
//#define USE_ULTRASONIC_EXTI13  //-Ultr-EXTI13-//
//#define USE_ULTRASONIC_EXTI14  //-Ultr-EXTI14-//
//#define USE_ULTRASONIC_EXTI15  //-Ultr-EXTI15-//


#ifdef USE_USART
#include "USART.h"
#endif

#ifdef USE_I2C
#include "I2C.h"
#endif

#ifdef USE_CAPTURE_TIM
#include "InputCapture_TIM.h"
#endif

#ifdef USE_CAPTURE_EXIT
#include "InputCapture_EXIT.h"
#endif

#ifdef USE_TIMER
#include "Timer.h"
#endif

#ifdef USE_ULTRASONIC
#include "Ultrasonic.h"
#endif
#endif

