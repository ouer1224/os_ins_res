#include"include.h"

//#define Port0_short					GPIO_Pin_2		//PC2
/*****************PORT1 �˿�**********************************************/
//#define Port1_short					GPIO_Pin_0		//PA0
/*****************PORT2 �˿�**********************************************/
//#define Port2_short					GPIO_Pin_1		//PA1
/*****************PORT3 �˿�**********************************************/
//#define Port3_short					GPIO_Pin_8		//PA8
//
void IN_PORT_init(void)
{
	 //���������
  GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_Configuration();
// 	GPIO_InitStructure.GPIO_Pin = Port0_short   ;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Mac_set_in |choose3_port;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  choose1_port |choose2_port;//Port3_short | Port2_short | Port1_short |
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���������
	
	GPIO_InitStructure.GPIO_Pin =  Mac_set_out ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	Mac_set_out_H;							//��ʼ��ʱ��Ϊ��
	}

//**************************************************************************************************
//��������:��ȡ�����ź�״̬
//����˵��:
//ע������:����ȥ������ʱ�ɽ����ʵ�����
//����˵��:����һ��8λ����
//**************************************************************************************************
u8 Read_port(void)
{
	return 0;
	}
//**************************************************************************************************
//��������:��ȡ�����ź�״̬
//����˵��:
//ע������:����ȥ������ʱ�ɽ����ʵ�����
//����˵��:����һ��8λ����
//**************************************************************************************************
u8 Read_short(void)
{
	u8 port_bit=0;
	if(read_port0_short!=0)				//��Դ����
	{
	//	delay_us(2);
		if(read_port0_short!=0)
		{
			port_bit |= 0x01;
		}
	}
	else port_bit &= 0xFE;
	
	if(read_port1_short!=0)				//��Դ����
	{
	//	delay_us(2);
		if(read_port1_short!=0)
		{
			port_bit |= 0x02;
		}
	}
		else port_bit &= 0xFD;
	
	if(read_port2_short!=0)				//��Դ����
	{
	//	delay_us(2);
		if(read_port2_short!=0)
		{
			port_bit |= 0x04;
		}
	}
		else port_bit &= 0xFB;	
	
// 	if(read_port3_short!=0)				//��Դ����
// 	{
// 	//	delay_us(2);
// 		if(read_port3_short!=0)
// 		{
// 			port_bit |= 0x08;
// 		}
// 	}		
// 	else port_bit &= 0xF7;
	return port_bit;
	}
//**************************************************************************************************
//��������:DO�������
//����˵��:����һ��8λ����
//ע������:���������и�С��ʱ�ٽ��к������
//����˵��:
//**************************************************************************************************
void Out_port(u8 dat)
{
	
}
