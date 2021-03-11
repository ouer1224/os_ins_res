//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& �ڲ�flash��� &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*�ļ�����:flash_copy_in_ram.c

//*�ļ�����:�ڲ�flash���

//*�ļ�����:��  ��

//*��������:2007��5��
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#include "include.h"



//***********************************************************************************************************************
//��������:flash����---���ڳ���������� �ӱ���flash���µ�flash����ַ
//����˵��:
//ע������:��������в��ܵ����κ����������ͳ��� ������ram�����оͻ��ܷ�
//����˵��:��
//***********************************************************************************************************************
void run_flash_copy_in_ram(uint32 length)
{
	volatile uint16 delay;
	volatile uint32 sector_addr;
	uint16 *mydata;
	
	//���ж�
	NVIC->DISABLE[0]=0xFFFFFFFF;
	NVIC->DISABLE[1]=0xFFFFFFFF;
	
	//����
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0xCDEF89AB;
			
	//ѭ��������ַ��Χ�ڵ�ÿһ������
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
		
	//���˫�ֽ����� ���п���
	sector_addr=FLASH_BASE_ADDR;
	length>>=1;
	mydata=(uint16 *)FLASH_BACKUP_BASE;
	while(length--)
	{
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//proceed to program the new data
		FLASH->CR|=(1<<0);
		//д������
		*((volatile uint16 *)sector_addr)=*mydata;
		
		//Wait for last operation to be completed
		while(FLASH->SR&(1<<0))for(delay=0;delay<500;delay++);
		
		//disable the PG Bit  exit program state
		FLASH->CR&=~(1<<0);
		
  		//ָ����һ������
  		mydata++;
  		sector_addr+=2;
  	}
	
	//���ÿ��Ź���λ
	while(1);
}


