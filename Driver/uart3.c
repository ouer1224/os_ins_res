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

//接收缓冲区

u16 rfid3_recv_length=0;

u8 uart3_recv_fifo[UART3_FIFO_SIZE];			
u16 uart3_recv_fifo_in=0;
u16 uart3_recv_fifo_out=0;
u16 uart3_recv_fifo_out_backup=0;

extern u8 sn;
/*******************************************************************************
        串口I/O对应初始化
*******************************************************************************/ 
void uart3_Gpio_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; //用于设置GPIO口的基本参数

    GPIO_InitStructure.GPIO_Pin = RS485_RE1 ;//选择的是那个管脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择最高的输出速度有2MHZ,10MHZ,50MHZ,
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//输出
    GPIO_Init(RS485_ENPORT, &GPIO_InitStructure); //按照上面的参数初始化一下


    GPIO_InitStructure.GPIO_Pin = TX3 ;//选择的是那个管脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择最高的输出速度有2MHZ,10MHZ,50MHZ,
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//输出
    GPIO_Init(RS485_PORT1, &GPIO_InitStructure); //按照上面的参数初始化一下

    GPIO_InitStructure.GPIO_Pin = RX3;//选择第11个管脚
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;// GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(RS485_PORT1, &GPIO_InitStructure);//按照上面的参数进行初始化

}
/*******************************************************************************
        串口3 接收中断
*******************************************************************************/
void UART3_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; //用于设置中断向量表中的参数的结构体
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //NVIC_PriorityGroupConfig
	//使能串口3中断

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  //使能或使能特定的通道
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//设置从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//NVIC_IRQChannel中定义的 IRQ通道被使能还是失能
    NVIC_Init(&NVIC_InitStructure);//按照上面的配置进行初始化
}
 

/*****************************************************************
          USART3 初始化 baud 波特率设置波特率
*****************************************************************/
void USART3_Init(unsigned int baud)
{
    USART_InitTypeDef USART_InitStructure;//用于USART参数设置的结构体

    USART_InitStructure.USART_BaudRate = baud;//设置波特率的值
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//一帧设置为八位的
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //设置一位的停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;	//偶校验位USART_Parity_No;//
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送和接收

    USART_Init(USART3, &USART_InitStructure); //按照上面的参数进行使能
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能接收中断
  	  
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); 	//使能DMA发送中断			
	USART_Cmd(USART3, ENABLE); //使能USART3
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
          如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */ 	
}


/*****************************************************************
          从 USART3 发送一个字节
*****************************************************************/
void USART3_SendByte(unsigned char temp)
{
	//RS485_ONE_SEND;
// 	delay_us(5000);
    USART_SendData(USART3, temp); //设置单个数据
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); //等待数据发送完
//	delay_us(1000);
//	RS485_ONE_RECEIVE;
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);  //配置发送完成后产生中断
}

/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void DMA_Config_Channel2(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel2);   //将DMA的通道1寄存器重设为缺省值
  	/*设置DMA源：内存地址&串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;	   
	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA3_DR_BASE;
	/*方向：从内存到外设*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE_UAER3;
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
    DMA_Init(DMA1_Channel2, &DMA_InitStructure); 	
   
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断	 
	DMA_ClearFlag(DMA1_FLAG_TC2); 
}
 
/*
 * 函数名：NVIC_Config
 * 描述  ：DMA 中断配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void DMA1_chan2_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置P[A|B|C|D|E]0为中断源 */
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
  DMA_Cmd(DMA1_Channel2, DISABLE);//先停止DMA;停止之后才容许配置，网上有些教程说DMA完成之后,会停下来，那是错误的。 
  DMA1_Channel2->CMAR  = (u32)pData; //重新设置源地址   传输之后，源地址已经增长了，所以要修改回来 
  DMA1_Channel2->CNDTR = Leng;   //重新设置发送数量   传输之后，DMA中的DMA_TX_LENG已经改变了   
  DMA_Cmd(DMA1_Channel2, ENABLE);  /* Enable USART3 TX DMA1 Channel */
  USART_ClearFlag(USART3, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */ 
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
//设置串口3 接收、发送均为DMA方式
void USART3_DMA_init(void)
{
//  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); /* Enable USART3 DMA Rxrequest */  
  USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); /* Enable USART3 DMA TX request */   
//  DMA_Cmd(DMA1_Channel2, ENABLE);  /* Enable USART3 TX DMA1 Channel */	  
//  DMA_Cmd(DMA1_Channel3, ENABLE);  /* Enable USART3 RX DMA1 Channel */   
  USART_Cmd(USART3, ENABLE); 
}

 
/**********************************************************
//函数作用://串口1中断接收DMA发送初始化
//参数说明:	使用串口1中断接收数据，DMA_ch4通道发送数据，数据发送完成产生中断
//注意事项:
//返回说明:无
***********************************************************/
void uart3_dma_init(void)
{
	uart3_Gpio_init();						//GPIO口对应初始化	TX RX 485R/T
	USART3_Init(115200);	 //  1000000
	UART3_NVIC();						    //串口一接收中断初始化
	DMA1_chan2_NVIC();						//DMA中断初始化
	DMA_Config_Channel2();					//DMA初始化
	RS485_ONE_RECEIVE;
	}

//DMA中断入口
void DMA1_Channel2_IRQHandler(void)
{	
//判断是否为DMA发送完成中断
   if(DMA_GetFlagStatus(DMA1_FLAG_TC2)==SET) 
   {  
	/* 清除DMA完成标志 */
	DMA_ClearFlag(DMA1_FLAG_TC2); 
	/* 关闭DMA传输 */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
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
	*ch = uart3_recv_fifo[uart3_recv_fifo_out];						//得到当前要读取的字符
	uart3_recv_fifo_out++;											//指向下一个要读取的字符
	if(uart3_recv_fifo_out==UART3_FIFO_SIZE)
		uart3_recv_fifo_out=0;	//如果到了fifo末尾 则重从头开始
	
	return 1;
	}
#if 0
u8 get_packet3(void)
{
	u8 temp;	
	u16 i,crc_j;
	static u8 fifo_buf[UART3_FIFO_SIZE];	
	static u32 timeout = 0;		   		//接收超时计数器
	static u8  recv_status = 0;	   	//状态机
	static u16 cmd_length = 0;	   	//除包头外所有数据长度
	static u16 uart3_crc = 0;		   	//数据CRC校验

	while(read_usart3_char(&temp))
	{
		switch(recv_status)
		{
			case 0:	//接收数据包头
 					if(temp==saveparam.route_addr)		//地址相同
					{	
						recv_status++;
						//uart3_recv_fifo_out_backup = uart3_recv_fifo_out;		
						timeout = get_clock();//清除接收超时
						uart3_crc=0;//清除校验位
						rfid3_recv_length=0;//清除总长度
						uart3_recv_buf[rfid3_recv_length++] = temp;//向缓存送数据
						if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//判断是否超长	
						{	rfid3_recv_length=0;
							recv_status=0;
							timeout=0;	
							uart3_recv_fifo_in=0;
							uart3_recv_fifo_out=0;
							return 0;
							}
					}  
					break;
			case 1:	//接收数据长度 1个字节
					recv_status++;
					cmd_length = temp;
					uart3_recv_buf[rfid3_recv_length++] = temp;//向缓存送数据
					if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//判断是否超长	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}
					if(temp>=sizeof(uart3_recv_buf))	//判断是否超长	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}	
					break;
			case 2://开始接收数据
  //					timeout =  get_clock();//清除接收超时
					//没有接收完毕 送缓冲区
					uart3_recv_buf[rfid3_recv_length++]=temp;//向缓存送数据
					if(rfid3_recv_length>=sizeof(uart3_recv_buf))	//判断是否超长	
					{	rfid3_recv_length=0;
						recv_status=0;
						timeout=0;	
						uart3_recv_fifo_in=0;
						uart3_recv_fifo_out=0;
						return 0;
						}
					//接收完毕 当前字节就是校验和 检查校验和
					//计算校验和
					if(rfid3_recv_length>cmd_length)			 //总长度与接收长度相同
					{
						timeout =  0;//清除接收超时
						recv_status=0;
							//进行判读数据
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
							return 0;			//CRC校验失败
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
								return 0;			//CRC校验失败
						}
					}
					break;
				default:
					break;
			}
			if((get_clock()-timeout)>=320)
			{
				//清标志
				recv_status=0;
				timeout=0;
				uart3_recv_fifo_in=0;
				uart3_recv_fifo_out=0;
				//当前起始符是假的 恢复底层fifo读指针
				//uart3_recv_fifo_out = uart3_recv_fifo_out_backup ;
				return 0;			//接收超时
			}	
		} ///while()
					//判断接收超时
	   if(timeout)
		{
			//超时说明 当前起始符是假的
			if((get_clock()-timeout)>=120)
			{
				//清标志
				recv_status=0;
				timeout=0;
				uart3_recv_fifo_in=0;
				uart3_recv_fifo_out=0;
				//当前起始符是假的 恢复底层fifo读指针
				//uart3_recv_fifo_out = uart3_recv_fifo_out_backup ;
				return 0;			//接收超时
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
	





