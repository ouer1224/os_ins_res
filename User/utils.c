/*
��app�ṩ�����м亯�����߹��ܴ�����.
*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "utils.h"
#include "../os/task.h"

/*
��16λ���ݵ�ƽ����ֵ.
����ֵ: 0xffff ffff ��ʾerr.������ֵ��ʾ ok
*/
uint32_t get_filterVal_avr(uint16_t * nums,uint32_t len)
{
	uint32_t avr=0;
	uint32_t i=0;

	if(len==0)
	{
		return 0xffffffff;
	}

	for(i=0;i<len;i++)
	{
		avr+=nums[i];
	}

	avr/=len;

	return avr;
}
