#include"include.h"

#include "../os/task.h"
#include "../os/sem.h"
#include "../os/queue.h"
#include "../os/mem_manage.h"
#include "ins_res.h"

static u16 timer2_ms = TIMER2_DEFAULT_MS; //���䶨ʱ������
static u32 delay_count = 0; //��ʱ������
static u32 timer2_clock = 0; //ϵͳʱ��
static u32 timer2_shake = 0; //ϵͳʱ��
//static u32 timer2_dog_clock=0;//��

void TIM2_NVICinit(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configures the TIM2.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	/* ---------------------------------------------------------------
	  TIM2 Configuration: Output Compare Timing Mode:
	  TIM2CLK = 36 MHz, Prescaler = 4, TIM2 counter clock = 7.2 MHz
	  CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz
	  CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
	  CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4 Hz
	  CC4 update rate = TIM2 counter clock / CCR4_Val =  878.9 Hz
	((1+TIM_Prescaler )/72M)*(1+TIM_Period )=((1+7199)/72M)*(1+9999)=1��	  9999
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = timer2_ms;		   //20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; // 35999;    //1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* TIM2 enable counter */
	//TIM_Cmd(TIM2, ENABLE);
}
//��ʱ��2��ʼ��
void TIM2_init(void)
{
	TIM2_NVICinit();
	TIM2_Configuration();
}
//����/ֹͣ��ʱ��
void start_tim2(u8 on)
{
	if(on == 1)
	{
		TIM_Cmd(TIM2, ENABLE);
	}
	else
	{
		TIM_Cmd(TIM2, DISABLE);
	}
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

extern SemCB sem_uart1rcv;
uint32_t g_timer=0;
extern mem_pool pool_timer2;
extern QueueCB queue_timer2;
void TIM2_IRQHandler(void)
{
	uint8_t *pr_send=NULL;
	uint8_t rc=0;
	static uint32_t i=0;
	TIM_Cmd(TIM2, DISABLE);
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
		//ϵͳʱ��
		timer2_clock ++;//= timer2_ms;
		//��ʱ����
		delay_count += timer2_ms;
		//ȥ��������
		timer2_shake++;
#if 1
		i++;
		g_timer++;
		if(i%500==0)
		{
			tog_pin_port(LED4);
			isem_release(&sem_uart1rcv);


			pr_send=iget_mem_from_pool(&pool_timer2,8);
			if(pr_send!=NULL)
			{
				pr_send[0]=2;
				pr_send[1]=i;
				pr_send[2]=i>>8;

				iput_dat_to_queue(&queue_timer2,pr_send,0);
			}
			else
			{

			}


		}		

		Sys_readyToSwitch();
	
#endif	
		//		//����ʱ5min, ���ȣ�����Ӳ��
		//		timer3_dog_clock+=timer3_ms;
		//		if(timer3_dog_clock>300000)while(1);
	}
	TIM_Cmd(TIM2, ENABLE);
}

//***************************************************************************************************
//��������:�õ�timer2ʱ��
//����˵��:��
//ע������:
//����˵��:��
//***************************************************************************************************
u32 timer2_get_clock(void)
{
	return timer2_clock;
}
//***************************************************************************************************
//��������:�õ�timer2ʱ��
//����˵��:��
//ע������:
//����˵��:��
//***************************************************************************************************
u32 timer2_get_shake(void)
{
	return timer2_shake;
}
//**************************************************************************************************
//��������:���Ӧ�ò��Ƿ�ʱ�䵽��
//����˵��:timer---Ӧ�ò㶨��ļ�����	interval---���м��
//ע������:
//����˵��:��
//**************************************************************************************************
u8 timer2_check_timer(u32 *timer, u32 interval)
{
	if(interval)
	{
		//�ж����
		if(timer2_clock < (*timer))
		{
			*timer = 0;
		}
		//�Ƚ��Ƿ�ʱ�䵽
		if(timer2_clock - (*timer) >= interval)
		{
			*timer = timer2_clock;
			return 1;
		}
	}
	return 0;
}
