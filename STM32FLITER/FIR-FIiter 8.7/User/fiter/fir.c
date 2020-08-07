#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include<reg51.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "arm_math.h"


#define TEST_LENGTH_SAMPLES  542  //512+30     /* �������� */ ǿ�м���һ�����ڵ����ݣ��Խ��
#define BLOCK_SIZE           1     /* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS             30     /* �˲���ϵ������ */
 

 
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;            /* ��Ҫ����arm_fir_f32�Ĵ��� */
char floatTOchar [10] = ""; 

static float32_t testOutput[TEST_LENGTH_SAMPLES];               /* �˲������� */
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/
static float32_t error[(NUM_TAPS/2)+1];       												//��ǰ�����洢
static float32_t nums[(NUM_TAPS/2)+1];       												//��ǰ��num�洢
int count ;                                                        //������ʱ�ļ���

static float32_t  *inputF32, *outputF32;
static arm_fir_instance_f32 S;

void arm_fir_f32_lp(float32_t num,int i);
void arm_fir_initialize (void);
void arm_fir_printf(void);			
			
/* ��ͨ�˲���ϵ�� ͨ��fadtool��ȡ*/
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
*        �� �� ��: arm_fir_f32_lp
*        ����˵��: ���ú���arm_fir_f32_lpʵ�ֵ�ͨ�˲���
*        ��    �Σ���
*        �� �� ֵ: ��
*********************************************************************************************************
*/

void arm_fir_initialize (void){
/* ��ʼ���ṹ��S */
arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
 
}
void arm_fir_f32_lp(float32_t num,int i)
{
/* ��ʼ�������������ָ�� */
inputF32 = &num; //����ԭʼ���ݵĳ�ʼָ��
outputF32 = &testOutput[0];

	
/* ʵ��һλ��FIR�˲� */ 
arm_fir_f32(&S, inputF32, outputF32+i , blockSize);
	
	
	//��ȡ�������ֵ
	if(i >= 30 && i<TEST_LENGTH_SAMPLES-1){
		count = i%15;
		if(i >= 45){
			printf("%f \r\n", num - error[count]);
		}else{
			nums[count] = num;
		}
		error[count] = *(outputF32+i);
		
	}else if(i == TEST_LENGTH_SAMPLES - 1){
		//���һ���������
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
	/* ��ӡ�˲����� */
for(i = TEST_LENGTH_SAMPLES - NUM_TAPS/2; i< TEST_LENGTH_SAMPLES; i++)
{
		sprintf(floatTOchar,"%.4f",testOutput[i]);
  	Usart_SendStr(DEBUG_USARTx,floatTOchar);
	  Usart_SendStr(DEBUG_USARTx,"\n");
    //printf("%f \r\n", testOutput[i]); //testInput_f32_50Hz_200Hz testOutput
}
/* ��ӡ�˲����� */
for(i = NUM_TAPS ; i<TEST_LENGTH_SAMPLES - NUM_TAPS/2; i++) 
{
		sprintf(floatTOchar,"%.4f",testOutput[i]);
  	Usart_SendStr(DEBUG_USARTx,floatTOchar);
	  Usart_SendStr(DEBUG_USARTx,"\n");
    //printf("%f \r\n", testOutput[i]); //testInput_f32_50Hz_200Hz testOutput
}
}