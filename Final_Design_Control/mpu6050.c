#include "mpu6050.h"
#define ms_cycle 1000
#define ms_100_delay 100000

u8 MPU_Init(void)
{ 
  u8 res;
  iic_init(SlaveAdr);//初始化IIC总线
  iic_write(MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
  __delay_cycles(ms_100_delay);
  iic_write(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
  MPU_Set_Gyro_Fsr(0);					//陀螺仪传感器,±250dps
  MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
  MPU_Set_Rate(125);						//设置采样率125Hz
  iic_write(MPU_INT_EN_REG,0X00);	//关闭所有中断
  iic_write(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
  iic_write(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
  iic_write(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
  res=iic_read(MPU_DEVICE_ID_REG);
  if(res==MPU_ADDR)//器件ID正确
  {
    iic_write(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
    iic_write(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
    MPU_Set_Rate(125);		        //设置采样率为125Hz
  }else return 1;
  return 0;
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_Gyro_Fsr(u8 fsr)
{
  iic_write(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_Accel_Fsr(u8 fsr)
{
  iic_write(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_LPF(u16 lpf)
{
  u8 data;
  if(lpf>=188)data=1;
  else if(lpf>=98)data=2;
  else if(lpf>=42)data=3;
  else if(lpf>=20)data=4;
  else if(lpf>=10)data=5;
  else data=6; 
  iic_write(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_Rate(u16 rate)
{
  u8 data;
  if(rate>1000)rate=1000;
  if(rate<4)rate=4;
  data=1000/rate-1;
  iic_write(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
  MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}


//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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













