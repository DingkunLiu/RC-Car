#include "main.h"
#include "Timer.h"

extern u8 TxBuffer[6];
void UART_Init( )
{
  UCA0CTL1 |= UCSWRST; //swrst=1
  P1SEL |= BIT1+BIT2;
  P1SEL2 |= BIT1+BIT2; //P1.1��P1.2���Ź�������
  UCA0CTL0= 0; //�ַ�֡��ʽ����
  UCA0CTL1|=(UCSSEL_1+UCRXEIE); //ʱ��ԴACLK, ȫ�շ�ʽ
  UCA0BR1=0; //����������
  UCA0BR0=3;
  UCA0MCTL = UCBRF_0+ UCBRS_3;
  UCA0CTL1&=~UCSWRST ; //swrst=0
}

//�жϺ���������mpuiic�У���Ϊ����ͬһ���ж�����