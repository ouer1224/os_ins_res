


#include "ins_res.h"


#include "uart3.h"
#include "uart1.h"

#include "timer2.h"

#include "os_sprintf.h"


#define CMD_MASTER_INIT				'I'
#define CMD_MASTER_INQUIRE			'Q'
#define CMD_MASTER_READ				'R'
#define CMD_MASTER_WRITE			'W'

#define ADRESS_MASTER				'0'



Msg_res_master s_msg_res_master=
{
	'$','Q','0',0,
	null,
	0x00,0x0d0a
};



/***********************************************
*fun     :初始化相关引脚为输出,且暂时设置为0
*name    :
*var     :
*return  :
************************************************/
uint32_t init_ins_res_port(void)
{

	gpioConfig(Lock_Neg_Relay_1 ,1);
	SetPinOutput(Lock_Neg_Relay_1,0);	//上升沿锁存,初始设置为0
	
	
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
*fun     :按照给定的dat,一次将其bit为1的对应的引脚,clear为0.
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
*fun     :按照给定的dat,一次将其bit为1的对应的引脚,clear为1.
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
*fun     :根据主机新输入的电阻值求出的电阻串组合和本地的电阻串组合,进行继电器闭合
			电阻串组合中,1表示选中该电阻,0表示不选中该电阻值
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
*fun     :获取本地已经设置的绝缘阻值
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
*fun     :发送Msg_res_master中的报文信息
*name    :
*var     :
*return  :返回值为0xffff ffff时,表示错误,返回值为1时,发送成功
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
	for(i=0;i<len;i++)
	{
		send_master_char(pr[i]);
	}

	len=msg->msg_head.len;
	pr=msg->pr;
	for(i=0;i<len;i++)
	{
		send_master_char(pr[i]);
	}
	
	len=sizeof(msg->msg_tail);
	pr=(void *)&(msg->msg_tail);
	for(i=0;i<len;i++)
	{
		send_master_char(pr[i]);
	}


	return FUN_OK;
}

/***********************************************
*fun     :计算累加和
*name    :
*var     :
*return  :32位的累加和
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


	for(i=0;i<8;i++)
	{
		send_master_char(s_dat_init[i]);
	}

	for(i=0;i<len;i++)
	{
		send_master_char(buf[i]);
	}

	i=cal_checksum(buf,len);
	i+=s_dat_init[4];
	i+=s_dat_init[5];
	i+=s_dat_init[6];
	i+=s_dat_init[7];
	
	s_dat_init[0]=i;
	s_dat_init[1]=i>>8;
	s_dat_init[2]=0x0d;
	s_dat_init[3]=0x0a;
	
	for(i=0;i<4;i++)
	{
		send_master_char(s_dat_init[i]);
	}
	
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
	
	if(len!=8)
	{
		errcode=1;
	}
	else
	{

		pr_dat_vcc=(void *)(buf);
		pr_dat_gnd=(void *)(buf+4);
	
		localresP=getLocalResVcc();
		localresN=getLocalResGnd();
	
		if((*pr_dat_vcc==localresP)&&(*pr_dat_gnd==localresN))
		{
			errcode=0;
		}
		else
		{
	
			Pinput=getCombinationRes(*pr_dat_vcc);
			Ninput=getCombinationRes(*pr_dat_gnd);
			Plocal=getCombinationRes(localresP);
			Nlocal=getCombinationRes(localresN);
	
			st=selectInsRes(Pinput,Ninput,Plocal,Nlocal);
			if(st==FUN_OK)
			{
				errcode=0;
				setLocalResVcc(*pr_dat_vcc);
				setLocalResGnd(*pr_dat_gnd);
			}

			msg_out("Pinput=%x Plocal=%x\n",Pinput,Plocal);
			msg_out("Ninput=%x Nlocal=%x\n",Ninput,Nlocal);
	
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
	

	pr=(void *)(s_buf_write+0);
	*pr=0;
	pr=(void *)(s_buf_write+4);
	*pr=getLocalResVcc();
	pr=(void *)(s_buf_write+8);
	*pr=getLocalResGnd();


	msg->msg_head.len=12;
	msg->msg_head.adress_dest=ADRESS_MASTER;
	msg->msg_head.funcode=CMD_MASTER_READ;
	msg->msg_head.head='$';

	msg->msg_tail.tail=0x0a0d;

	msg->pr=s_buf_write;

	msg->msg_tail.check=cal_checksum(msg->pr,msg->msg_head.len)%0xffff;

	send_msg_res(msg);


	return FUN_OK;
}



uint32_t judge_checkSum(uint8_t *buf,uint8_t len,uint16_t checksum)
{
	uint32_t i=0;
	uint32_t check=0;
	for(i=0;i<len;i++)
	{
		check=check+buf[i];
	}
	check%=0xffff;
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









