#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "arm_math.h"

#define TEST_LENGTH_SAMPLES  320    /* �������� */
#define BLOCK_SIZE           32     /* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS             29     /* �˲���ϵ������ */
 
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;            /* ��Ҫ����arm_fir_f32�Ĵ��� */
 
static float testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* ������ */
static float testOutput[TEST_LENGTH_SAMPLES];               /* �˲������� */
static float firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/


typedef struct
      {
          uint16_t numTaps;     /**< number of filter coefficients in the filter. */
      float *pState;      /**< points to the state variable array. The array is of length numTaps+blockSize-1. */
          float *pCoeffs;    /**< points to the coefficient array. The array is of length numTaps. */
      } arm_fir_instance_f32;
			
/* ��ͨ�˲���ϵ�� ͨ��fadtool��ȡ*/
const float firCoeffs32LP[NUM_TAPS] = {
  -0.001822523074f,  -0.001587929321f,  1.226008847e-18f,  0.003697750857f,  0.008075430058f,
  0.008530221879f,   -4.273456581e-18f, -0.01739769801f,   -0.03414586186f,  -0.03335915506f,
  8.073562366e-18f,  0.06763084233f,    0.1522061825f,     0.2229246944f,    0.2504960895f,
  0.2229246944f,     0.1522061825f,     0.06763084233f,    8.073562366e-18f, -0.03335915506f,
  -0.03414586186f,   -0.01739769801f,   -4.273456581e-18f, 0.008530221879f,  0.008075430058f,
  0.003697750857f,   1.226008847e-18f,  -0.001587929321f,  -0.001822523074f
};
/*
*********************************************************************************************************
*        �� �� ��: arm_fir_f32_lp
*        ����˵��: ���ú���arm_fir_f32_lpʵ�ֵ�ͨ�˲���
*        ��    �Σ���
*        �� �� ֵ: ��
*********************************************************************************************************
*/
static void arm_fir_f32_lp(void)
{
uint32_t i;
arm_fir_instance_f32 S;
float  *inputF32, *outputF32;
 
/* ��ʼ�������������ָ�� */
inputF32 = &testInput_f32_50Hz_200Hz[0];
outputF32 = &testOutput[0];
 
/* ��ʼ���ṹ��S */
arm_fir_init_f32(&S, NUM_TAPS, (float *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
 
/* ʵ��FIR�˲� */
for(i=0; i < numBlocks; i++)
{
arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
}
 
/* ��ӡ�˲����� */
for(i=0; i<TEST_LENGTH_SAMPLES; i++)
{
printf("%f\r\n", testOutput[i]);
}
}


