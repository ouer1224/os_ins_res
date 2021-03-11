#include"include.h"

extern u8 DMA3_DR_BASE[UART3_FIFO_SIZE];

extern u8 uart3_recv_buf[UART3_FIFO_SIZE];
//extern u8 uart3_recv_dat[UART3_FIFO_SIZE];

extern u8 uart3_recv_fifo[UART3_FIFO_SIZE];			
extern u16 uart3_recv_fifo_in;
extern u16 uart3_recv_fifo_out;

extern u16 out_led1Pull,out_led2Pull,out_ledAllPull;

u8 port_buff_CNG[4]={0x00,0x00,0x00,0x00};		//Ĭ��0-10V���
										// 2,1 4,3 ͨ��
/*
   0000 		 0001 	 0010    0011     0100    0101     0110
  0~10V  -10~+10V  0~5V    -5~+5V  0~20mA  4~20mA   -20~+20mA
           
*/
u8 software_version[]={0x01 ,0x01};//Ӳ���汾�� ����汾��
/*******************************************************************************
       ʹ������GPIO
*******************************************************************************/
void RCC_Configuration(void)
{	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
						  RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE); 	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_BKP | RCC_APB1Periph_PWR ,  ENABLE);
     //����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	}
/*******************************************************************************
        ����ʱ�Ӿ��� 72MHZ
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
 //***********************************************************************************************************************
//��������:ϵͳ��ʼ������
//����˵��:
//ע������:
//����˵��:��
//***********************************************************************************************************************
 void sysint(void)
 {
//	SystemInit();	
//	while(SysTick_Config(SystemCoreClock /1000)); 		//����ϵͳʱ��
	BKP_DeInit();
	RCC_Configuration();						
	LED_init();										//LED���ƿڳ�ʼ��
	IN_PORT_init();	 							//����ڳ�ʼ��
	AD5422_init();								//DAоƬ���ƽų�ʼ��
	TIM2_init();									//��ʱ��2��ʼ��
	uart3_dma_init();							//USART3_Init(9600);						    //����3��ʼ��  485ͨѶ1
//	dog_init(5); 								//���Ź���ʼ��
}


u8 addre_init(void)
{
	u32 timeout_link1 = 0;
	u8 u3_datread;
	u8 send_dat_fifo[20];
	u16 crc_dat;
	u8 old_addre,new_addre,i;
	u8 config_error = 0;
	u8 *p=(u8 *)&saveparam;
	//�ȶ����ݼĴ��������Ϊ0xff �ٶ�flash��
	u16 Data; 
	Data = BKP_ReadBackupRegister(BKP_DR1);
	if((Data==0x00)||(Data==0xff))
	{
		if(p[1]==0xff)	//���Ϊ0xffʱ����ַΪ0xfe
		{
			saveparam.route_addr= new_addre = 0xfe;
		}
		else 
		{
			saveparam.route_addr = p[1];
			Vin24_LED_Green;
			Vout24_LED_Green;
			Mac_set_out_L;
			return 1;
		}
	}
	else 
		{
			saveparam.route_addr = Data;
			Vin24_LED_Green;
			Vout24_LED_Green;
			Mac_set_out_L;
			return 1;
		}
MOD_LED_Green;
while(1)
{	
	u3_datread = uart3_datread();
	if(u3_datread==0x01)
	{
		Link_Green;timeout_link1= get_clock();	
		if(read_mac_set_in==0)
		{
			switch(uart3_recv_buf[2])
			{
				case 0x01:			//��ַ�趨
					if(read_mac_set_in==0)
					{
						if(uart3_recv_buf[3]==(new_addre+1))
						{
							saveparam.route_addr = BKP_ReadBackupRegister(BKP_DR1);
							Vout24_LED_Green;
							Mac_set_out_L;	
							return 1;
							}
						old_addre = saveparam.route_addr;	 
						new_addre = uart3_recv_buf[3];
							/* ʹ��д��BKP */
						PWR_BackupAccessCmd(ENABLE);
						/* ����¼�����λ pin Event(TE) */
						BKP_ClearFlag();
						for(i=0;i<3;i++)
						{
								BKP_WriteBackupRegister(BKP_DR1, new_addre);	//д�뱸�ݼĴ���
								if(BKP_ReadBackupRegister(BKP_DR1) ==	new_addre)
								{
									PWR_BackupAccessCmd(DISABLE);
									config_error = 0;
									break;
								}
							}
						if(config_error)//��ַ����ʧ��
						{
							saveparam.route_addr = old_addre;  //��α��ݼĴ���д��ʧ�ܻ�ԭģ���ַ
						}
						else						//��ַ���ݳɹ� 
							{
								Vin24_LED_Green;

							}
						DMA3_DR_BASE[0] = 0x01;						//Ŀ���ַ
						DMA3_DR_BASE[1] = send_dat_fifo[0] = 5;		//����
						DMA3_DR_BASE[2] = send_dat_fifo[1] = 1;		//����
						DMA3_DR_BASE[3] = send_dat_fifo[2] = config_error;		//״̬ 0-�ɹ� 1-ʧ�� 
						crc_dat = CRC16(send_dat_fifo,3);
						DMA3_DR_BASE[4] = crc_dat;		//CRC��λ
						DMA3_DR_BASE[5] = crc_dat>>8;		//CRC��λ
						delay_us(10);									
						uart3_datsend(DMA3_DR_BASE,6);		//��������
					}
					break;
				}
			}
		}
	else if((get_clock()-timeout_link1)>=1000)
	{
		Link_Red;
		}
	}
}

 u8 error_data,short_error,OUT_short_error;
 u8 port_out=0,read_buf=0;
//  static u8 in_buf=0;
 u32 config_dat[4];

float Vin_Final[4];
u32 Vin_U[4],Vin[4],Vin_count[4][AD_count];
static u8 config = 1;
//״̬��ϵͳ����
void sys_proc(void)
{
	
	u8 u3_dat = 0;
	u8 send_dat_fifo[20];
	u16 crc_dat;

	u8 config_error = 0;
 	static u8 state_buf = 0;

//  	static u8 Status_buf =0;		
//  	static u8 type_buf =0;				//���Ͷ�ȡ		
	static u32 timeout_link = 0;
 	static u8 error_led = 0;
 	static u32 short_error_time;
	static u32 OUT_buf[4],ReceiveOutBuf[4];
	u32 ReceiveBuf = 0;
/////////////////////////////////////////////////////////////////////////////
/////////////��������ʱ��//////////////////////
// if(read_channel1_time > get_clock()) read_channel1_time = get_clock();	

// if((get_clock()-read_channel1_time)>100)
// {
// 	read_channel1_time = get_clock();
//  					DMA3_DR_BASE[0] = 0xf5;
//  					uart3_datsend(DMA3_DR_BASE,1);		//��������	
// }




////////////////////////////////////////////////////////////////////////////	
/*
   0000 		 0001 	 0010    0011     0100    0101     0110
  0~10V  -10~+10V  0~5V    -5~+5V  0~20mA  4~20mA   -20~+20mA
           
*/

if(config==1)
{
		config = 0;
	/*************************ͨ��һ****************************************/
	///OUT_buf[0]ͨ��һ
	switch(port_buff_CNG[0]&0x0f)
	{
		case 0x00:			//0-10V
			config_dat[0] = C_DATA_P10V;
			OUT_buf[0] = 0;	
			break;
		case 0x01:			//-10 - +10V		Ԥ��
			config_dat[0] = C_DATA_PN10V;
			OUT_buf[0] = 0x7fff;	
			break;
		case 0x02:			//0 -5V					Ԥ��
			config_dat[0] = C_DATA_P5V;
			OUT_buf[0] = 0;	
			break;
		case 0x03:			//-5 - +5V			Ԥ��
			config_dat[0] = C_DATA_PN5V;
			OUT_buf[0] = 0x7fff;	
			break;
// 		case 0x04:			//0 -20MA		
// 			config_dat[0] = C_DATA_4MA;
// 			OUT_buf[0] = 0;	
// 			break;
// 		case 0x05:			//4 -20MA		
// 			config_dat[0] = C_DATA_20MA;
// 			OUT_buf[0] = 0;	
// 			break;
// 		case 0x06:			//4 - 24MA		Ԥ��
// 			config_dat[0] = C_DATA_24MA;
// 			OUT_buf[0] = 0;	
// 			break;
		default:
			config_dat[0] = C_DATA_P10V;
			OUT_buf[0] = 0;	
			break;
	}

	/*************************ͨ����****************************************/
	///OUT_buf[1]ͨ����
	switch(port_buff_CNG[0]&0xf0)
	{
		case 0x00:			//0-10V
			config_dat[1] = C_DATA_P10V;
			OUT_buf[1] = 0;	
			break;
		case 0x10:			//-10 - +10V		Ԥ��
			config_dat[1] = C_DATA_PN10V;
			OUT_buf[1] = 0x7fff;	
			break;
		case 0x20:			//0 -5V					Ԥ��
			config_dat[1] = C_DATA_P5V;
			OUT_buf[1] = 0;	
			break;
		case 0x30:			//-5 - +5V			Ԥ��
			config_dat[1] = C_DATA_PN5V;
			OUT_buf[1] = 0x7fff;	
			break;
// 		case 0x40:			//0 -20MA		
// 			config_dat[1] = C_DATA_4MA;
// 			OUT_buf[1] = 0;	
// 			break;
// 		case 0x50:			//4 -20MA		
// 			config_dat[1] = C_DATA_20MA;
// 			OUT_buf[1] = 0;	
// 			break;
// 		case 0x60:			//0 - +24MA		Ԥ��
// 			config_dat[1] = C_DATA_24MA;
// 			OUT_buf[1] = 0;	
// 			break;
		default:
			config_dat[1] = C_DATA_P10V;
			OUT_buf[1] = 0;	
			break;
	}

	/*************************ͨ����****************************************/
	///OUT_buf[2]ͨ����
	switch(port_buff_CNG[1]&0x0f)
	{
		case 0x00:			//0-10V
			config_dat[2] = C_DATA_P10V;
			OUT_buf[2] = 0;	
			break;
		case 0x01:			//-10 - +10V		Ԥ��
			config_dat[2] = C_DATA_PN10V;
			OUT_buf[2] = 0x7fff;	
			break;
		case 0x02:			//0 -5V					Ԥ��
			config_dat[2] = C_DATA_P5V;
			OUT_buf[2] = 0;	
			break;
		case 0x03:			//-5 - +5V			Ԥ��
			config_dat[2] = C_DATA_PN5V;
			OUT_buf[2] = 0x7fff;	
			break;
// 		case 0x04:			//0 -20MA		
// 			config_dat[2] = C_DATA_4MA;
// 			OUT_buf[2] = 0;	
// 			break;
// 		case 0x05:			//4 -20MA		
// 			config_dat[2] = C_DATA_20MA;
// 			OUT_buf[2] = 0;	
// 			break;
// 		case 0x06:			//0 - +24MA		Ԥ��
// 			config_dat[2] = C_DATA_24MA;
// 			OUT_buf[2] = 0;	
// 			break;
		default:
			config_dat[2] = C_DATA_P10V;
			OUT_buf[2] = 0;	
			break;
	}
	/*************************ͨ���� Ԥ��************************
	///OUT_buf[3]ͨ����
	switch(port_buff_CNG[1]&0x0f)
	{
		case 0x00:			//0-10V
			config_dat[3] = C_DATA_P10V;
			OUT_buf[3] = 0;	
			break;
		case 0x10:			//-10 - +10V		Ԥ��
			config_dat[3] = C_DATA_PN10V;
			OUT_buf[3] = 0x7fff;	
			break;
		case 0x20:			//0 -5V					Ԥ��
			config_dat[3] = C_DATA_P5V;
			OUT_buf[3] = 0;	
			break;
		case 0x30:			//-5 - +5V			Ԥ��
			config_dat[3] = C_DATA_PN5V;
			OUT_buf[3] = 0x7fff;	
			break;
		case 0x40:			//0 -20MA		
			config_dat[3] = C_DATA_4MA;
			OUT_buf[3] = 0;	
			break;
		case 0x50:			//4 -20MA		
			config_dat[3] = C_DATA_20MA;
			OUT_buf[3] = 0;	
			break;
		case 0x60:			//0 - +24MA		Ԥ��
			config_dat[3] = C_DATA_24MA;
			OUT_buf[3] = 0;	
			break;
		default:
			break;
	}
	****************/	
write_data_AD5422(CONTROL_DATA+config_dat[0],CONTROL_DATA+config_dat[1],CONTROL_DATA+config_dat[2]);
/* ʹ��д��BKP */
PWR_BackupAccessCmd(ENABLE);
/* ����¼�����λ pin Event(TE) */
BKP_ClearFlag();
BKP_WriteBackupRegister(BKP_DR2, port_buff_CNG[0]);	//д�뱸�ݼĴ���
BKP_WriteBackupRegister(BKP_DR3, port_buff_CNG[1]);	//д�뱸�ݼĴ���
PWR_BackupAccessCmd(DISABLE);
}

	/*************************ͨ��һ****************************************/
	///OUT_buf[0]ͨ��һ
	switch(port_buff_CNG[0]&0x0f)
	{
		case 0x02:			//0 -5V					Ԥ��
		case 0x00:			//0-10V
		if((ReceiveOutBuf[0]&0x8000)==0)
		{
			if(ReceiveOutBuf[0]>=27649)
			{
				OUT_short_error|=0x01;
				OUT_buf[0] = 0xFFFF;
			}
		else
			{
			OUT_short_error&=0xFE;
			OUT_buf[0] = ReceiveOutBuf[0]*65535/27648;	
			}
		}
		else 
		{
			OUT_short_error|=0x01;
			OUT_buf[0] = 0;
			}
			break;

		case 0x03:			//-5 - +5V			Ԥ��
		case 0x01:			//-10 - +10V		Ԥ��
			if((ReceiveOutBuf[0]&0x8000)==0)		//����
			{
				if(ReceiveOutBuf[0]>=27649)
					{
						OUT_short_error|=0x01;
						OUT_buf[0] = 0XFFFF;
					}
				else if(ReceiveOutBuf[0]==0)
						{
							OUT_short_error&=0xFE;
							OUT_buf[0] = 0x7fff;	
							}
					else
					{
					OUT_short_error&=0xFE;
					OUT_buf[0] = ReceiveOutBuf[0]*32767/27648+32767;	
					}
				}
			else 						//����
			{	
				ReceiveBuf = (~(ReceiveOutBuf[0]&0x7FFF - 1))&0x00007FFF;
				if(ReceiveBuf>=27649)
				{
					OUT_short_error|=0x01;
					OUT_buf[0] = 0;
					}
					else
						{
						OUT_short_error&=0xFE;
						OUT_buf[0] = 32767 - ReceiveBuf*32767/27647;
					}
			}
			break;
// 		case 0x04:			//0 -20MA		

// 			OUT_buf[0] = 0;	
// 			break;
// 		case 0x05:			//4 -20MA		

// 			OUT_buf[0] = 0;	
// 			break;
// 		case 0x06:			//4 - 24MA		Ԥ��

// 			OUT_buf[0] = 0;	
// 			break;
		default:

			OUT_buf[0] = 0;	
			break;
	}

	/*************************ͨ����****************************************/
	///OUT_buf[1]ͨ����
	switch(port_buff_CNG[0]&0xf0)
	{
		case 0x20:			//0 -5V					Ԥ��
		case 0x00:			//0-10V
			if((ReceiveOutBuf[1]&0x8000)==0)
			{
				if(ReceiveOutBuf[1]>=27649)
					{
					OUT_short_error|=0x02;
					OUT_buf[1] = 0XFFFF;
				}
			else
				{
				OUT_short_error&=0xFD;
				OUT_buf[1] = ReceiveOutBuf[1]*65535/27648;	
				}
			}
			else
			{
				OUT_short_error|=0x02;
				OUT_buf[1] = 0;
				}
				break;
				
		case 0x30:			//-5 - +5V			Ԥ��
		case 0x10:			//-10 - +10V		Ԥ��
			if((ReceiveOutBuf[1]&0x8000)==0)		//����
			{
				if(ReceiveOutBuf[1]>=27649)
					{
						OUT_short_error|=0x02;
						OUT_buf[1] = 0XFFFF;
					}
				else if(ReceiveOutBuf[1]==0)
						{
							OUT_short_error&=0xFD;
							OUT_buf[1] = 0x7fff;	
							}
					else
					{
					OUT_short_error&=0xFD;
					OUT_buf[1] = ReceiveOutBuf[1]*32767/27648+32767;	
					}
				}
			else 						//����
			{	
				ReceiveBuf = (~(ReceiveOutBuf[1]&0x7FFF - 1))&0x00007FFF;
				if(ReceiveBuf>=27649)
				{
					OUT_short_error|=0x02;
					OUT_buf[1] = 0;
					}
					else
						{
						OUT_short_error&=0xFD;
						OUT_buf[1] = 32767 - ReceiveBuf*32767/27647;
					}
			}
			break;
// 		case 0x40:			//0 -20MA		

// 			OUT_buf[1] = 0;	
// 			break;
// 		case 0x50:			//4 -20MA		

// 			OUT_buf[1] = 0;	
// 			break;
// 		case 0x60:			//0 - +24MA		Ԥ��

// 			OUT_buf[1] = 0;	
// 			break;
		default:

			OUT_buf[1] = 0;	
			break;
	}

	/*************************ͨ����****************************************/
	///OUT_buf[2]ͨ����
	switch(port_buff_CNG[1]&0x0f)
	{
		case 0x02:			//0 -5V					Ԥ��
		case 0x00:			//0-10V
		if((ReceiveOutBuf[2]&0x8000)==0)
		{
			if(ReceiveOutBuf[2]>=27649)
				{
				OUT_short_error|=0x04;
				OUT_buf[2] = 0XFFFF;
			}
		else
			{
			OUT_short_error&=0xFB;
			OUT_buf[2] = ReceiveOutBuf[2]*65535/27648;	
			}
		}
		else
		{
				OUT_short_error|=0x04;
				OUT_buf[2] = 0;
			}
			break;
			
		case 0x03:			//-5 - +5V			Ԥ��			
		case 0x01:			//-10 - +10V		Ԥ��
			if((ReceiveOutBuf[2]&0x8000)==0)		//����
			{
				if(ReceiveOutBuf[2]>=27649)
					{
						OUT_short_error|=0x04;
						OUT_buf[2] = 0XFFFF;
					}
				else if(ReceiveOutBuf[2]==0)
						{
							OUT_short_error&=0xFB;
							OUT_buf[2] = 0x7fff;	
							}
					else
					{
					OUT_short_error&=0xFB;
					OUT_buf[2] = ReceiveOutBuf[2]*32767/27648+32767;	
					}
				}
			else 						//����
			{	
				ReceiveBuf = (~(ReceiveOutBuf[2]&0x7FFF - 1))&0x00007FFF;
				if(ReceiveBuf>=27649)
				{
					OUT_short_error|=0x04;
					OUT_buf[2] = 0;
					}
					else
						{
						OUT_short_error&=0xFB;
						OUT_buf[2] = 32767 - ReceiveBuf*32767/27647;
					}
			}
			break;

// 		case 0x04:			//0 -20MA		

// 			OUT_buf[2] = 0;	
// 			break;
// 		case 0x05:			//4 -20MA		

// 			OUT_buf[2] = 0;	
// 			break;
// 		case 0x06:			//0 - +24MA		Ԥ��

// 			OUT_buf[2] = 0;	
// 			break;
		default:

			OUT_buf[2] = 0;	
			break;
	}
	/*************************ͨ���� Ԥ��************************
	///OUT_buf[3]ͨ����
	switch(port_buff_CNG[1]&0x0f)
	{
		case 0x00:			//0-10V
			if(ReceiveOutBuf[3]>=27649)
			{
				OUT_short_error|=0x08;
				OUT_buf[3] = 0;
			}
		else
			OUT_buf[3] = ReceiveOutBuf[3]*65535/27648;	
			break;
		case 0x10:			//-10 - +10V		Ԥ��

			OUT_buf[3] = 0x7fff;	
			break;
		case 0x20:			//0 -5V					Ԥ��
			if(ReceiveOutBuf[3]>=27649)
			{
				OUT_short_error|=0x08;
				OUT_buf[3] = 0;
			}
		else
			OUT_buf[3] = ReceiveOutBuf[3]*65535/27648;	
			break;
		case 0x30:			//-5 - +5V			Ԥ��

			OUT_buf[3] = 0x7fff;	
			break;
		case 0x40:			//0 -20MA		

			OUT_buf[3] = 0;	
			break;
		case 0x50:			//4 -20MA		

			OUT_buf[3] = 0;	
			break;
		case 0x60:			//0 - +24MA		Ԥ��

			OUT_buf[3] = 0;	
			break;
		default:
			OUT_buf[3] = 0;	
			break;
	}
	****************/	

write_data_AD5422(DAC_DATA+OUT_buf[0],DAC_DATA+OUT_buf[1],DAC_DATA+OUT_buf[2]);
error_led_out(OUT_short_error);
error_data = (OUT_short_error<<4)|(OUT_short_error>>4);
if(timeout_link > get_clock()) timeout_link = get_clock();	
	u3_dat = uart3_datread();		
	if(u3_dat==1)
	{
		timeout_link = get_clock();
		Link_Green;
		switch(uart3_recv_buf[2])
		{
// 			case 0x01:			//��ַ�趨
// 				if(read_mac_set_in==0)
// 				{
// 					config_error = 1;
// 					old_addre = saveparam.route_addr;
// 					saveparam.route_addr = uart3_recv_buf[3];
// 					  /* ʹ��д��BKP */
// 					PWR_BackupAccessCmd(ENABLE);
// 					/* ����¼�����λ pin Event(TE) */
// 					BKP_ClearFlag();
// 					for(i=0;i<3;i++)
// 					{
// 							BKP_WriteBackupRegister(BKP_DR1, saveparam.route_addr);	//д�뱸�ݼĴ���
// 							if(BKP_ReadBackupRegister(BKP_DR1) ==	saveparam.route_addr)
// 							{
// 								PWR_BackupAccessCmd(DISABLE);
// 								config_error = 0;
// 								break;
// 							}
// 						}
// 					if(config_error)//��ַ����ʧ��
// 					{
// 						saveparam.route_addr = old_addre;  //��α��ݼĴ���д��ʧ�ܻ�ԭģ���ַ
// 					}
// 					else						//��ַ���ݳɹ� 
// 						{
// 							Vin24_LED_Green;
// 							Vout24_LED_Green;
// 						}
// 					DMA3_DR_BASE[0] = 0x01;						//Ŀ���ַ
// 					DMA3_DR_BASE[1] = send_dat_fifo[0] = 5;		//����
// 					DMA3_DR_BASE[2] = send_dat_fifo[1] = 1;		//����
// 					DMA3_DR_BASE[3] = send_dat_fifo[2] = config_error;		//״̬ 0-�ɹ� 1-ʧ�� 
// 					crc_dat = CRC16(send_dat_fifo,3);
// 					DMA3_DR_BASE[4] = crc_dat;		//CRC��λ
// 					DMA3_DR_BASE[5] = crc_dat>>8;		//CRC��λ
// 					delay_ms(2);									
// 					uart3_datsend(DMA3_DR_BASE,6);		//��������
// 					Mac_set_out_L;	
// 				}
// 				break;
			case 0x02:			//��ȡ��վ����
  			DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
				DMA3_DR_BASE[1] = send_dat_fifo[0] = 6;		//����
				DMA3_DR_BASE[2] = send_dat_fifo[1] = 2;		//����
				DMA3_DR_BASE[3] = send_dat_fifo[2] = 0;		//״̬ 0-�ɹ� 1-ʧ��
				DMA3_DR_BASE[4] = send_dat_fifo[3] = saveparam.sn;	//ģ������
				crc_dat = CRC16(send_dat_fifo,4);
				DMA3_DR_BASE[5] = crc_dat;		//CRC��λ
				DMA3_DR_BASE[6] = crc_dat>>8;		//CRC��λ
				uart3_datsend(DMA3_DR_BASE,7);		//��������
				break;
			case 0x03:			//��ȡ��д������
				state_buf = uart3_recv_buf[3];
				switch(saveparam.sn)
				{
// 					case 1://8DI
// 					case 2://8DO
// 					case 3://8DI/DO
// 					case 9://4DI4DO
// 					case 4://6DI
// 					case 5://6DO
// 					case 6://6DI/DO
// 						break;
// 					case 7://4AI
// 						break;
					case 0x15://3VO
						if(uart3_recv_buf[1]<10)
						{
							DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
							DMA3_DR_BASE[1] = send_dat_fifo[0] = 6;		//����
							DMA3_DR_BASE[2] = send_dat_fifo[1] = 3;		//����
							DMA3_DR_BASE[3] = send_dat_fifo[2] = 1;		//״̬ 0-�ɹ� 1-ʧ��
							DMA3_DR_BASE[4] = send_dat_fifo[3] = 0;//error_data;		//�źŷ���ERROR�ź�
							crc_dat = CRC16(send_dat_fifo,4);
							DMA3_DR_BASE[5] = crc_dat;		//CRC��λ
							DMA3_DR_BASE[6] = crc_dat>>8;		//CRC��λ
							uart3_datsend(DMA3_DR_BASE,7);		//��������			
							}
						else
						{
							ReceiveOutBuf[0] = uart3_recv_buf[3];
							ReceiveOutBuf[0] = (ReceiveOutBuf[0]<<8) + uart3_recv_buf[4];
							ReceiveOutBuf[1] = uart3_recv_buf[5];;
							ReceiveOutBuf[1] = (ReceiveOutBuf[1]<<8) + uart3_recv_buf[6];
							ReceiveOutBuf[2] = uart3_recv_buf[7];;
							ReceiveOutBuf[2] = (ReceiveOutBuf[2]<<8) + uart3_recv_buf[8];
	// 						ReceiveOutBuf[3] = uart3_recv_buf[9];//����
	// 						ReceiveOutBuf[3] = (ReceiveOutBuf[3]<<8) + uart3_recv_buf[10];//����
						
							DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
							DMA3_DR_BASE[1] = send_dat_fifo[0] = 6;		//����
							DMA3_DR_BASE[2] = send_dat_fifo[1] = 3;		//����
							DMA3_DR_BASE[3] = send_dat_fifo[2] = 0;		//״̬ 0-�ɹ� 1-ʧ��
							DMA3_DR_BASE[4] = send_dat_fifo[3] = error_data;		//�źŷ���ERROR�ź�
							crc_dat = CRC16(send_dat_fifo,4);
							DMA3_DR_BASE[5] = crc_dat;		//CRC��λ
							DMA3_DR_BASE[6] = crc_dat>>8;		//CRC��λ
							uart3_datsend(DMA3_DR_BASE,7);		//��������				
						}
						break;
				}
				break;
			case 0x04:			//�趨������ͨ�������ͣ�һ��ͨ��ռ����λ��00���� 01 ��� 11 ����/���
				state_buf = 0;   //����ʱ�Ȱ������0
				switch(saveparam.sn)
					{
						case 0x15://3VO
								port_buff_CNG[0] = uart3_recv_buf[4];
								port_buff_CNG[1] = uart3_recv_buf[3];
								config_error = 0;		//������ģ�飬���óɹ�
								config = 1;
							break;
						default:
							config_error = 1;		//���������������
							break;
						}
					DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
					DMA3_DR_BASE[1] = send_dat_fifo[0] = 5;		//����
					DMA3_DR_BASE[2] = send_dat_fifo[1] = 4;		//����
					DMA3_DR_BASE[3] = send_dat_fifo[2] = config_error;		//״̬ 0-�ɹ� 1-ʧ��
					crc_dat = CRC16(send_dat_fifo,3);
					DMA3_DR_BASE[4] = crc_dat;				//CRC��λ
					DMA3_DR_BASE[5] = crc_dat>>8;			//CRC��λ
					uart3_datsend(DMA3_DR_BASE,6);		//��������
				break;
			case 0x05:			//�����趨��ַ���ѵ�ǰ��ַд��FLASH
					save_param();
					DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
					DMA3_DR_BASE[1] = send_dat_fifo[0] = 5;		//����
					DMA3_DR_BASE[2] = send_dat_fifo[1] = 5;		//����
					DMA3_DR_BASE[3] = send_dat_fifo[2] = 0;		//״̬ 0-�ɹ� 1-ʧ��
					crc_dat = CRC16(send_dat_fifo,3);
					DMA3_DR_BASE[4] = crc_dat;				//CRC��λ
					DMA3_DR_BASE[5] = crc_dat>>8;			//CRC��λ
					uart3_datsend(DMA3_DR_BASE,6);		//��������
				break;
			case 0x06:			//����趨��ַ��������ַ�ָ�0XFF
					//����flash
					if(flash_erase(PARAM_ADDR,sizeof(saveparam_t)))
						config_error = 0;
					else config_error = 1;
					DMA3_DR_BASE[0] = 1;		//Ŀ���ַ
					DMA3_DR_BASE[1] = send_dat_fifo[0] = 5;		//����
					DMA3_DR_BASE[2] = send_dat_fifo[1] = 6;		//����
					DMA3_DR_BASE[3] = send_dat_fifo[2] = config_error;		//״̬ 0-�ɹ� 1-ʧ��
					crc_dat = CRC16(send_dat_fifo,3);
					DMA3_DR_BASE[4] = crc_dat;				//CRC��λ
					DMA3_DR_BASE[5] = crc_dat>>8;			//CRC��λ
					uart3_datsend(DMA3_DR_BASE,6);		//��������
				break;
			case 0x68:									//�ظ���ǰ��ַ����  ����
					DMA3_DR_BASE[0] = saveparam.route_addr;		//�����ַ��
					DMA3_DR_BASE[1] = 0x7E;//����		
					crc_dat = CRC16(DMA3_DR_BASE,2);
					DMA3_DR_BASE[2] = crc_dat;				//CRC��λ
					DMA3_DR_BASE[3] = crc_dat>>8;			//CRC��λ
					uart3_datsend(DMA3_DR_BASE,4);		//��������
				break;
			case 0x64:								//��������
					DMA3_DR_BASE[0] = saveparam.route_addr;		//�����ַ��
					DMA3_DR_BASE[1] = 0x7E;//����		
					DMA3_DR_BASE[2] = 0x55;			
					DMA3_DR_BASE[3] = 0x55;	
					DMA3_DR_BASE[4] = 0x55;			
					DMA3_DR_BASE[5] = 0x55;		
					uart3_datsend(DMA3_DR_BASE,6);		//��������
					DOWNLOAD();
				break;
			case 0x07:								    //��ѯӲ��������汾
					DMA3_DR_BASE[0] = saveparam.route_addr;//1;		//Ŀ���ַ
					DMA3_DR_BASE[1] = send_dat_fifo[0] = 7;		//����
					DMA3_DR_BASE[2] = send_dat_fifo[1] = 7;		//����
					DMA3_DR_BASE[3] = send_dat_fifo[2] = 0;		//״̬ 0-�ɹ� 1-ʧ��
					DMA3_DR_BASE[4] = send_dat_fifo[3] = software_version[0];	//Ӳ���汾��
					DMA3_DR_BASE[5] = send_dat_fifo[4] = software_version[1];	//����汾��
					crc_dat = CRC16(send_dat_fifo,5);
					DMA3_DR_BASE[6] = crc_dat;		//CRC��λ
					DMA3_DR_BASE[7] = crc_dat>>8;		//CRC��λ
					uart3_datsend(DMA3_DR_BASE,8);		//��������
					break;
		}
	}
	else if(u3_dat ==2)		//CRCУ��ʧ��
	{
		timeout_link = get_clock();
		DMA3_DR_BASE[0] = 1;		//Ŀ���ַ
		DMA3_DR_BASE[1] = 3;		//����
		DMA3_DR_BASE[2] = uart3_recv_buf[2] ;		//����
		DMA3_DR_BASE[3] = 1;		//����
		uart3_datsend(DMA3_DR_BASE,4);		//��������
		}
	else if((get_clock()-timeout_link)>=1000)
	{
		Link_Red;
		}
				
//MODָʾ�ƣ���û��ERRORʱָʾ��Ϊ�̣�����ERROR����ʱMOD��˸
//if((error_data==0)&&(short_error==0x00))
if((OUT_short_error==0)&&(error_led==0))
	{
		out_led1Pull=out_led2Pull=5000;
		short_error_time = 0;
		}
	else 
	{
			short_error_time++;
			if(short_error_time>20)
			{out_led1Pull=out_led2Pull=0;
			short_error_time = 0;}
		}
	led_blink(0,out_led1Pull+out_led2Pull,out_ledAllPull-out_led1Pull-out_led2Pull);		 //LED���ָʾ
	dog();
}
