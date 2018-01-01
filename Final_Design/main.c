#include "Motor.h"
#include "Bluetooth.h"
#include "Music.h"

char Motor_init_flag=0,Music_init_flag=0;

char RxBuffer[10];
char rcv_end=0;
char state=0;//state 0停止 1motion 2 music

void Sys_Init();
void Sys_Clock_Init();

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Sys_Init();
  while(1)
  {
    if(rcv_end)
    {
      rcv_end=0;//清接收标识
      if(RxBuffer[4]==0x0d&&RxBuffer[5]==0x0a)//收到结束符
      {
        if(RxBuffer[2])//标识运动
        {
          state=1;
        }
        if(RxBuffer[3])//标识要唱歌了
        {//注意从这个状态跳出时要重新初始化GPIO和PWM
          state=2;
          
        }
        if((RxBuffer[2]==0)&&(RxBuffer[3]==0))
        {
          state=0;
        }
      }
    }
    if(state==1)
    {
       if(!Motor_init_flag)
       {
         Motor_GPIO_Init();
         Motor_PWM_Init();
         Music_init_flag=0;
         Motor_init_flag=1;
       }
       Motor_Motion(RxBuffer[0],RxBuffer[1]);
    }
    else if(state==2)
    {
      if(!Music_init_flag)
      {
        Music_Init();
        Music_init_flag=1;
        Motor_init_flag=0;
      }
      Music();
    }
    else
    {
      Music_init_flag=0;
      Motor_init_flag=0;
      Motor_Stop();
    }
  }
}

void Sys_Init()
{
  Sys_Clock_Init();
  Motor_GPIO_Init();
  Motor_PWM_Init();
  UART_Init();
  _EINT();
}

void Sys_Clock_Init()
{
  BCSCTL1 = CALBC1_1MHZ; 
  DCOCTL = CALDCO_1MHZ;
}
