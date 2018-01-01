#include "Bluetooth.h"

extern char RxBuffer[10];
extern char rcv_end;

void UART_Init( )
{
  UCA0CTL1 |= UCSWRST; //swrst=1
  P1SEL |= BIT1+BIT2;
  P1SEL2 |= BIT1+BIT2; //P1.1��P1.2���Ź�������
  UCA0CTL0= 0; //�ַ�֡��ʽ����
  UCA0CTL1|=(UCSSEL_2+UCRXEIE); //ʱ��ԴACLK, ȫ�շ�ʽ
  UCA0BR1=0; //����������
  UCA0BR0=104;
  UCA0MCTL = UCBRF_0+ UCBRS_1;
  UCA0CTL1&=~UCSWRST ; //swrst=0
  //���շ��жϷֿ�
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


