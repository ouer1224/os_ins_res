

#ifndef __INS_RES_H__
#define __INS_RES_H__


#include "stm32f10x.h"


#define FUN_OK				1
#define FUN_NO_COMPLETE		0
#define FUN_ERR		0xffffffff



#ifndef NULL
#define NULL	((void *)0)
#endif


#ifndef null
#define null	((void *)0)
#endif



#define comUartPort		3


#if comUartPort==3
#define read_master_char	read_usart3_char
#define send_master_char	USART3_SendByte
#define send_master_dat		Uart3_SendArray
#elif comUartPort==1
#define read_master_char	read_usart1_char
#define send_master_char	USART1_SendByte
#define send_master_dat 	Uart1_SendArray
#endif




#define OPEN_RELAY		1
#define CLOSE_RELAY		0


#define __gpioConfigInput(port,pin,reg)	(port)->reg=((port)->reg&(~(0x0f<<((pin)*4))))|(((0x01<<2)|(00))<<((pin)*4))
#define __gpioConfigOutput(port,pin,fun,reg)	(port)->reg=((port)->reg&(~(0x0f<<((pin)*4))))|(((fun<<2)|(0x03))<<((pin)*4))

#define __gpioConfig(port,pin,direction,reg) (direction==0?(__gpioConfigInput(port,pin,reg)):(__gpioConfigOutput(port,pin,00,reg)))

#define __compareAB(a,b)		((a)>=(b)?1:0)



#if 0
#define __operaGpioOutput(port,pin,reg,val)		(port)->reg=((port)->reg&(~(0x01<<(pin)))|(val<<(pin)))
#define SetgpioOutput(Pin,val)				__operaGpioOutput(Pin&0xffffff00,Pin&0xff,ODR,val)		
#else

#define __setGpioOutput(port,pin,reg)	(port)->reg=(0x01)<<(pin)

#define SetPinOutput(Pin,val)				(val==0?\
		(__setGpioOutput((GPIO_TypeDef*)(((uint32_t)Pin)&0xffffff00),(uint32_t)Pin&0xff,BRR)):\
		(__setGpioOutput((GPIO_TypeDef*)(((uint32_t)Pin)&0xffffff00),(uint32_t)Pin&0xff,BSRR)))


#define __setPortOutput(port,reg,val)	(port)->reg=(val)

#define SetPortOutPut(Port,val)			__setPortOutput((GPIO_TypeDef*)(((uint32_t)Port)&0xffffff00),BSRR,val)
#define ClearPortOutput(Port,val)		__setPortOutput((GPIO_TypeDef*)(((uint32_t)Port)&0xffffff00),BRR,val)

#endif
#define __getGpioInput(port,pin,reg)		((((port)->reg)>>(pin))&0x01)

#define GetgpioInput(Pin)					__getGpioInput((GPIO_TypeDef*)(((uint32_t)Pin)&0xffffff00),(uint32_t)Pin&0xff,IDR)		
#define GetPortInput(Port)					((GPIO_TypeDef*)(Port)->IDR)

#define gpioConfig(Pin,direction)		__compareAB((uint32_t)(Pin)&0xff,8)?\
		__gpioConfig(\
		(GPIO_TypeDef*)((uint32_t)(Pin)&0xffffff00),\
		(uint32_t)(Pin)&0xff-8,\
		direction,CRH):\
		__gpioConfig(\
		(GPIO_TypeDef*)((uint32_t)(Pin)&0xffffff00),\
		(uint32_t)(Pin)&0xff,\
		direction,CRL)



#define PORTA_P0			(uint8_t *)GPIOA+0
#define PORTA_P1			(uint8_t *)GPIOA+1
#define PORTA_P2			(uint8_t *)GPIOA+2
#define PORTA_P3			(uint8_t *)GPIOA+3
#define PORTA_P4			(uint8_t *)GPIOA+4
#define PORTA_P5			(uint8_t *)GPIOA+5
#define PORTA_P6			(uint8_t *)GPIOA+6
#define PORTA_P7			(uint8_t *)GPIOA+7
#define PORTA_P8			(uint8_t *)GPIOA+8
#define PORTA_P9			(uint8_t *)GPIOA+9
#define PORTA_P10			(uint8_t *)GPIOA+10
#define PORTA_P11			(uint8_t *)GPIOA+11
#define PORTA_P12			(uint8_t *)GPIOA+12
#define PORTA_P13			(uint8_t *)GPIOA+13
#define PORTA_P14			(uint8_t *)GPIOA+14
#define PORTA_P15			(uint8_t *)GPIOA+15

#define PORTB_P0			(uint8_t *)GPIOB+0
#define PORTB_P1			(uint8_t *)GPIOB+1
#define PORTB_P2			(uint8_t *)GPIOB+2
#define PORTB_P3			(uint8_t *)GPIOB+3
#define PORTB_P4			(uint8_t *)GPIOB+4
#define PORTB_P5			(uint8_t *)GPIOB+5
#define PORTB_P6			(uint8_t *)GPIOB+6
#define PORTB_P7			(uint8_t *)GPIOB+7
#define PORTB_P8			(uint8_t *)GPIOB+8
#define PORTB_P9			(uint8_t *)GPIOB+9
#define PORTB_P10			(uint8_t *)GPIOB+10
#define PORTB_P11			(uint8_t *)GPIOB+11
#define PORTB_P12			(uint8_t *)GPIOB+12
#define PORTB_P13			(uint8_t *)GPIOB+13
#define PORTB_P14			(uint8_t *)GPIOB+14
#define PORTB_P15			(uint8_t *)GPIOB+15


#define PORTC_P0			(uint8_t *)GPIOC+0
#define PORTC_P1			(uint8_t *)GPIOC+1
#define PORTC_P2			(uint8_t *)GPIOC+2
#define PORTC_P3			(uint8_t *)GPIOC+3
#define PORTC_P4			(uint8_t *)GPIOC+4
#define PORTC_P5			(uint8_t *)GPIOC+5
#define PORTC_P6			(uint8_t *)GPIOC+6
#define PORTC_P7			(uint8_t *)GPIOC+7
#define PORTC_P8			(uint8_t *)GPIOC+8
#define PORTC_P9			(uint8_t *)GPIOC+9
#define PORTC_P10			(uint8_t *)GPIOC+10
#define PORTC_P11			(uint8_t *)GPIOC+11
#define PORTC_P12			(uint8_t *)GPIOC+12
#define PORTC_P13			(uint8_t *)GPIOC+13
#define PORTC_P14			(uint8_t *)GPIOC+14
#define PORTC_P15			(uint8_t *)GPIOC+15






#define Lock_Neg_Relay_1	PORTA_P3
#define Lock_Neg_Relay_2	PORTA_P2
#define Lock_Neg_Relay_3	PORTA_P1


#define Lock_Pos_Relay_1	PORTB_P14
#define Lock_Pos_Relay_2	PORTB_P13
#define Lock_Pos_Relay_3	PORTB_P12


#define Pin_DB0				PORTB_P1
#define Pin_DB1				PORTB_P0
#define Pin_DB2				PORTC_P5
#define Pin_DB3				PORTC_P4
#define Pin_DB4				PORTA_P7
#define Pin_DB5				PORTA_P6
#define Pin_DB6				PORTA_P5
#define Pin_DB7				PORTA_P4


#define LED1				PORTC_P9
#define LED2				PORTA_P8
#define LED3				PORTA_P11
#define LED4				PORTA_P12



typedef struct
{
	uint8_t head;
	uint8_t funcode;
	uint8_t adress_dest;
	uint8_t len;

}Msg_res_head;

typedef struct
{
	uint16_t check;
	uint16_t tail;

}Msg_res_tail;


typedef struct
{
	Msg_res_head msg_head;

	void *pr;
	
	Msg_res_tail msg_tail;

}Msg_res_master;








uint32_t loop_ins_res(void);
uint32_t init_ins_res_port(void);
uint32_t init_led_port(void);
uint32_t tog_pin_port(void *pin);
uint32_t deal_master_cmd(uint8_t *buf);





#endif









