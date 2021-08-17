
/*����adc7699������
ʹ��spi����
��оƬʹ��*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "ad7699.h"
#include "../os/task.h"


#define IN0 (0x3c49 << 2) // �����ԣ�ȫ�����ڲ���׼4.096V������ͨ�������������ض�CFG
#define IN1 (0x3cc9 << 2)
#define IN2 (0x3d49 << 2)
#define IN3 (0x3dc9 << 2)
#define IN4 (0x3e49 << 2)
#define IN5 (0x3ec9 << 2)
#define IN6 (0x3f49 << 2)
#define IN7 (0x3fc9 << 2)
#define M 8 // channel num



fnDri uint8_t init_ad7699(void)
{
	uint16_t tmpreg = 0;
	GPIO_InitTypeDef GPIO_InitStructure; //��������GPIO�ڵĻ�������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTBʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI2ʱ��ʹ��

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //��������Ĳ�����ʼ��һ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			  //sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			  //MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			  //MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //��������Ĳ������г�ʼ��

	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //�Ƿ��б�Ҫ�����ٴγ�ʼ��

	tmpreg = SPI2->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
						 (SPI_DataSize_16b) | (SPI_CPOL_Low) |
						 (SPI_NSS_Soft) | (SPI_CPHA_1Edge) | //�ƺ��͵��� ��1����
						 (SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI2->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI2->I2SCFGR &= ((uint16_t)0xF7FF);

	SPI2->CR1 |= (0x01 << 6);



	return 1;
}


fnDri uint8_t start_conver_ad7699(void)
{
	uint32_t i=0;

	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	for (i = 0; i < 10; i++)
	{
		//delay
	}

	GPIO_ResetBits(GPIOB, GPIO_Pin_12);

	TaskDelay(1);

	return 1;
}


fnPrv void spi16BitWriteCfgReadVal(SPI_TypeDef *spi ,uint16_t *pwbuf,uint16_t *prbuf,uint32_t len)
{
	uint32_t txsize = 0, rxsize = 0;
	uint8_t txallowed=0;
	uint32_t countrx=0,counttx=0;

	txsize=len;
	rxsize=len;

	while ((txsize > 0) || (rxsize > 0)) //�о�����һ���ȽϺõ�˼·,�����ڷ��͵�ͬʱ��ؽ���.
	{
		if ((txsize > 0) && (txallowed == 1) && ((spi->SR & 0x02) != 0))
		{
			spi->DR = pwbuf[counttx];
			counttx++;
			txsize--;
			txallowed = 0;
		}

		if ((rxsize > 0) && ((spi->SR & 0x01) != 0))
		{

			prbuf[countrx] = spi->DR;
			rxsize--;
			countrx++;
			txallowed = 1;
		}
	}

}


fnDri uint16_t LoopReadVal_7699(uint8_t id )
{
	uint16_t ad7699_cfg[M] = { IN1,IN2, IN3, IN4, IN5, IN6, IN7, IN0}; // CFG���У���data��1������
	uint16_t rxdata=0;

	spi16BitWriteCfgReadVal(SPI2,ad7699_cfg+id,&rxdata,1);

	return rxdata;
}

