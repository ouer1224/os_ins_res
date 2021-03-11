#include"include.h"

//#define Port0_short					GPIO_Pin_2		//PC2
/*****************PORT1 端口**********************************************/
//#define Port1_short					GPIO_Pin_0		//PA0
/*****************PORT2 端口**********************************************/
//#define Port2_short					GPIO_Pin_1		//PA1
/*****************PORT3 端口**********************************************/
//#define Port3_short					GPIO_Pin_8		//PA8
//
void IN_PORT_init(void)
{
	 //输入口配置
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
	
	//输出口配置
	
	GPIO_InitStructure.GPIO_Pin =  Mac_set_out ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	Mac_set_out_H;							//初始化时都为高
	}

//**************************************************************************************************
//函数作用:读取输入信号状态
//参数说明:
//注意事项:其中去抖动延时可进行适当调节
//返回说明:返回一个8位数据
//**************************************************************************************************
u8 Read_port(void)
{
	return 0;
	}
//**************************************************************************************************
//函数作用:读取输入信号状态
//参数说明:
//注意事项:其中去抖动延时可进行适当调节
//返回说明:返回一个8位数据
//**************************************************************************************************
u8 Read_short(void)
{
	u8 port_bit=0;
	if(read_port0_short!=0)				//电源故障
	{
	//	delay_us(2);
		if(read_port0_short!=0)
		{
			port_bit |= 0x01;
		}
	}
	else port_bit &= 0xFE;
	
	if(read_port1_short!=0)				//电源故障
	{
	//	delay_us(2);
		if(read_port1_short!=0)
		{
			port_bit |= 0x02;
		}
	}
		else port_bit &= 0xFD;
	
	if(read_port2_short!=0)				//电源故障
	{
	//	delay_us(2);
		if(read_port2_short!=0)
		{
			port_bit |= 0x04;
		}
	}
		else port_bit &= 0xFB;	
	
// 	if(read_port3_short!=0)				//电源故障
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
//函数作用:DO输出数据
//参数说明:传递一个8位数据
//注意事项:输出后最好有个小延时再进行后面操作
//返回说明:
//**************************************************************************************************
void Out_port(u8 dat)
{
	
}
