/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led 驱动&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:uart1.h

文件作用:串口UART1初始化

文件作者:GuiYang

创建日期:2013年4月

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#ifndef uart3_H
#define uart3_H

#define TX3  		GPIO_Pin_10		 //PB10
#define RX3  		GPIO_Pin_11		 //PB11

#define RS485_RE1  GPIO_Pin_12		 //PB12

#define RS485_PORT1	GPIOB

//#define RS485_DE1_HIGHT		GPIO_SetBits(RS485_PORT1, RS485_DE1)
//#define RS485_DE1_LOW 		GPIO_ResetBits(RS485_PORT1, RS485_DE1)

#define RS485_RE1_HIGHT		GPIO_SetBits(RS485_PORT1, RS485_RE1)
#define RS485_RE1_LOW 		GPIO_ResetBits(RS485_PORT1, RS485_RE1)

#define RS485_ONE_RECEIVE	RS485_RE1_LOW
#define RS485_ONE_SEND		RS485_RE1_HIGHT

#define USART3_DR_Base  0x40004804	

#define  SENDBUFF_SIZE  20
//#define RAM3_BASE       0x20000000
//#define RAM3_BB_BASE    0x23000000
#define UART3_FIFO_SIZE	 15

//最多支持的表数量
#define MBUS_TOTAL_NUM		101
//最大数据长度
#define MBUS_DATA_LENGTH	32
//接收缓冲区
#define CMD_BUF_SIZE		(MBUS_TOTAL_NUM*MBUS_DATA_LENGTH+100)//多开辟一块空间用于包头
static u8 rf_recv_buf[CMD_BUF_SIZE];

void USART3_Init(unsigned int baud);
void USART3_SendByte(unsigned char temp);
void NVIC3_Config(void);

void DMA_Config_Channel2(void);

void USART3_DMA_init(void);
void Uart3_SendArray(u8 *pData,u16 Leng);
void uart3_dma_init(void);
u8 read_usart3_char(u8 *ch);
u8 get_packet3(void);


//通讯数据格式  主站发送
typedef  struct 
{ u8 addre  ; //站地址
	u8 Length   	;//数据包长度
	u8 command  	;//命令
	u8 data[5]	;//数据包内容
	u16 CRCValue ;	//16CRC校验值
}Receive_dat;

extern Receive_dat receive_data;


typedef struct  
{ 	u8 StationNo ; //站地址
		u8 Length  ; //数据长度
		u8 command ; //命令
		u8 Status ;  //返回状态00--成功
		u8 data[5] ;//回复包内容
		u16 CRCValue ;// 16CRC校验
	}Send_dat;

extern Send_dat send_data;
#endif
