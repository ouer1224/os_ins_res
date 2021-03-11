#include "include.h"
FLASH_Status status = FLASH_COMPLETE;


//***********************************************************************************************************************
//函数作用:写入flash
//参数说明:start--开始地址  length--长度
//注意事项:这个函数中不能调用任何其他函数和常量 否则在ram中运行就会跑飞
//返回说明:无
//***********************************************************************************************************************
u8 flash_write(u8 *buf, u32 start, u32 length)
{
	volatile u16 delay;
	u16 *mydata;
	volatile u32 irq_status0,irq_status1;
	

// 	//保存中断
// 	irq_status0=NVIC->ENABLE[0];
// 	irq_status1=NVIC->ENABLE[1];
// 	
// 	//关中断
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//解锁	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
// 	FLASH->KEYR = 0x45670123;
// 	FLASH->KEYR = 0xCDEF89AB;
		
	//变成双字节属性
	length>>=1;
	mydata=(u16 *)buf;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//写入数据
		*((volatile u16 *)start)=*mydata;
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
	
		//指向下一个数据
		mydata++;
		start+=2;
  	}
	//上锁		
	FLASH_Lock();
	//上锁		
//	FLASH->CR|=(1<<7);
// 	
// 	//开中断
// 	NVIC->ENABLE[0]=irq_status0;
// 	NVIC->ENABLE[1]=irq_status1;
	
	return 1;
}
//读取指定地址FLASH数据
u32  ReaddataFlash(u32 Address)
{
	u32 readdat;
	
//	RCC_Configuration();  
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	RCC_HSICmd(ENABLE);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2); //flash操作的延时

	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_Unlock();
	readdat = *((u16 *)Address);

	FLASH_Lock();
	if(readdat!=0)
	readdat = 1;
	return readdat;			
}



//***********************************************************************************************************************
//函数作用:擦除flash
//参数说明:start--开始地址  length--长度
//注意事项:这个函数中不能调用任何其他函数和常量 否则在ram中运行就会跑飞
//返回说明:无
//***********************************************************************************************************************
u8 flash_erase(u32 start, u32 length)
{
	volatile u16 delay;
	u32 sector_addr;
	volatile u32 irq_status0,irq_status1;

// 	//保存中断
// 	irq_status0=NVIC->ENABLE[0];
// 	irq_status1=NVIC->ENABLE[1];
// 	
// 	//关中断
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
// 	
	//解锁
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
  
	//循环擦除地址范围内的每一个扇区
	for(sector_addr=start;sector_addr<start+length;sector_addr+=SECTORSIZE)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//erase the page
		FLASH->CR|=(1<<1);
		FLASH->AR=sector_addr;
		FLASH->CR|=(1<<6);
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//disable the PER Bit  exit erase state
		FLASH->CR&=~(1<<1);
	}
	
	//上锁
	FLASH->CR|=(1<<7);
// 	//开中断
// 	NVIC->ENABLE[0]=irq_status0;
// 	NVIC->ENABLE[1]=irq_status1;

	return 1;
}


//***********************************************************************************************************************
//函数作用:flash拷贝---用于程序下载完毕 从备份flash更新到flash基地址
//参数说明:
//注意事项:这个函数中不能调用任何其他函数和常量 否则在ram中运行就会跑飞
//返回说明:无
//***********************************************************************************************************************
void flash_copy(u32 length)
{
	//运行分散加载到ram中的函数
	run_flash_copy_in_ram(length);
}



//***********************************************************************************************************************
//函数作用:flash拷贝---用于程序下载完毕 从备份flash更新到flash基地址
//参数说明:
//注意事项:这个函数中不能调用任何其他函数和常量 否则在ram中运行就会跑飞
//返回说明:无
//***********************************************************************************************************************
void run_flash_copy_in_ram(u32 length)
{
	volatile u16 delay;
	volatile u32 sector_addr;
	u16 *mydata;
// 	
// 	//关中断
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//解锁
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0xCDEF89AB;
			
	//循环擦除地址范围内的每一个扇区
	for(sector_addr=FLASH_BASE_ADDR;sector_addr<FLASH_BASE_ADDR+length/1024;sector_addr+=SECTORSIZE)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
  
		//proceed to erase the page
		FLASH->CR|=(1<<1);
		FLASH->AR=sector_addr;
		FLASH->CR|=(1<<6);
    
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		
		//disable the PER Bit
		FLASH->CR&=~(1<<1);
	}
		
	//变成双字节属性 进行拷贝
	sector_addr=FLASH_BASE_ADDR;
	length>>=1;
	mydata=(u16 *)FLASH_BACKUP_BASE;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//写入数据
		*((volatile u16 *)sector_addr)=*mydata;
		
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
		
  		//指向下一个数据
  		mydata++;
  		sector_addr+=2;
  	}
	dog_init(5);
	//利用看门狗复位
	while(1);
}
