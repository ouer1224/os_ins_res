
/*
����dac5422������.����ʹ���������ַ�ʽ:
1.��ͨģʽ,����Ƭģʽ,Ҳ����spi�Ĳ�����ʽ
2.�ջ���ģʽ,��spi���ߵĴ�����ʽ
*/


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "../os/task.h"
#include "ad5422.h"


#define AD5410_LATCH_HIGH() GPIOA->BSRR = GPIO_Pin_4
#define AD5410_LATCH_LOW() GPIOA->BRR = GPIO_Pin_4


fnPrv void AD5410xEnableOperate(void)
{
	uint32_t i = 0;
	//����LATCH
	AD5410_LATCH_HIGH();

	for (i = 0; i < 0xff; i++)
	{
	}

	//����LATCH
	AD5410_LATCH_LOW();
}

fnPrv void AD5410xDisableOperate(void)
{
	uint32_t i = 0;

	//����LATCH
	AD5410_LATCH_LOW();

	for (i = 0; i < 0xff; i++)
	{
	}
	//����LATCH
	AD5410_LATCH_HIGH();
}
#define numofad542 8
#define sizeofad542dat 3
fnPrv __inline void spiWriteData(uint8_t *val, uint32_t len)
{

	uint32_t i = 0;
	uint8_t txsize = 0, rxsize = 0;
	uint8_t txallowed = 0;

	AD5410xEnableOperate();

	txsize = len;
	rxsize = 0;
	txallowed = 1;

	for (i = 0; i < len;)
	{
		if ((SPI1->SR & 0x02) != 0)
		{
			SPI1->DR = val[i];
			i++;
		}
	}

	while ((SPI1->SR & (0x01 << 7)) != 0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.

	AD5410xDisableOperate();
}

/*8bit д����*/
fnDri uint8_t AD5410xWriteReg_chain(uint8_t id, uint8_t RegAddr, uint16_t value)
{

	uint8_t SendData[sizeofad542dat * numofad542] = {0};
	uint8_t j=0;
	uint32_t i = 0;
	uint8_t txsize = 0, rxsize = 0;
	uint8_t txallowed = 0;
	uint8_t len=0;

	memset(SendData, 0, sizeof(SendData));

	AD5410xEnableOperate();

	SendData[(8 - 1 - id) * sizeofad542dat + 0] = RegAddr;
	SendData[(8 - 1 - id) * sizeofad542dat + 1] = value >> 8;
	SendData[(8 - 1 - id) * sizeofad542dat + 2] = value & 0xff;


	for(j=0;j<8;j++)
	{
		len=3;
		txsize = len;
		rxsize = 0;
		txallowed = 1;

		for (i = 0; i < len;)
		{
			if ((SPI1->SR & 0x02) != 0)
			{
				SPI1->DR = SendData[j*3+i];
				i++;
			}
		}


	}
	
	while ((SPI1->SR & (0x01 << 7)) != 0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.		

	AD5410xDisableOperate();

	return 0;
}

//! ��ʹ�õ�ʱ����ʲô? �����������cf֮ǰ,��ô������δ�����ȥ?
fnPrv uint8_t AD5410xSoftReset(void)
{
	uint8_t datChain[sizeofad542dat]={0};
	uint32_t i=0;
	for (i = 0; i < numofad542; i++)
	{
		memset(datChain, 0, sizeofad542dat);

		datChain[0] = 0x56;
		datChain[1] = 0;
		datChain[2] = 1;

		AD5410xWriteReg_chain(i,datChain[0],(datChain[1]<<8)|(datChain[2]));
	}
	return 1;
}


/*��8bit + �ջ����ķ�ʽ��ʼ��5422*/
fnDri uint8_t init_ad5422_chain(void)
{

	uint16_t tmpreg = 0;
	uint8_t datChain[sizeofad542dat]={0};
	uint32_t i=0;
	GPIO_InitTypeDef GPIO_InitStructure; //��������GPIO�ڵĻ�������

	/*---dac ��spi �ĳ�ʼ��*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTBʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI2ʱ��ʹ��

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��������Ĳ�����ʼ��һ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			  //sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			  //MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			  //MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��

	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	GPIO_SetBits(GPIOA, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //�Ƿ��б�Ҫ�����ٴγ�ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		 //latch
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	tmpreg = SPI1->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
						 (SPI_DataSize_8b) | (SPI_CPOL_Low) |
						 (SPI_NSS_Soft) | (SPI_CPHA_1Edge) | //�ƺ��͵��� ��1����
						 (SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI1->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI1->I2SCFGR &= ((uint16_t)0xF7FF);

	SPI1->CR1 |= (0x01 << 6);

	TaskDelay(2);

	//����Ϊ �ջ���
	for (i = 0; i < numofad542; i++)
	{

		memset(datChain, 0, 3);

		datChain[0] = 0x55;
		datChain[1] = 0x31;
		datChain[2] = 0x00 | (0x01 << 3);
		//! �ƺ�ֻ�� one by one �Ľ�������.��Ϊֻ�е�1�����ú�,���ܴ������ݸ���2��.
		AD5410xWriteReg_chain(i,datChain[0],(datChain[1]<<8)|(datChain[2]));
	}

	return 1;
}

/*
�ջ����ķ�ʽ����dac�����
vol��λΪmv*/
fnDri uint8_t set5422VolOut_chain(uint8_t id,uint32_t vol)
{
	uint8_t datChain[sizeofad542dat]={0};
	uint32_t i=0;

	memset(datChain, 0, sizeofad542dat);

	datChain[0] = 0x01;

	i= (0xffff/6000.0*vol);

	datChain[1] = (i >> 8)&0xff;
	datChain[2] = i &0xff;

	//! �ƺ�ֻ�� one by one �Ľ�������.��Ϊֻ�е�1�����ú�,���ܴ������ݸ���2��.
	AD5410xWriteReg_chain(id,datChain[0],(datChain[1]<<8)|(datChain[2]));

	return 1;
}




