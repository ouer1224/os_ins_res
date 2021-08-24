/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&主循环工作函数&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:SYS_WORK.C

文件作用:线序测试仪工作主循环

文件作者:Bai Bing

创建日期:2017年8月
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#include "include.h "

extern u16 uart1_recv_length;
extern u8 uart1_recv_buf[UART1_FIFO_SIZE];
u8 command;
u8 Sand_data[600];
u8 REALY_DATA_1,REALY_DATA_2,REALY_DATA_3,REALY_DATA_4,REALY_DATA_5;
u16 crc_data = 0;
u8 send_fifo_buf[600] = {0};

/*******************************************************************************
       使能外设GPIO
*******************************************************************************/
void RCC_Configuration(void)
{	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE); 	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_BKP | RCC_APB1Periph_PWR ,  ENABLE);     //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
/*******************************************************************************
        设置时钟晶振 72MHZ
*******************************************************************************/
void SysClock_Init(void)
{
  ErrorStatus HSEStartUpStatus;     
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2); 
	    RCC_HCLKConfig(RCC_SYSCLK_Div1); 	  
	    RCC_PCLK2Config(RCC_HCLK_Div1); 	
	    RCC_PCLK1Config(RCC_HCLK_Div2);	
	    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	
	    RCC_PLLCmd(ENABLE);	
	    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
	    	;
		}	
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	
	    while(RCC_GetSYSCLKSource() != 0x08){
	    	;
		}
	}
}



void SYS_WORK(void)
{	
	u16 x;
	if(get_packet1() == 1)
	{
		switch(uart1_recv_buf[3])
		{
			case 0X01:
        if(uart1_recv_buf[4] != 00)		
				{					
				break;
				}	
				REALY_DATA_1 = uart1_recv_buf[5];
				REALY_DATA_2 = uart1_recv_buf[6];
				REALY_DATA_3 = uart1_recv_buf[7];
				REALY_DATA_4 = uart1_recv_buf[8];
				REALY_DATA_5 = uart1_recv_buf[9];
				REALY_DATA_DOWN();
				Sand_data[0] = 0X55;
				Sand_data[1] = 0X00;
				Sand_data[2] = 0X0B;
				Sand_data[3] = 0X01;
				Sand_data[4] = 0X01;
				Sand_data[5] = uart1_recv_buf[5];
				Sand_data[6] = uart1_recv_buf[6];
				Sand_data[7] = uart1_recv_buf[7];
				Sand_data[8] = uart1_recv_buf[8];
				Sand_data[9] = uart1_recv_buf[9];
				for(x = 0 ; x < 8 ; x++)
				{
					send_fifo_buf[x] = Sand_data[x + 1];
				}
				crc_data =	CRC16(send_fifo_buf , 8);
				Sand_data[10] = crc_data & 0xff;
				crc_data >>= 8;
				Sand_data[11] = crc_data & 0xff;
				Sand_data[12] = 0xAA;
				for(x = 0 ; x < 13 ; x++)
				{
					USART1_SendByte(Sand_data[x]);
				}
        break;				
			default:
				break;	
		}
		
  }
}

//***********************************************************************************************************************
//函数作用:下发继电器数据操作
//参数说明:无
//注意事项:
//返回说明:无
//***********************************************************************************************************************
void REALY_DATA_DOWN(void)
{	
	Write_Relay_data(REALY_DATA_1,REALY_DATA_2,REALY_DATA_3,REALY_DATA_4,REALY_DATA_5);
}
