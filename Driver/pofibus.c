/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:POFIBUS.c

�ļ�����:POFBUSͨѶ���ֳ��򣬰���74HC574��VPC3оƬ

�ļ�����:GuiYang

��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "

u8 pofibus_buff_in[POFIBUS_BYTE];
u8 pofibus_buff_out[POFIBUS_BYTE];
u8 profibu_read_bit=0;
GPIO_InitTypeDef GPIO_InitStructure; 
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:void POFBUS_init(void)
�ļ�����:74HC574��VPC3оƬ�ܽſ��Ƴ�ʼ��
�ļ�����:GuiYang
��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void POFBUS_gpio_init(void)
{
//VPC3���ƽ�����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin = WR_VPC3|RD_VPC3|RST_VPC3|ALE_VPC3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//VPC3 �жϽ�����Ϊ����ģʽ	 ��������
	GPIO_InitStructure.GPIO_Pin = NINT_VPC3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//75HC574���ƽ�����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin = OE_574 | CLK_574;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	EXTI_LINE7();
	NVIC_LINE7();
	}

void EXTI_LINE7(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);	   //PB7�ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
/*******************************************************************************
        �ж���NVIC�е�����
*******************************************************************************/
void NVIC_LINE7(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:void OUT_574_data(u8 data)
�ļ�����:74HC574�����������
�ļ�����:GuiYang
��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void OUT_574_data(u8 data)
{
	//75HC574���ݽ�����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 
									|GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	write_DAT_574(data);
	}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:u8 READ_574_data(void)
�ļ�����:74HC574���ڶ�������
�ļ�����:GuiYang
��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
u8 READ_574_data(void)
{
	u16 readDat;
	//75HC574���ݽ�����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 
									|GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	readDat = read_DAT_574&0x00ff;
	return readDat;
}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:vvoid write_vpc3_dat(u8 addres,u8 data)
�ļ�����:VPC3оƬд���ַ�����ݣ����а�����74HC574д����
�ļ�����:GuiYang
��������:2013��4��

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void write_vpc3_dat(u16 addres,u8 data)
{
	u8 add_low,add_high;
	add_low = (u8)addres;
	add_high = (u8)(addres>>8);
	WR_VPC3_H;						    //д����
	ALE_VPC3_L;
	ALE_VPC3_H;
	CLK_574_L;
 	OE_574_L;
	delay_us(100);
	OUT_574_data(add_high);			  	//���������574 Din
	CLK_574_H;							//CLK�����أ����������Qn
	OUT_574_data(add_low);	
	ALE_VPC3_L;
	delay_us(10);
	WR_VPC3_L;
	delay_us(10);
	OUT_574_data(data);
	delay_us(10);
	WR_VPC3_H;

}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:u8 read_vpc3_dat(u8 addres)
�ļ�����:VPC3оƬ�����������а�����74HC574������
�ļ�����:GuiYang
��������:2013��4��
_____|----|______________________|-----|_____			   ALE

-------\/---------------------\/---\/-----
_______/\_____________________/\___/\_____				   AB10--0

_______/------\________/------\_____/------\__ 
	   \______/		   \______/		\______/			   DB7--0

-----------------|___________|---------------|__		   XRD
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
u8 read_vpc3_dat(u16 addres)
{
	u8 add_low,add_high;
	u8 data;
	add_low = (u8)addres;
	add_high = (u8)(addres>>8);
	RD_VPC3_H;						    //������
	ALE_VPC3_L;
	ALE_VPC3_H;
	CLK_574_L;
 	OE_574_L;
	delay_us(100);
	OUT_574_data(add_high);			  	//���������574 Din
	CLK_574_H;							//CLK�����أ����������Qn
	OUT_574_data(add_low);	
	ALE_VPC3_L;
	delay_us(10);
	RD_VPC3_L;
	delay_us(10);
	data = read_DAT_574;
	delay_us(10);
	RD_VPC3_H;
	return data;
}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& POFIBUS����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:void VPC3_init(void)
�ļ�����:VPC3�����ʼ��
�ļ�����:GuiYang
��������:2013��4��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/  	
u16 pull_switch,hex_pull;	
void VPC3_init(void)
{
	
	pull_switch = read_hc165();	  //��ȡ���뿪����ֵ

	RST_VPC3_L;	
	delay_ms(10);
	RST_VPC3_H;					 //RESET
	write_vpc3_dat(0x0001,0x02);
	write_vpc3_dat(0x0001,0x02);
	}
u8 pofbus_out(u8 *data,u8 leng)
{
	return 1;	
}
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
       
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
}
