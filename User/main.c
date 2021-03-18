#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"

#include "xtos.h"





void delay(int c) 
{
    for (int i = 0; i < c; i++);
}

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32_t taskA_Stk[TASKA_STK_SIZE];
static uint32_t taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_struct taskA;
static struct xtos_task_struct taskB;

void task_switch() 
{
    if (gp_xtos_cur_task == &taskA)
        gp_xtos_next_task = &taskB;
    else
        gp_xtos_next_task = &taskA;

    xtos_context_switch();
}

void taska() {
    while (1) {
        //led_set_color(100, 0, 5);
        
		for (int i = 0; i < 1000; i++)
		{
            delay(10000);
		}
		tog_pin_port(LED3);
		tog_pin_port(LED1);
        task_switch();
    }
}

void taskb() {
    while (1) {
        //led_set_color(5, 0, 100);
        for (int i = 0; i < 1000; i++)
		{
            delay(10000);
		}
		tog_pin_port(LED4);
		tog_pin_port(LED2);
        task_switch();
    }
}






/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/


u8 read_data[8] = {0};
u8 test_data[512] = {0};
u8 command = 0;
u8 Sand_data[600] = {0};
u8 REALY_DATA_1, REALY_DATA_2, REALY_DATA_3, REALY_DATA_4, REALY_DATA_5;

int main(void)
{
	/* Set the Vector Table base location at 0x3000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	BKP_DeInit();
	RCC_Configuration();
	uart1_dma_init();
	uart3_dma_init();
	TIM2_init();

	init_ins_res_port();
	init_led_port();
	
	
	
    xtos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    xtos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    xtos_start();

    while (1) {

    }
	return 1;
}

