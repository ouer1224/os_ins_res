 //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& �������� &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*�ļ�����:save.c

//*�ļ�����:��������

//*�ļ�����:��  ��

//*��������:2008��10��
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#ifndef SAVE_H
#define SAVE_H


 #define DEBUG_RECEIVE_CHAR 	   read_usart3_char
//�����������ݽṹ����
typedef struct
{
	u8 sn;//���к�
	u8 route_addr;//·�ɵ�ַ
			
	//У���			
	u16 crc;
}saveparam_t;
extern saveparam_t saveparam;



extern void save_init(void);
extern void save_load_default(void);
extern void save_param(void);
void download(void);
static u8 receive(u8 *ch);
static u8 wiat_char(u8 ch);
void  SavaParamter(void);
void downloadYmodem(void);
#endif


