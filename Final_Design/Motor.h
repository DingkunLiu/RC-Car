
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "io430.h"


//IO²Ù×÷º¯Êý
#define  SET_LEFT_MODE1  P1OUT|=BIT3
#define  RESET_LEFT_MODE1 P1OUT&=~BIT3

#define  SET_LEFT_MODE2 P1OUT|=BIT4
#define  RESET_LEFT_MODE2 P1OUT&=~BIT4

#define  SET_LEFT_PHASE P1OUT|=BIT5
#define  RESET_LEFT_PHASE P1OUT&=~BIT5

#define  SET_LEFT_nSLEEP P1OUT|=BIT7
#define  RESET_LEFT_nSLEEP P1OUT&=~BIT7

#define  SET_RIGHT_MODE1 P2OUT|=BIT2
#define  RESET_RIGHT_MODE1 P2OUT&=~BIT2

#define  SET_RIGHT_MODE2 P2OUT|=BIT3
#define  RESET_RIGHT_MODE2 P2OUT&=~BIT3

#define  SET_RIGHT_PHASE P2OUT|=BIT0
#define  RESET_RIGHT_PHASE P2OUT&=~BIT0

#define  SET_RIGHT_nSLEEP P2OUT|=BIT5
#define  RESET_RIGHT_nSLEEP P2OUT&=~BIT5

void Motor_GPIO_Init();
void Motor_PWM_Init();
void Motor_Motion(unsigned char x,unsigned char y);
void Motor_Sleep();
void Motor_Stop();

#endif