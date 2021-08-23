/*
向app提供部分中间函数或者功能处理函数.
*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "utils.h"
#include "../os/task.h"

/*
求16位数据的平均数值.
返回值: 0xffff ffff 表示err.其他数值表示 ok
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
