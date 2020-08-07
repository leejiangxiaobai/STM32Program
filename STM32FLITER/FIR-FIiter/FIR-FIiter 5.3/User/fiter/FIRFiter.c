#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "stdio.h"
#include "bsp_usart.h"

#ifndef _ARM_MATH_H
#define _ARM_MATH_H

#include "arm_math.h"


#endif


const int BL = 29;//阶数
const double B[29] = {//系数
  -0.001822523074,-0.001587929321,1.226008847e-18, 0.003697750857, 0.008075430058,
   0.008530221879,-4.273456581e-18, -0.01739769801, -0.03414586186, -0.03335915506,
  8.073562366e-18,  0.06763084233,   0.1522061825,   0.2229246944,   0.2504960895,
     0.2229246944,   0.1522061825,  0.06763084233,8.073562366e-18, -0.03335915506,
   -0.03414586186, -0.01739769801,-4.273456581e-18, 0.008530221879, 0.008075430058,
   0.003697750857,1.226008847e-18,-0.001587929321,-0.001822523074
};



double Buffer[BL];//采样历史数据




double Fir(double Input)
{
double Output=0.0;//数据输出
unsigned int Index;//下标索引

 //采样数据移位
for(Index=BL;Index>0;Index--){
Buffer[Index]=Buffer[Index-1];
}

Buffer[0]=Input;

//计算输出
for(Index=0;Index<BL+1;Index++)
{
Output+=B[Index]*Buffer[Index];
}

return Output;
}
