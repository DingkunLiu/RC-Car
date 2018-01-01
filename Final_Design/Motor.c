#include "Motor.h"
#include "math.h"

#define pi 3.14
#define T 128
#define min 0x40
#define max 0xC0
#define max_min 0x40

void Motor_GPIO_Init()
{
  
  P1SEL&=~(BIT3+BIT4+BIT5+BIT7);
  P1SEL2&=~(BIT3+BIT4+BIT5+BIT7);
  P1DIR|=(BIT3+BIT4+BIT5+BIT7);
  
  P2SEL&=~(BIT0+BIT2+BIT3+BIT5);
  P2SEL2&=~(BIT0+BIT2+BIT3+BIT5);
  P2DIR|=(BIT0+BIT2+BIT3+BIT5);
  
  SET_LEFT_nSLEEP;
  SET_RIGHT_nSLEEP;
  RESET_LEFT_MODE2;
  RESET_RIGHT_MODE2;
  
}

void Motor_PWM_Init()
{
  //P2.1 ENABLE_RIGHT
  P2SEL|=BIT1;
  P2SEL2&=~BIT1;
  P2DIR|=BIT1;
  P2OUT&=~BIT1;
  
  //P1.6 ENABLE_LEFT
  P1SEL|=BIT6;
  P1SEL2&=~BIT6;
  P1DIR|=BIT6;
  P1OUT&=~BIT6;
  
  TA0CCR0=0;
  TA0CCR1=0;
  TA0CCTL1|=OUTMOD_2;
  
  TA1CCR0=0;
  TA1CCR1=0;
  TA1CCTL1|=OUTMOD_2;
  
  //ENABLE Simutaneous
  TA0CTL=TACLR+TASSEL_1+MC_1;//ACLK
  TA1CTL=TACLR+TASSEL_1+MC_1;//ACLK
}

void Motor_Motion(unsigned char x,unsigned char y)
{
  short left_temp,right_temp;
  short temp;
  
  SET_LEFT_nSLEEP;
  SET_RIGHT_nSLEEP;
  SET_LEFT_MODE1;
  SET_RIGHT_MODE1;
  
  if(y<T)//ºó
  {
    RESET_LEFT_PHASE;
    RESET_RIGHT_PHASE;
    left_temp=((T-y)*T/(max_min));
    right_temp=left_temp;
  }
  else//Ç°
  {
    SET_LEFT_PHASE;
    SET_RIGHT_PHASE;
    left_temp=((y-T)*T/(max_min));
    right_temp=left_temp;
  }
   temp=((T-x)*T/(max_min))/3;
   left_temp=left_temp-temp;
   right_temp=right_temp+temp;

  if(left_temp<T)
    TA0CCR1=T-left_temp;
  else
    TA0CCR1=0;
  if(right_temp<T)
    TA1CCR1=T-right_temp;
  else
    TA1CCR1=0;
  TA0CCR0=T;
  TA1CCR0=T;
}


void Motor_Stop()
{
  RESET_LEFT_nSLEEP;
  RESET_RIGHT_nSLEEP;
  TA0CCR1=T;
  TA1CCR1=T;
  RESET_LEFT_MODE1;
  RESET_RIGHT_MODE1;
}

