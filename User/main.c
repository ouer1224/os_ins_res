#include "include.h"
#include "ins_res.h"
#include "uart3.h"
#include "os_sprintf.h"
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


	msg_out("\nI am Ins Res Bord\n");

#if 0
	GPIOC->CRH=0x03<<4;
	GPIOC->BRR=0x01<<9;

	
	GPIOA->CRH|=0x03<<((8-8)*4);
	GPIOA->BRR=0x01<<8;


	GPIOA->CRH|=0x03<<((12-8)*4);
	GPIOA->BRR=0x01<<12;



	GPIOA->CRH|=0x03<<((11-8)*4);
	GPIOA->BRR=0x01<<11;
#endif	

	GPIOA->CRH|=0x03<<((8-8)*4);
	GPIOA->BRR=0x01<<8;

	msg_out("GPIOA->CRH=%x\n",GPIOA->CRH);
	msg_out("GPIOB->CRH=%x\n",GPIOB->CRH);
	msg_out("GPIOC->CRH=%x\n",GPIOC->CRH);

	
	while(1)
	{
		delay_us(relay_time);
		loop_ins_res();

	}
}

