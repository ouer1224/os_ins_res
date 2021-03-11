/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& hc574 ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:hc574.c

�ļ�����:hc574����

�ļ�����:Bai Bing

��������:2017��8��
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "

extern u8 read_data[8];

//***********************************************************************************************************************
//��������:HC574���ƹܽų�ʼ��
//����˵��:��
//ע������:
//����˵��:��
//***********************************************************************************************************************
void hc574_init(void)
{
//hc574���ƹܽŵ��������
  GPIO_InitTypeDef GPIO_InitStructure; 

	GPIO_InitStructure.GPIO_Pin = Out_CLK_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Out_CLK_2 | Out_CLK_3 | Out_CLK_4 | Out_CLK_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	Write_Relay_data(0,0,0,0,0);
}

//***********************************************************************************************************************
//��������:HC574���ݹܽ�����Ϊ���
//����˵��:��
//ע������:
//����˵��:��
//***********************************************************************************************************************
void Parallel_Port_Out(void)
{
	//hc574�������߹ܽ��������
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin = DB0 | DB1 | DB2 | DB3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DB6 | DB7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DB4 | DB5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//***********************************************************************************************************************
//��������:����Ƽ̵�����HC574д��������
//����˵��:��
//ע������:
//����˵��:��
//***********************************************************************************************************************
void Write_Relay_data(u8 OUT_REALY_DATA_1,u8 OUT_REALY_DATA_2,u8 OUT_REALY_DATA_3,u8 OUT_REALY_DATA_4,u8 OUT_REALY_DATA_5)
{
	Parallel_Port_Out();                         //������������Ϊ���
	Write_Parallel_Port(OUT_REALY_DATA_1);        //��1-8������̵������ݷ�������
	Out_CLK_1_LOW;
	delay_us(relay_time);
	Out_CLK_1_Hi;
	delay_us(relay_time);
	Out_CLK_1_LOW;
	delay_us(relay_time);
	Write_Parallel_Port(OUT_REALY_DATA_2);        //��9-16������̵������ݷ�������
	Out_CLK_2_LOW;
	delay_us(relay_time);
	Out_CLK_2_Hi;
	delay_us(relay_time);
	Out_CLK_2_LOW;
	delay_us(relay_time);
	Write_Parallel_Port(OUT_REALY_DATA_3);        //��17-24������̵������ݷ�������
	Out_CLK_3_LOW;
	delay_us(relay_time);
	Out_CLK_3_Hi;
	delay_us(relay_time);
	Out_CLK_3_LOW;
	delay_us(relay_time);
	Write_Parallel_Port(OUT_REALY_DATA_4);        //��25-32������̵������ݷ�������
	Out_CLK_4_LOW;
	delay_us(relay_time);
	Out_CLK_4_Hi;
	delay_us(relay_time);
	Out_CLK_4_LOW;
	delay_us(relay_time);
	Write_Parallel_Port(OUT_REALY_DATA_5);        //��33-40������̵������ݷ�������
	Out_CLK_5_LOW;
	delay_us(relay_time);
	Out_CLK_5_Hi;
	delay_us(relay_time);
	Out_CLK_5_LOW;
	delay_us(relay_time);
}
//***********************************************************************************************************************
//��������:д���������ϵ�����
//����˵��:��
//ע������:
//����˵��:��
//***********************************************************************************************************************
void Write_Parallel_Port(u8 Out_data)
{
  if((Out_data & 0X01) == 0x01)
	{
	DB0_Hi;
	}
	if((Out_data & 0X01) == 0)
	{
	DB0_Low;
	}
	if((Out_data & 0X02) == 0x02)
	{
	DB1_Hi;
	}
	if((Out_data & 0X02) == 0)
	{
	DB1_Low;
	}
	if((Out_data & 0X04) == 0x04)
	{
	DB2_Hi;
	}
	if((Out_data & 0X04) == 0)
	{
	DB2_Low;
	}
	if((Out_data & 0X08) == 0x08)
	{
	DB3_Hi;
	}
	if((Out_data & 0X08) == 0)
	{
	DB3_Low;
	}
	if((Out_data & 0X10) == 0x10)
	{
	DB4_Hi;
	}
	if((Out_data & 0X10) == 0)
	{
	DB4_Low;
	}
	if((Out_data & 0X20) == 0x20)
	{
	DB5_Hi;
	}
	if((Out_data & 0X20) == 0)
	{
	DB5_Low;
	}
	if((Out_data & 0X40) == 0x40)
	{
	DB6_Hi;
	}
	if((Out_data & 0X40) == 0)
	{
	DB6_Low;
	}
	if((Out_data & 0X80) == 0x80)
	{
	DB7_Hi;
	}
	if((Out_data & 0X80) == 0)
	{
	DB7_Low;
	}
}
