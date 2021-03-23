
#include "null.h"
#include "stdint.h"
#include "sem.h"
#include "stm32f10x.h"


extern void OStaskDelay(uint32_t dly);


uint32_t sem_creat(SemCB * pr,uint32_t maxVal,uint32_t initVal)
{
	uint32_t st=0;
	if(pr==NULL)
	{
		return os_null_pr;
	}
	if(initVal>maxVal)
	{
		return os_false;
	}
	st=__get_CONTROL();
	if(st&0x02!=0)	/*����ʹ��psp,�����߳�ģʽ,�Ѿ���ʼ����,��û�����ж���*/
	{
		st=1;
	}
	if(st==1)
	{
		input_critical_area();
	}
	pr->maxVal=maxVal;
	pr->curVal=initVal;
	if(st==1)
	{
		exit_critical_area();
	}

	return os_true;
}

/*��ȡ�ź���*/
uint32_t __sem_acquire(SemCB *pr)
{
	uint32_t rt=os_false;
	if(pr==NULL)
	{
		return os_null_pr;
	}
	

	if(pr->curVal>0)
	{
		pr->curVal--;
		rt=os_true;
	}
	else
	{
		rt=os_false;
	}


	return rt;

}

/*��������ʱ�ȴ����ٽ����������ź�����ȡ����*/
uint32_t sem_acquire(SemCB *pr,uint32_t delay)
{
	uint32_t rc=0;
	
	if(pr==NULL)
	{
		return os_null_pr;
	}


		input_critical_area();
		rc=__sem_acquire(pr);
		if(rc==os_true)
		{
			delay=0;
		}
		else if(rc==os_false)
		{

		}
		else if(rc==os_null_pr)
		{
			delay=0;
		}
		else
		{
			delay=0;
		}

		if(delay>0)
		{

			OS_setCurInfoSpdTask((uint32_t)pr,delay);

		}

		exit_critical_area();


		rc=os_true;
		if(delay>0)
		{
			OS_readyToSwitch();
			//todo:�Ӵ�λ�ÿ�ʼ,��Ӧ�ý����ٽ���
			while(gp_selfos_cur_task->state==OS_SUSPEND);
			
			if(gp_selfos_cur_task->spd_source==os_spd_timeout)
			{
				gp_selfos_cur_task->spd_source=os_spd_init;
				rc=os_false;
			}
			else
			{
				input_critical_area();
				rc=__sem_acquire(pr);
				exit_critical_area();

			}
		}


	return rc;
}

/*�ͷ��ź���*/
void sem_release(SemCB *pr)
{
	uint8_t need_rel=0;
	if(pr==NULL)
	{
		return ;
	}
	input_critical_area();

	if(pr->curVal==0)
	{

	}

	if(pr->curVal<pr->maxVal)
	{
		pr->curVal++;
	}
	else
	{
		;
	}

	need_rel=1;

	
	if(need_rel==1)
	{
		OS_relSpdTask((uint32_t)pr);
	}


	
	exit_critical_area();

	return;
}


/***********************************************
 *fun     :	���ж��п���ʹ�õĻ�ȡ�ź�������,�������в��ܼ��볬ʱ����
 *name    :
 *var     :
 *return  :os_true:��ʾ��ȡ���ź���,os_fault��ʾû�л�ȡ��,os_null_pr��ʾerr
 ************************************************/
uint32_t isem_acquire(SemCB *pr)
{
	uint32_t rc=0;
	
	if(pr==NULL)
	{
		return os_null_pr;
	}
	rc=__sem_acquire(pr);

	return rc;
}

/************************************************ 
 *fun     :���ж����ͷ��ź���
 *name    :
 *var     :
 *return  :
 ************************************************/

void isem_release(SemCB *pr)
{

	if(pr==NULL)
	{
		return ;
	}
	if(pr->curVal==0)
	{

	}
	if(pr->curVal<pr->maxVal)
	{
		pr->curVal++;
	}
	else
	{
		;
	}
	/*�������ͷŸ��ź�����,�Ƿ�����ĳ����������,�����,�򽫸�������뵽run̬,������������ʱ�����Ҫ����run̬���������ȼ����*/
	OS_relSpdTask((uint32_t)pr);

	return;
}

























