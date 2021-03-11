/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:INPORT_init.h

�ļ�����:����I/O��ʼ��

�ļ�����:GuiYang

��������:2013��4��

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#ifndef INPORT_init_H
#define INPORT_init_H
/*****************PORT0 �˿�**********************************************/
#define Port0_short					GPIO_Pin_2		//PC2
/*****************PORT1 �˿�**********************************************/
#define Port1_short					GPIO_Pin_0		//PA0
/*****************PORT2 �˿�**********************************************/
#define Port2_short					GPIO_Pin_1		//PA1
/*****************PORT3 �˿�**********************************************/
#define Port3_short					GPIO_Pin_8		//PA8
/*****************���˿�**********************************************/
#define read_port0_short 				(GPIO_ReadInputDataBit(GPIOC, Port0_short))//PC2
#define read_port1_short  			(GPIO_ReadInputDataBit(GPIOA, Port1_short))//PA0
#define read_port2_short				(GPIO_ReadInputDataBit(GPIOA, Port2_short))//PA1
#define read_port3_short  			(GPIO_ReadInputDataBit(GPIOA, Port3_short))//PA8


///////////////////////����ѡ���//////////////////////////////////////////////////////////////
/*****************Choose1 �˿�**********************************************/
#define choose1_port				GPIO_Pin_6		//PA6
/*****************Choose2 �˿�**********************************************/
#define choose2_port				GPIO_Pin_7		//PA7
/*****************Choose3 �˿�**********************************************/
#define choose3_port				GPIO_Pin_5		//PB5

#define read_choose1  								(GPIO_ReadInputDataBit(GPIOA, choose1_port))//PA6
#define read_choose2									(GPIO_ReadInputDataBit(GPIOA, choose2_port))//PA7
#define read_choose3  								(GPIO_ReadInputDataBit(GPIOB, choose3_port))//PB5

/*****************д�˿�*************************************************/

/*****************��ַ�˿�*************************************************/
#define Mac_set_in							GPIO_Pin_0		//PB0
#define Mac_set_out							GPIO_Pin_5		//PC5

#define read_mac_set_in 				(GPIO_ReadInputDataBit(GPIOB, Mac_set_in))//PB0

#define Mac_set_out_H	  				GPIO_SetBits(GPIOC,Mac_set_out)		   			//PC5
#define Mac_set_out_L	  				GPIO_ResetBits(GPIOC,Mac_set_out)			   	//PC5

void IN_PORT_init(void);
void Out_port(u8 dat);
u8 Read_port(void);
u8 Read_short(void);

#endif
