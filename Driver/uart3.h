/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:uart1.h

�ļ�����:����UART1��ʼ��

�ļ�����:GuiYang

��������:2013��4��

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

//���֧�ֵı�����
#define MBUS_TOTAL_NUM		101
//������ݳ���
#define MBUS_DATA_LENGTH	32
//���ջ�����
#define CMD_BUF_SIZE		(MBUS_TOTAL_NUM*MBUS_DATA_LENGTH+100)//�࿪��һ��ռ����ڰ�ͷ
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


//ͨѶ���ݸ�ʽ  ��վ����
typedef  struct 
{ u8 addre  ; //վ��ַ
	u8 Length   	;//���ݰ�����
	u8 command  	;//����
	u8 data[5]	;//���ݰ�����
	u16 CRCValue ;	//16CRCУ��ֵ
}Receive_dat;

extern Receive_dat receive_data;


typedef struct  
{ 	u8 StationNo ; //վ��ַ
		u8 Length  ; //���ݳ���
		u8 command ; //����
		u8 Status ;  //����״̬00--�ɹ�
		u8 data[5] ;//�ظ�������
		u16 CRCValue ;// 16CRCУ��
	}Send_dat;

extern Send_dat send_data;
#endif
