/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& hc574 驱动&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
文件名称:hc574.h

文件作用:hc574驱动

文件作者:Bai Bing

创建日期:2017年8月
使用说明:
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

#ifndef hc574_H
#define hc574_H

#define relay_time   1000            //继电器控制稳定时间

/************************574控制与数据管脚**************************************/
#define Out_CLK_1   GPIO_Pin_6      //PC6
#define Out_CLK_2   GPIO_Pin_15     //PB15
#define Out_CLK_3   GPIO_Pin_14     //PB14
#define Out_CLK_4   GPIO_Pin_13     //PB13
#define Out_CLK_5   GPIO_Pin_12     //PB12

#define DB0   GPIO_Pin_4      //PA4
#define DB1   GPIO_Pin_5      //PA5
#define DB2   GPIO_Pin_6      //PA6
#define DB3   GPIO_Pin_7      //PA7
#define DB4   GPIO_Pin_4      //PC4
#define DB5   GPIO_Pin_5      //PC5
#define DB6   GPIO_Pin_0      //PB0
#define DB7   GPIO_Pin_1      //PB1

/************************574运行**************************************/
#define Out_CLK_1_LOW             GPIO_ResetBits(GPIOC,Out_CLK_1)
#define Out_CLK_2_LOW             GPIO_ResetBits(GPIOB,Out_CLK_2)
#define Out_CLK_3_LOW             GPIO_ResetBits(GPIOB,Out_CLK_3)
#define Out_CLK_4_LOW             GPIO_ResetBits(GPIOB,Out_CLK_4)
#define Out_CLK_5_LOW             GPIO_ResetBits(GPIOB,Out_CLK_5)

#define Out_CLK_1_Hi              GPIO_SetBits(GPIOC,Out_CLK_1)
#define Out_CLK_2_Hi              GPIO_SetBits(GPIOB,Out_CLK_2)
#define Out_CLK_3_Hi              GPIO_SetBits(GPIOB,Out_CLK_3)
#define Out_CLK_4_Hi              GPIO_SetBits(GPIOB,Out_CLK_4)
#define Out_CLK_5_Hi              GPIO_SetBits(GPIOB,Out_CLK_5)

#define DB0_Hi                    GPIO_SetBits(GPIOA,DB0)
#define DB1_Hi                    GPIO_SetBits(GPIOA,DB1)
#define DB2_Hi                    GPIO_SetBits(GPIOA,DB2)
#define DB3_Hi                    GPIO_SetBits(GPIOA,DB3)
#define DB4_Hi                    GPIO_SetBits(GPIOC,DB4)
#define DB5_Hi                    GPIO_SetBits(GPIOC,DB5)
#define DB6_Hi                    GPIO_SetBits(GPIOB,DB6)
#define DB7_Hi                    GPIO_SetBits(GPIOB,DB7)
#define DB0_Low                   GPIO_ResetBits(GPIOA,DB0)
#define DB1_Low                   GPIO_ResetBits(GPIOA,DB1)
#define DB2_Low                   GPIO_ResetBits(GPIOA,DB2)
#define DB3_Low                   GPIO_ResetBits(GPIOA,DB3)
#define DB4_Low                   GPIO_ResetBits(GPIOC,DB4)
#define DB5_Low                   GPIO_ResetBits(GPIOC,DB5)
#define DB6_Low                   GPIO_ResetBits(GPIOB,DB6)
#define DB7_Low                   GPIO_ResetBits(GPIOB,DB7)

/************************函数声明**************************************/

void hc574_init(void);
void Parallel_Port_Out(void);
void Write_Parallel_Port(u8 Out_data);
void Write_Relay_data(u8 OUT_REALY_DATA_1,u8 OUT_REALY_DATA_2,u8 OUT_REALY_DATA_3,u8 OUT_REALY_DATA_4,u8 OUT_REALY_DATA_5);

#endif
