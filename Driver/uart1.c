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
        串口I/O对应初始化
*******************************************************************************/
void uart1_Gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //用于设置GPIO口的基本参数
	GPIO_InitStructure.GPIO_Pin = TX ;//选择的是那个管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择最高的输出速度有2MHZ,10MHZ,50MHZ,
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//
	GPIO_Init(GPIOA, &GPIO_InitStructure); //按照上面的参数初始化一下
	GPIO_InitStructure.GPIO_Pin = RX;//选择第十个管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//按照上面的参数进行初始化
}
/*******************************************************************************
        全部中断在此配置
*******************************************************************************/
void UART1_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; //用于设置中断向量表中的参数的结构体
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //NVIC_PriorityGroupConfig
	//使能串口1中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //使能或使能特定的通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//设置从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//NVIC_IRQChannel中定义的 IRQ通道被使能还是失能
	NVIC_Init(&NVIC_InitStructure);//按照上面的配置进行初始化
}
/*
 * 函数名：NVIC_Config
 * 描述  ：DMA 中断配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void DMA1_chan4_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* 配置P[A|B|C|D|E]0为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*****************************************************************
          USART1 初始化 baud 波特率
*****************************************************************/
void USART1_Init(unsigned int baud)
{
	USART_InitTypeDef USART_InitStructure;//用于USART参数设置的结构体
	USART_InitStructure.USART_BaudRate = baud;//设置波特率的值
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//一帧设置为八位的
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //设置一位的停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//没有奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送和接收
	USART_Init(USART1, &USART_InitStructure); //按照上面的参数进行使能
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接收中断
	USART_Cmd(USART1, ENABLE); //使能USART1
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	      如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 	//使能DMA发送中断
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	      如下语句解决第1个字节无法正确发送出去的问题 */
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/*****************************************************************
          从 USART1 发送一个字节
*****************************************************************/
void USART1_SendByte(unsigned char temp)
{
	USART_SendData(USART1, temp); //设置单个数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //等待数据发送完
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//使能发送完成中断
}

/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void DMA_Config_Channel4(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/*设置DMA源：内存地址&串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_DR_BASE;
	/*方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/*DMA模式：一次传输，循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	/*优先级：高*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/*配置DMA1的4通道*/
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
}


/**********************************************************
//函数作用://串口1中断接收DMA发送初始化
//参数说明:	使用串口1中断接收数据，DMA_ch4通道发送数据，数据发送完成产生中断
//注意事项:
//返回说明:无
***********************************************************/
void uart1_dma_init(void)
{
	uart1_Gpio_init();						//GPIO口对应初始化	TX RX 485R/T
	UART1_NVIC();						    //串口一接收中断初始化
	DMA1_chan4_NVIC();						//DMA1_chan4中断初始化
	DMA_Config_Channel4();					//DMA初始化
	USART1_Init(115200);
}
/***************************************************
串口1中断入口程序
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
串口1中断入口程序
***************************************************/
//DMA中断入口
void DMA1_Channel4_IRQHandler(void)
{
	//判断是否为DMA发送完成中断
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
	{
		/* 清除DMA完成标志 */
		DMA_ClearFlag(DMA1_FLAG_TC4);
		/* 关闭DMA传输 */
		DMA_Cmd(DMA1_Channel4, DISABLE);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);//使能发送完成中断
	}
}

u8 read_usart1_char(u8 *ch)
{
	if(uart1_recv_fifo_out == uart1_recv_fifo_in)
	{
		return 0;
	}
	*ch = uart1_recv_fifo[uart1_recv_fifo_out];						//得到当前要读取的字符
	uart1_recv_fifo_out++;											//指向下一个要读取的字符
	if(uart1_recv_fifo_out == UART1_FIFO_SIZE)
	{
		uart1_recv_fifo_out = 0;	//如果到了fifo末尾 则重从头开始
	}
	return 1;
}

u8 get_packet1(void)
{
	u8 temp;
	static u8 fifo_buf[UART1_FIFO_SIZE];
	u16 i, crc_j;
	static u8  recv_status = 0;	 //状态机
	static u16 cmd_length = 0;	 //除包头外所有数据长度
	static u32 timeout = 0;		 //接收超时计数器
	static u16 uart1_crc = 0;		 //数据CRC校验
	//timeout1 = get_clock();
	while(read_usart1_char(&temp))
	{
		switch(recv_status)
		{
			case 0:	//接收数据包头
				if(temp == 0x55)
				{
					recv_status++;
					//						uart1_recv_fifo_out_backup = uart1_recv_fifo_out;
					timeout = get_clock();//清除接收超时
					uart1_crc = 0; //清除校验位
					uart1_recv_length = 0; //清除总长度
					uart1_recv_buf[uart1_recv_length++] = temp;//向缓存送数据
					if(uart1_recv_length >= sizeof(uart1_recv_buf)) //判断是否超长
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
			case 1:	//接收数据长度 2个字节 高位在前
				recv_status++;
				cmd_length = temp;
				uart1_recv_buf[uart1_recv_length++] = temp;//向缓存送数据
				if(uart1_recv_length >= sizeof(uart1_recv_buf))//判断是否超长
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				break;
			case 2:	//接收数据长度低位
				recv_status++;
				cmd_length <<= 8;
				cmd_length = cmd_length | temp;
				cmd_length += 2 ;						 //加上包头和结束字节
				uart1_recv_buf[uart1_recv_length++] = temp;//向缓存送数据
				if(uart1_recv_length >= sizeof(uart1_recv_buf))	//判断是否超长
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				break;
			case 3://开始接收数据
				uart1_recv_buf[uart1_recv_length++] = temp; //向缓存送数据
				if(uart1_recv_length >= sizeof(uart1_recv_buf))	//判断是否超长
				{
					uart1_recv_length = 0;
					recv_status = 0;
					timeout = 0;
					uart1_recv_fifo_in = 0;
					uart1_recv_fifo_out = 0;
					return 0;
				}
				//接收完毕 当前字节就是校验和 检查校验和
				//计算校验和
				if(uart1_recv_length >= cmd_length)			 //总长度与接收长度相同
				{
					timeout =  0;//清除接收超时
					recv_status = 0;//状态机
					//进行判读数据
					if(uart1_recv_buf[uart1_recv_length - 1] != 0XAA)
					{
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						recv_status = 0;
						timeout = 0;
						uart1_recv_fifo_in = 0;
						uart1_recv_fifo_out = 0;
						return 0;			//包尾校验失败
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
						return 0;			//CRC校验失败
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
						return 0;			//CRC校验失败
					}
				}
				break;
			default:
				break;
		}
		if((get_clock() - timeout) >= 320)
		{
			//清标志
			recv_status = 0;
			timeout = 0;
			uart1_recv_fifo_in = 0;
			uart1_recv_fifo_out = 0;
			//当前起始符是假的 恢复底层fifo读指针
			return 0;			//接收超时
		}
	} ///while()
	//判断接收超时
	if(timeout)
	{
		//超时说明 当前起始符是假的
		if((get_clock() - timeout) >= 120)
		{
			//清标志
			recv_status = 0;
			timeout = 0;
			uart1_recv_fifo_in = 0;
			uart1_recv_fifo_out = 0;
			//当前起始符是假的 恢复底层fifo读指针
			return 0;			//接收超时
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


