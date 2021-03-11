/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led 驱动&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:74HC165.c

文件作用:并入串出，读取外部拔码开关信号

文件作者:GuiYang

创建日期:2013年4月
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "

void HC165_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_Configuration();
	GPIO_InitStructure.GPIO_Pin = LD_165|CLK_165;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(HC165_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DATA_165 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(HC165_PORT, &GPIO_InitStructure);
}
	u16 r_dta165,i,o_data165;  	
	u8 bai,shi,ge;
u16 read_hc165(void)
{


	r_dta165 = o_data165 = 0;
	LD_165_LOW;
	delay_us(1000); 
	LD_165_HIGHT;
//	CLK_165_HIGHT;
	 for(i=0;i<16;i++)
	 {
	 	r_dta165 <<=1;
		
		if(read_DATA_165)
	 		r_dta165 |=1;  
		CLK_165_LOW;
		delay_us(10);
		CLK_165_HIGHT;
		delay_us(10);
	
		}
	LD_165_HIGHT;


	if((r_dta165&0x1000)!=0)o_data165|=0x8000;
	if((r_dta165&0x4000)!=0)o_data165|=0x4000;
	if((r_dta165&0x8000)!=0)o_data165|=0x2000;
	if((r_dta165&0x2000)!=0)o_data165|=0x1000;

	if((r_dta165&0x0100)!=0)o_data165|=0x0200;
	if((r_dta165&0x0400)!=0)o_data165|=0x0100;
	if((r_dta165&0x0800)!=0)o_data165|=0x0800;
	if((r_dta165&0x0200)!=0)o_data165|=0x0400;

	if((r_dta165&0x0010)!=0)o_data165|=0x0080;
	if((r_dta165&0x0040)!=0)o_data165|=0x0040;
	if((r_dta165&0x0080)!=0)o_data165|=0x0020;
	if((r_dta165&0x0020)!=0)o_data165|=0x0010;

	bai =  ((o_data165&0xf000)>>12)*100 ;
	shi =  ((o_data165&0x0f00)>>8)*10;
	ge =   (o_data165&0x00f0)>>4 ;
	o_data165 = bai+shi+ge;							  //转换为十六进制数据

//   o_data165>>= 4 ;
	return 	o_data165;
	}
