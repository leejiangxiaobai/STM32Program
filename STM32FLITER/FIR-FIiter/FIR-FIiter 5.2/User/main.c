#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "stdio.h"
#include "bsp_usart.h"


void Delay( uint32_t count )
{
	for(; count!=0; count--);
}

int cur = 0;

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	LED_GPIO_Config();
	KEY_GPIO_Config();
	USART_Config();
	
	while(1)
	{
//		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) ==KEY_ON )
//			LED_G_TOGGLE;
//			LED_G(OFF);
//			Delay(0xFFFFF);
//		  
//			LED_G(ON);
			Delay(0xFFFFF);		
			printf("cur =  %d \r\n",cur);
			cur++;
			
		
		
		// if
	}
}

// ��ҵ

// ��KEY2�Ĵ���Ҳ��ɣ���һ�Σ�LED_R_TOGGLE

