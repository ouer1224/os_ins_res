/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led 驱动&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:uart1.h

文件作用:串口UART1初始化

文件作者:GuiYang

创建日期:2013年4月

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#ifndef uart1_H
#define uart1_H

#define TX  		GPIO_Pin_9		 //PA9
#define RX  		GPIO_Pin_10		 //PA10

#define  SENDBUFF_SIZE  600
#define  UART1_FIFO_SIZE  600

/* RAM 地址*/

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
