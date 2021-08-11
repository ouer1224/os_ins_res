#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"

#include "../os/task.h"

#include "../os/mem_manage.h"
#include "../os/queue.h"
#include "../os/sem.h"
#include "../os/smallMem.h"
/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/

u8 read_data[8] = {0};
u8 test_data[512] = {0};
u8 command = 0;
u8 Sand_data[600] = {0};
u8 REALY_DATA_1, REALY_DATA_2, REALY_DATA_3, REALY_DATA_4, REALY_DATA_5;

/*---����-----*/
#define TASK_DEAL_INSRES_STK_SIZE 128
static unsigned int task_deal_ins_res_Stk[TASK_DEAL_INSRES_STK_SIZE];
volatile struct selfos_task_struct tcb_task_deal_ins_res;

#define TASKB_STK_SIZE 128
static unsigned int taskB_Stk[TASKB_STK_SIZE];
volatile struct selfos_task_struct taskB;

#define TASKC_STK_SIZE 128
static unsigned int taskC_Stk[TASKC_STK_SIZE];
volatile struct selfos_task_struct taskC;

#define TASK_UART1_RCV_STK_SIZE 256
static unsigned int task_uart1_rcv_Stk[TASK_UART1_RCV_STK_SIZE];
volatile struct selfos_task_struct tcb_task_uart1_rcv;

#define TASK_UART3_SND_STK_SIZE 128
static unsigned int task_uart3_snd_Stk[TASK_UART3_SND_STK_SIZE];
volatile struct selfos_task_struct tcb_task_uart3_snd;


#define TASK_RUN_STK_SIZE 128
static unsigned int task_run_Stk[TASK_RUN_STK_SIZE];
volatile struct selfos_task_struct tcb_task_run;



/*--�ź���----*/
SemCB testsem;
SemCB sem_uart1rcv;
SemCB sem_uart3rcv;
SemCB sem_deal_complete;

/*----�ڴ��----*/
#define LEN_UART1_RCV_MEM 12
#define DEEP_UART1_RCV_MEM 16
static mem_pool pool_uart1_rcv;
static uint8_t buf_mempool_uart1_rcv[LEN_UART1_RCV_MEM * DEEP_UART1_RCV_MEM];

#define LEN_UART3_SND_MEM 12
#define DEEP_UART3_SND_MEM 16
static mem_pool pool_uart3_snd;
static uint8_t buf_mempool_uart3_snd[LEN_UART3_SND_MEM * DEEP_UART3_SND_MEM];


#define LEN_TIMER2_MEM 12
#define DEEP_TIMER2_MEM 16
mem_pool pool_timer2;
static uint8_t buf_mempool_timer2[LEN_TIMER2_MEM*DEEP_TIMER2_MEM];



/*---����-----*/
#define DEEP_QUEUE_UART1_RCV DEEP_UART1_RCV_MEM
static uint32_t queue_mem_uart1_rcv[DEEP_QUEUE_UART1_RCV];
QueueCB queue_uart1_rcv;

#define DEEP_QUE_UART3_SND DEEP_UART3_SND_MEM
static uint32_t que_mem_uart3_snd[DEEP_QUE_UART3_SND];
QueueCB queue_uart3_snd;

#define DEEP_QUE_TIMER2		DEEP_TIMER2_MEM
static uint32_t que_mem_timer2[DEEP_QUE_TIMER2];
QueueCB queue_timer2;




uint32_t uput_dat_to_queue(QueueCB *pr_q, mem_pool *pr_pool, uint8_t *pr_dat, uint32_t len, uint32_t delay)
{
	uint8_t *pr_send = NULL;
	uint32_t rc = 0;

	while (len > 0)
	{
		pr_send = get_mem_from_pool(pr_pool, LEN_UART1_RCV_MEM);
		if (pr_send != NULL)
		{
			if (len >= 8)
			{
				pr_send[0] = 8;
				os_memcpy(pr_send + 1, pr_dat, 8);

				rc = put_dat_to_queue(pr_q, pr_send, delay, 0);
				if (rc != os_true)
				{
					return rc;
				}
				len = len - 8;
				pr_dat += 8;
			}
			else
			{
				pr_send[0] = len;
				os_memcpy(pr_send + 1, pr_dat, len);

				rc = put_dat_to_queue(pr_q, pr_send, delay, 0);
				if (rc != os_true)
				{
					return rc;
				}
				len = 0;
			}
		}
	}

	return os_true;
}

uint32_t send_dat_to_uart3(uint8_t *buf, uint32_t len)
{

	return uput_dat_to_queue(&queue_uart3_snd, &pool_uart3_snd, buf, len, 100);
}


void task_deal_ins_res(void)
{
	uint32_t rc = 0;
	uint32_t count = 0;
	uint32_t i = 0;
	uint8_t *pr_rcv = NULL;
	uint32_t len = 0;
	uint8_t buf[48] = 0;
	uint8_t *pr = buf;
	uint8_t j=0;

	msg_out("a run\n");
	TaskDelay(1500);
	while (1)
	{
		j++;
		if(j%50==0)
		{
			tog_pin_port(LED1);
		}
		rc = get_dat_from_queue(&queue_uart1_rcv, &pr_rcv, 100, 0);
		if (rc == os_true)
		{
			len += pr_rcv[0];
			if(len<=48)
			{
				os_memcpy(pr, pr_rcv + 1, pr_rcv[0]);
				pr = pr + pr_rcv[0];
			}
			else
			{
				len = 0;
				pr = buf;
				msg_out("err: mem overflow\n");
			}
			free_mem_to_pool(&pr_rcv);
			
		}
		else
		{
			if (len != 0)
			{
				msg_out("\n rc= %x dat_rcv = ", rc);
				for (i = 0; i < len; i++)
				{
					msg_out("%x ", buf[i]);
				}
				msg_out("\n");
				deal_master_cmd(buf);
			}
			len = 0;
			pr = buf;
		}
	}
}

void fun_taskb(void)
{
	uint32_t rc = 0;
	uint32_t count = 0;
	uint32_t i = 0;
	uint32_t time_now = 0, time_pre = 0;
	msg_out("b run\n");
	TaskDelay(800);

	time_pre = get_OS_time();
	while (1)
	{
		//msg_out("b send sem\n");
		//sem_release(&testsem);
		
		//TaskDelay(1000);

		time_now = get_OS_time();
		if (time_now != time_pre)
		{
			//msg_out("--diff time=%d\n",time_now-time_pre);
		}
		time_pre = time_now;

		if (sem_acquire(&sem_uart1rcv, 2000) == os_true)/*2ms����һ�δ�������,�����ٽ����Ĺ���*/
		{
			//msg_out("get uart1rcv sem\n");	

		}
		else
		{
			//msg_out("No uart1rcv sem\n");	
		}
		tog_pin_port(LED2);

		
	}
}


void fun_taskc(void)
{
	uint32_t rc = 0;
	uint32_t count = 0;
	uint32_t i = 0;
	uint32_t time_start=0;
	uint8_t *pr_rcv=NULL;
	uint8_t buf[32];


	msg_out("c run\n");
	TaskDelay(1100);
	for (i = 0; i < 30; i++)
	{
		msg_out("%c ", i + '0');
		buf[i]=i+'0';
	}


	GetStartDelayTime(&time_start);
	while (1)
	{

#if 1
		rc=get_dat_from_queue(&queue_timer2,&pr_rcv,1000,0);
		if(rc==os_true)
		{
			#if 0
			msg_out("ilen=%d\n",pr_rcv[0]);
			for(i=0;i<pr_rcv[0];i++)
			{
				msg_out("%d ",pr_rcv[i+1]);
			}
			msg_out("\n");
			#endif
			free_mem_to_pool(&pr_rcv);	
		}
	
#else


		TaskDelayPeriodic(20,&time_start);

		send_dat_to_uart3(buf,30);
#endif

		tog_pin_port(LED3);


	}
}


#define Adress_Ins_Res '1'

void task_uart1_rcv(void)
{
	uint8_t *pr_send = NULL;
	uint8_t *pr_dat = NULL;
	uint32_t st = 0;
	uint32_t rc = 0;
	uint32_t i = 0;
	uint32_t len = 0;
	uint8_t *prmem=NULL;

	TaskDelay(200);
	msg_out("uart1 rcv run\n");
	while (1)
	{

#if 1
		st = getDatFromMaster(Adress_Ins_Res, &pr_dat);
		if (st == FUN_OK)
		{
			len = pr_dat[3];
			msg_out("rcv ok,buf[3]=%d len=%d\n", pr_dat[3], len);

			for (i = 0; i < len + 8; i++)
			{
				msg_out("%x ", pr_dat[i]);
			}
			len = len + 8;

			#if 0
			prmem=os_malloc(len);
			for(i=0;i<len;i++)
			{
				prmem[i]=pr_dat[i];
			}
			msg_out("\nsmall mem dat=\n");
			for(i=0;i<len;i++)
			{
				msg_out("%x ",prmem[i]);
			}
			os_free(prmem);
			#endif

			uput_dat_to_queue(&queue_uart1_rcv, &pool_uart1_rcv, pr_dat, len, 2000);

		}
		else
		{
			TaskDelay(1);
		}
#else
		st=get_dat_from_queue(&queue_timer2,&pr_dat,1000,0);
		if(st==os_true)
		{
			msg_out("ilen=%d\n",pr_dat[0]);
			for(i=0;i<pr_dat[0];i++)
			{
				msg_out("%d ",pr_dat[i+1]);
			}
			msg_out("\n");
			uput_dat_to_queue(&queue_uart1_rcv, &pool_uart1_rcv, pr_dat, pr_dat[0], 2000);
			free_mem_to_pool(&pr_dat);	
		}


#endif
		
	}
}

void task_uart3_snd(void)
{
	uint32_t st = 0;
	uint8_t *buf = NULL;
	uint32_t i = 0;
	uint32_t len = 0;

	task_sleep(500);
	while (1)
	{

		st = get_dat_from_queue(&queue_uart3_snd, &buf, 1, 0);
		if (st == os_true)
		{
			len = buf[0];

#if 0			
			msg_out("\n the dat u3 need snd len=%d dat=\n", len);
			for (i = 0; i < len; i++)
			{
				msg_out("%x ", buf[i + 1]);
			}
			msg_out("\n");
#else
			RS485_ONE_SEND;
			Uart3_SendArray(buf+1,len);
			free_mem_to_pool(&buf);
			task_sleep(1);
			//RS485_ONE_RECEIVE;
#endif		
			
		}
		else
		{
			RS485_ONE_RECEIVE;

		}
	}
}

static uint16_t s_sr1=0;
static uint16_t s_sr2=0;

static uint16_t s_st_tx=0;
static uint16_t s_st_rx=0;

static uint16_t s_dat1=0,s_dat2=0;


#define u8 uint8_t

#define ADC_REG_STAT ((u8)(0x00<<3))
#define ADC_REG_MODE ((u8)(0x01<<3))
#define ADC_REG_CONFIG ((u8)(0x02<<3))
#define ADC_REG_DATA ((u8)(0x03<<3))
#define ADC_REG_ID ((u8)(0x04<<3))
#define ADC_REG_IO ((u8)(0x05<<3))
#define ADC_REG_OFFSET ((u8)(0x06<<3))

#define ADC_OP_READ ((u8)(0x40))
#define ADC_OP_WRITE ((u8)(0x00))


#define ADC_SPI_CS_CLR GPIOB->BSRR=GPIO_Pin_12
#define ADC_SPI_CS_SET GPIOB->BRR=GPIO_Pin_12


#define IN0             (0x3c49 << 2)           // �����ԣ�ȫ�����ڲ���׼4.096V������ͨ�������������ض�CFG
#define IN1             (0x3cc9 << 2)           
#define IN2             (0x3d49 << 2)  
#define IN3             (0x3dc9 << 2)  
#define IN4             (0x3e49 << 2)  
#define IN5             (0x3ec9 << 2)  
#define IN6             (0x3f49 << 2)  
#define IN7             (0x3fc9 << 2)  
#define M               8                       // channel num 


#define AD5410_LATCH_HIGH() GPIOC->BSRR=GPIO_Pin_4
#define AD5410_LATCH_LOW() GPIOC->BRR=GPIO_Pin_4

void AD5410xEnableOperate(void)
{
	uint32_t i=0;
	  //����LATCH
	AD5410_LATCH_HIGH();

	for(i=0;i<0xff;i++)
	{

	}

	//����LATCH
	AD5410_LATCH_LOW();

}

void AD5410xDisableOperate(void)
{
	uint32_t i=0;

	//����LATCH
	AD5410_LATCH_LOW();


	for(i=0;i<0xff;i++)
	{

	}
	//����LATCH
	AD5410_LATCH_HIGH();

}
#if 0
static __inline void spiWriteData(uint8_t *val ,uint32_t len)
{

	uint32_t count=0;
	uint8_t txsize=0,rxsize=0;
	uint8_t txallowed=0;
	

	AD5410xEnableOperate();

	txsize=len;
	rxsize=0;
	txallowed=1;
	
	
	
	while((txsize>0)||(rxsize>0)) //�о�����һ���ȽϺõ�˼·,�����ڷ��͵�ͬʱ��ؽ���.
	{
		if((txsize>0)&&(txallowed==1)&&((SPI1->SR & 0x02)!=0) )
		{
			SPI1->DR = val[len-txsize];
			txsize--;
		}
	}
	
	while((SPI1->SR & (0x01<<7))!=0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.


	AD5410xDisableOperate();


}
#else
static __inline void spiWriteData(uint8_t *val ,uint32_t len)
{

	uint32_t i=0;
	uint8_t txsize=0,rxsize=0;
	uint8_t txallowed=0;
	

	AD5410xEnableOperate();

	txsize=len;
	rxsize=0;
	txallowed=1;
	
	for(i=0;i<len;)
	{
		if((SPI1->SR &0x02)!=0)
		{
			SPI1->DR = val[i];
			i++;
		}
	}

	
	while((SPI1->SR & (0x01<<7))!=0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.


	AD5410xDisableOperate();


}



#endif

static uint8_t AD5410xWriteReg(uint8_t RegAddr, uint16_t value)
{
  
	uint8_t SendData[3]={0x00, 0x00, 0x00};
	uint32_t i=0;

	AD5410xEnableOperate();

	SendData[0] = RegAddr;
	SendData[1] = value>>8;
	SendData[2] = value&0xff;

	for(i=0;i<3;i++)
	{
		while((SPI1->SR & 0x02)!=0)
		{
			SPI1->DR = SendData[i];
		}
	}
	
	//while((SPI1->SR & 0x02)!=0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.
	while((SPI2->SR & (0x01<<7))!=0);


	AD5410xDisableOperate();
   
  return 0;
}
static uint8_t s_54_rx[3];
static uint32_t AD5410xReadReg(uint8_t whichreg)
{
  
	uint8_t SendData[3]={0x02, 0x00, 0x00};
	uint8_t rxdata[3]={0,0,0};
	uint32_t count=0;
	uint8_t datrx=0,dattx=0;
	uint8_t txsize=0,rxsize=0;
	uint8_t txallowed=0;

	
	uint32_t i=0;

	AD5410xEnableOperate();

	SendData[1] = whichreg>>8;
	SendData[2] = whichreg&0xff;

	spiWriteData(SendData,3);

	for(i=0;i<100;i++)
	{
	}


	AD5410xEnableOperate();

	SendData[0]=0;
	SendData[1]=0;
	SendData[1]=0;

	txsize=3;
	rxsize=3;
	txallowed=1;
	
	
	
	while((txsize>0)||(rxsize>0)) //�о�����һ���ȽϺõ�˼·,�����ڷ��͵�ͬʱ��ؽ���.
	{
		if((txsize>0)&&(txallowed==1)&&((SPI1->SR & 0x02)!=0) )
		{
			SPI1->DR = SendData[3-txsize];
			txsize--;
			txallowed=0;
		}
	
		if( (rxsize>0)&&((SPI1->SR&0x01)!=0) )
		{
	
			datrx  =  SPI1->DR;
			
			txallowed=1;
			rxdata[3-rxsize]= datrx;
			rxsize--;

		}
	
	
	}

	
	while((SPI2->SR & (0x01<<7))!=0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.


	AD5410xDisableOperate();


	s_54_rx[0]=rxdata[0];
	s_54_rx[1]=rxdata[1];
	s_54_rx[2]=rxdata[2];
	
  return (rxdata[0]<<16) | (rxdata[1]<<8) | rxdata[2];
}



static uint8_t AD5410xSoftReset()
{
	uint8_t SendValidData[3]={0x56, 0x00, 0x00};
	uint32_t i=0;
	AD5410xEnableOperate();
	for(i=0;i<3;i++)
	{
		while((SPI1->SR & 0x02)!=0)
		{
			SPI1->DR = SendValidData[i];
		}
	}
	//while((SPI2->SR & 0x02)!=0); //�Ƿ�Ҫ����,�ж�txΪ�յı�־,����Ҫ�ж�busy��־.
	while((SPI1->SR & (0x01<<7))!=0);
	for(i=0;i<100;i++)
	{
	}
	AD5410xDisableOperate();
	return 0;
}


void task_run(void)
{
	uint32_t rc=0;
	uint32_t i=0;
	uint8_t flag=0;
	volatile uint16_t tmp=0;
	uint32_t time_start=0;
	uint16_t tmpreg = 0;
	GPIO_InitTypeDef GPIO_InitStructure; //��������GPIO�ڵĻ�������

	uint16_t SPI_TX_BUF[1];     //CFG�Ĵ�������
	uint16_t SPI_RX_BUF[1];


	uint32_t txsize=0,rxsize=0;
	uint8_t txallowed=1;

	uint8_t adst=0;

	uint8_t count=0;
	uint16_t ad7689_cfg[M] = {IN2, IN3, IN4, IN5, IN6, IN7, IN0, IN1 };  // CFG���У���data��2������
	uint16_t rxdata[M];
	uint16_t val_cont=0;

	uint8_t ad54_sendbuf[3];

	uint32_t readdat=0;

	task_sleep(500);

/*---adc ��spi��ʼ��---*/

#if 0
	SPI2->I2SCFGR = 0;

	SPI2->CR1=0;
	SPI2->CR1 |= (0x01<<2)|(0x03<<3);
	
	SPI2->CR2=0;
	SPI2->CR2 |= (0x01<<2);

	SPI2->CR1 |= (0x01<<6);
#endif

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;//nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��������Ĳ�����ʼ��һ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��������Ĳ������г�ʼ��

	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //�Ƿ��б�Ҫ�����ٴγ�ʼ��
	
	
	tmpreg = SPI2->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
					(SPI_DataSize_16b) | (SPI_CPOL_Low) |  
					(SPI_NSS_Soft) | (SPI_CPHA_1Edge) |   //�ƺ��͵��� ��1����
					(SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI2->CR1 = tmpreg;
	
	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI2->I2SCFGR &= ((uint16_t)0xF7FF); 

	SPI2->CR1 |= (0x01<<6);

#if 1
/*---dac ��spi �ĳ�ʼ��*/
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,  ENABLE );//SPI2ʱ��ʹ�� 


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����ߵ�����ٶ���2MHZ,10MHZ,50MHZ,


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;//nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��������Ĳ�����ʼ��һ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��������Ĳ������г�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��������Ĳ������г�ʼ��

	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_SetBits(GPIOA,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //�Ƿ��б�Ҫ�����ٴγ�ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;//latch
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);

	tmpreg = SPI1->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
					(SPI_DataSize_8b) | (SPI_CPOL_Low) |  
					(SPI_NSS_Soft) | (SPI_CPHA_1Edge) |   //�ƺ��͵��� ��1����
					(SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI1->CR1 = tmpreg;
	
	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI1->I2SCFGR &= ((uint16_t)0xF7FF); 

	SPI1->CR1 |= (0x01<<6);


	TaskDelay(500);

	//val_cont=(0x11<<8)|(0x00);
	///AD5410xWriteReg(0x55,val_cont);

	ad54_sendbuf[0]=0x55;
	ad54_sendbuf[1]=0x11;
	ad54_sendbuf[2]=0x00;
	spiWriteData(ad54_sendbuf,3);

#endif	
	
	GetStartDelayTime(&time_start);
	for(;;)
	{
		msg_out("task running %d \n",rc);
		rc++;

		TaskDelayPeriodic(1000,&time_start);
#if 1
		msg_out("CR1=%x\n",SPI2->CR1);
		msg_out("SR=%x\n",SPI2->SR);
		if(flag==1)
		{
			flag=0;
			tmp = SPI2->SR;
			SPI2->CR1 = tmpreg;
			
		}
#endif
		if(adst==0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_12);

			#if 1
			for(i=0;i<10;i++)
			{
				//delay
			}
			#else
				TaskDelay(10);
			#endif

			
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);

			TaskDelay(1);
			
			adst=1;
			txsize=1;
			rxsize=1;

			SPI_TX_BUF[0] = ad7689_cfg[count];
			
		}

		if(adst==1)
		{

			while((txsize>0)||(rxsize>0)) //�о�����һ���ȽϺõ�˼·,�����ڷ��͵�ͬʱ��ؽ���.
			{
				if((txsize>0)&&(txallowed==1)&&((SPI2->SR & 0x02)!=0) )
				{
					SPI2->DR = SPI_TX_BUF[0];
					txsize--;
					txallowed=0;
				}
			
				if( (rxsize>0)&&((SPI2->SR&0x01)!=0) )
				{
			
					SPI_RX_BUF[0]  =  SPI2->DR;
					rxsize--;
					txallowed=1;
					rxdata[count]= SPI_RX_BUF[0];
					count=(count+1)%M;

				}
			
			
			}

			msg_out("count=%d   rx=%x   v=%d\n",count,SPI_RX_BUF[0],(uint32_t)(SPI_RX_BUF[0]*1.0/0xffff*4096));

			adst=0;
		}

#if 1
		if((rc%4)==0)
		{
			//readdat=AD5410xReadReg(0x01);
			//AD5410xWriteReg(0x01,0x7fff);
			ad54_sendbuf[0]=0x01;
			ad54_sendbuf[1]=(0xffff/4)>>8;
			ad54_sendbuf[2]=0xffff/4;
			spiWriteData(ad54_sendbuf,3);
		}
		else if((rc%4)==1)
		{

			ad54_sendbuf[0]=0x01;
			ad54_sendbuf[1]=(0xffff/4*2)>>8;
			ad54_sendbuf[2]=0xffff/4*2;

			spiWriteData(ad54_sendbuf,3);
			//AD5410xWriteReg(0x01,0x00);
		}
		else if((rc%4)==2)
		{
			ad54_sendbuf[0]=0x01;
			ad54_sendbuf[1]=(0xffff/4*3)>>8;
			ad54_sendbuf[2]=0xffff/4*3;

			spiWriteData(ad54_sendbuf,3);
		}
		else if((rc%4)==3)
		{
			ad54_sendbuf[0]=0x01;
			ad54_sendbuf[1]=(0xffff/4*4)>>8;
			ad54_sendbuf[2]=0xffff/4*4;

			spiWriteData(ad54_sendbuf,3);
		}
#endif		

	}

}




#define mem_buf_size	1024
static uint8_t s_buf[mem_buf_size];

int main(void)
{
	uint32_t rc = 0;

	close_all_interruct();
	/* Set the Vector Table base location at 0x3000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	BKP_DeInit();
	RCC_Configuration();
	uart1_dma_init();

	uart3_dma_init();

	init_ins_res_port();
	init_led_port();


	tog_pin_port(LED1);
	tog_pin_port(LED3);

	TIM2_init();
	
	/*---��ʼ��С�ڴ��---*/
	rc=init_mem(s_buf,mem_buf_size);
	if(rc!=1)
	{
		while(1);
	}
	/*--��������--*/
	/*taskA��ʱΪ���ݴ���,�����ȼ��ݶ�Ϊ���*/
	selfos_create_task(&tcb_task_deal_ins_res, task_deal_ins_res, &task_deal_ins_res_Stk[TASK_DEAL_INSRES_STK_SIZE - 1], 4);
	selfos_create_task(&taskB, fun_taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 10);
	selfos_create_task(&taskC, fun_taskc, &taskC_Stk[TASKC_STK_SIZE - 1], 10);
	/*������������ȼ��Ը�һ��*/
	selfos_create_task(&tcb_task_uart1_rcv, task_uart1_rcv,
					   &task_uart1_rcv_Stk[TASK_UART1_RCV_STK_SIZE - 1], 5);
	selfos_create_task(&tcb_task_uart3_snd, task_uart3_snd,
					   &task_uart3_snd_Stk[TASK_UART3_SND_STK_SIZE - 1], 9);

	selfos_create_task(&tcb_task_run, task_run,
					   &task_run_Stk[TASK_RUN_STK_SIZE - 1], 3);

	
					   

	/*--�����ź���---*/
	sem_creat(&testsem, 1, 1);
	sem_creat(&sem_uart1rcv, 1, 1);
	sem_creat(&sem_uart3rcv, 1, 1);
	sem_creat(&sem_deal_complete, 1, 1);

	/*--�����ڴ��----*/
	rc = creat_mem_pool(&pool_uart1_rcv, buf_mempool_uart1_rcv, LEN_UART1_RCV_MEM, DEEP_UART1_RCV_MEM);
	if (rc != os_true)
	{
		while(1);
	}
	rc = creat_mem_pool(&pool_uart3_snd, buf_mempool_uart3_snd, LEN_UART3_SND_MEM, DEEP_UART3_SND_MEM);
	if (rc != os_true)
	{
		while(1);
	}
	rc=creat_mem_pool(&pool_timer2,buf_mempool_timer2,LEN_TIMER2_MEM,DEEP_TIMER2_MEM);
	if(rc != os_true)
	{
		while(1);
	}


	/*--��������----*/
	queue_creat(&queue_uart1_rcv, queue_mem_uart1_rcv, DEEP_QUEUE_UART1_RCV);
	queue_creat(&queue_uart3_snd, que_mem_uart3_snd, DEEP_QUE_UART3_SND);
	queue_creat(&queue_timer2,que_mem_timer2,DEEP_QUE_TIMER2);

	/*--selfos_start��������������,���ܱ��޸�λ��--*/
	selfos_start();
	open_all_interruct();

	while (1)
	{
	}
	return 1;
}
