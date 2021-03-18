#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"

#include "../os/task.h"



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



void taska(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;

	TaskDelay(500);
	while (1) 
	{
		tog_pin_port(LED1);
		TaskDelay(800);
		

	}
}

void taskb(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;

	//TaskDelay(800);
	while (1) 
	{
		tog_pin_port(LED2);
		//TaskDelay(800);
		

	}
}


void taskc(void) 
{
	uint32_t rc=0;
	uint32_t count=0;
	uint32_t i=0;

	//TaskDelay(1100);
	while (1) 
	{
		tog_pin_port(LED3);
		TaskDelay(800);
		
		

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

	
	selfos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1],1);  
	selfos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1],1);  
	selfos_create_task(&taskC, taskc, &taskC_Stk[TASKC_STK_SIZE - 1],1);
	
	
	TIM2_init();
	open_all_interruct();
 	selfos_start();
	
	
    while (1) 
	{

    }
	return 1;
}

