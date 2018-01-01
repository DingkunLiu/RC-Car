#include "Timer.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "USART.h"

u8 TxBuffer[6]={0,0,0,0,0x0d,0x0a};

//key state
char state=0;//0,不动，1，运动，2，音乐；

void Sys_Init();

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Sys_Init();
  
  while(1)
  {
    if(!state)
    {
      TxBuffer[2]=0;
      TxBuffer[3]=0;
      P2OUT|=BIT0+BIT1;
    }
    else if(state==1)
    {
      TxBuffer[2]=1;
      TxBuffer[3]=0;
      P2OUT|=BIT1;
      P2OUT&=~BIT0;
    }
    else
    {
      TxBuffer[2]=0;
      TxBuffer[3]=1;
      P2OUT&=~BIT1;
      P2OUT|=BIT0;
    }
  }
}


//BIT3 start BIT4 motion BIT4 music
void key_Init()
{
  P1IE&=~(BIT3|BIT4|BIT5);
  P1DIR&=~(BIT3+BIT4+BIT5);
  P1SEL&=~(BIT3+BIT4+BIT5);
  P1SEL2&=~(BIT3+BIT4+BIT5);
  P1REN|=BIT3+BIT4+BIT5;
  P1OUT|=BIT3+BIT4+BIT5;
  P1IES|=BIT3|BIT4|BIT5;
  P1IFG&=~(BIT3+BIT4+BIT5);
  P1IE|=(BIT3+BIT4+BIT5);
  
}

void debug_Init()//LED指示state
{
  P2SEL&=~(BIT0+BIT1);
  P2SEL2&=~(BIT0+BIT1);
  P2DIR|=(BIT0+BIT1+BIT2);
  P2OUT|=(BIT0+BIT1);
  P2OUT&=~BIT2;
}

void Sys_Init()
{
  debug_Init();
  if(CALBC1_1MHZ!=0xff)
  {
    BCSCTL1=CALBC1_1MHZ;
    DCOCTL=CALDCO_1MHZ;
  }
   _EINT();//开总控位，初始化过程需要中断；
  key_Init();
  Timer_Init();
  MPU_Init();
  UART_Init(); 
}

#pragma vector=PORT1_VECTOR
__interrupt void port_int()//IO1中断，控制按键按下后的变化
{
  if((P1IFG&BIT3))
  {
    state=0;
    P1IFG&=~BIT3;//清中断标识位
  }
  else if((P1IFG&BIT4))
  {
    state=1;
    P1IFG&=~BIT4;//同
  }
  else if((P1IFG&BIT5))
  {
    state=2;
    P1IFG&=~BIT5;//同
  }
}