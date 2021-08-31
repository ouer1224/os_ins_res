


#include "ins_res.h"


#include "uart3.h"
#include "uart1.h"

#include "timer2.h"

#include "os_sprintf.h"
#include "../os/task.h"
#include "../os/sem.h"
#include "../os/queue.h"
#include "../os/mem_manage.h"
#include "../os/smallMem.h"

#define CMD_MASTER_INIT				'I'
#define CMD_MASTER_INQUIRE			'Q'
#define CMD_MASTER_READ				'R'
#define CMD_MASTER_WRITE			'W'

#define ADRESS_MASTER				'0'

extern QueueCB queue_adc_result;
extern SemCB sem_getadc_result;
extern QueueCB queue_dac_set;
extern mem_pool pool_dac_set;


Msg_res_master s_msg_res_master=
{
	'$','Q','0',0,
	null,
	0x00,0x0d0a
};



/***********************************************
*fun     :��ʼ���������Ϊ���,����ʱ����Ϊ0
*name    :
*var     :
*return  :
************************************************/
uint32_t init_ins_res_port(void)
{

	gpioConfig(Lock_Neg_Relay_1 ,1);
	SetPinOutput(Lock_Neg_Relay_1,0);	//����������,��ʼ����Ϊ0
	
	
	gpioConfig(Lock_Neg_Relay_2 ,1);
	SetPinOutput(Lock_Neg_Relay_2,0);
	
	gpioConfig(Lock_Neg_Relay_3 ,1);
	SetPinOutput(Lock_Neg_Relay_3,0);
	
	gpioConfig(Lock_Pos_Relay_1 ,1);
	SetPinOutput(Lock_Pos_Relay_1,0);
	
	gpioConfig(Lock_Pos_Relay_2 ,1);
	SetPinOutput(Lock_Pos_Relay_2,0);
	
	gpioConfig(Lock_Pos_Relay_3 ,1);
	SetPinOutput(Lock_Pos_Relay_3,0);
	
	gpioConfig(Pin_DB0	,1);
	SetPinOutput(Pin_DB0,0);
	
	gpioConfig(Pin_DB1	,1);
	SetPinOutput(Pin_DB1,0);
	
	gpioConfig(Pin_DB2	,1);
	SetPinOutput(Pin_DB2,0);
	
	gpioConfig(Pin_DB3	,1);	
	SetPinOutput(Pin_DB3,0);
	
	gpioConfig(Pin_DB4	,1);
	SetPinOutput(Pin_DB4,0);
	
	gpioConfig(Pin_DB5	,1);
	SetPinOutput(Pin_DB5,0);
	
	gpioConfig(Pin_DB6	,1);
	SetPinOutput(Pin_DB6,0);
	
	gpioConfig(Pin_DB7	,1);
	SetPinOutput(Pin_DB7,0);

	
	return FUN_OK;
}

uint32_t init_led_port(void)
{
	gpioConfig(LED1	,1);
	SetPinOutput(LED1,0);

	msg_out("0_A.CRH=%x CRL=%x\n",GPIOA->CRH,GPIOA->CRL);
	
	gpioConfig(LED2	,1);
	SetPinOutput(LED2,0);
	
	msg_out("led2 A.CRH=%x CRL=%x\n",GPIOA->CRH,GPIOA->CRL);

	gpioConfig(LED3	,1);
	SetPinOutput(LED3,0);
	msg_out("led3 A.CRH=%x CRL=%x\n",GPIOA->CRH,GPIOA->CRL);

	gpioConfig(LED4	,1);
	SetPinOutput(LED4,0);
	msg_out("led4 A.CRH=%x CRL=%x\n",GPIOA->CRH,GPIOA->CRL);

	return FUN_OK;
}
uint32_t tog_pin_port(void *pin)
{
		
	if(GetgpioInput(pin)!=0)
	//if(((GPIOC->IDR>>9)&0x01)!=0)
	{
		SetPinOutput(pin,0);
		//GPIOC->BRR=0x01<<9;
	}
	else
	{
		SetPinOutput(pin,1);
		//GPIOC->BSRR=0x01<<9;
	}

	return FUN_OK;
}

/***********************************************
*fun     :���ո�����dat,һ�ν���bitΪ1�Ķ�Ӧ������,clearΪ0.
*name    :
*var     :
*return  :
************************************************/
uint32_t openRelayGroup(uint8_t dat,uint32_t group)
{
	uint32_t i=0;

	(dat>>0)&0x01?SetPinOutput(Pin_DB0,0):0;
	(dat>>1)&0x01?SetPinOutput(Pin_DB1,0):0;	
	(dat>>2)&0x01?SetPinOutput(Pin_DB2,0):0;
	(dat>>3)&0x01?SetPinOutput(Pin_DB3,0):0;
	(dat>>4)&0x01?SetPinOutput(Pin_DB4,0):0;
	(dat>>5)&0x01?SetPinOutput(Pin_DB5,0):0;
	(dat>>6)&0x01?SetPinOutput(Pin_DB6,0):0;
	(dat>>7)&0x01?SetPinOutput(Pin_DB7,0):0;
	

	SetPinOutput(group,1);

	for(i=0;i<0xff;i++)
	{
	}
	
	SetPinOutput(group,0);


	return FUN_OK;
}


/***********************************************
*fun     :���ո�����dat,һ�ν���bitΪ1�Ķ�Ӧ������,clearΪ1.
*name    :
*var     :
*return  :
************************************************/
uint32_t closeRelayGroup(uint8_t dat,uint32_t group)
{
	uint32_t i=0;

	(dat>>0)&0x01?SetPinOutput(Pin_DB0,1):0;
	(dat>>1)&0x01?SetPinOutput(Pin_DB1,1):0;	
	(dat>>2)&0x01?SetPinOutput(Pin_DB2,1):0;
	(dat>>3)&0x01?SetPinOutput(Pin_DB3,1):0;
	(dat>>4)&0x01?SetPinOutput(Pin_DB4,1):0;
	(dat>>5)&0x01?SetPinOutput(Pin_DB5,1):0;
	(dat>>6)&0x01?SetPinOutput(Pin_DB6,1):0;
	(dat>>7)&0x01?SetPinOutput(Pin_DB7,1):0;

	SetPinOutput(group,1);

	for(i=0;i<0xff;i++)
	{
	}
	
	SetPinOutput(group,0);


	return FUN_OK;
}



uint32_t OpenSelectPosRelay(uint32_t opendat)
{
	
	openRelayGroup((opendat>>0)&0xff,(uint32_t)Lock_Pos_Relay_1);
	openRelayGroup((opendat>>8)&0xff,(uint32_t)Lock_Pos_Relay_2);
	openRelayGroup((opendat>>16)&0xff,(uint32_t)Lock_Pos_Relay_3);

	return FUN_OK;
}

uint32_t OpenSelectNegRelay(uint32_t opendat)
{
	
	openRelayGroup((opendat>>0)&0xff,(uint32_t)Lock_Neg_Relay_1);
	openRelayGroup((opendat>>8)&0xff,(uint32_t)Lock_Neg_Relay_2);
	openRelayGroup((opendat>>16)&0xff,(uint32_t)Lock_Neg_Relay_3);

	return FUN_OK;
}



uint32_t CloseSelectPosRelay(uint32_t closedat)
{
	
	closeRelayGroup((closedat>>0)&0xff,(uint32_t)Lock_Pos_Relay_1);
	closeRelayGroup((closedat>>8)&0xff,(uint32_t)Lock_Pos_Relay_2);
	closeRelayGroup((closedat>>16)&0xff,(uint32_t)Lock_Pos_Relay_3);

	return FUN_OK;
}

uint32_t CloseSelectNegRelay(uint32_t closedat)
{
	
	closeRelayGroup((closedat>>0)&0xff,(uint32_t)Lock_Neg_Relay_1);
	closeRelayGroup((closedat>>8)&0xff,(uint32_t)Lock_Neg_Relay_2);
	closeRelayGroup((closedat>>16)&0xff,(uint32_t)Lock_Neg_Relay_3);

	return FUN_OK;
}

/***********************************************
*fun     :��������������ĵ���ֵ����ĵ��贮��Ϻͱ��صĵ��贮���,���м̵����պ�
			���贮�����,1��ʾѡ�иõ���,0��ʾ��ѡ�иõ���ֵ
*name    :
*var     :
*return  :
************************************************/
uint32_t selectInsRes(uint32_t Pinput,uint32_t Ninput,uint32_t Plocal,uint32_t Nlocal)
{

	uint32_t opendat=0,closedat=0;
#if 0
	uint32_t tmp=0;
	tmp=Pinput^Plocal;
	opendat=Plocal&(~tmp);
	closedat=Pinput&(~tmp);

	OpenSelectPosRelay(opendat);
	CloseSelectPosRelay(closedat);

	tmp=Ninput^Nlocal;
	opendat=Nlocal&(~tmp);
	closedat=Ninput&(~tmp);
	OpenSelectNegRelay(opendat);
	CloseSelectNegRelay(closedat);
#else

	opendat=~Pinput;
	closedat=Pinput;

	msg_out("OB->ODR=%x C->ODR=%x\n",GPIOB->ODR,GPIOC->ODR);
	OpenSelectPosRelay(opendat);
	CloseSelectPosRelay(closedat);

	msg_out("PB->ODR=%x C->ODR=%x\n",GPIOB->ODR,GPIOC->ODR);


	opendat=~Ninput;
	closedat=Ninput;
	OpenSelectNegRelay(opendat);
	CloseSelectNegRelay(closedat);
	msg_out("NB->ODR=%x C->ODR=%x\n",GPIOB->ODR,GPIOC->ODR);


#endif
	return FUN_OK;
}



static uint32_t s_resbuf[24]={
	100,200,200,500,
	1000,2000,2000,5000,
	10000,20000,20000,50000,
	100000,200000,200000,500000,
	1000000,2000000,2000000,5000000,
	10000000,20000000,20000000,50000000
};

uint32_t getCombinationRes(uint32_t res)
{
	int32_t i=0;
	
	uint32_t dat=0;
	
	if(res>=111000000)
	{
		return 0xffffff;
	}
	for(i=23;i>=0;i--)
	{
		if(res>=s_resbuf[i])
		{
			res=res-s_resbuf[i];
			dat=dat|(0x01<<i);
		}
	}
	
	return dat;
}

/***********************************************
*fun     :��ȡ�����Ѿ����õľ�Ե��ֵ
*name    :
*var     :
*return  :
************************************************/
static uint32_t s_localres_vcc=0;
static uint32_t s_localres_gnd=0;
uint32_t getLocalResVcc(void)
{
	return s_localres_vcc;
}

uint32_t getLocalResGnd(void)
{
	return s_localres_gnd;
}

uint32_t setLocalResVcc(uint32_t val)
{
	s_localres_vcc=val;
	return FUN_OK;
}

uint32_t setLocalResGnd(uint32_t val)
{
	s_localres_gnd=val;
	return FUN_OK;
}


/***********************************************
*fun     :����Msg_res_master�еı�����Ϣ
*name    :
*var     :
*return  :����ֵΪ0xffff ffffʱ,��ʾ����,����ֵΪ1ʱ,���ͳɹ�
************************************************/
uint32_t send_msg_res(Msg_res_master * msg)
{
	uint32_t i=0;
	uint32_t len=0;
	uint8_t *pr=NULL;


	if((msg==NULL)||((msg->pr==NULL)&&(msg->msg_head.len!=0)))
	{	
		return FUN_ERR;
	}	

	len=sizeof(msg->msg_head);
	pr=(void *)&(msg->msg_head);

	send_master_dat(pr,len);


	len=msg->msg_head.len;
	pr=msg->pr;

	send_master_dat(pr,len);
	
	len=sizeof(msg->msg_tail);
	pr=(void *)&(msg->msg_tail);

	send_master_dat(pr,len);

	return FUN_OK;
}

/***********************************************
*fun     :�����ۼӺ�
*name    :
*var     :
*return  :32λ���ۼӺ�
************************************************/
uint32_t cal_checksum(uint8_t *buf,uint32_t len)
{
	uint32_t check=0;
	uint32_t i=0;
	for(i=0;i<len;i++)
	{
		check+=buf[i];
	}

	return check;
}

static uint8_t s_dat_init[12];
uint32_t handle_init_msg(uint8_t *buf,uint32_t len,Msg_res_master *msg)
{
	uint32_t i=0;
	if(len==0)
	{
		return FUN_ERR;
	}

	s_dat_init[0]='$';
	s_dat_init[1]='I';
	s_dat_init[2]='0';
	s_dat_init[3]=len+4;
	s_dat_init[4]=0;
	s_dat_init[5]=0;
	s_dat_init[6]=0;
	s_dat_init[7]=0;


	send_master_dat(s_dat_init,8);

	send_master_dat(buf,len);

	
	i=cal_checksum(buf,len);
	i+=s_dat_init[4];
	i+=s_dat_init[5];
	i+=s_dat_init[6];
	i+=s_dat_init[7];
	
	s_dat_init[0]=i;
	s_dat_init[1]=i>>8;
	s_dat_init[2]=0x0d;
	s_dat_init[3]=0x0a;
	

	send_master_dat(s_dat_init,4);

	
	return FUN_OK;
}


static uint8_t s_dat_inquire[]="INSU_RES";
uint32_t handle_inquire_msg(uint8_t *buf,uint32_t len,Msg_res_master *msg)
{

	if(len!=0)
	{
		return FUN_ERR;
	}

	msg->msg_head.len=sizeof(s_dat_inquire)-1;
	msg->msg_head.adress_dest=ADRESS_MASTER;
	msg->msg_head.funcode=CMD_MASTER_INQUIRE;
	msg->msg_head.head='$';

	msg->msg_tail.tail=0x0a0d;

	msg->pr=s_dat_inquire;

	msg->msg_tail.check=cal_checksum(msg->pr,msg->msg_head.len)%0xffff;

	send_msg_res(msg);
	return FUN_OK;
}

static uint8_t s_buf_write[12];

uint32_t handle_write_msg(uint8_t *buf,uint32_t len,Msg_res_master *msg)
{
	uint32_t *pr_dat_vcc=NULL;
	uint32_t *pr_dat_gnd=NULL;
	uint32_t *pr=NULL;
	uint32_t Pinput=0,Ninput=0,Plocal=0,Nlocal=0;	
	uint32_t localresP=0,localresN=0;
	uint8_t errcode=1;
	uint32_t st=0;
	uint8_t *pr_send = NULL;
	uint32_t i=0;
	uint32_t dac_set[9]={0};
	
	if(len!=8)
	{
		errcode=1;
	}
	else
	{

		for(i=0;i<8;i++)
		{
			dac_set[i]=i*100+1000;
		}
		pr_send=get_mem_from_pool(&pool_dac_set,32);
		if(pr_send!=NULL)
		{
			memcpy(pr_send,dac_set,32);	

			put_dat_to_queue(&queue_dac_set,pr_send,1000,0);

			msg_out("## setdac=%x  %d\n",dac_set[0],dac_set[0]);
		}
		else
		{
			msg_out("!!!!excep: no mem\n");
		}

	}
	
	pr=(void *)(s_buf_write+0);
	*pr=errcode;
	pr=(void *)(s_buf_write+4);
	*pr=getLocalResVcc();
	pr=(void *)(s_buf_write+8);
	*pr=getLocalResGnd();

	msg->msg_head.len=12;
	msg->msg_head.adress_dest=ADRESS_MASTER;
	msg->msg_head.funcode=CMD_MASTER_WRITE;
	msg->msg_head.head='$';

	msg->msg_tail.tail=0x0a0d;

	msg->pr=s_buf_write;

	msg->msg_tail.check=cal_checksum(msg->pr,msg->msg_head.len)%0xffff;

	send_msg_res(msg);


	return FUN_OK;
}

uint32_t handle_read_msg(uint8_t *buf,uint32_t len,Msg_res_master *msg)
{

	uint32_t *pr=NULL;
	uint32_t st=0;
	uint8_t *pr_rcv=NULL;
	uint32_t adc_buf[8+1];
	uint32_t i=0;
	uint8_t which7699=0;

	get_dat_from_queue(&queue_adc_result,&pr_rcv,0,0);
	
	for(i=0;i<3; i++)
	{
		sem_release(&sem_getadc_result);
		msg_out("==read_msg\n");
		st=get_dat_from_queue(&queue_adc_result,&pr_rcv,3000,0);
		if(st==os_true)
		{
			memcpy(adc_buf,pr_rcv,32);
			free_mem_to_pool(&pr_rcv);

			which7699=adc_buf[0]>>31;
			for(i=0;i<8;i++)
			{
				adc_buf[0]&=~(0x01<<31);
			}
			msg_out("which7699=%d  dac_res=%d\n",which7699,adc_buf[0]);
		}
		else
		{
			//exception
		}		
	}
	


	pr=(void *)(s_buf_write+0);
	*pr=0;
	pr=(void *)(s_buf_write+4);
	*pr=getLocalResVcc();
	pr=(void *)(s_buf_write+8);
	*pr=getLocalResGnd();


	msg->msg_head.len=32;
	msg->msg_head.adress_dest=ADRESS_MASTER;
	msg->msg_head.funcode=CMD_MASTER_READ;
	msg->msg_head.head='$';

	msg->msg_tail.tail=0x0a0d;

	msg->pr=s_buf_write;

	msg->msg_tail.check=cal_checksum(msg->pr,msg->msg_head.len)%0xffff;

	send_msg_res(msg);


	return FUN_OK;
}


/***********************************************
*fun     :�ж�У����,���У��Ϊ0xaa55,��buf�е�˳����55aa,��Ĭ������ȷ��,����У׼
*name    :
*var     :
*return  :
************************************************/
uint32_t judge_checkSum(uint8_t *buf,uint8_t len,uint16_t checksum)
{
	uint32_t i=0;
	uint32_t check=0;
	for(i=0;i<len;i++)
	{
		check=check+buf[i];
	}
	check%=0xffff;

	if(checksum==0xaa55)
	{
		return FUN_OK;
	}

	
	if(check!=checksum)
	{
		msg_out("judge=%x\n",check);
		return FUN_ERR;
	}

	return FUN_OK;
}



#define Pos_Cmd		1
#define Pos_Len		3


uint32_t deal_master_cmd(uint8_t *buf)
{

	uint8_t cmd=0;

	uint32_t len=0;
	uint8_t rc=0;
	uint16_t check=0;


	cmd=buf[Pos_Cmd];
	len=buf[Pos_Len];

	check=(buf[Pos_Len+len+1]<<0)|(buf[Pos_Len+len+2]<<8);



	rc=judge_checkSum(buf+Pos_Len+1,len,check);
	if(rc!=FUN_OK)
	{
		msg_out("\ncheck err\n");
		msg_out("check=%x %x\n",buf[Pos_Len+len+1],buf[Pos_Len+len+2]);		
		return FUN_ERR;
	}


	switch (cmd)
	{
		case CMD_MASTER_INIT:
		{
			handle_init_msg(buf+Pos_Len+1,len,&s_msg_res_master);
		}
		break;

		case CMD_MASTER_INQUIRE:
		{
			handle_inquire_msg(buf+Pos_Len+1,len,&s_msg_res_master);
		}
		break;

		case CMD_MASTER_READ:
		{
			handle_read_msg(buf+Pos_Len+1,len,&s_msg_res_master);
		}
		break;

		case CMD_MASTER_WRITE:
		{
			handle_write_msg(buf+Pos_Len+1,len,&s_msg_res_master);
		}
		break;


	
	}

	
	return FUN_OK;
}


#define Adress_Ins_Res		'1'

#define State_None		0
#define State_Head		1
#define State_Fun		2
#define State_Adress	3
#define State_Length	4
#define State_Data		5
#define State_CheckSum	6
#define State_Tail		7



#define Max_Dat_Len		248
static uint8_t s_datbuf[Max_Dat_Len+4];


uint32_t getDatFromMaster(uint8_t address ,uint8_t **bufout)
{

	uint16_t rc=0;
	uint8_t dat=0;
	static uint8_t st=State_Head;
	static uint8_t *pr=s_datbuf;
	static uint16_t len=0;

	*bufout=NULL;


	while(read_master_char(&dat)!=0)
	{

		rc++;
		if(rc>Max_Dat_Len)
		{
			goto err_exit;
		}

		*pr=dat;
		pr++;

		//msg_out("%d ",dat);
	
		switch(st)
		{
	
			case State_Head:
			{
				if(dat=='$')
				{
					st=State_Fun;
				}	
				else
				{
					pr=s_datbuf;
					rc=0;
				}
	
			}
			break;
	
			case State_Fun:
			{
				if((dat<'A')||(dat>'Z'))
				{
					goto err_exit;
				}	

				st=State_Adress;
			}
			break;
	
			case State_Adress:
			{
				if(dat!=address)
				{
					goto err_exit;	
				}
				st=State_Length;
				
			}
			break;
	
			case State_Length:
			{
				if(dat>Max_Dat_Len)
				{
					goto err_exit;
				}
				len=dat;
				if(len==0)
				{
					st=State_CheckSum;
					len=2;
				}
				else
				{
					st=State_Data;
				}
			}
			break;
	
			case State_Data:
			{
				len--;
				
				if(len>0)
				{
					
				}
				else
				{
					len=2;
					st=State_CheckSum;
				}
				
			}
			break;
	
			case State_CheckSum:
			{
				len--;
				if(len>0)
				{

				}
				else
				{
					len=2;
					st=State_Tail;
				}
			}
			break;
	
			case State_Tail:
			{
				len--;
				if(len>0)
				{

				}
				else
				{
					len=0;
					
					goto ok_exit;
				}
	
			}
			break;
	
			default:
				break;
	
		}

	}

noComplete_exit:
	return FUN_NO_COMPLETE;


	
ok_exit:
	rc=0;
	st=State_Head;
	pr=s_datbuf;
	*bufout=s_datbuf;
	return FUN_OK;

err_exit:
	rc=0;
	st=State_Head;
	pr=s_datbuf;
	return FUN_ERR;
	
}







uint32_t loop_ins_res(void)
{
	uint8_t *buf=NULL;
	uint8_t st=0;
	uint32_t localtime=0;
	static uint32_t pre_time=0;
	static uint8_t step=0;
	static uint32_t i=0;

	st=getDatFromMaster(Adress_Ins_Res,&buf);

	if((st==1)&&(buf!=NULL))
	{
		deal_master_cmd(buf);
		
	}
	

	switch(step)
	{
		case 0:
		{
			
			localtime=timer2_get_clock();
			if(pre_time!=localtime)
			{
				i++;
				if((i%500)==0)
				{
					step++;
					tog_pin_port(LED3);
					tog_pin_port(LED4);
					
				}
			}
			
			pre_time=localtime;

			
		}
		break;

		case 1:
		{

			localtime=timer2_get_clock();
			if(pre_time!=localtime)
			{
				i++;
				if((i%500)==0)
				{
					step++;
					tog_pin_port(LED1);
					os_printf("task %d run\r\n",timer2_get_clock());
					

				}
			}
			
			pre_time=localtime;

			
			
		}
		break;

		case 2:
		{

			localtime=timer2_get_clock();
			if(pre_time!=localtime)
			{
				i++;
				if((i%500)==0)
				{
					tog_pin_port(LED2);

					step=0;

				}
			}
			
			pre_time=localtime;


		


		}
		break;

		default:
			step=0;
		break;

	}


	


	return FUN_OK;
}









