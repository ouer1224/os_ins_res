


#include "ins_res.h"


/***********************************************
*fun     :初始化相关引脚为输出,且暂时设置为0
*name    :
*var     :
*return  :
************************************************/
uint8_t init_ins_res_port(void)
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

	
	return 1;
}




/***********************************************
*fun     :按照给定的dat,一次将其bit为1的对应的引脚,clear为0.
*name    :
*var     :
*return  :
************************************************/
uint8_t openRelayGroup(uint8_t dat,uint32_t group)
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


	return 1;
}


/***********************************************
*fun     :按照给定的dat,一次将其bit为1的对应的引脚,clear为1.
*name    :
*var     :
*return  :
************************************************/
uint8_t closeRelayGroup(uint8_t dat,uint32_t group)
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


	return 1;
}



uint8_t OpenSelectPosRelay(uint32_t opendat)
{
	
	openRelayGroup((opendat>>0)&0xff,(uint32_t)Lock_Pos_Relay_1);
	openRelayGroup((opendat>>8)&0xff,(uint32_t)Lock_Pos_Relay_2);
	openRelayGroup((opendat>>16)&0xff,(uint32_t)Lock_Pos_Relay_3);

	return 1;
}

uint8_t OpenSelectNegRelay(uint32_t opendat)
{
	
	openRelayGroup((opendat>>0)&0xff,(uint32_t)Lock_Neg_Relay_1);
	openRelayGroup((opendat>>8)&0xff,(uint32_t)Lock_Neg_Relay_2);
	openRelayGroup((opendat>>16)&0xff,(uint32_t)Lock_Neg_Relay_3);

	return 1;
}



uint8_t CloseSelectPosRelay(uint32_t closedat)
{
	
	openRelayGroup((closedat>>0)&0xff,(uint32_t)Lock_Pos_Relay_1);
	openRelayGroup((closedat>>8)&0xff,(uint32_t)Lock_Pos_Relay_2);
	openRelayGroup((closedat>>16)&0xff,(uint32_t)Lock_Pos_Relay_3);

	return 1;
}

uint8_t CloseSelectNegRelay(uint32_t closedat)
{
	
	openRelayGroup((closedat>>0)&0xff,(uint32_t)Lock_Neg_Relay_1);
	openRelayGroup((closedat>>8)&0xff,(uint32_t)Lock_Neg_Relay_2);
	openRelayGroup((closedat>>16)&0xff,(uint32_t)Lock_Neg_Relay_3);

	return 1;
}

/***********************************************
*fun     :根据主机新输入的电阻值求出的电阻串组合和本地的电阻串组合,进行继电器闭合
			电阻串组合中,1表示选中该电阻,0表示不选中该电阻值
*name    :
*var     :
*return  :
************************************************/
uint8_t selectInsRes(uint32_t Pinput,uint32_t Ninput,uint32_t Plocal,uint32_t Nlocal)
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
	OpenSelectPosRelay(opendat);
	CloseSelectPosRelay(closedat);

	opendat=~Ninput;
	closedat=Ninput;
	OpenSelectPosRelay(opendat);
	CloseSelectPosRelay(closedat);


#endif
	return 1;
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


























#define CMD_MASTER_INIT				'I'
#define CMD_MASTER_INQUIRE			'Q'
#define CMD_MASTER_READ				'R'
#define CMD_MASTER_WRITE			'W'


uint8_t deal_master_cmd(void)
{

	uint8_t cmd=0;
	uint32_t res_cmd=0;

	switch (cmd)
	{
		case CMD_MASTER_INIT:
		{

		}
		break;

		case CMD_MASTER_INQUIRE:
		{

		}
		break;

		case CMD_MASTER_READ:
		{

		}
		break;

		case CMD_MASTER_WRITE:
		{

		}
		break;


	
	}

	
	return 1;
}











