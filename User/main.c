#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"

#include "../os/task.h"

#include "../os/mem_manage.h"
#include "../os/queue.h"
#include "../os/sem.h"


/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/


u8 read_data[8] = {0};
u8 test_data[512] = {0};
u8 command = 0;
u8 Sand_data[600] = {0};
u8 REALY_DATA_1, REALY_DATA_2, REALY_DATA_3, REALY_DATA_4, REALY_DATA_5;


/*---任务-----*/
#define TASKA_STK_SIZE 128
static unsigned int taskA_Stk[TASKA_STK_SIZE];
volatile struct selfos_task_struct taskA;


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




/*--信号量----*/
SemCB testsem;
SemCB sem_uart1rcv;
SemCB sem_uart3rcv;
SemCB sem_deal_complete;



/*----内存块----*/
#define LEN_UART1_RCV_MEM	12
#define DEEP_UART1_RCV_MEM	15
static mem_pool pool_uart1_rcv;
static uint8_t buf_mempool_uart1_rcv[LEN_UART1_RCV_MEM*DEEP_UART1_RCV_MEM];

#define LEN_UART3_SND_MEM	12
#define DEEP_UART3_SND_MEM	16
static mem_pool pool_uart3_snd;
static uint8_t buf_mempool_uart3_snd[LEN_UART3_SND_MEM*DEEP_UART3_SND_MEM];







/*---队列-----*/
#define DEEP_QUEUE_UART1_RCV	12
static uint32_t queue_mem_uart1_rcv[DEEP_QUEUE_UART1_RCV];
QueueCB queue_uart1_rcv;



#define DEEP_QUE_UART3_SND		DEEP_UART3_SND_MEM
static uint32_t que_mem_uart3_snd[DEEP_QUE_UART3_SND];
QueueCB queue_uart3_snd;








void fun_taska(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;
	uint8_t *pr_rcv=NULL;
	uint32_t len=0;
	uint8_t buf[48]=0;
	uint8_t *pr=buf;

	msg_out("a run\n");
	TaskDelay(500);
	while (1) 
	{
			
		tog_pin_port(LED1);
		rc=get_dat_from_queue(&queue_uart1_rcv,&pr_rcv,100,0);
		if(rc==os_true)
		{
			len+=pr_rcv[0];
			os_memcpy(pr,pr_rcv+1,pr_rcv[0]);
			free_mem_to_pool(&pr_rcv);
			pr=pr+len;
		}
		else
		{
			if(len!=0)
			{
				msg_out("\n rc= %x dat_rcv = ",rc);
				for(i=0;i<len;i++)
				{
					msg_out("%x ",buf[i]);	
				}
				msg_out("\n");
				
				deal_master_cmd(buf);
				
			}
			len=0;
			pr=buf;

		}
		

		
	}
}

void fun_taskb(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;
	uint32_t time_now=0,time_pre=0;
	msg_out("b run\n");
	TaskDelay(800);

	time_pre=get_OS_time();
	while (1) 
	{
		//msg_out("b send sem\n");
		sem_release(&testsem);
		tog_pin_port(LED2);
		TaskDelay(1000);

		time_now=get_OS_time();
		if(time_now!=time_pre)
		{
			//msg_out("--diff time=%d\n",time_now-time_pre);
		}

		time_pre=time_now;
	}
}


void fun_taskc(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;
	msg_out("c run\n");
	TaskDelay(1100);
	for(i=0;i<30;i++)
	{
		msg_out("%c ",i+'0');
	}
	while (1) 
	{
		rc=sem_acquire(&testsem,4000);
		if(rc==os_true)
		{
			//msg_out("c got sem\n");
			tog_pin_port(LED3);
		}
		//TaskDelay(2000);
		

	}
}


uint32_t uput_dat_to_queue(QueueCB *pr_q,mem_pool *pr_pool,uint8_t * pr_dat,uint32_t len,uint32_t delay)
{
	uint8_t *pr_send=NULL;
	uint32_t rc=0;

	while(len>0)
	{
		pr_send=get_mem_from_pool(pr_pool,LEN_UART1_RCV_MEM);
		if(pr_send!=NULL)
		{
			if(len>=8)
			{
				pr_send[0]=8;
				os_memcpy(pr_send+1,pr_dat,8);

				rc=put_dat_to_queue(pr_q,pr_send,delay,0);
				if(rc!=os_true)
				{
					return rc;
				}
				len=len-8;
				pr_dat+=8;
			}
			else
			{
				pr_send[0]=len;
				os_memcpy(pr_send+1,pr_dat,len);

				rc=put_dat_to_queue(pr_q,pr_send,delay,0);
				if(rc!=os_true)
				{
					return rc;
				}
				len=0;
			}
		}
	}

	return os_true;

}

uint32_t send_dat_to_uart3(uint8_t *buf,uint32_t len)
{

	return uput_dat_to_queue(&queue_uart3_snd,&pool_uart3_snd,buf,len,100);

}

#define Adress_Ins_Res		'1'

void task_uart1_rcv(void)
{
	uint8_t *pr_send=NULL;
	uint8_t *pr_dat=NULL;
	uint32_t st=0;
	uint32_t rc=0;
	uint32_t i=0;
	uint32_t len=0;

	TaskDelay(200);
	msg_out("uart1 rcv run\n");
	while(1)
	{

		st=getDatFromMaster(Adress_Ins_Res,&pr_dat);
		if(st==FUN_OK)
		{
			len=pr_dat[3];
			msg_out("rcv ok,buf[3]=%d len=%d\n",pr_dat[3],len);

			for(i=0;i<len+8;i++)
			{
				msg_out("%x ",pr_dat[i]);
			}
			len=len+8;
		
			uput_dat_to_queue(&queue_uart1_rcv,&pool_uart1_rcv,pr_dat,len,2000);
			
			send_dat_to_uart3(pr_dat,len);
			
		}
		else
		{
			TaskDelay(1);
		}

	}
}

void task_uart3_snd(void)
{
	uint32_t st=0;
	uint8_t *buf=NULL;
	uint32_t i=0;
	uint32_t len=0;

	
	task_sleep(500);
	while(1)
	{

		st=get_dat_from_queue(&queue_uart3_snd, &buf, 1000, 0);
		if(st==os_true)
		{
			len=buf[0];
			msg_out("\n the dat u3 need snd len=%d dat=\n",len);
			for(i=0;i<len;i++)
			{
				msg_out("%x ",buf[i+1]);
			}
			msg_out("\n");

		}
		else
		{

		}

	}

}


int main(void)
{
	uint32_t rc=0;
	
	
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


	/*--创建信号量---*/
	sem_creat(&testsem, 1,1);
	sem_creat(&sem_uart1rcv, 1,1);
	sem_creat(&sem_uart3rcv, 1,1);
	sem_creat(&sem_deal_complete,1,1);

	/*--创建内存块----*/

	
	rc=creat_mem_pool(&pool_uart1_rcv,buf_mempool_uart1_rcv,LEN_UART1_RCV_MEM,DEEP_UART1_RCV_MEM);
	if(rc!=os_true)
	{
		while(1);
	}

	rc=creat_mem_pool(&pool_uart3_snd,buf_mempool_uart3_snd,LEN_UART3_SND_MEM,DEEP_UART3_SND_MEM);
	if(rc!=os_true)
	{
		while(1);
	}



	/*--创建队列----*/
	queue_creat(&queue_uart1_rcv,queue_mem_uart1_rcv,DEEP_QUEUE_UART1_RCV);
	queue_creat(&queue_uart3_snd,que_mem_uart3_snd, DEEP_QUE_UART3_SND);




	/*--创建任务--*/
	/*taskA暂时为数据处理,其优先级暂定为最高*/
	selfos_create_task(&taskA, fun_taska, &taskA_Stk[TASKA_STK_SIZE - 1],3);  
	selfos_create_task(&taskB, fun_taskb, &taskB_Stk[TASKB_STK_SIZE - 1],10);  
	selfos_create_task(&taskC, fun_taskc, &taskC_Stk[TASKC_STK_SIZE - 1],10);
	/*接收任务的优先级略高一点*/
	selfos_create_task(&tcb_task_uart1_rcv, task_uart1_rcv,\
						&task_uart1_rcv_Stk[TASK_UART1_RCV_STK_SIZE - 1],5);
	selfos_create_task(&tcb_task_uart3_snd, task_uart3_snd,\
						&task_uart3_snd_Stk[TASK_UART3_SND_STK_SIZE - 1],6);

	



 	selfos_start();
	open_all_interruct();

	
    while (1) 
	{

    }
	return 1;
}

