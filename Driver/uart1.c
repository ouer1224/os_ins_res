#include"include.h"
#include "../os/task.h"
#include "../os/sem.h"

extern SemCB sem_uart3rcv;

//uint8_t SendBuff[SENDBUFF_SIZE];
u8 DMA_DR_BASE[UART1_FIFO_SIZE];
u8 *DMA_DT_BASE;
u16 rx_size;
u16 tx_size;
u8 read_buff;
u8 read_bit;

u8 uart1_recv_buf[UART1_FIFO_SIZE];
u8 uart1_recv_dat[UART1_FIFO_SIZE];
u16 uart1_recv_length = 0;

u8 uart1_recv_fifo[UART1_FIFO_SIZE];
u16 uart1_recv_fifo_in = 0;
u16 uart1_recv_fifo_out = 0;

u16 uart1_recv_fifo_out_backup = 0;






/*******************************************************************************
        ����I/O��Ӧ��ʼ��
*******************************************************************************/
void uart1_Gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //��������GPIO�ڵĻ�������
	GPIO_InitStructure.GPIO_Pin = TX ;//ѡ������Ǹ��ܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��������Ĳ�����ʼ��һ��
	GPIO_InitStructure.GPIO_Pin = RX;//ѡ���ʮ���ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��������Ĳ������г�ʼ��
}
/*******************************************************************************
        ȫ���ж��ڴ�����
*******************************************************************************/
void UART1_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; //���������ж��������еĲ����Ľṹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //NVIC_PriorityGroupConfig
	//ʹ�ܴ���1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //ʹ�ܻ�ʹ���ض���ͨ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//���ô����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//NVIC_IRQChannel�ж���� IRQͨ����ʹ�ܻ���ʧ��
	NVIC_Init(&NVIC_InitStructure);//������������ý��г�ʼ��
}
/*
 * ��������NVIC_Config
 * ����  ��DMA �ж�����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void DMA1_chan4_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*****************************************************************
          USART1 ��ʼ�� baud ������
*****************************************************************/
void USART1_Init(unsigned int baud)
{
	USART_InitTypeDef USART_InitStructure;//����USART�������õĽṹ��
	USART_InitStructure.USART_BaudRate = baud;//���ò����ʵ�ֵ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//һ֡����Ϊ��λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //����һλ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//û����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��ͺͽ���
	USART_Init(USART1, &USART_InitStructure); //��������Ĳ�������ʹ��
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE); //ʹ��USART1
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	      �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC); /* �巢����Ǳ�־��Transmission Complete flag */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 	//ʹ��DMA�����ж�
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	      �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/*****************************************************************
          �� USART1 ����һ���ֽ�
*****************************************************************/
void USART1_SendByte(unsigned char temp)
{
	USART_SendData(USART1, temp); //���õ�������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //�ȴ����ݷ�����
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//ʹ�ܷ�������ж�
}

/*
 * ��������DMA_Config
 * ����  ��DMA ���ڵĳ�ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void DMA_Config_Channel4(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_DR_BASE;
	/*���򣺴��ڴ浽����*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
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
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); //����DMA������ɺ�����ж�
}


/**********************************************************
//��������://����1�жϽ���DMA���ͳ�ʼ��
//����˵��:	ʹ�ô���1�жϽ������ݣ�DMA_ch4ͨ���������ݣ����ݷ�����ɲ����ж�
//ע������:
//����˵��:��
***********************************************************/
void uart1_dma_init(void)
{
	uart1_Gpio_init();						//GPIO�ڶ�Ӧ��ʼ��	TX RX 485R/T
	UART1_NVIC();						    //����һ�����жϳ�ʼ��
	DMA1_chan4_NVIC();						//DMA1_chan4�жϳ�ʼ��
	DMA_Config_Channel4();					//DMA��ʼ��
	USART1_Init(115200);
}
/***************************************************
����1�ж���ڳ���
***************************************************/

#define deep_bufrcv		8
uint8_t buf_uart1_rcv[deep_bufrcv+4];
uint32_t count_uart1_rcv=0;
uint32_t preT_u1rcv=0xffffffff,nowT_u1rcv=0;

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		isem_release(&sem_uart3rcv);
#if 1	
		uart1_recv_fifo[uart1_recv_fifo_in++] = USART_ReceiveData(USART1) ;
		if(uart1_recv_fifo_in >= UART1_FIFO_SIZE)
		{
			uart1_recv_fifo_in = 0;
		}
#else
		buf_uart1_rcv[count_uart1_rcv]=USART_ReceiveData(USART1);
		count_uart1_rcv++;
		if(count_uart1_rcv>=deep_bufrcv)
		{

		}
		else
		{

		}

		nowT_u1rcv=get_OS_time();
		if(preT_u1rcv!=nowT_u1rcv)
		{
			/**/
		}
		preT_u1rcv=nowT_u1rcv;


#endif
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
}
/***************************************************
����1�ж���ڳ���
***************************************************/
//DMA�ж����
void DMA1_Channel4_IRQHandler(void)
{
	//�ж��Ƿ�ΪDMA��������ж�
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
	{
		/* ���DMA��ɱ�־ */
		DMA_ClearFlag(DMA1_FLAG_TC4);
		/* �ر�DMA���� */
		DMA_Cmd(DMA1_Channel4, DISABLE);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);//ʹ�ܷ�������ж�
	}
}

u8 read_usart1_char(u8 *ch)
{
	if(uart1_recv_fifo_out == uart1_recv_fifo_in)
	{
		return 0;
	}
	*ch = uart1_recv_fifo[uart1_recv_fifo_out];						//�õ���ǰҪ��ȡ���ַ�
	uart1_recv_fifo_out++;											//ָ����һ��Ҫ��ȡ���ַ�
	if(uart1_recv_fifo_out == UART1_FIFO_SIZE)
	{
		uart1_recv_fifo_out = 0;	//�������fifoĩβ ���ش�ͷ��ʼ
	}
	return 1;
}

u8 get_packet1(void)
{
	u8 temp;
	static u8 fifo_buf[UART1_FIFO_SIZE];
	u16 i, crc_j;
	static u8  recv_status = 0;	 //״̬��
	static u16 cmd_length = 0;	 //����ͷ���������ݳ���
	static u32 timeout = 0;		 //���ճ�ʱ������
	static u16 uart1_crc = 0;		 //����CRCУ��
	//timeout1 = get_clock();
	while(read_usart1_char(&temp))
	{
		switch(recv_status)
		{
			case 0:	//�������ݰ�ͷ
				if(temp == 0x55)
				{
					recv_status++;
					//						uart1_recv_fifo_out_backup = uart1_recv_fifo_out;
					timeout = get_clock();//������ճ�ʱ
					uart1_crc = 0; //���У��λ
					uart1_recv_length = 0; //����ܳ���
					uart1_recv_buf[uart1_recv_length++] = temp;//�򻺴�������
					if(uart1_recv_length >= sizeof(uart1_recv_buf)) //�ж��Ƿ񳬳�
					{
						uart1_recv_length = 0;
						recv_status = 0;
						timeout = 0;
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						memset(uart1_recv_fifo, 0, sizeof(uart1_recv_fifo));
						return 0;
					}
				}
				break;
			case 1:	//�������ݳ��� 2���ֽ� ��λ��ǰ
				recv_status++;
				cmd_length = temp;
				uart1_recv_buf[uart1_recv_length++] = temp;//�򻺴�������
				if(uart1_recv_length >= sizeof(uart1_recv_buf))//�ж��Ƿ񳬳�
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				break;
			case 2:	//�������ݳ��ȵ�λ
				recv_status++;
				cmd_length <<= 8;
				cmd_length = cmd_length | temp;
				cmd_length += 2 ;						 //���ϰ�ͷ�ͽ����ֽ�
				uart1_recv_buf[uart1_recv_length++] = temp;//�򻺴�������
				if(uart1_recv_length >= sizeof(uart1_recv_buf))	//�ж��Ƿ񳬳�
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				break;
			case 3://��ʼ��������
				uart1_recv_buf[uart1_recv_length++] = temp; //�򻺴�������
				if(uart1_recv_length >= sizeof(uart1_recv_buf))	//�ж��Ƿ񳬳�
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				//������� ��ǰ�ֽھ���У��� ���У���
				//����У���
				if(uart1_recv_length >= cmd_length)			 //�ܳ�������ճ�����ͬ
				{
					timeout =  0;//������ճ�ʱ
					recv_status = 0;//״̬��
					//�����ж�����
					if(uart1_recv_buf[uart1_recv_length - 1] != 0XAA)
					{
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						recv_status = 0;
						timeout = 0;
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						return 0;			//��βУ��ʧ��
					}
					crc_j = uart1_recv_buf[uart1_recv_length - 2];
					crc_j <<= 8;
					crc_j = crc_j | uart1_recv_buf[uart1_recv_length - 3];
					if(cmd_length <= 3)
					{
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						recv_status = 0;
						timeout = 0;
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						return 0;			//CRCУ��ʧ��
					}
					for(i = 0 ; i < cmd_length - 4 ; i++)
					{
						fifo_buf[i] = uart1_recv_buf[i + 1];
					}
					uart1_crc =	CRC16(fifo_buf, cmd_length - 4);
					if(uart1_crc == crc_j)
					{
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						return 1;
					}
					else
					{
						recv_status = 0;
						timeout = 0;
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						return 0;			//CRCУ��ʧ��
					}
				}
				break;
			default:
				break;
		}
		if((get_clock() - timeout) >= 320)
		{
			//���־
			recv_status = 0;
			timeout = 0;
			uart1_recv_fifo_in = 0;
			uart1_recv_fifo_out = 0;
			//��ǰ��ʼ���Ǽٵ� �ָ��ײ�fifo��ָ��
			return 0;			//���ճ�ʱ
		}
	} ///while()
	//�жϽ��ճ�ʱ
	if(timeout)
	{
		//��ʱ˵�� ��ǰ��ʼ���Ǽٵ�
		if((get_clock() - timeout) >= 120)
		{
			//���־
			recv_status = 0;
			timeout = 0;
			uart1_recv_fifo_in = 0;
			uart1_recv_fifo_out = 0;
			//��ǰ��ʼ���Ǽٵ� �ָ��ײ�fifo��ָ��
			return 0;			//���ճ�ʱ
		}
	}
	return 0;
}

void Uart1_SendArray(u8 *pData,u16 Leng)
{
	while(Leng>0)
	{
		USART1_SendByte(*pData);
		Leng--;
		pData++;
	}

}


