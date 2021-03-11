/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:uart1.h

�ļ�����:����UART1��ʼ��

�ļ�����:GuiYang

��������:2013��4��

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#ifndef uart1_H
#define uart1_H

#define TX  		GPIO_Pin_9		 //PA9
#define RX  		GPIO_Pin_10		 //PA10

#define  SENDBUFF_SIZE  600
#define  UART1_FIFO_SIZE  600

/* RAM ��ַ*/

#define USART1_DR_Base  0x40013804

//#define RAM_BASE       0x20000000
//#define RAM_BB_BASE    0x22000000
void USART1_Init(unsigned int baud);
void uart1_Gpio_init(void);
void DMA_Config_Channel4(void);
void USART1_SendByte(unsigned char temp);
void Uart1_SendArray(u8 *pData,u16 Leng);
void uart1_dma_init(void);
u8 get_packet1(void);

#endif
