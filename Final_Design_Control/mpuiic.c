#include "mpuiic.h"
#include "main.h"

extern u8 TxBuffer[6];
unsigned char iic_SendBuffer[2],iic_RecvBuffer;
signed char iic_SendPtr=0;

void iic_init (unsigned char SlaveAddr)
{
	P1SEL |= BIT6+BIT7;                       // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6+BIT7;                       // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;         // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2+UCSWRST;              // Use SMCLK, keep SW reset
	UCB0BR0 = 10;                             // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2COA = 0x01A5;
	UCB0I2CSA = SlaveAddr;                    // Set slave address
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
}

void iic_write_init()
{
  UCB0CTL1 |= UCTR;
  IFG2 &= ~UCB0TXIFG;
  IE2 &= ~UCB0RXIE;               // disable Receive ready interrupt
  IE2 |= UCB0TXIE;                // enable Transmit ready interrupt
}

void iic_read_init()
{
  UCB0CTL1 &= ~UCTR;              // UCTR=0 => Receive Mode (R/W bit = 1)
  IFG2 &= ~UCB0RXIFG;
  IE2 &= ~UCB0TXIE;               // disable Transmit ready interrupt
  IE2 |= UCB0RXIE;                // enable Receive ready interrupt
}

void iic_write(char slaveAd,char data)
{
  iic_SendPtr=1;
  iic_SendBuffer[1]=slaveAd;
  iic_SendBuffer[0]=data;
  
  while(UCB0STAT&UCBUSY);//wait until iic module has finished
  iic_write_init();
  _DINT();
  UCB0CTL1|=UCTXSTT;//主控模式下生成发送起始条件
  __bis_SR_register(CPUOFF + GIE);// Waiting for Sending.
  UCB0CTL1 |= UCTXSTP;           // I2C stop condition
  while (UCB0CTL1 & UCTXSTP);         // Ensure stop condition got sent
}

unsigned char iic_read(unsigned char add)
{
  while (UCB0STAT & UCBUSY);          // wait until I2C module has finished all operations
  //Change to Write Mode.
  iic_SendBuffer[0] = add;
  iic_SendPtr = 0;
  iic_write_init();
  _DINT();
  UCB0CTL1 |= UCTXSTT;                 // start condition generation
                                         // => I2C communication is started
  __bis_SR_register(CPUOFF + GIE);     // Enter LPM0 w/ interrupts

	//Change to Read Mode.
 iic_read_init();
 UCB0CTL1 |= UCTXSTT;                    // I2C start condition
 while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
 UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
 __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts
 while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
 return iic_RecvBuffer;
}



//iic should use same interrupt vector for Rx/Tx
//发送中断还要用这里
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  static unsigned char i=0;//发送计数
  if (UCB0TXIFG & IFG2)                           // TX
  {
    UCB0TXBUF = iic_SendBuffer[iic_SendPtr];      // Load TX buffer
    iic_SendPtr--;                               // Decrement TX byte counter
    if (iic_SendPtr < 0) 
    {
      while (!(IFG2 & UCB0TXIFG));            // wait for tx complete
      IE2 &= ~UCB0TXIE;                       // disable interrupts.
      IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
      LPM0_EXIT;   // Exit LPM0
    }
  }
  else if (UCB0RXIFG & IFG2)                      // RX
  {
    iic_RecvBuffer = UCB0RXBUF;                  // store received data in buffer
    __bic_SR_register_on_exit(LPM0_bits);       // Exit LPM0
  }
  else if(IFG2&UCA0TXIFG)
  {
    UCA0TXBUF=TxBuffer[i++];
    if(i==6)
    {
      IE2&=~UCA0TXIE;
      i=0;  
    }
  }
}


