#include "include.h"
#include "uart3.h"
#include "ins_res.h"
#include "..\os\task.h"
u8 uart3_recv_buf[UART3_FIFO_SIZE];
//u8 uart3_recv_dat[UART3_FIFO_SIZE];
u8 DMA3_DR_BASE[UART3_FIFO_SIZE];
//u8 *DMA3_DT_BASE; 
 
u8 read_buff3;
u8 read_bit3;

//���ջ�����

u16 rfid3_recv_length=0;

u8 uart3_recv_fifo[UART3_FIFO_SIZE];			
u16 uart3_recv_fifo_in=0;
u16 uart3_recv_fifo_out=0;
u16 uart3_recv_fifo_out_backup=0;

extern u8 sn;
/*******************************************************************************
        ����I/O��Ӧ��ʼ��
*******************************************************************************/ 
void uart3_Gpio_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; //��������GPIO�ڵĻ�������

    GPIO_InitStructure.GPIO_Pin = RS485_RE1 ;//ѡ������Ǹ��ܽ�
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���
    GPIO_Init(RS485_ENPORT, &GPIO_InitStructure); //��������Ĳ�����ʼ��һ��


    GPIO_InitStructure.GPIO_Pin = TX3 ;//ѡ������Ǹ��ܽ�
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//���
    GPIO_Init(RS485_PORT1, &GPIO_InitStructure); //��������Ĳ�����ʼ��һ��

    GPIO_InitStructure.GPIO_Pin = RX3;//ѡ���11���ܽ�
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;// GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(RS485_PORT1, &GPIO_InitStructure);//��������Ĳ������г�ʼ��

}
/*******************************************************************************
        ����3 �����ж�
*******************************************************************************/
void UART3_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; //���������ж��������еĲ����Ľṹ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //NVIC_PriorityGroupConfig
	//ʹ�ܴ���3�ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  //ʹ�ܻ�ʹ���ض���ͨ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//���ô����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//NVIC_IRQChannel�ж���� IRQͨ����ʹ�ܻ���ʧ��
    NVIC_Init(&NVIC_InitStructure);//������������ý��г�ʼ��
}
 

/*****************************************************************
          USART3 ��ʼ�� baud ���������ò�����
*****************************************************************/
void USART3_Init(unsigned int baud)
{
    USART_InitTypeDef USART_InitStructure;//����USART�������õĽṹ��

    USART_InitStructure.USART_BaudRate = baud;//���ò����ʵ�ֵ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//һ֡����Ϊ��λ��
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //����һλ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;	//żУ��λUSART_Parity_No;//
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��ͺͽ���

    USART_Init(USART3, &USART_InitStructure); //��������Ĳ�������ʹ��
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
  	  
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); 	//ʹ��DMA�����ж�			
	USART_Cmd(USART3, ENABLE); //ʹ��USART3
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
          �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC); /* �巢����Ǳ�־��Transmission Complete flag */ 	
}


/*****************************************************************
          �� USART3 ����һ���ֽ�
*****************************************************************/
void USART3_SendByte(unsigned char temp)
{
	//RS485_ONE_SEND;
// 	delay_us(5000);
    USART_SendData(USART3, temp); //���õ�������
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); //�ȴ����ݷ�����
//	delay_us(1000);
//	RS485_ONE_RECEIVE;
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);  //���÷�����ɺ�����ж�
}

/*
 * ��������DMA_Config
 * ����  ��DMA ���ڵĳ�ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void DMA_Config_Channel2(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel2);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
  	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;	   
	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA3_DR_BASE;
	/*���򣺴��ڴ浽����*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE_UAER3;
	/*�����ַ����*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	/*�������ݵ�λ*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*�ڴ����ݵ�λ 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	/*DMAģʽ��һ�δ��䣬ѭ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	/*���ȼ�����*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/*����DMA1��4ͨ��*/		   
    DMA_Init(DMA1_Channel2, &DMA_InitStructure); 	
   
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�	 
	DMA_ClearFlag(DMA1_FLAG_TC2); 
}
 
/*
 * ��������NVIC_Config
 * ����  ��DMA �ж�����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void DMA1_chan2_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void Uart3_SendArray(u8 *pData,u16 Leng)
{
#if 0	
  RS485_ONE_SEND;
  DMA_Cmd(DMA1_Channel2, DISABLE);//��ֹͣDMA;ֹ֮ͣ����������ã�������Щ�̳�˵DMA���֮��,��ͣ���������Ǵ���ġ� 
  DMA1_Channel2->CMAR  = (u32)pData; //��������Դ��ַ   ����֮��Դ��ַ�Ѿ������ˣ�����Ҫ�޸Ļ��� 
  DMA1_Channel2->CNDTR = Leng;   //�������÷�������   ����֮��DMA�е�DMA_TX_LENG�Ѿ��ı���   
  DMA_Cmd(DMA1_Channel2, ENABLE);  /* Enable USART3 TX DMA1 Channel */
  USART_ClearFlag(USART3, USART_FLAG_TC); /* �巢����Ǳ�־��Transmission Complete flag */ 
#else
	RS485_ONE_SEND;

	while(Leng>0)
	{
		Leng--;
		USART3_SendByte(*pData);
		pData++;
	}
	task_sleep(1);

	RS485_ONE_RECEIVE;
#endif
  
}
//���ô���3 ���ա����;�ΪDMA��ʽ
void USART3_DMA_init(void)
{
//  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); /* Enable USART3 DMA Rxrequest */  
  USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); /* Enable USART3 DMA TX request */   
//  DMA_Cmd(DMA1_Channel2, ENABLE);  /* Enable USART3 TX DMA1 Channel */	  
//  DMA_Cmd(DMA1_Channel3, ENABLE);  /* Enable USART3 RX DMA1 Channel */   
  USART_Cmd(USART3, ENABLE); 
}

 
/**********************************************************
//��������://����1�жϽ���DMA���ͳ�ʼ��
//����˵��:	ʹ�ô���1�жϽ������ݣ�DMA_ch4ͨ���������ݣ����ݷ�����ɲ����ж�
//ע������:
//����˵��:��
***********************************************************/
void uart3_dma_init(void)
{
	uart3_Gpio_init();						//GPIO�ڶ�Ӧ��ʼ��	TX RX 485R/T
	USART3_Init(115200);	 //  1000000
	UART3_NVIC();						    //����һ�����жϳ�ʼ��
	DMA1_chan2_NVIC();						//DMA�жϳ�ʼ��
	DMA_Config_Channel2();					//DMA��ʼ��
	RS485_ONE_RECEIVE;
	}

//DMA�ж����
void DMA1_Channel2_IRQHandler(void)
{	
//�ж��Ƿ�ΪDMA��������ж�
   if(DMA_GetFlagStatus(DMA1_FLAG_TC2)==SET) 
   {  
	/* ���DMA��ɱ�־ */
	DMA_ClearFlag(DMA1_FLAG_TC2); 
	/* �ر�DMA���� */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
	}	
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
	 	uart3_recv_fifo[uart3_recv_fifo_in++]=USART_ReceiveData(USART3) ;
		if(uart3_recv_fifo_in>=UART3_FIFO_SIZE)uart3_recv_fifo_in=0;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
	 	USART_ClearITPendingBit(USART3, USART_IT_TC);
		//RS485_ONE_RECEIVE;
	}
}


u8 read_usart3_char(u8 *ch)
{
	if(uart3_recv_fifo_out==uart3_recv_fifo_in)return 0;
	*ch = uart3_recv_fifo[uart3_recv_fifo_out];						//�õ���ǰҪ��ȡ���ַ�
	uart3_recv_fifo_out++;											//ָ����һ��Ҫ��ȡ���ַ�
	if(uart3_recv_fifo_out==UART3_FIFO_SIZE)
		uart3_recv_fifo_out=0;	//�������fifoĩβ ���ش�ͷ��ʼ
	
	return 1;
	}
#if 0
u8 get_packet3(void)
{
	u8 temp;	
	u16 i,crc_j;
	static u8 fifo_buf[UART3_FIFO_SIZE];	
	static u32 timeout = 0;		   		//���ճ�ʱ������
	static u8  recv_status = 0;	   	//״̬��
	static u16 cmd_length = 0;	   	//����ͷ���������ݳ���
	static u16 uart3_crc = 0;		   	//����CRCУ��

	while(read_usart3_char(&temp))
	{
		switch(recv_status)
		{
			case 0:	//�������ݰ�ͷ
 					if(temp==saveparam.route_addr)		//��ַ��ͬ
					{	
						recv_status++;
						//uart3_recv_fifo_out_backup = uart3_recv_fifo_out;		
						timeout = get_clock();//������ճ�ʱ
						uart3_crc=0;//���У��λ
						rfid3_recv_length=0;//����ܳ���
						uart3_recv_buf[rfid3_recv_length++] = temp;//�򻺴�������
						if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//�ж��Ƿ񳬳�	
						{	rfid3_recv_length=0;
							recv_status=0;
							timeout=0;	
							uart3_recv_fifo_in=0;
							uart3_recv_fifo_out=0;
							return 0;
							}
					}  
					break;
			case 1:	//�������ݳ��� 1���ֽ�
					recv_status++;
					cmd_length = temp;
					uart3_recv_buf[rfid3_recv_length++] = temp;//�򻺴�������
					if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//�ж��Ƿ񳬳�	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}
					if(temp>=sizeof(uart3_recv_buf))	//�ж��Ƿ񳬳�	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}	
					break;
			case 2://��ʼ��������
  //					timeout =  get_clock();//������ճ�ʱ
					//û�н������ �ͻ�����
					uart3_recv_buf[rfid3_recv_length++]=temp;//�򻺴�������
					if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//�ж��Ƿ񳬳�	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}
					//������� ��ǰ�ֽھ���У��� ���У���
					//����У���
					if(rfid3_recv_length>cmd_length)			 //�ܳ�������ճ�����ͬ
					{
						timeout =  0;//������ճ�ʱ
						recv_status=0;
							//�����ж�����
						crc_j = uart3_recv_buf[rfid3_recv_length-1];
						crc_j<<= 8;
						crc_j = crc_j|uart3_recv_buf[rfid3_recv_length-2];	
						if(cmd_length<=2)
						{
							uart3_recv_fifo_in=0;
							uart3_recv_fifo_out=0;
							recv_status=0;
							timeout=0;	
							uart3_recv_fifo_in=0;
							uart3_recv_fifo_out=0;
							return 0;			//CRCУ��ʧ��
							}
						for(i=0;i<cmd_length-1;i++)
						{
							fifo_buf[i]=uart3_recv_buf[i+1];
						}
						uart3_crc =	CRC16(fifo_buf,cmd_length-2);
						if((uart3_crc ==crc_j )&&(uart3_recv_buf[0]==saveparam.route_addr))
						{
							uart3_recv_fifo_in=0;
							uart3_recv_fifo_out=0;	
							return 1;
							}
						else 
							{
								recv_status=0;
								timeout=0;	
								uart3_recv_fifo_in=0;
								uart3_recv_fifo_out=0;
								//uart3_recv_fifo_out = uart3_recv_fifo_out_backup ;
								return 0;			//CRCУ��ʧ��
						}
					}
					break;
				default:
					break;
			}
			if((get_clock()-timeout)>=320)
			{
				//���־
				recv_status=0;
				timeout=0;
				uart3_recv_fifo_in=0;
				uart3_recv_fifo_out=0;
				//��ǰ��ʼ���Ǽٵ� �ָ��ײ�fifo��ָ��
				//uart3_recv_fifo_out = uart3_recv_fifo_out_backup ;
				return 0;			//���ճ�ʱ
			}	
		} ///while()
					//�жϽ��ճ�ʱ
	   if(timeout)
		{
			//��ʱ˵�� ��ǰ��ʼ���Ǽٵ�
			if((get_clock()-timeout)>=120)
			{
				//���־
				recv_status=0;
				timeout=0;
				uart3_recv_fifo_in=0;
				uart3_recv_fifo_out=0;
				//��ǰ��ʼ���Ǽٵ� �ָ��ײ�fifo��ָ��
				//uart3_recv_fifo_out = uart3_recv_fifo_out_backup ;
				return 0;			//���ճ�ʱ
			}
		}
	 return 0;
	}
	
#else

u8 get_packet3(void)
{
		return 1;
}


#endif	
	





