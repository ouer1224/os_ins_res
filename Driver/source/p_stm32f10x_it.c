/**
  ******************************************************************************
  * @file USART/DMA_Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Hardwaredef.h"
#include "dp_inc.h"
#include "debug.h"


/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_DMA_Interrupt
  * @{
  */ 



extern u8 gEventFlag;

#define   BAUDDETECTFLAG    0x1
 #define   PRMFINISHEDFLAG   0x2
 #define   CFGFINISHEDFLAG    0x3
  #define   OUTPUTFLAG       0x4
extern  u16  gData;
u8  cfgbytebuffer[10];
  
 #define   NOTHING                    0xff
extern VPC3_SYSTEM_STRUC               dp_sys;     // global system structure
 u8     SimReadEMIMUX16(u16 address);
 void  SimWriteEMIMUX16(u8 value, u16 address);
extern CFG_STRUCT   cfg;

#define RXNE   0x5
#define TXE    0x7
#define TCE    0x6
#define TE     0x3
#define RE     0x2
#define UE     0xd
extern u8 TxBuffer[20] ;
extern u8 RxBuffer[10];
u8 NbrOfDataToTransfer = 10;
u8 NbrOfDataToRead = 10;
extern u8 Rxlocation;
extern u8 Txlocation;
extern u8 Txcounter ; 
extern u8 Rxcounter ;

u32 SysTick_count=0;


extern bool bReadFlag;

void TimingDelay_Decrement(void);


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
  
void SysTick_Handler(void)
{
	  SysTick_count++;
}




void TIM3_IRQHandler(void)
{
 	
}



/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART1_IRQHandler(void)
{

 	if(USART1->SR&(1<<RXNE))
	{
	
	  RxBuffer[Rxcounter++] = USART1->DR;		  
	
	  if(Rxcounter == NbrOfDataToRead)
	  {
		/* Disable the USART Receive interrupt */
		    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
     
	  }
	  //if(RxCounter==3)bReadFlag=TRUE;
	  if(Rxcounter==RxBuffer[0])
	  {
	     bReadFlag=TRUE;
		  //  USART1->CR1=0;//RXIEENABLE;
      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	       
	  }
	 
	  return;
	}
	
	
	

}


void EXTI9_5_IRQHandler (void)
{

   
      /*-----------------------------------------------------------------------*/
      /* IND_NEW_PRM_DATA                                                      */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )
          if(VPC3_GET_IND_NEW_PRM_DATA())
          {
      
              
              dp_sys.vpc3_con = VPC3_PRM_FINISHED;
      
              do
              {
               
                  if( user_chk_new_prm_data( VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
                  {
                      dp_sys.vpc3_con = VPC3_SET_PRM_DATA_OK();
       
      
                  }//if( user_chk_new_prm_data( VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
                  else
                  {
                      dp_sys.vpc3_con = VPC3_SET_PRM_DATA_NOT_OK();
       
      
                  }//else of if( user_chk_new_prm_data( VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
              }
              while( dp_sys.vpc3_con == VPC3_PRM_CONFLICT );
      
          }//if(VPC3_GET_IND_NEW_PRM_DATA())
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )

            /*-----------------------------------------------------------------------*/
            /* IND_NEW_EXT_PRM_DATA                                                  */
            /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )
                if(VPC3_GET_IND_NEW_EXT_PRM_DATA())
                {
                    dp_sys.vpc3_con = VPC3_PRM_FINISHED;
            
                    do
                    {
                        if( user_chk_new_ext_prm_data( (VPC3_STRUC_PRM_BLOCK_PTR)VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
                        {
                            dp_sys.vpc3_con = VPC3_SET_PRM_DATA_OK();
            
                            
                        }//if( user_chk_new_ext_prm_data( VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
                        else
                        {
                            dp_sys.vpc3_con = VPC3_SET_PRM_DATA_NOT_OK();
                        }//else of if( user_chk_new_ext_prm_data( VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() ) == DP_OK )
                    }
                    while( dp_sys.vpc3_con == VPC3_PRM_CONFLICT );
                }//if(VPC3_GET_IND_NEW_EXT_PRM_DATA())
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )


      /*-----------------------------------------------------------------------*/
      /* check config data , application specific!                             */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )

          if(VPC3_GET_IND_NEW_CFG_DATA())
          {
      
      
              dp_sys.vpc3_con = VPC3_CFG_FINISHED;
      
              do
              {
           
                  switch( user_chk_new_cfg_data() )
                  {
                      case VPC3_CFG_OK:
                      {
                         
                          dp_sys.vpc3_con = VPC3_SET_CFG_DATA_OK();
                        //  gEventFlag=CFGFINISHEDFLAG;
                       //  gData=VPC3->slave_addr;
      
                          break;
                      }//case VPC3_CFG_OK:
      
                      case VPC3_CFG_FAULT:
                      {
      
                          dp_sys.vpc3_con = VPC3_SET_CFG_DATA_NOT_OK();
      
      
                          break;
                      }//case VPC3_CFG_FAULT:
      
                      case VPC3_CFG_UPDATE:
                      {
                          // Calculate the length of the input and output using the configuration bytes

                        //  if( DP_OK != vpc3_calculate_inp_outp_len ( VPC3_GET_CFG_BUF_PTR(), VPC3_GET_CFG_LEN() ) )
                         if( DP_OK != vpc3_calculate_inp_outp_len ( cfg.cfg_data, VPC3_GET_CFG_LEN()  ))
                          {
      
      
                              dp_sys.vpc3_con = VPC3_SET_CFG_DATA_NOT_OK();
      
      
                          }//if( DP_OK != vpc3_calculate_inp_outp_len (cfg_ptr,VPC3_GET_CFG_LEN()) )
                          else
                          {
                              // set IO-Length
                              vpc3_set_io_data_len();
      
      
                              dp_sys.vpc3_con = VPC3_SET_CFG_DATA_UPDATE();
      
      
                          }//else of if( DP_OK != vpc3_calculate_inp_outp_len (cfg_ptr,VPC3_GET_CFG_LEN()) )
                          break;
                      }//case VPC3_CFG_UPDATE:
      
                      default:
                      {
                          break;
                      }//default:
                  }//switch( user_chk_new_cfg_data() )
              }
              while ( dp_sys.vpc3_con == VPC3_CFG_CONFLICT );
      
          }//if(VPC3_GET_IND_NEW_CFG_DATA())
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )
      


      
      
      /*-----------------------------------------------------------------------*/
      /* IND_GO_LEAVE_DATA_EX                                                  */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )
          if(VPC3_GET_IND_GO_LEAVE_DATA_EX())
          {
      
              user_isr_go_leave_data_ex();
           //   print_string("GL");
            //  start_serial_communication();
              VPC3_CON_IND_GO_LEAVE_DATA_EX();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_DX_OUT                                                            */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )
          if (VPC3_GET_IND_DX_OUT())
          {
      
              user_isr_dx_out();
              VPC3_CON_IND_DX_OUT();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_DIAG_BUF_CHANGED                                                  */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )
          if(VPC3_GET_IND_DIAG_BUF_CHANGED())
          {
              user_isr_diag_buf_changed();
              VPC3_CON_IND_DIAG_BUF_CHANGED();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_WD_DP_TIMEOUT                                                     */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )
          if(VPC3_GET_IND_WD_DP_TIMEOUT() )
          {
              user_isr_new_wd_dp_timeout();
              VPC3_CON_IND_WD_DP_TIMEOUT();    // acknowledge
          }
#endif//#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_MAC_RESET                                                         */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )
          if(VPC3_GET_IND_MAC_RESET())
          {
              
              VPC3_CON_IND_MAC_RESET();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_BAUDRATE_DETECT                                                   */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )
          if(VPC3_GET_IND_BAUDRATE_DETECT())
          {
      
              user_isr_baudrate_detect();
              VPC3_CON_IND_BAUDRATE_DETECT();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_NEW_GC_COMMAND                                                    */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )
          if(VPC3_GET_IND_NEW_GC_COMMAND())
          {
              user_isr_new_gc_command();
              VPC3_CON_IND_NEW_GC_COMMAND();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )
      
      /*-----------------------------------------------------------------------*/
      /* IND_NEW_SSA_DATA                                                      */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )
          if(VPC3_GET_IND_NEW_SSA_DATA())
          {
              user_isr_new_ssa_data();
              dp_sys.vpc3_con = VPC3_FREE_SSA_BUF(); //release the Set Slave Address buffer
              VPC3_CON_IND_NEW_SSA_DATA();
          }
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )
      
      
      /*-----------------------------------------------------------------------*/
      /* IND_USER_TIMER_CLOCK                                                  */
      /*-----------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )
          if(VPC3_GET_IND_USER_TIMER_CLOCK() )
          {
              user_isr_user_timer_clock();
      
      
              VPC3_CON_IND_USER_TIMER_CLOCK();
          }//if(VPC3_GET_IND_USER_TIMER_CLOCK() )
#endif//#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )
      
      
      
      /*-----------------------------------------------------------------------*/
      /* reset interrupt line                                                  */
      /*-----------------------------------------------------------------------*/
      VPC3_SET_EOI();

      EXTI->PR=0xffff;

}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
