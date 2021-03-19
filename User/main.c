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


#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
#define TASKC_STK_SIZE 1024



static unsigned int taskA_Stk[TASKA_STK_SIZE];
static unsigned int taskB_Stk[TASKB_STK_SIZE];
static unsigned int taskC_Stk[TASKC_STK_SIZE];
static unsigned int taskD_Stk[TASKB_STK_SIZE];


volatile struct selfos_task_struct taskA;
volatile struct selfos_task_struct taskB;
volatile struct selfos_task_struct taskC;

/*--–≈∫≈¡ø----*/
SemCB testsem;





void taska(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;

	msg_out("a run\n");
	TaskDelay(500);
	while (1) 
	{
		msg_out("a is running\n");
		tog_pin_port(LED1);
		TaskDelay(500);
		

	}
}

void taskb(void) 
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


void taskc(void) 
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



	
	selfos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1],1);  
	selfos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1],1);  
	selfos_create_task(&taskC, taskc, &taskC_Stk[TASKC_STK_SIZE - 1],1);

 	selfos_start();

	sem_creat(&testsem, 1,1);

	open_all_interruct();

	
    while (1) 
	{

    }
	return 1;
}

