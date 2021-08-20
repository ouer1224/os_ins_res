#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"

#include "../os/task.h"

#include "../os/mem_manage.h"
#include "../os/queue.h"
#include "../os/sem.h"
#include "../os/smallMem.h"
#include "ad7699.h"
#include "ad5422.h"
/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/

u8 read_data[8] = {0};
u8 test_data[512] = {0};
u8 command = 0;
u8 Sand_data[600] = {0};
u8 REALY_DATA_1, REALY_DATA_2, REALY_DATA_3, REALY_DATA_4, REALY_DATA_5;

/*---任务-----*/
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

/*--信号量----*/
SemCB testsem;
SemCB sem_uart1rcv;
SemCB sem_uart3rcv;
SemCB sem_deal_complete;

/*----内存块----*/
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
static uint8_t buf_mempool_timer2[LEN_TIMER2_MEM * DEEP_TIMER2_MEM];

/*---队列-----*/
#define DEEP_QUEUE_UART1_RCV DEEP_UART1_RCV_MEM
static uint32_t queue_mem_uart1_rcv[DEEP_QUEUE_UART1_RCV];
QueueCB queue_uart1_rcv;

#define DEEP_QUE_UART3_SND DEEP_UART3_SND_MEM
static uint32_t que_mem_uart3_snd[DEEP_QUE_UART3_SND];
QueueCB queue_uart3_snd;

#define DEEP_QUE_TIMER2 DEEP_TIMER2_MEM
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
	uint8_t j = 0;

	msg_out("a run\n");
	TaskDelay(1500);
	while (1)
	{
		j++;
		if (j % 50 == 0)
		{
			tog_pin_port(LED1);
		}
		rc = get_dat_from_queue(&queue_uart1_rcv, &pr_rcv, 100, 0);
		if (rc == os_true)
		{
			len += pr_rcv[0];
			if (len <= 48)
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

		if (sem_acquire(&sem_uart1rcv, 2000) == os_true) /*2ms发送一次串口数据,测试临界区的故障*/
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
	uint32_t time_start = 0;
	uint8_t *pr_rcv = NULL;
	uint8_t buf[32];

	msg_out("c run\n");
	TaskDelay(1100);
	for (i = 0; i < 30; i++)
	{
		msg_out("%c ", i + '0');
		buf[i] = i + '0';
	}

	GetStartDelayTime(&time_start);
	while (1)
	{

#if 1
		rc = get_dat_from_queue(&queue_timer2, &pr_rcv, 1000, 0);
		if (rc == os_true)
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

		TaskDelayPeriodic(20, &time_start);

		send_dat_to_uart3(buf, 30);
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
	uint8_t *prmem = NULL;

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
		st = get_dat_from_queue(&queue_timer2, &pr_dat, 1000, 0);
		if (st == os_true)
		{
			msg_out("ilen=%d\n", pr_dat[0]);
			for (i = 0; i < pr_dat[0]; i++)
			{
				msg_out("%d ", pr_dat[i + 1]);
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
			Uart3_SendArray(buf + 1, len);
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


void task_run(void)
{
	uint32_t rc = 0;
	uint32_t i = 0;
	uint8_t flag = 0;
	volatile uint16_t tmp = 0;
	uint32_t time_start = 0;
	uint16_t tmpreg = 0;
	GPIO_InitTypeDef GPIO_InitStructure; //用于设置GPIO口的基本参数

	uint16_t SPI_TX_BUF[1]; //CFG寄存器内容
	uint16_t SPI_RX_BUF[1];

	uint32_t txsize = 0, rxsize = 0;
	uint8_t txallowed = 1;

	uint8_t adst = 0;

	uint8_t count = 0;

	uint16_t val_cont = 0;

	uint8_t ad54_sendbuf[3];

	uint32_t readdat = 0;

	uint8_t datChain[8][3];

	task_sleep(500);

	/*---adc 的spi初始化---*/

	#if 0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTB时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI2时钟使能

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //选择最高的输出速度有2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //按照上面的参数初始化一下
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			  //sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			  //MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			  //MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //按照上面的参数进行初始化

	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //是否有必要进行再次初始化

	tmpreg = SPI2->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
						 (SPI_DataSize_16b) | (SPI_CPOL_Low) |
						 (SPI_NSS_Soft) | (SPI_CPHA_1Edge) | //似乎就得是 第1个沿
						 (SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI2->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI2->I2SCFGR &= ((uint16_t)0xF7FF);

	SPI2->CR1 |= (0x01 << 6);
	#else
	osassert(init_ad7699());
	osassert(init7699SelectIO());
	#endif



#if 0
	/*---dac 的spi 的初始化*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTB时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI2时钟使能

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //选择最高的输出速度有2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数初始化一下
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			  //sck
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			  //MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			  //MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  //
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //按照上面的参数进行初始化

	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	GPIO_SetBits(GPIOA, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //是否有必要进行再次初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		 //latch
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	tmpreg = SPI1->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= 0x3040;

	tmpreg |= (uint16_t)((uint32_t)(SPI_Direction_2Lines_FullDuplex) | (SPI_Mode_Master) |
						 (SPI_DataSize_8b) | (SPI_CPOL_Low) |
						 (SPI_NSS_Soft) | (SPI_CPHA_1Edge) | //似乎就得是 第1个沿
						 (SPI_BaudRatePrescaler_16) | (SPI_FirstBit_MSB));
	/* Write to SPIx CR1 */
	SPI1->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI1->I2SCFGR &= ((uint16_t)0xF7FF);

	SPI1->CR1 |= (0x01 << 6);

	msg_out("start chaline\n");

	TaskDelay(2500);


#if 0
	ad54_sendbuf[0]=0x55;
	ad54_sendbuf[1]=0x11;
	ad54_sendbuf[2]=0x00;
	spiWriteData(ad54_sendbuf,3);
#else


	for (i = 0; i < numofad542; i++)
	{

		memset(datChain, 0, 24);

		datChain[i][0] = 0x56;
		datChain[i][1] = 0;
		datChain[i][2] = 1;
		//spiChainWrite((void *)datChain, numofad542, sizeofad542dat);

		AD5410xWriteReg(i,0x55,(datChain[i][1]<<8)|(datChain[i][2]));
	}

TaskDelay(1);


	//配置为 菊花链
	for (i = 0; i < numofad542; i++)
	{

		memset(datChain, 0, 24);

		datChain[i][0] = 0x55;
		datChain[i][1] = 0x31;
		datChain[i][2] = 0x01 | (0x01 << 3);
		//spiChainWrite((void *)datChain, numofad542, sizeofad542dat);

		AD5410xWriteReg(i,0x55,(datChain[i][1]<<8)|(datChain[i][2]));
	}

	msg_out("cfg chainline\n");


#endif

#else
	init_ad5422_chain();
#endif


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //PORTC时钟使能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //选择最高的输出速度有2MHZ,10MHZ,50MHZ,

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOC, &GPIO_InitStructure);				  //按照上面的参数初始化一下

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			  //nss
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //
	GPIO_Init(GPIOC, &GPIO_InitStructure);				  //按照上面的参数初始化一下	

	GetStartDelayTime(&time_start);
	for (;;)
	{
		msg_out("task running %d \n", rc);
		rc++;

		TaskDelayPeriodic(1000, &time_start);
#if 1
		msg_out("CR1=%x\n", SPI2->CR1);
		msg_out("SR=%x\n", SPI2->SR);
		if (flag == 1)
		{
			flag = 0;
			tmp = SPI2->SR;
			SPI2->CR1 = tmpreg;
		}
#endif

		//GPIO_SetBits(GPIOC, GPIO_Pin_6);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		//selectWhich7699(1,1);
		selectWhich7699(rc%2,1);
		if (adst == 0)
		{
			adst=1;
			start_conver_ad7699();
		}

		if (adst == 1)
		{
			count = (count + 1) % 8;
			SPI_RX_BUF[0]=LoopReadVal_7699(count);

			msg_out("count=%d   rx=%x   v=%d\n", count, SPI_RX_BUF[0], (uint32_t)(SPI_RX_BUF[0] * 1.0 / 0xffff * 4096));

			adst = 0;
		}
		selectWhich7699(0xff,1);
#if 0
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
#else

		if ((rc % 4) == 0)
		{
			for(i=0;i<8;i++)
			{
				set5422VolOut(i,1000);
			}
		}
		#if 1
		else if ((rc % 4) == 1)
		{
			for(i=0;i<8;i++)
			{
				set5422VolOut(i,5000);
			}
		}
		else if ((rc % 4) == 2)
		{
			for(i=0;i<8;i++)
			{
				set5422VolOut(i,3000);
			}			
		}
		else if ((rc % 4) == 3)
		{
			for(i=0;i<8;i++)
			{
				set5422VolOut(i,0);
			}
		}
		#endif
#endif
	}
}

#define mem_buf_size 1024
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

	/*---初始化小内存块---*/
	rc = init_mem(s_buf, mem_buf_size);
	if (rc != 1)
	{
		while (1)
			;
	}
	/*--创建任务--*/
	/*taskA暂时为数据处理,其优先级暂定为最高*/
	selfos_create_task(&tcb_task_deal_ins_res, task_deal_ins_res, &task_deal_ins_res_Stk[TASK_DEAL_INSRES_STK_SIZE - 1], 4);
	selfos_create_task(&taskB, fun_taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 10);
	selfos_create_task(&taskC, fun_taskc, &taskC_Stk[TASKC_STK_SIZE - 1], 10);
	/*接收任务的优先级略高一点*/
	selfos_create_task(&tcb_task_uart1_rcv, task_uart1_rcv,
					   &task_uart1_rcv_Stk[TASK_UART1_RCV_STK_SIZE - 1], 5);
	selfos_create_task(&tcb_task_uart3_snd, task_uart3_snd,
					   &task_uart3_snd_Stk[TASK_UART3_SND_STK_SIZE - 1], 9);

	selfos_create_task(&tcb_task_run, task_run,
					   &task_run_Stk[TASK_RUN_STK_SIZE - 1], 3);

	/*--创建信号量---*/
	sem_creat(&testsem, 1, 1);
	sem_creat(&sem_uart1rcv, 1, 1);
	sem_creat(&sem_uart3rcv, 1, 1);
	sem_creat(&sem_deal_complete, 1, 1);

	/*--创建内存块----*/
	rc = creat_mem_pool(&pool_uart1_rcv, buf_mempool_uart1_rcv, LEN_UART1_RCV_MEM, DEEP_UART1_RCV_MEM);
	if (rc != os_true)
	{
		while (1)
			;
	}
	rc = creat_mem_pool(&pool_uart3_snd, buf_mempool_uart3_snd, LEN_UART3_SND_MEM, DEEP_UART3_SND_MEM);
	if (rc != os_true)
	{
		while (1)
			;
	}
	rc = creat_mem_pool(&pool_timer2, buf_mempool_timer2, LEN_TIMER2_MEM, DEEP_TIMER2_MEM);
	if (rc != os_true)
	{
		while (1)
			;
	}

	/*--创建队列----*/
	queue_creat(&queue_uart1_rcv, queue_mem_uart1_rcv, DEEP_QUEUE_UART1_RCV);
	queue_creat(&queue_uart3_snd, que_mem_uart3_snd, DEEP_QUE_UART3_SND);
	queue_creat(&queue_timer2, que_mem_timer2, DEEP_QUE_TIMER2);

	/*--selfos_start函数必须最后调用,不能被修改位置--*/
	selfos_start();
	open_all_interruct();

	while (1)
	{
	}
	return 1;
}
