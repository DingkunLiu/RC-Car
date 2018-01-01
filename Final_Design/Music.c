#include "Music.h"
#include "Motor.h"

const unsigned char note[25]={124,117,111,104,98,93,88,83,78,73,69,65,62,58,54,52,49,46,43,41,38,36,34,32,0};
//const unsigned char AWM[]={};
unsigned char T_beat;
extern char RxBuffer[10];
extern char rcv_end;

#define k 0.6//占空比1-k

void Music_Init()

{
  SET_LEFT_MODE1;
  SET_RIGHT_MODE1;
  SET_LEFT_nSLEEP;
  SET_RIGHT_nSLEEP;
  //PHASE_LEFT TA0.0
  P1SEL|=BIT5;
  P1SEL2&=~BIT5;
  P1DIR|=BIT5;
  //PHASE_RIGHT TB2.0
  P2SEL|=BIT0;
  P2SEL2&=~BIT0;
  P2DIR|=BIT0;
  
  TA0CCR0=(note[0]>>1);
  //占空比设成0.4
  TA0CCR1=25;
  TA0CCTL1|=OUTMOD_2;//翻转，复位
  TA0CCTL0|=OUTMOD_4;//翻转

  TA1CCR0=(note[0]>>1);
  TA1CCR1=25;
  TA1CCTL1|=OUTMOD_2;
  TA1CCTL0|=OUTMOD_4;//翻转
  
  //ENABLE Simutaneously
  TA0CTL=TACLR+TASSEL_1+MC_1;//ACLK
  TA1CTL=TACLR+TASSEL_1+MC_1;//ACLK
}

//private function
void get_Note(unsigned char L_R,unsigned char this_note)
{
  if(!L_R)//等于0是左，1是右
  {
    TA0CCR0=note[this_note];
    TA0CCR1=(int)(k*note[this_note]);
  }
  else
  {
    TA1CCR0=note[this_note];
    TA1CCR1=(int)(k*note[this_note]);
  }
}
//用看门狗定时器延时
void Delay_beat(unsigned char beat,char type)
{
  T_beat=beat;
  if(!type)//长节奏
    WDTCTL=WDT_ADLY_250;//设置看门狗定时器
  else//断开连音，推荐beat=4左右?我没试过
    WDTCTL=WDT_ADLY_16;
  IE1|=WDTIE;//开分控位
  while(T_beat!=0);
  IE1&=~WDTIE;//关看门狗定时器
}

void Music()
{
  //两个8度 2，5，7
 /* get_Note(1,D);
  get_Note(0,H_D);
  Delay_beat(4,0);
  get_Note(1,G);
  get_Note(0,H_G);
  Delay_beat(4,0);
  get_Note(1,B);
  get_Note(0,H_B);
  Delay_beat(4,0);*/
  //一小段克莱门蒂钢琴奏鸣曲第三乐章.右手旋律降了一个8度。占空比别太高。。会烧马达的
  get_Note(1,_C);
  get_Note(0,H_E);
  Delay_beat(1,0);
  get_Note(0,H_D);
  Delay_beat(1,0);
  get_Note(0,H_C);
  get_Note(1,E);
  Delay_beat(2,0);
  get_Note(1,G);
  Delay_beat(2,0);
  
  get_Note(0,H_G);
  get_Note(1,_C);
  Delay_beat(2,0);
  get_Note(1,E);
  Delay_beat(2,0);
  get_Note(0,H_F);
  get_Note(1,G);
  Delay_beat(1,0);
  get_Note(0,H_E);
  Delay_beat(1,0);
  
  get_Note(0,H_D);
  get_Note(1,_C);
  Delay_beat(2,0);
  get_Note(0,H_F);
  get_Note(1,D);
  Delay_beat(2,0);
  get_Note(0,B);
  get_Note(1,F);
  Delay_beat(2,0);
  
  get_Note(0,H_C);
  get_Note(1,_C);
  Delay_beat(2,0);
  get_Note(0,G);
  get_Note(1,E);
  Delay_beat(2,0);
  get_Note(1,G);
  Delay_beat(2,0);
  //一共四小节
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR()
{
  --T_beat;
}