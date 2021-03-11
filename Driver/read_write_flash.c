#include "include.h"
FLASH_Status status = FLASH_COMPLETE;


//***********************************************************************************************************************
//��������:д��flash
//����˵��:start--��ʼ��ַ  length--����
//ע������:��������в��ܵ����κ����������ͳ��� ������ram�����оͻ��ܷ�
//����˵��:��
//***********************************************************************************************************************
u8 flash_write(u8 *buf, u32 start, u32 length)
{
	volatile u16 delay;
	u16 *mydata;
	volatile u32 irq_status0,irq_status1;
	

// 	//�����ж�
// 	irq_status0=NVIC->ENABLE[0];
// 	irq_status1=NVIC->ENABLE[1];
// 	
// 	//���ж�
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//����	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
// 	FLASH->KEYR = 0x45670123;
// 	FLASH->KEYR = 0xCDEF89AB;
		
	//���˫�ֽ�����
	length>>=1;
	mydata=(u16 *)buf;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//д������
		*((volatile u16 *)start)=*mydata;
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<1000;delay++);
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
	
		//ָ����һ������
		mydata++;
		start+=2;
  	}
	//����		
	FLASH_Lock();
	//����		
//	FLASH->CR|=(1<<7);
// 	
// 	//���ж�
// 	NVIC->ENABLE[0]=irq_status0;
// 	NVIC->ENABLE[1]=irq_status1;
	
	return 1;
}
//��ȡָ����ַFLASH����
u32  ReaddataFlash(u32 Address)
{
	u32 readdat;
	
//	RCC_Configuration();  
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	RCC_HSICmd(ENABLE);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2); //flash��������ʱ

	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_Unlock();
	readdat = *((u16 *)Address);

	FLASH_Lock();
	if(readdat!=0)
	readdat = 1;
	return readdat;			
}



//***********************************************************************************************************************
//��������:����flash
//����˵��:start--��ʼ��ַ  length--����
//ע������:��������в��ܵ����κ����������ͳ��� ������ram�����оͻ��ܷ�
//����˵��:��
//***********************************************************************************************************************
u8 flash_erase(u32 start, u32 length)
{
	volatile u16 delay;
	u32 sector_addr;
	volatile u32 irq_status0,irq_status1;

// 	//�����ж�
// 	irq_status0=NVIC->ENABLE[0];
// 	irq_status1=NVIC->ENABLE[1];
// 	
// 	//���ж�
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
// 	
	//����
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
  
	//ѭ��������ַ��Χ�ڵ�ÿһ������
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
	
	//����
	FLASH->CR|=(1<<7);
// 	//���ж�
// 	NVIC->ENABLE[0]=irq_status0;
// 	NVIC->ENABLE[1]=irq_status1;

	return 1;
}


//***********************************************************************************************************************
//��������:flash����---���ڳ���������� �ӱ���flash���µ�flash����ַ
//����˵��:
//ע������:��������в��ܵ����κ����������ͳ��� ������ram�����оͻ��ܷ�
//����˵��:��
//***********************************************************************************************************************
void flash_copy(u32 length)
{
	//���з�ɢ���ص�ram�еĺ���
	run_flash_copy_in_ram(length);
}



//***********************************************************************************************************************
//��������:flash����---���ڳ���������� �ӱ���flash���µ�flash����ַ
//����˵��:
//ע������:��������в��ܵ����κ����������ͳ��� ������ram�����оͻ��ܷ�
//����˵��:��
//***********************************************************************************************************************
void run_flash_copy_in_ram(u32 length)
{
	volatile u16 delay;
	volatile u32 sector_addr;
	u16 *mydata;
// 	
// 	//���ж�
// 	NVIC->DISABLE[0]=0xFFFFFFFF;
// 	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//����
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0xCDEF89AB;
			
	//ѭ��������ַ��Χ�ڵ�ÿһ������
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
		
	//���˫�ֽ����� ���п���
	sector_addr=FLASH_BASE_ADDR;
	length>>=1;
	mydata=(u16 *)FLASH_BACKUP_BASE;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//д������
		*((volatile u16 *)sector_addr)=*mydata;
		
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
		
  		//ָ����һ������
  		mydata++;
  		sector_addr+=2;
  	}
	dog_init(5);
	//���ÿ��Ź���λ
	while(1);
}
