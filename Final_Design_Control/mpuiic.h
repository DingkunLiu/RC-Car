#ifndef _MPUIIC_H_
#define _MPUIIC_H_

#include "io430.h"
#include "in430.h"
#include "main.h"

void iic_init(unsigned char SlaveAddr);
void iic_write(char slaveAd,char data);
unsigned char iic_read(unsigned char Add);

#endif