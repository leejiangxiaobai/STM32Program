#include "stm32f10x.h"   // 相当于51单片机中的  #include<reg51.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "arm_math.h"


#define TEST_LENGTH_SAMPLES  542  //512+30     /* 采样点数 */ 强行加了一个周期的数据，以解决
#define BLOCK_SIZE           1     /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS             30     /* 滤波器系数个数 */
 

 
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;            /* 需要调用arm_fir_f32的次数 */
char floatTOchar [10] = ""; 

static float32_t testOutput[TEST_LENGTH_SAMPLES];               /* 滤波后的输出 */
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* 状态缓存，大小numTaps + blockSize - 1*/
static float32_t error[(NUM_TAPS/2)+1];       												//先前的误差存储
static float32_t nums[(NUM_TAPS/2)+1];       												//先前的num存储
int count ;                                                        //求解误差时的计数

static float32_t  *inputF32, *outputF32;
static arm_fir_instance_f32 S;

void arm_fir_f32_lp(float32_t num,int i);
void arm_fir_initialize (void);
void arm_fir_printf(void);			
			
/* 低通滤波器系数 通过fadtool获取*/
const float32_t firCoeffs32LP[NUM_TAPS] =   {
   0.004936690866748, 0.005626533676114, 0.007628538054799,  0.01086487508882,
    0.01519653586089,  0.02042971411469,  0.02632496655762,  0.03260870639446,
    0.03898646408759,  0.04515725627291,   0.0508283442016,  0.05572964026369,
    0.05962703640287,  0.06233398088215,  0.06372071727504,  0.06372071727504,
    0.06233398088215,  0.05962703640287,  0.05572964026369,   0.0508283442016,
    0.04515725627291,  0.03898646408759,  0.03260870639446,  0.02632496655762,
    0.02042971411469,  0.01519653586089,  0.01086487508882, 0.007628538054799,
   0.005626533676114, 0.004936690866748
};


/*
*********************************************************************************************************
*        函 数 名: arm_fir_f32_lp
*        功能说明: 调用函数arm_fir_f32_lp实现低通滤波器
*        形    参：无
*        返 回 值: 无
*********************************************************************************************************
*/

void arm_fir_initialize (void){
/* 初始化结构体S */
arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
 
}
void arm_fir_f32_lp(float32_t num,int i)
{
/* 初始化输入输出缓存指针 */
inputF32 = &num; //赋予原始数据的初始指针
outputF32 = &testOutput[0];

	
/* 实现一位的FIR滤波 */ 
arm_fir_f32(&S, inputF32, outputF32+i , blockSize);
	
	
	//获取采样误差值
	if(i >= 30 && i<TEST_LENGTH_SAMPLES-1){
		count = i%15;
		if(i >= 45){
			printf("%f \r\n", num - error[count]);
		}else{
			nums[count] = num;
		}
		error[count] = *(outputF32+i);
		
	}else if(i == TEST_LENGTH_SAMPLES - 1){
		//最后一个数的误差
		count = i%15;
		printf("%f \r\n", num - error[count]);
		
		for(int i = 0;i < 15;i++){
				if(count+i >=15){
					printf("%f \r\n", nums[i] - error[count+i-15]);
				}else{
					printf("%f \r\n", nums[i] - error[count+i]);
				}
		}
	}	
}

void arm_fir_printf(void){
	int i;
	/* 打印滤波后结果 */
for(i = TEST_LENGTH_SAMPLES - NUM_TAPS/2; i< TEST_LENGTH_SAMPLES; i++)
{
		sprintf(floatTOchar,"%.4f",testOutput[i]);
  	Usart_SendStr(DEBUG_USARTx,floatTOchar);
	  Usart_SendStr(DEBUG_USARTx,"\n");
    //printf("%f \r\n", testOutput[i]); //testInput_f32_50Hz_200Hz testOutput
}
/* 打印滤波后结果 */
for(i = NUM_TAPS ; i<TEST_LENGTH_SAMPLES - NUM_TAPS/2; i++) 
{
		sprintf(floatTOchar,"%.4f",testOutput[i]);
  	Usart_SendStr(DEBUG_USARTx,floatTOchar);
	  Usart_SendStr(DEBUG_USARTx,"\n");
    //printf("%f \r\n", testOutput[i]); //testInput_f32_50Hz_200Hz testOutput
}
}