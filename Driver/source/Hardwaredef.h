#ifndef HARDWAREDEF_H
#define  HARDWAREDEF_H

#include"include.h"

#include "dp_cfg.h"            // default configuration

#include "dp_if.h"              // vpc3 structure, makros

#include "Dpl_list.h"
#include "string.h"
//#define  PAM90



typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define __no_operation()				  __NOP()

#define  A0PIN      GPIO_PinSource2
#define  A0PORT    GPIOD

#define  A1PIN       GPIO_PinSource8
#define  A2PIN       GPIO_PinSource9
#define  A3PIN       GPIO_PinSource10
#define  A4PIN       GPIO_PinSource11
#define  A5PIN       GPIO_PinSource12
#define  LOWBYTEPORT   GPIOA


#define PSANDMONOFLIPMASK   0x28  
 // power flag is PA3,Monoflipflop is PA5

#define MONORISEEDGEMASK     0x20
#define PSFALLEDGEMASK   0x8

#define CCR_ENABLE_Set          ((u32)0x00000001)
#define CCR_ENABLE_Reset        ((u32)0xFFFFFFFE)

#define  PIN_0_BitNumber  0x0
#define  PIN_1_BitNumber  0x1
#define  PIN_2_BitNumber  0x2
#define  PIN_3_BitNumber  0x3
#define  PIN_4_BitNumber  0x4
#define  PIN_5_BitNumber  0x5
#define  PIN_6_BitNumber  0x6
#define  PIN_7_BitNumber  0x7
#define  PIN_8_BitNumber  0x8
#define  PIN_9_BitNumber  0x9
#define  PIN_10_BitNumber  0xa
#define  PIN_11_BitNumber  0xb
#define  PIN_12_BitNumber  0xc
#define  PIN_13_BitNumber  0xd
#define  PIN_14_BitNumber  0xe
#define  PIN_15_BitNumber  0xf

#define WR_PINPB   GPIO_Pin_5
#define RD_PINPB   GPIO_Pin_6
#define ALE_PINPB  GPIO_Pin_9
#define NINT_PINPB GPIO_Pin_7
#define VPRESET_PINPB GPIO_Pin_8


#define HC165CLK_PINPB  GPIO_Pin_7
#define HC165CLK_PINNUmber PIN_7_BitNumber

#define HC165LD_PINPB  GPIO_Pin_8
#define HC165LD_PINNumber  PIN_8_BitNumber

#define HC165DATA_PINPB  GPIO_Pin_6
#define HC165DATA_PINNumber PIN_6_BitNumber



#define MODULE_LED_PINPC GPIO_Pin_4
#define MODULELED_PINNumber PIN_4_BitNumber

#define VIN_PINPC GPIO_Pin_12
#define VIN_PINNumber  PIN_12_BitNumber

#define VOUT_PINPC GPIO_Pin_11
#define VOUT_PinNumber  PIN_11_BitNumber


#define HC574OE_PINPC    GPIO_Pin_5
#define HC574CLK_PINPC   GPIO_Pin_4


#define TIM1_OFFSET    (TIM1_BASE - PERIPH_BASE)
#define TIM1_CR1_OFFSET        (TIM1_OFFSET + 0x00)
#define CEN_BitNumber     0x00
#define TIM1_CR1_CEN_BB   (PERIPH_BB_BASE + (TIM1_CR1_OFFSET * 32)\
                                             + (CEN_BitNumber * 4))

#define TIM3_OFFSET    (TIM3_BASE-PERIPH_BASE)
#define TIM3_CR1_OFFSET     (TIM3_OFFSET+0x00)

#define TIM3_CR1_CEN_BB    (PERIPH_BB_BASE+(TIM3_CR1_OFFSET*32)\
	                                         + (CEN_BitNumber*4))

#define  TIM2_OFFSET   (TIM2_BASE-PERIPH_BASE)
#define  TIM2_CR1_OFFSET     (TIM2_OFFSET+0x00)
#define TIM2_CR1_CEN_BB    (PERIPH_BB_BASE+(TIM2_CR1_OFFSET*32)\
	                                         + (CEN_BitNumber*4))

#define  CODEHIGHMASK     0x3fc0
#define  CODELOWMASK      0x1f00

#define  GPIOA_OFFSET   (GPIOA_BASE-PERIPH_BASE)
#define  GPIOB_OFFSET    (GPIOB_BASE-PERIPH_BASE)
#define  GPIOC_OFFSET    (GPIOC_BASE-PERIPH_BASE)
#define  GPIOD_OFFSET    (GPIOD_BASE-PERIPH_BASE)

#define  GPIOA_IDR_OFFSET     (GPIOA_OFFSET+0x08)
#define  GPIOA_ODR_OFFSET     (GPIOA_OFFSET+0xc)
#define  GPIOB_IDR_OFFSET   (GPIOB_OFFSET+0x08)
#define  GPIOB_ODR_OFFSET   (GPIOB_OFFSET+0xc)
#define  GPIOC_IDR_OFFSET   (GPIOC_OFFSET+0x08)
#define  GPIOC_ODR_OFFSET   (GPIOC_OFFSET+0xc)
#define  GPIOD_IDR_OFFSET   (GPIOD_OFFSET+0x08)
#define  GPIOD_ODR_OFFSET   (GPIOD_OFFSET+0xc)


#define  HC165CLK   (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
	                                    +(HC165CLK_PINNUmber*4)) 

#define  HC165LD   (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
	                                    +(HC165LD_PINNumber*4)) 

#define  HC165DATA (*(vu32*)(PERIPH_BB_BASE+(GPIOB_IDR_OFFSET*32)\
	                                    +(HC165DATA_PINNumber*4)))

#define  VPRESET   (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
	                                    +(PIN_8_BitNumber*4))  



#define  MODLED    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(MODULELED_PINNumber*4)) 
	                                    
#define  VINLED    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(VIN_PINNumber*4)) 


#define  VOUTLED    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(VOUT_PinNumber*4)) 

	
#define  ALEPIN      (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
	                                    +(PIN_9_BitNumber*4))

#define  WRPIN    (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
	                                    +(PIN_5_BitNumber*4))
#define  RDPIN    (PERIPH_BB_BASE+(GPIOB_ODR_OFFSET*32)\
                                      +(PIN_6_BitNumber*4))


#define  NINTPIN  (PERIPH_BB_BASE+(GPIOB_IDR_OFFSET*32)\
	                                    +(PIN_7_BitNumber*4))

#define  FPGARD    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(PIN_8_BitNumber*4))
#define  FPGAWR    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
                                      +(PIN_7_BitNumber*4))

#define  UPDOWN    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(PIN_4_BitNumber*4))
	                                    
#define  CFGCS     (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
	                                    +(PIN_4_BitNumber*4))
	                                    
#define  CSB    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
                                      +(PIN_5_BitNumber*4))
#define  CSA    (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
                                       +(PIN_6_BitNumber*4))

#define  HC574NOE  (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
                                      +(PIN_5_BitNumber*4))
#define  HC574CLK  (PERIPH_BB_BASE+(GPIOC_ODR_OFFSET*32)\
                                      +(PIN_4_BitNumber*4))

#define  CHANNELIN0_7    0
#define  CHANNELIN8_15   1
#define  CHANNEL_IN0_7SC   2
#define  CHANNEL_IN8_15SC  3
#define  CHANNEL_OUT0_7    4
#define  CHANNEL_OUT8_15   5
#define  CHANNEL_OUT0_7SC  6
#define  CHANNEL_OUT8_15SC  7
#define  CHANNEL_OUT0_7STATUS  8
#define  CHANNEL_OUT8_15STATUS  9
#define  CHANNEL_ALARM0_7  10
#define  CHANNEL_ALARM8_15  11


#define  A10BIT  (1<<10)
#define  A11BIT  (1<<11)

#define  NA10BIT (~A10BIT)
#define  NA11BIT (~A11BIT)

#define  SETHC165CLK  *(vu32*)HC165CLK=1;
#define  CLRHC165CLK  *(vu32*)HC165CLK=0;

#define  SETHC165LD   *(vu32*)HC165LD=1;
#define  CLRHC165LD   *(vu32*)HC165LD=0;





#define  SET574NOE *(vu32*)HC574NOE=1;
#define  CLR574NOE  *(vu32*)HC574NOE=0;
#define  SET574CLK  *(vu32*)HC574CLK=1;
#define  CLR574CLK   *(vu32*)HC574CLK=0;

#define  SETCSA  *(vu32*)CSA=1;
#define  CLRCSA  *(vu32*)CSA=0;

#define  SETCSB   *(vu32*)CSB=1;
#define  CLRCSB   *(vu32*)CSB=0;

#define  SETUPDOWN  *(vu32*)UPDOWN=1;
#define  CLRUPDOWN  *(vu32*)UPDOWN=0;

#define  SETCFGCS   *(vu32*)CFGCS=1;
#define  CLRCFGCS   *(vu32*)CFGCS=0;

#define SETALE   *(vu32*)ALEPIN=1;
#define CLRALE   *(vu32*)ALEPIN=0;

#define  SETWR   *(vu32*)WRPIN=1;
#define CLRWR    *(vu32*)WRPIN=0;

#define  SETRD    *(vu32*)RDPIN=1;
#define  CLRRD    *(vu32*)RDPIN=0;

#define  ENVPRESET  *(vu32*)VPRESET=1;
#define  DISVPRESET  *(vu32*)VPRESET=0;

#define  SETFPGARD   *(vu32*)FPGARD=1;
#define  CLRFPGARD   *(vu32*)FPGARD=0;

#define  SETFPGAWR   *(vu32*)FPGAWR=1;
#define  CLRFPGAWR   *(vu32*)FPGAWR=0;


#define  SETMODLED  *(vu32*)MODLED=1;
#define  CLRMODLED  *(vu32*)MODLED=0;

#define  SETVINLED  *(vu32*)VINLED=1;
#define  CLRVINLED  *(vu32*)VINLED=0;

#define  SETVOUTLED  *(vu32*)VOUTLED=1;
#define  CLRVOUTLED  *(vu32*)VOUTLED=0;


#define  	 CLRLOAD   *(vu32*)HC165LD=0;
#define    SETLOAD   *(vu32*)HC165LD=1;

#define   	CLRCLOCK  *(vu32*)HC165CLK=0;
#define    SETCLOCK  *(vu32*)HC165CLK=1;

#define  SERDATAPIN   (*(vu32*)HC165DATA)

//#define   DMACMDENABLE  (DMA_DIR_PeripheralDST|DMA_PeripheralInc_Disable|\
	   //                                        DMA_MemoryInc_Enable|DMA_PeripheralDataSize_HalfWord|\
	        //                                   DMA_MemoryDataSize_HalfWord|DMA_Mode_Normal|\
	             //                             DMA_Priority_Medium|DMA_M2M_Disable|CCR_ENABLE_Set\
	                  //                        |DMA_IT_TC)



#define   DMACMDENABLE  (DMA_DIR_PeripheralDST|DMA_PeripheralInc_Disable|\
	                                        DMA_MemoryInc_Enable|DMA_PeripheralDataSize_Word|\
	                                         DMA_MemoryDataSize_Byte|DMA_Mode_Normal|\
	                                          DMA_Priority_Medium|DMA_M2M_Disable|CCR_ENABLE_Set )

#define   DMACMDDISABLE  (DMA_DIR_PeripheralDST|DMA_PeripheralInc_Disable|\
	                                           DMA_MemoryInc_Enable|DMA_PeripheralDataSize_Word|\
	                                           DMA_MemoryDataSize_Byte|DMA_Mode_Normal|\
	                                          DMA_Priority_Medium|DMA_M2M_Disable)

#define UART1_DR_Address (0x40013800+0x4)




#define FLASHSTARTADDRESS          0x08000000

#define  SSIIDLE    0
#define  SSITRANSFER   1
#define  SSIBIT0    0
#define  SSIBIT1    1
#define  SSIBIT2    2
#define  SSIBIT3    3
#define  SSIBIT4    4
#define  SSIBIT5    5
#define  SSIBIT6    6
#define  SSIBIT7    7
#define  SSIBIT8    8
#define  SSIBIT9    9
#define  SSIBIT10    10
#define  SSIBIT11    11
#define  SSIBIT12    12
#define  SSIBIT13    13
#define  SSIBIT14    14
#define  SSIBIT15    15
#define  SSIBIT16    16
#define  SSIBIT17    17
#define  SSIBIT18    18
#define  SSIBIT19    19
#define  SSIBIT20    20
#define  SSIBIT21    21
#define  SSIBIT22    22
#define  SSIBIT23    23
#define  SSIBIT24    24
#define  SSIBIT25    25
#define  SSIBITV      26

#define   MAXCODEVALUE  0x01ffffff

typedef union {
          u32 value;
          u8 codedim[4];
      }CodeValueUnion;



#pragma pack(1)
typedef  struct{

  u8   ModuleAlConfig;
  u8   ChannelInPol[2];
  u8   ChannelConfig[4];
  u8   ChannelMap[16];
  u8   ReversedByte[10];

}ModPrmData;


#define MAINVERSION  0x1
#define SLAVEVERSION 0x1

#pragma pack()

#endif
