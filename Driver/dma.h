#ifndef __DMA_H
#define	__DMA_H
#include"system.h"


#define SENDBUFF_SIZE 5000

/* RAM µÿ÷∑*/

#define USART1_DR_Base  0x40013804

#define RAM_BASE       0x20000000
#define RAM_BB_BASE    0x22000000

void DMA_Config_Channel4(u8 size);
void DMA_Chan4_Init(void);
void DMA_Config_Channel5(void);
void NVIC_Config(void);

#endif
