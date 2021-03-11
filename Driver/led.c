/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:led.c

�ļ�����:ledָʾI/O�ڳ�ʼ��

�ļ�����:GuiYang

��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "
//��˸��ʱ��
static u32 led_blink_timer[1]={0};
u16 out_led1Pull=5000,out_led2Pull=5000,out_ledAllPull=10000;
void LED_init(void)
{
	 //LED�Ƶ��������
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
	error_led_out(0x00);					//����ERRORָʾ����
	}


//***********************************************************************************************************************
//��������:ledָʾ����˸
//����˵��:��
//ע������:
//����˵��:��
//***********************************************************************************************************************
void led_blink(u8 led_no, u16 on_time, u16 off_time)
{
	//�жϳ���Χ	
	if(led_no>=1)return;
	
	//�����ǰϨ�� �ж�ʱ�䵽���Ժ����
	if(GPIO_ReadOutputDataBit(MODE,MOD_LED))
	{
		if(checktimer(&led_blink_timer[led_no],off_time))
		{
			MOD_LED_Red;	
		}		
	}
	//�����ǰ���� �ж�ʱ�䵽���Ժ�Ϩ��
	else
	{
		if(checktimer(&led_blink_timer[led_no],on_time))
		{
			MOD_LED_Green;
		}
	}
}

//***********************************************************************************************************************
//��������:errorָʾ�����
//����˵��:��
//ע������:
//����˵��:��
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
