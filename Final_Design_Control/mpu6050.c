#include "mpu6050.h"
#define ms_cycle 1000
#define ms_100_delay 100000

u8 MPU_Init(void)
{ 
  u8 res;
  iic_init(SlaveAdr);//��ʼ��IIC����
  iic_write(MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
  __delay_cycles(ms_100_delay);
  iic_write(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
  MPU_Set_Gyro_Fsr(0);					//�����Ǵ�����,��250dps
  MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
  MPU_Set_Rate(125);						//���ò�����125Hz
  iic_write(MPU_INT_EN_REG,0X00);	//�ر������ж�
  iic_write(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
  iic_write(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
  iic_write(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
  res=iic_read(MPU_DEVICE_ID_REG);
  if(res==MPU_ADDR)//����ID��ȷ
  {
    iic_write(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
    iic_write(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
    MPU_Set_Rate(125);		        //���ò�����Ϊ125Hz
  }else return 1;
  return 0;
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
void MPU_Set_Gyro_Fsr(u8 fsr)
{
  iic_write(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
void MPU_Set_Accel_Fsr(u8 fsr)
{
  iic_write(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
void MPU_Set_LPF(u16 lpf)
{
  u8 data;
  if(lpf>=188)data=1;
  else if(lpf>=98)data=2;
  else if(lpf>=42)data=3;
  else if(lpf>=20)data=4;
  else if(lpf>=10)data=5;
  else data=6; 
  iic_write(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
void MPU_Set_Rate(u16 rate)
{
  u8 data;
  if(rate>1000)rate=1000;
  if(rate<4)rate=4;
  data=1000/rate-1;
  iic_write(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
  MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}


//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
short MPU_Get_Accelerometer_x()
{ 
  u8 H,L;
  H = iic_read(MPU_ACCEL_XOUTH_REG);
  L = iic_read(MPU_ACCEL_XOUTL_REG);
  return (H<<8)+L;
}

short MPU_Get_Accelerometer_y()
{
  u8 H,L;
  H = iic_read(MPU_ACCEL_YOUTH_REG);
  L = iic_read(MPU_ACCEL_YOUTL_REG);
  return (H<<8)+L;
}

short MPU_Get_Accelerometer_z()
{
  u8 H,L;
  H = iic_read(MPU_ACCEL_ZOUTH_REG);
  L = iic_read(MPU_ACCEL_ZOUTL_REG);
  return (H<<8)+L;
}













