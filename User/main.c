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




/*---队列-----*/
#define DEEP_QUEUE_UART1_RCV	12
static uint32_t queue_mem_uart1_rcv[DEEP_QUEUE_UART1_RCV];
QueueCB queue_uart1_rcv;




void fun_taska(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;
	uint8_t *pr_rcv=NULL;
	uint32_t len=0;
	uint8_t buf[12]=0;

	msg_out("a run\n");
	TaskDelay(500);
	while (1) 
	{
		
		tog_pin_port(LED1);
		TaskDelay(500);
		msg_out("deal ok\n");


		rc=get_dat_from_queue(&queue_uart1_rcv,&pr_rcv,5000,0);
		if(rc!=NULL)
		{
			len=pr_rcv[0];
			os_memcpy(buf,pr_rcv+1,len);
			free_mem_to_pool(&pr_rcv);


			msg_out("\n dat_rcv = ");
			for(i=0;i<len;i++)
			{
				msg_out("%d ",buf[i]);	
			}
			msg_out("\n");
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
		msg_out("b send sem\n");
		sem_release(&testsem);
		tog_pin_port(LED2);
		TaskDelay(1000);

		time_now=get_OS_time();
		if(time_now!=time_pre)
		{
			msg_out("--diff time=%d\n",time_now-time_pre);
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
	while (1) 
	{
		rc=sem_acquire(&testsem,4000);
		if(rc==os_true)
		{
			msg_out("c got sem\n");
			tog_pin_port(LED3);
		}
		//TaskDelay(2000);
		

	}
}

void task_uart1_rcv(void)
{
	uint8_t *pr_send=NULL;
	uint8_t buf[8];
	uint32_t st=0;
	uint32_t rc=0;
	uint32_t i=0;
	

	TaskDelay(200);
	msg_out("uart1 rcv run\n");
	while(1)
	{

		//st=sem_acquire(&sem_deal_complete,(uint32_t)(-1));
		//st=getDatFromMaster(Adress_Ins_Res,&buf);

		TaskDelay(1000);

		st=os_true;
		if(st==os_true)
		{
			msg_out("rcv ok\n");
			for(i=0;i<8;i++)
			{
				buf[i]=i+timer2_get_clock();
			}
			
			pr_send=get_mem_from_pool(&pool_uart1_rcv,LEN_UART1_RCV_MEM);

			if(pr_send!=NULL)
			{
				pr_send[0]=8;
				os_memcpy(pr_send+1,buf,8);

				rc=put_dat_to_queue(&queue_uart1_rcv,pr_send,2000,0);
				if(rc!=os_true)
				{
					;
				}

			}

			
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


	/*--创建队列----*/
	queue_creat(&queue_uart1_rcv,queue_mem_uart1_rcv,DEEP_QUEUE_UART1_RCV);



	/*--创建任务--*/
	selfos_create_task(&taskA, fun_taska, &taskA_Stk[TASKA_STK_SIZE - 1],5);  
	selfos_create_task(&taskB, fun_taskb, &taskB_Stk[TASKB_STK_SIZE - 1],5);  
	selfos_create_task(&taskC, fun_taskc, &taskC_Stk[TASKC_STK_SIZE - 1],5);
	selfos_create_task(&tcb_task_uart1_rcv, task_uart1_rcv, &task_uart1_rcv_Stk[TASKC_STK_SIZE - 1],5);

 	selfos_start();
	open_all_interruct();

	
    while (1) 
	{

    }
	return 1;
}

