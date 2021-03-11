/**************************  Filename: dp_inc.h  *****************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Tel: ++49-9132-744-200  */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax: ++49-9132-744-204  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Description: Extern modules to be included in all DP files.               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       P. Fredehorst                                    */
/*                          Tel. : ++49-9132/744-214                         */
/*                          Fax. :              -204                         */
/*                          eMail: pfredehorst@profichip.com                 */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

    - header include hierarchy for system environment

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_INC_H
#define DP_INC_H

#include "Hardwaredef.h"


/*****************************************************************************/
/* header include hierarchy for system environment */


/*------------------------------------------------------------------------*/
/* Debug Codes                                                            */
/*------------------------------------------------------------------------*/
typedef enum
{
    START__                                 = ((u8)0x01),

    DP_INTERRUPT__                          = ((u8)0x20),
    DP_INTERRUPT_PRM__                      = ((u8)0x21),
    DP_INTERRUPT_CFG__                      = ((u8)0x22),
    
    FDL_MAC_INDICATION__					= ((u8)0x30),
    FDL_FLC_INDICATION__					= ((u8)0x31),
    
    MSAC_C2_INPUT_QUEUE__                   = ((u8)0x40),  
    MSAC_C2_PROCESS__                       = ((u8)0x41),  
    MSAC_C2_PRU_PROVIDE__                   = ((u8)0x42),  
    MSAC_C2_REALIZE_IND_REC__               = ((u8)0x43),  
    MSAC_C2_REALIZE_RESP_SENT__             = ((u8)0x44),  
	MSAC_C2_CREATE_DISCONNECT_PDU__         = ((u8)0x45), 
	MSAC_C2_SEND_RESPONSE_DATA__            = ((u8)0x46), 
	MSAC_C2_HANDLE_USER_RESPONSE__          = ((u8)0x47), 
	MSAC_C2_RESP_PDU_PROVIDE__              = ((u8)0x48),
      
    FATAL_ERROR__		                    = ((u8)0xFF)  
} DEBUG_CODE;

/*------------------------------------------------------------------------*/
/* Debug Structure                                                        */
/*------------------------------------------------------------------------*/
#define MAX_NR_OF_DEBUG     256



/*------------------------------------------------------------------------*/
/* dp_user.c                                                              */
/*------------------------------------------------------------------------*/
//extern VPC3_STRUC      VPC3_PTR     p_Vpc3;     // pointer to Vpc3 structure
//extern VPC3_SYSTEM_STRUC            dp_sys;     // global system structure

/*------------------------------------------------------------------------*/
/* dp_main.c                                                              */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* dp_user.c                                                              */
/*------------------------------------------------------------------------*/
extern void                 user_main                       ( void );
extern void                 fatal_error                     ( DP_ERROR_FILE file, u16 line, VPC3_ERRCB_PTR errcb_ptr );
extern DPV1_RET_VAL         user_dpv1_read_req              ( u8 sap, DPV1_PTR pdu );
extern DPV1_RET_VAL         user_dpv1_write_req             ( u8 sap, DPV1_PTR pdu );
extern DPV1_RET_VAL         user_c2_initiate_req            ( u8 sap, DPV1_PTR pdu );
extern DPV1_RET_VAL         user_c2_data_transport_req      ( u8 sap, DPV1_PTR pdu );
extern DPV1_RET_VAL         user_c2_abort_ind               ( u8 sap,DPV1_PTR pdu );
extern void                 user_alarm_ack_req              ( ALARM_STATUS_PDU_PTR alarm_ptr );
extern void                 user_alarm_state_info           ( u8 alarm_type_bit_field, u8 sequence_depth );

/*------------------------------------------------------------------------*/
/* dp_if.c                                                                */
/*------------------------------------------------------------------------*/
extern void                 DP_GetVersion                   ( void );
extern DP_ERROR_CODE        vpc3_initialization             ( u8 slave_address, CFG_STRUCT cfg );
extern void                 START_VPC3                      ( void );
extern void                 STOP_VPC3                       ( void );
extern void                 vpc3_dpv1_statemachine          ( void );
extern u8                vpc3_cfg_data_ok                ( void );
extern u8                vpc3_cfg_data_update            ( void );
extern u8                vpc3_cfg_data_not_ok            ( void );
extern DP_ERROR_CODE        vpc3_calculate_inp_outp_len     ( u8  *cfg_ptr, u8 cfg_len );
extern void                 vpc3_set_io_data_len            ( void );
extern u16   vpc3_get_dinbufptr              ( void );
extern u16   vpc3_get_doutbufptr             ( u8  *state_ptr );
extern u16   vpc3_get_diagbufptr             ( void );
extern u8                vpc3_get_master                 ( void );
extern DP_ERROR_CODE        set_diagnosis                   ( VPC3_UNION_DIAG_PTR user_diag, u8 user_diag_len, u8 diag_control, u8 check_diag_flag );
extern void                 reset_diagnostic_buffer         ( void );
extern u16   vpc3_get_free_memory            ( u16 *mem_len );
extern u8                set_alarm                       ( ALARM_STATUS_PDU_PTR user_alarm_ptr, u8 callback );



/*------------------------------------------------------------------------*/
/* dp_isr.c                                                                */
/*------------------------------------------------------------------------*/
extern void                 vpc3_isr( void );






#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )
    extern u8 user_chk_new_prm_data( u16 prm_ptr, u8 len );
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )

#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )
    extern u8 user_chk_new_ext_prm_data( VPC3_STRUC_PRM_BLOCK_PTR prmblock_ptr, u8 len );
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )

#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )
    extern u8 user_chk_new_cfg_data( void );
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )

#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )
    extern void user_isr_go_leave_data_ex( void );
#endif//#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )

#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )
    extern void user_isr_dx_out( void );
#endif//#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )

#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )
    extern void user_isr_diag_buf_changed( void );
#endif//#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )

#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )
    extern void user_isr_new_wd_dp_timeout( void );
#endif//#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )

#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )
	#ifdef DP_TIMESTAMP
    	extern void user_isr_clock_synchronisation( void );
    #else
    	extern void user_isr_mac_reset( void );
	#endif//#ifdef DP_TIMESTAMP
#endif//#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )

#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )
    extern void user_isr_baudrate_detect( void );
#endif//#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )

#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )
    extern void user_isr_new_gc_command( void );
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )

#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )
    extern void user_isr_new_ssa_data( void );
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )

#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )
    extern void user_isr_user_timer_clock( void );
#endif//#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )

#ifdef DP_SUBSCRIBER
    #if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 )
        extern void user_isr_dxb_out( void );
    #endif//#if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 )

    #if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 )
        extern void user_isr_dxb_link_error( void );
    #endif//#if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 )
#endif//#ifdef DP_SUBSCRIBER

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DP_INC.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2004. Confidential.                         */
/*****************************************************************************/
