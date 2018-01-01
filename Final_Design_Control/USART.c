#include "main.h"
#include "Timer.h"

extern u8 TxBuffer[6];
void UART_Init( )
{
  UCA0CTL1 |= UCSWRST; //swrst=1
  P1SEL |= BIT1+BIT2;
  P1SEL2 |= BIT1+BIT2; //P1.1和P1.2引脚功能设置
  UCA0CTL0= 0; //字符帧格式设置
  UCA0CTL1|=(UCSSEL_1+UCRXEIE); //时钟源ACLK, 全收方式
  UCA0BR1=0; //波特率设置
  UCA0BR0=3;
  UCA0MCTL = UCBRF_0+ UCBRS_3;
  UCA0CTL1&=~UCSWRST ; //swrst=0
}

//中断函数定义在mpuiic中，因为共享同一个中断向量