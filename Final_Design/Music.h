#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "io430.h"
#include "in430.h"

//define note
#define _C 0
#define C_s 1
#define D 2
#define D_s 3
#define E 4
#define F 5
#define F_s 6
#define G 7
#define G_s 8
#define A 9
#define A_s 10
#define B 11
#define H_C 12
#define H_C_s 13
#define H_D 14
#define H_D_s 15
#define H_E 16
#define H_F 17
#define H_F_s 18
#define H_G 19
#define H_G_s 20
#define H_A 21
#define H_A_s 22
#define H_B 23
#define No 24


void Music_Init();
void Music();
void Delay_beat(unsigned char beat,char type);

#endif