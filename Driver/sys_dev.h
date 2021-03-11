//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&系统初始化模块&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*文件名称:sys_dev.h

//*文件作用:系统初始化

//*文件作者:bentyking

//*创建日期:2008年10月
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#ifndef SYS_DEV_H
#define SYS_DEV_H


#define start_stop_tim2		start_tim2
#define get_clock	timer2_get_clock
#define checktimer	timer2_check_timer

#define uart3_datread get_packet3
#define uart3_datsend Uart3_SendArray

#define dog		dog_feed		//喂狗
#define SerialPutString(x) Serial_PutString((uint8_t*)(x))

#define DOWNLOAD downloadYmodem

#define MAX_SEND_COUNT	3		//发送最大次数

#define ON		1
#define OFF		0 

#define send_buff	Uart3_SendArray
#define LVCOUNT 1

#define AD_count 6 
void RCC_Configuration(void);
void sysint(void);
void sys_proc(void);
u8 addre_init(void);
void SysClock_Init(void);
void RCC_Configuration(void);


#endif
