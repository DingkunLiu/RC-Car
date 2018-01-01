#include "Timer.h"
#include "main.h"
#include "USART.h"
#include "mpu6050.h"

extern u8 TxBuffer[6];

void Timer_Init()
{
  TA0CTL|=TACLR+TASSEL_2+MC_1+ID_1;
  TA0CCR0=49999;//10Hz interrupt
  TA0CCTL0|=CCIE;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_ISR()
{
  short ax,ay;
  ax=MPU_Get_Accelerometer_x()+32768;
  ay=MPU_Get_Accelerometer_y()+32768;
  TxBuffer[0]=(ax>>8);
  TxBuffer[1]=(ay>>8);
  IE2|=UCA0TXIE;
}