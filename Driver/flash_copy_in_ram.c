//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 内部flash编程 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*文件名称:flash_copy_in_ram.c

//*文件作用:内部flash编程

//*文件作者:翟  鹏

//*创建日期:2007年5月
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#include "include.h"



//***********************************************************************************************************************
//函数作用:flash拷贝---用于程序下载完毕 从备份flash更新到flash基地址
//参数说明:
//注意事项:这个函数中不能调用任何其他函数和常量 否则在ram中运行就会跑飞
//返回说明:无
//***********************************************************************************************************************
void run_flash_copy_in_ram(uint32 length)
{
	volatile uint16 delay;
	volatile uint32 sector_addr;
	uint16 *mydata;
	
	//关中断
	NVIC->DISABLE[0]=0xFFFFFFFF;
	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//解锁
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0xCDEF89AB;
			
	//循环擦除地址范围内的每一个扇区
	for(sector_addr=FLASH_BASE_ADDR;sector_addr<FLASH_BASE_ADDR+length;sector_addr+=SECTORSIZE)
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
	mydata=(uint16 *)FLASH_BACKUP_BASE;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//写入数据
		*((volatile uint16 *)sector_addr)=*mydata;
		
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
		
  		//指向下一个数据
  		mydata++;
  		sector_addr+=2;
  	}
	
	//利用看门狗复位
	while(1);
}


