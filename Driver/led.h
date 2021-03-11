/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& led ����&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
�ļ�����:led.h

�ļ�����:led����

�ļ�����:GuiYang

��������:2013��4��
ʹ��˵��:ָʾ��Ϊ����˫ɫ�ƣ��������������Ϊ�̵�ָʾ��I/O���Ϊ�ߣ����ֹ�������ʱΪ��
ָʾ��I/OΪ�͵�ƽ
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
#ifndef LED_H
#define LED_H

#define LED_Green		  1
#define LED_Red	  		0
/************************ERRORָʾ��**************************************/
#define Port0_pin2_error		GPIO_Pin_2		//PA2

#define Port1_pin2_error		GPIO_Pin_1		//PA1

#define Port2_pin2_error		GPIO_Pin_0		//PA0

// #define Port3_pin4_error		GPIO_Pin_1		//PA1
// #define Port3_pin2_error		GPIO_Pin_0		//PC0
/********************��Դָʾ��*******************************************/

#define Vin24_LED	  				GPIO_Pin_2		//PC2
#define Vout24_LED	  			GPIO_Pin_3		//PC3
/********************����ָʾ�� *****************************************/
#define Link								GPIO_Pin_1		//PB1
#define MOD_LED		  				GPIO_Pin_3		//PA3

#define V24_PORT	  	GPIOC

#define MODE	  	GPIOA

#define Vout24_LED_Green	  	GPIO_SetBits(V24_PORT,Vout24_LED)		   			//PC3
#define Vout24_LED_Red	  		GPIO_ResetBits(V24_PORT,Vout24_LED)			   	

#define Vin24_LED_Green	  		GPIO_SetBits(V24_PORT,Vin24_LED)		   			//PC2
#define Vin24_LED_Red	  			GPIO_ResetBits(V24_PORT,Vin24_LED)

#define MOD_LED_Green		  		GPIO_SetBits(MODE,MOD_LED)								//PA3
#define MOD_LED_Red		  			GPIO_ResetBits(MODE,MOD_LED)

#define Link_Green		  			GPIO_SetBits(GPIOB,Link)								//PB1
#define Link_Red		  				GPIO_ResetBits(GPIOB,Link)
/************************ERRORָʾ��**************************************/
#define Port0_pin2_error_Green	  			GPIO_SetBits(GPIOA,Port0_pin2_error)		   			//PA2
#define Port0_pin2_error_Red  					GPIO_ResetBits(GPIOA,Port0_pin2_error)		


#define Port1_pin2_error_Green 					GPIO_SetBits(GPIOA,Port1_pin2_error)		   			//PA1
#define Port1_pin2_error_Red		  			GPIO_ResetBits(GPIOA,Port1_pin2_error)	


#define Port2_pin2_error_Green	  			GPIO_SetBits(GPIOA,Port2_pin2_error)		   			//PA0
#define Port2_pin2_error_Red  					GPIO_ResetBits(GPIOA,Port2_pin2_error)	


void LED_init(void);
void led_blink(u8 led_no, u16 on_time, u16 off_time);
void error_led_out(u8 dat);
#endif
