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




void fun_taska(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;

	msg_out("a run\n");
	TaskDelay(500);
	while (1) 
	{
		
		tog_pin_port(LED1);
		TaskDelay(500);
		msg_out("deal ok\n");
		sem_release(&sem_deal_complete);
		
	}
}

void fun_taskb(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;
	msg_out("b run\n");
	TaskDelay(800);
	while (1) 
	{
		msg_out("b send sem\n");
		sem_release(&testsem);
		tog_pin_port(LED2);
		TaskDelay(1000);
	
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
	uint8_t *buf=NULL;
	uint8_t st=0;	

	TaskDelay(200);
	msg_out("uart1 rcv run\n");
	while(1)
	{

		st=sem_acquire(&sem_deal_complete,(uint32_t)(-1));
		//st=getDatFromMaster(Adress_Ins_Res,&buf);
		if(st==os_true)
		{
			msg_out("rcv ok\n");
			sem_release(&sem_uart3rcv);
		}

	}
}


int main(void)
{
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

