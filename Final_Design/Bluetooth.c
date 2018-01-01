#include "Bluetooth.h"

extern char RxBuffer[10];
extern char rcv_end;

void UART_Init( )
{
  UCA0CTL1 |= UCSWRST; //swrst=1
  P1SEL |= BIT1+BIT2;
  P1SEL2 |= BIT1+BIT2; //P1.1和P1.2引脚功能设置
  UCA0CTL0= 0; //字符帧格式设置
  UCA0CTL1|=(UCSSEL_2+UCRXEIE); //时钟源ACLK, 全收方式
  UCA0BR1=0; //波特率设置
  UCA0BR0=104;
  UCA0MCTL = UCBRF_0+ UCBRS_1;
  UCA0CTL1&=~UCSWRST ; //swrst=0
  //开收发中断分控
  IE2|=UCA0RXIE;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void UCA0RX_ISR()
{
  static unsigned char j=0;
  RxBuffer[j++]=UCA0RXBUF;
  if(j==6)
  {
    j=0;
    rcv_end=1;
  }
}


