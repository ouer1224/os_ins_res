//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&��������&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*�ļ�����:save.c

//*�ļ�����:��������

//*�ļ�����:

//*��������:2004��5��
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#include "include.h"



//���������ַ
#define PARAM_ADDR	FLASH_PARAM_BASE
#define MODE_SPIDER	  0x15


//�����������ݽṹ
saveparam_t saveparam;

//Ĭ�ϲ���
static saveparam_t const defaultparam=
{
	MODE_SPIDER,//���к�		
	//0x01  0x02  0x03   0x04  0x05  0x06    0x07  0x08     0x09		(M8)	0x0A  		0x0B  		0x0C 			0x0D 					0X0E	
	//8DI   8DO   8AU    6DI   6DO   6DI/DO  4AI	  4AO    4DI4DO		(M8)	8DI(M8)   8DO(M8)   8AU(M8)   4DI4DO(M8)  	4-20ma/0-10V
	
	//0x10 		0x11  0x12   0x13  0x14  0x15   0x16  0x17      0x18        0x19
	//3AI     3VI   4AI   4VI    3AO   3VO    4AO   4VO    3�������   4�������
	0xff,//·�ɵ�ַ
	//У���
	0,
};

//������
#define PTU_BUF_SIZE		1024
static u8 ptu_recv_buf[PTU_BUF_SIZE];

// static u16 ptu_recv_length=0;
// static char ptu_send_buf[128];
u8  PAddress;
//***********************************************************************************************************************
//��������:�������
//����˵��:
//ע������:
//����˵��:����ɹ�����1
//***********************************************************************************************************************
void save_param(void)
{
	u32 i;
   	u8 sum=0;
	u8 *p=(u8 *)&saveparam;

	//����У���
	for(i=0;i<sizeof(saveparam_t)-1;i++)sum+=p[i];
	p[i]=0x55-sum;
		
	//����flash
	flash_erase(PARAM_ADDR,sizeof(saveparam_t));
	//д��flash
	flash_write((u8 *)&saveparam,PARAM_ADDR,sizeof(saveparam_t));
}

//***********************************************************************************************************************
//��������:װ��ȱʡ
//����˵��:
//ע������:
//����˵��:�ɹ�����1
//***********************************************************************************************************************
void save_load_default(void)
{
	u32 i;
	u8 *p=(u8 *)&saveparam;
	u8 const *p_default=(u8 const *)&defaultparam;
	
	for(i=0;i<sizeof(saveparam_t);i++)p[i]=p_default[i];


}

//***********************************************************************************************************************
//��������:������ʼ��
//����˵��:
//ע������:
//����˵��:�ɹ�����1
//***********************************************************************************************************************
void save_init(void)
{
	u32 i;
	u8 sum=0;
	u8 *p=(u8 *)&saveparam;

	
	//��flash��װ������
	memcpy((u8 *)&saveparam,(u8 *)PARAM_ADDR,sizeof(saveparam_t));
	//�˶�У���
	for(i=0;i<sizeof(saveparam_t);i++)sum+=p[i];
	//У�����ȷ
	if(0x55==sum)
	{
		//print_line("param ok");
		
	}
	else
	{
		//У��ʹ��� װ��ȱʡ
		save_load_default();
		//print_line("load default");
// 		save_param();
	}
}

//���س���
static u8 wiat_char(u8 ch)
{
	u8 temp;
	u32 timeout=0;
	
	while(!DEBUG_RECEIVE_CHAR(&temp))
	{
		dog();
		if(++timeout>=3000000)
		{
			return 0;	
		}
	}
	if(temp!=ch)return 0;
	return 1;
}
static u8 receive(u8 *ch)
{
	u32 timeout=0;
	
	while(!DEBUG_RECEIVE_CHAR(ch))
	{
		dog();
		if(++timeout>=3000000)
		{
			return 0;	
		}
	}
	return 1;
}
	#define CLI() __set_PRIMASK(1) 
	#define SEI() __set_PRIMASK(0)
void downloadYmodem(void)
{
   
//	CLI();
//    SerialPutString("\r\n======================================================================");
//    SerialPutString("\r\n=              (C) COPYRIGHT 2010 STMicroelectronics                 =");
//    SerialPutString("\r\n=                                                                    =");
//    SerialPutString("\r\n=     In-Application Programming Application  (Version 3.3.0)        =");
//    SerialPutString("\r\n=                                                                    =");
//    SerialPutString("\r\n=                                   By MCD Application Team          =");
//    SerialPutString("\r\n======================================================================");
//    SerialPutString("\r\n\r\n");
//    Main_Menu ();

	PAddress = 0;				 
	SavaParamter();
	dog_init(5); 
	while(1);
	}
void download(void)
{
	u32 total_length;
	u32 recv_num=0;
	u8 temp;
	u16 i;
	u16 checksum_add0;//����PC���ۼ�У���
	u16 checksum_xor0;//����PC�����У���
	u16 checksum_add1=0;//�Լ�������ۼ�У���
	u16 checksum_xor1=0;//�Լ���������У���

	DEBUG_RECEIVE_CHAR(&temp);//ɨ������
	DEBUG_RECEIVE_CHAR(&temp);//ɨ������
	DEBUG_RECEIVE_CHAR(&temp);//ɨ������


	//1.������ʼ��
	if(!wiat_char('A'))return;//�ȴ���ʼ��1
	if(!wiat_char('R'))return;//�ȴ���ʼ��1
	if(!wiat_char('M'))return;//�ȴ���ʼ��1
	if(!wiat_char(0x07))return;//�ȴ�CPU�ͺŴ���
	if(!wiat_char(0x10))return;//�ȴ�CPU�ͺŴ���
	
	//2.����ͬ����
	if(!wiat_char(0x55))return;
	if(!wiat_char(0xAA))return;
	
	//3.���������ܳ���
	if(!receive(&temp))return;
	total_length=(u32)temp;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<8;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<16;
	if(!receive(&temp))return;
	total_length|=(u32)temp<<24;
	if(total_length>FLASH_BACKUP_LENGTH)
	{
		return;
	}
	//��������flash����
	if(!flash_erase(FLASH_BACKUP_BASE,total_length))
	{

		return;
	}
	
	//4.ѭ���������ݰ� һ�߷���һ�߼���У���
	while(recv_num<total_length)
	 {
		u16 count;
		u8 encrypt;
                		
		//�ж��Ƿ�ʣ�²���һ��
		if(total_length-recv_num>=PTU_BUF_SIZE)count=PTU_BUF_SIZE;
		else   count=total_length-recv_num;
                //�������ݵ�����
		for(i=0;i<count;i++)
		{
			if(!receive(&temp))return;
			ptu_recv_buf[i]=temp;
		}
//		//���ݽ���
		encrypt=ptu_recv_buf[0]^0x55;
		for(i=1;i<count;i++)ptu_recv_buf[i]^=encrypt;
		//�ѽ��ջ��������д��flash
		if(!flash_write(ptu_recv_buf,FLASH_BACKUP_BASE+recv_num,count))
		{
		
			return;
		}
		//����У���
		for(i=0;i<count;i++)checksum_add1+=*((u8 *)(FLASH_BACKUP_BASE+recv_num+i));//�����ۼ�У���
		for(i=0;i<count;i+=2)checksum_xor1^=*((u16 *)(FLASH_BACKUP_BASE+recv_num+i));//�������У���
		//ָ����һ������
		recv_num+=count;
	}
	
	//5.��������PC��У���
	if(!receive(&temp))return;
	checksum_add0=(u32)temp;
	if(!receive(&temp))return;
	checksum_add0|=(u32)temp<<8;
	if(!receive(&temp))return;
	checksum_xor0=(u32)temp;
	if(!receive(&temp))return;
	checksum_xor0|=(u32)temp<<8;
			
	//6.�ж�У���
	if((checksum_add0!=checksum_add1) || (checksum_xor0!=checksum_xor1))
	{
		
		return;
	}

	delay_ms(100);//�ȴ��������
	
	//7.������������
	//flash_erase(FLASH_PARAM_BASE,sizeof(saveparam_t));
	
	//8.���г������---�ӱ���flash����������flash ����������ÿ��Ź���λ
	flash_copy(total_length);
}


void  SavaParamter(void)
{	
	 
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(FLASHAddress);
	FLASH_ProgramWord(DP_FLASH_ADDRESS, PAddress);
//	FLASH_ProgramWord(DP_FLASH_NADDRESS, NPAddress);
   	FLASH_Lock();	
}
