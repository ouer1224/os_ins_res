#ifndef hc165_H
#define hc165_H

#define DATA_165				 GPIO_Pin_12		//PC12	  ��������
#define LD_165		  			 GPIO_Pin_11		//PC11	  ʹ�ܿ���
#define CLK_165		 		 	 GPIO_Pin_10		//PC10	  ʱ�ӿ���

#define HC165_PORT				 GPIOC

#define read_DATA_165		(GPIO_ReadInputDataBit(HC165_PORT, DATA_165))

#define LD_165_HIGHT		 	GPIO_SetBits(HC165_PORT,LD_165)				
#define LD_165_LOW		  		GPIO_ResetBits(HC165_PORT,LD_165)			

#define CLK_165_HIGHT		 	GPIO_SetBits(HC165_PORT,CLK_165)				
#define CLK_165_LOW		  		GPIO_ResetBits(HC165_PORT,CLK_165)

void HC165_init(void);
u16 read_hc165(void);
extern void delay_ms(u32 nTime);
#endif
