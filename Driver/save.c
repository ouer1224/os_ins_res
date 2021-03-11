//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&参数保存&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*文件名称:save.c

//*文件作用:参数保存

//*文件作者:

//*创建日期:2004年5月
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#include "include.h"



//参数保存地址
#define PARAM_ADDR	FLASH_PARAM_BASE
#define MODE_SPIDER	  0x15


//参数保存数据结构
saveparam_t saveparam;

//默认参数
static saveparam_t const defaultparam=
{
	MODE_SPIDER,//序列号		
	//0x01  0x02  0x03   0x04  0x05  0x06    0x07  0x08     0x09		(M8)	0x0A  		0x0B  		0x0C 			0x0D 					0X0E	
	//8DI   8DO   8AU    6DI   6DO   6DI/DO  4AI	  4AO    4DI4DO		(M8)	8DI(M8)   8DO(M8)   8AU(M8)   4DI4DO(M8)  	4-20ma/0-10V
	
	//0x10 		0x11  0x12   0x13  0x14  0x15   0x16  0x17      0x18        0x19
	//3AI     3VI   4AI   4VI    3AO   3VO    4AO   4VO    3点可配置   4点可配置
	0xff,//路由地址
	//校验和
	0,
};

//缓冲区
#define PTU_BUF_SIZE		1024
static u8 ptu_recv_buf[PTU_BUF_SIZE];

// static u16 ptu_recv_length=0;
// static char ptu_send_buf[128];
u8  PAddress;
//***********************************************************************************************************************
//函数作用:保存参数
//参数说明:
//注意事项:
//返回说明:保存成功返回1
//***********************************************************************************************************************
void save_param(void)
{
	u32 i;
   	u8 sum=0;
	u8 *p=(u8 *)&saveparam;

	//计算校验和
	for(i=0;i<sizeof(saveparam_t)-1;i++)sum+=p[i];
	p[i]=0x55-sum;
		
	//擦除flash
	flash_erase(PARAM_ADDR,sizeof(saveparam_t));
	//写入flash
	flash_write((u8 *)&saveparam,PARAM_ADDR,sizeof(saveparam_t));
}

//***********************************************************************************************************************
//函数作用:装载缺省
//参数说明:
//注意事项:
//返回说明:成功返回1
//***********************************************************************************************************************
void save_load_default(void)
{
	u32 i;
	u8 *p=(u8 *)&saveparam;
	u8 const *p_default=(u8 const *)&defaultparam;
	
	for(i=0;i<sizeof(saveparam_t);i++)p[i]=p_default[i];


}

//***********************************************************************************************************************
//函数作用:参数初始化
//参数说明:
//注意事项:
//返回说明:成功返回1
//***********************************************************************************************************************
void save_init(void)
{
	u32 i;
	u8 sum=0;
	u8 *p=(u8 *)&saveparam;

	
	//从flash中装载数据
	memcpy((u8 *)&saveparam,(u8 *)PARAM_ADDR,sizeof(saveparam_t));
	//核对校验和
	for(i=0;i<sizeof(saveparam_t);i++)sum+=p[i];
	//校验和正确
	if(0x55==sum)
	{
		//print_line("param ok");
		
	}
	else
	{
		//校验和错误 装载缺省
		save_load_default();
		//print_line("load default");
// 		save_param();
	}
}

//下载程序
static u8 wiat_char(u8 ch)
{
	u8 temp;
	u32 timeout=0;
	
	while(!DEBUG_RECEIVE_CHAR(&temp))
	{
		dog();
		if(++timeout>=3000000)
		{
			return 0;	
		}
	}
	if(temp!=ch)return 0;
	return 1;
}
static u8 receive(u8 *ch)
{
	u32 timeout=0;
	
	while(!DEBUG_RECEIVE_CHAR(ch))
	{
		dog();
		if(++timeout>=3000000)
		{
			return 0;	
		}
	}
	return 1;
}
	#define CLI() __set_PRIMASK(1) 
	#define SEI() __set_PRIMASK(0)
void downloadYmodem(void)
{
   
//	CLI();
//    SerialPutString("\r\n======================================================================");
//    SerialPutString("\r\n=              (C) COPYRIGHT 2010 STMicroelectronics                 =");
//    SerialPutString("\r\n=                                                                    =");
//    SerialPutString("\r\n=     In-Application Programming Application  (Version 3.3.0)        =");
//    SerialPutString("\r\n=                                                                    =");
//    SerialPutString("\r\n=                                   By MCD Application Team          =");
//    SerialPutString("\r\n======================================================================");
//    SerialPutString("\r\n\r\n");
//    Main_Menu ();

	PAddress = 0;				 
	SavaParamter();
	dog_init(5); 
	while(1);
	}
void download(void)
{
	u32 total_length;
	u32 recv_num=0;
	u8 temp;
	u16 i;
	u16 checksum_add0;//来自PC的累加校验和
	u16 checksum_xor0;//来自PC的异或校验和
	u16 checksum_add1=0;//自己计算的累加校验和
	u16 checksum_xor1=0;//自己计算的异或校验和

	DEBUG_RECEIVE_CHAR(&temp);//扫除垃圾
	DEBUG_RECEIVE_CHAR(&temp);//扫除垃圾
	DEBUG_RECEIVE_CHAR(&temp);//扫除垃圾


	//1.接收起始符
	if(!wiat_char('A'))return;//等待起始符1
	if(!wiat_char('R'))return;//等待起始符1
	if(!wiat_char('M'))return;//等待起始符1
	if(!wiat_char(0x07))return;//等待CPU型号代码
	if(!wiat_char(0x10))return;//等待CPU型号代码
	
	//2.接收同步码
	if(!wiat_char(0x55))return;
	if(!wiat_char(0xAA))return;
	
	//3.接收数据总长度
	if(!receive(&temp))return;
	total_length=(u32)temp;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<8;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<16;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<24;
	if(total_length>FLASH_BACKUP_LENGTH)
	{
		return;
	}
	//擦除备份flash扇区
	if(!flash_erase(FLASH_BACKUP_BASE,total_length))
	{

		return;
	}
	
	//4.循环接收数据包 一边发送一边计算校验和
	while(recv_num<total_length)
	 {
		u16 count;
		u8 encrypt;
                		
		//判断是否剩下不到一包
		if(total_length-recv_num>=PTU_BUF_SIZE)count=PTU_BUF_SIZE;
		else   count=total_length-recv_num;
                //接收数据到缓存
		for(i=0;i<count;i++)
		{
			if(!receive(&temp))return;
			ptu_recv_buf[i]=temp;
		}
//		//数据解密
		encrypt=ptu_recv_buf[0]^0x55;
		for(i=1;i<count;i++)ptu_recv_buf[i]^=encrypt;
		//把接收缓存的数据写入flash
		if(!flash_write(ptu_recv_buf,FLASH_BACKUP_BASE+recv_num,count))
		{
		
			return;
		}
		//计算校验和
		for(i=0;i<count;i++)checksum_add1+=*((u8 *)(FLASH_BACKUP_BASE+recv_num+i));//计算累加校验和
		for(i=0;i<count;i+=2)checksum_xor1^=*((u16 *)(FLASH_BACKUP_BASE+recv_num+i));//计算异或校验和
		//指向下一包数据
		recv_num+=count;
	}
	
	//5.接收来自PC的校验和
	if(!receive(&temp))return;
	checksum_add0=(u32)temp;
	if(!receive(&temp))return;
	checksum_add0|=(u32)temp<<8;
	if(!receive(&temp))return;
	checksum_xor0=(u32)temp;
	if(!receive(&temp))return;
	checksum_xor0|=(u32)temp<<8;
			
	//6.判断校验和
	if((checksum_add0!=checksum_add1) || (checksum_xor0!=checksum_xor1))
	{
		
		return;
	}

	delay_ms(100);//等待发送完毕
	
	//7.擦除参数区域
	//flash_erase(FLASH_PARAM_BASE,sizeof(saveparam_t));
	
	//8.进行程序搬移---从备份flash拷贝到运行flash 拷贝完毕利用看门狗复位
	flash_copy(total_length);
}


void  SavaParamter(void)
{	
	 
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(FLASHAddress);
	FLASH_ProgramWord(DP_FLASH_ADDRESS, PAddress);
//	FLASH_ProgramWord(DP_FLASH_NADDRESS, NPAddress);
   	FLASH_Lock();	
}
