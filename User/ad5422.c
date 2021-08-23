
/*
用于dac5422的驱动.可以使用如下两种方式:
1.普通模式,即单片模式,也就是spi的并联方式
2.菊花链模式,即spi总线的串联方式
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
	//拉高LATCH
	AD5410_LATCH_HIGH();

	for (i = 0; i < 0xff; i++)
	{
	}

	//拉低LATCH
	AD5410_LATCH_LOW();
}

fnPrv void AD5410xDisableOperate(void)
{
	uint32_t i = 0;

	//拉低LATCH
	AD5410_LATCH_LOW();

	for (i = 0; i < 0xff; i++)
	{
	}
	//拉高LATCH
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

	while ((SPI1->SR & (0x01 << 7)) != 0); //是否要加入,判断tx为空的标志,还是要判断busy标志.

	AD5410xDisableOperate();
}

/*8bit 写操作*/
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
	
	while ((SPI1->SR & (0x01 << 7)) != 0); //是否要加入,判断tx为空的标志,还是要判断busy标志.		

	AD5410xDisableOperate();

	return 0;
}

//! 其使用的时机是什么? 如果是在配置cf之前,那么数据如何传递下去?
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


/*以8bit + 菊花链的方式初始化5422*/
fnDri uint8_t init_ad5422_chain(void)
{

	uint16_t tmpreg = 0;
	uint8_t datChain[sizeofad542dat]={0};
	uint32_t i=0;
	GPIO_InitTypeDef GPIO_InitStructure; //用于设置GPIO口的基本参数

	/*---dac 的spi 的初始化*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTB时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI2时钟使能

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //选择最高的输出速度有2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数初始化一下
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			  //sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			  //MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			  //MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化

	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	GPIO_SetBits(GPIOA, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //是否有必要进行再次初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		 //latch
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	tmpreg = SPI1->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
						 (SPI_DataSize_8b) | (SPI_CPOL_Low) |
						 (SPI_NSS_Soft) | (SPI_CPHA_1Edge) | //似乎就得是 第1个沿
						 (SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI1->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI1->I2SCFGR &= ((uint16_t)0xF7FF);

	SPI1->CR1 |= (0x01 << 6);

	TaskDelay(2);

	//配置为 菊花链
	for (i = 0; i < numofad542; i++)
	{

		memset(datChain, 0, 3);

		datChain[0] = 0x55;
		datChain[1] = 0x31;
		datChain[2] = 0x00 | (0x01 << 3);
		//! 似乎只能 one by one 的进行配置.因为只有第1个配置后,才能传输数据给第2个.
		AD5410xWriteReg_chain(i,datChain[0],(datChain[1]<<8)|(datChain[2]));
	}

	return 1;
}

/*
菊花链的方式设置dac的输出
vol单位为mv*/
fnDri uint8_t set5422VolOut_chain(uint8_t id,uint32_t vol)
{
	uint8_t datChain[sizeofad542dat]={0};
	uint32_t i=0;

	memset(datChain, 0, sizeofad542dat);

	datChain[0] = 0x01;

	i= (0xffff/6000.0*vol);

	datChain[1] = (i >> 8)&0xff;
	datChain[2] = i &0xff;

	//! 似乎只能 one by one 的进行配置.因为只有第1个配置后,才能传输数据给第2个.
	AD5410xWriteReg_chain(id,datChain[0],(datChain[1]<<8)|(datChain[2]));

	return 1;
}




