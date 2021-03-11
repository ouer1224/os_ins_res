/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led 驱动&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:led.c

文件作用:led指示I/O口初始化

文件作者:GuiYang

创建日期:2013年4月
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "
//闪烁定时器
static u32 led_blink_timer[1]={0};
u16 out_led1Pull=5000,out_led2Pull=5000,out_ledAllPull=10000;
void LED_init(void)
{
	 //LED灯的输出配置
  GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_Configuration();
	GPIO_InitStructure.GPIO_Pin = MOD_LED | Port1_pin2_error | Port0_pin2_error | Port2_pin2_error ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MODE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Vin24_LED | Vout24_LED ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(V24_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Link ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	MOD_LED_Green;
	error_led_out(0x00);					//所有ERROR指示灯灭
	}


//***********************************************************************************************************************
//函数作用:led指示灯闪烁
//参数说明:无
//注意事项:
//返回说明:无
//***********************************************************************************************************************
void led_blink(u8 led_no, u16 on_time, u16 off_time)
{
	//判断超范围	
	if(led_no>=1)return;
	
	//如果当前熄灭 判断时间到了以后点亮
	if(GPIO_ReadOutputDataBit(MODE,MOD_LED))
	{
		if(checktimer(&led_blink_timer[led_no],off_time))
		{
			MOD_LED_Red;	
		}		
	}
	//如果当前是亮 判断时间到了以后熄灭
	else
	{
		if(checktimer(&led_blink_timer[led_no],on_time))
		{
			MOD_LED_Green;
		}
	}
}

//***********************************************************************************************************************
//函数作用:error指示灯输出
//参数说明:无
//注意事项:
//返回说明:无
//***********************************************************************************************************************
void error_led_out(u8 dat)
{
			if((dat&0x01)==0)
			{
				Port0_pin2_error_Green;
			}
			else Port0_pin2_error_Red;
			
			if((dat&0x02)==0)
			{
				Port1_pin2_error_Green ;
			}
			else Port1_pin2_error_Red;
			
			if((dat&0x04)==0)
			{
				Port2_pin2_error_Green ;
			}
			else Port2_pin2_error_Red;
			
		
// 			if((dat&0x80)==0)
// 			{
// 				Port3_pin2_error_Green ;
// 			}
// 			else Port3_pin2_error_Red;
// 			



}
