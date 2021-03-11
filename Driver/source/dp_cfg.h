/************************  Filename: dp_cfg.h ********************************/
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
/* Function: Configuration file for VPC3+                                    */
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

    - compiler switches
    - user defines
        -
        - length of buffers
        - defines for MSAC2
        - defines for MSAC1
        - defines for ALARM
        - defines for ISOCHRON
        - defines for DXB PUBLISHER
        - defines for DXB SUBSCRIBER
        - Hardwaremode
        - Interruptevents
    - calculating area (do not edit this part)

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_CFG_H
#define DP_CFG_H

/*--------------------------------------------------------------------------*/
/* profibus services                                                        */
/*--------------------------------------------------------------------------*/
/* todo: setup required services */

/*------------------------------------------*/
/* NOTICE: Only VPC3+/B supports DPV1, DPV2 */
/*------------------------------------------*/

/* DPV1 MSAC1 */
//#define DP_MSAC_C1

/* DPV1 MSAC2 */
//#define DP_MSAC_C2

/* DPV1 Alarms */
//#define DP_ALARM

/* I&M - Identification & Maintenence Functions */
//#define DPV1_IM_SUPP

/* DPV2  */
//#define DP_SUBSCRIBER

//#define DP_DEBUG_ENABLE

/*---------------------------------------------------------------------------*/
/* User defines                                                              */
/*---------------------------------------------------------------------------*/
/* todo */

/*-----------------------------------------------------------------------*/
/* general slave parameter                                               */
/*-----------------------------------------------------------------------*/

#include "Hardwaredef.h"


//#define DP_ADDR                     ((u8)0x07)   // Slave address
#define IDENT_NR                    ((u16)0xe79)	// PROFIBUS Ident Number
//#define  IDENT_NR           ((u16)0x0599)
//#define  IDENT_NR            ((u16)0x5868)
//  #define  IDENT_NR            ((u16)0xaaab)
//#define  IDENT_NR            ((u16)0x06ae)  //id for 5860
//#define  IDENT_NR             ((u16)0x9080)
#define USER_WD                     ((u16)0x01FF) // User Watchdog
                                                    // set watchdog greater than 0x00FF, if serial printouts used
													// The UserWatchdog is only active in DataExchange. The UserWatchdog
													// isn't timebased, it's a counter of DataExchange-telegrams.
													 	
/*-----------------------------------------------------------------------*/
/* define buffer length                                                  */
/*-----------------------------------------------------------------------*/
#define DIN_BUFSIZE                 ((u8)64)   // Length of the DIn Buffer (Data Slave to Master)                0..244
#define DOUT_BUFSIZE                ((u8)64)   // Length of the DOut Buffer     (Data Master to Slave)           0..244
#define PRM_BUFSIZE                 ((u8)33)   // Length of the Parameter Buffer                                 7..244
#define DIAG_BUFSIZE                ((u8)60)   // Length of the Diagnosis Buffer                                 6..244
#define CFG_BUFSIZE                 ((u8)0x6)   // Length of the Configuration Buffer                             1..244
#define SSA_BUFSIZE                 ((u8)0x00)   // Length of the Input Data in the Set_Slave_Address-Buffer 0 and 4..244
                                                    // 0: SetSlaveAddress will be deactivated!

/*-----------------------------------------------------------------------*/
/* settings for MSAC_C1                                                  */
/*-----------------------------------------------------------------------*/
#define C1_LEN                      ((u8)0xF4)   // Length of the MSAC_C1 Data
                                                    // The parameter specifies the maximum length of user data including
                                                    // Function_Num, Slot_number, Index, Length, transferred on the
                                                    // MSAC_1 communication channel.
                                                    // Type: Unsigned8 (0 .. 244)

/*-----------------------------------------------------------------------*/
/* settings for MSAC_C1 Alarm                                            */
/*-----------------------------------------------------------------------*/
#define DP_ALARM_OVER_SAP50                         // Alarm Acknowledge over SAP50

/*-----------------------------------------------------------------------*/
/* settings for MSAC_C2                                                  */
/*-----------------------------------------------------------------------*/
#define DP_MSAC_C2_Time                             // enables time control for msac-c2
#define C2_NUM_SAPS                 ((u8)0x03)   // Number of MSAC_C2 connections
#define C2_LEN                      ((u8)0xF4)   // Length of MSAC_C2 Data
                                                    // The parameter specifies the maximum length of an Initiate Request
                                                    // PDU including the Function_Num to the Resource Manager.
                                                    // The parameter specifies the maximum length of user data including
                                                    // Function_Num, Slot_number, Index, Length, transferred on the
                                                    // MSAC_2 communication channel.
                                                    // Type: Unsigned8 (0,52.. 244)
#define C2_FEATURES_SUPPORTED_1     ((u8)0x01)   // default: 0x01, Read and Write supported
#define C2_FEATURES_SUPPORTED_2     ((u8)0x00)   // default: 0x00
#define C2_PROFILE_FEATURES_1       ((u8)0x00)   // default: 0x00, vendor or profile specific
#define C2_PROFILE_FEATURES_2       ((u8)0x00)   // default: 0x00, vendor or profile specific
#define C2_PROFILE_NUMBER           ((u16)0x0000) // default: 0x0000, vendor or profile specific

/*-----------------------------------------------------------------------*/
/* settings for DPV2 - Isochron Mode                                     */
/*-----------------------------------------------------------------------*/
#define SYNCH_PULSEWIDTH            ((u8)0xFF)   // Width of Synchpulse in 1/12µs

/*-----------------------------------------------------------------------*/
/* settings for DPV2 - DXB Subscriber                                    */
/*-----------------------------------------------------------------------*/
#define MAX_LINK_SUPPORTED          ((u8)0x05)   // Number of Links
#define MAX_DATA_PER_LINK           ((u8)0x10)   // maximal Number of Data per Link

/*---------------------------------------------------------------------------*/
/* set hardware modes                                                        */
/*---------------------------------------------------------------------------*/
//#define DP_VPC3_4KB_MODE							// (only VPC3+B, VPC3+C)

/*-----------------------------------------------------------------------*/
/* ModeRegister0 (7..0) ( page 15 )                                      */
/*-----------------------------------------------------------------------*/
//
//  bit  7       6      5        4         3       2       1          0
//  --------------------------------------------------------------------------
//  | Freeze | Sync | Early | Int_Pol | MinTSDR | Res | Dis_Stop | Dis_Start |
//  |  supp. | supp.|  RDY  |         |         |     | Control  | Control   |
//  --------------------------------------------------------------------------
//       1       1      0        0         0       0       0          0        = 0xC0 // Default
//
#define INIT_VPC3_MODE_REG_L        ((u8)0xC0)
//#define INIT_VPC3_MODE_REG_L        ((u8)0x04)

/*-----------------------------------------------------------------------*/
/* ModeRegister0 (15..8) ( page 15 )                                     */
/*-----------------------------------------------------------------------*/
//
//  bit15      14       13           12           11           10         9         8
//  ------------------------------------------------------------------------------------
//  | Res | PrmCmd | Spec_Clear | Spec_Prm |  SetExtPrm  | User_Time | EOI_Time |  DP  |
//  |       |  Supp    |    Mode     | Buf_Mode  |     Supp      |    Base     |   Base     | Mode |
//  ------------------------------------------------------------------------------------
//     0       0        1            0              0           1         1         1   = 0x27 // Default
//
#define INIT_VPC3_MODE_REG_H       ((u8)0x27) //((u8)0x27)


/*-----------------------------------------------------------------------*/
/* ModeRegister2 (7..0) ( page 19 )   (only VPC3+B, VPC3+C)              */
/*-----------------------------------------------------------------------*/
//
//  bit  7          6          5        4       3          2          1            0
//  --------------------------------------------------------------------------------------
//  |  4KB  |   No_Check   | SYNC_  | SYNC_ | DX_Int_ | DX_Int_ |  No_Check_  | NEW_GC_  |
//  |  Mode | Prm_Reserved |  Pol   |  ENA  |  Port   |  Mode   | GC_RESERVED | Int_Mode |
//  --------------------------------------------------------------------------------------
//       1          0          0        0       0          0          0            1        = 0x01 or 0x81
//
#ifdef DP_VPC3_4KB_MODE
    #define INIT_VPC3_MODE_REG_2    ((u8)0x81)
#else
    #define INIT_VPC3_MODE_REG_2    ((u8)0x01)
#endif

/*---------------------------------------------------------------------------*/
/* set interrupt indications                                                 */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Interrupt Mask Register (7..0) ( page 23 )                            */
/*-----------------------------------------------------------------------*/
//  bit7       6          5           4             3             2          1         0
//  ----------------------------------------------------------------------------------------
//  | DXB | New_Ext  | DXB_LINK | User_Timer |    WD_DP        | Baud_Rate | Go/Leave |  MAC  |
//  | OUT | PRM_Data |  ERROR   |   Clock       | Mode_Timeout |   detect    |  DataEx    | Reset |
//  ----------------------------------------------------------------------------------------
//     1       0          1           1             1             1          1         0      = 0xBE // Default
//
#define INIT_VPC3_IND_L 0xe //0xC //0xBE

/*-----------------------------------------------------------------------*/
/* Interrupt Mask Register (15..8) ( page 23 )                           */
/*-----------------------------------------------------------------------*/
//  bit  15         14       13        12            11       10       9         8
//  -----------------------------------------------------------------------------------
//  | REQ_PDU | POLL_END | DX_OUT | Diag_Buffer | New_PRM | NewCfg | NewSSA |  NewGC  |
//  |   Ind       |    Ind       |             |   Changed  |   Data      |  Data    |  Data    | Command |
//  -----------------------------------------------------------------------------------
//       1          1        1          1            1        1        0         0      = 0xFC // Default
//
#define INIT_VPC3_IND_H  0x3c //0xFC

/*---------------------------------------------------------------------------*/
/* end of user defines                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* !!!!! do not edit this calculating part !!!!!                             */
/*---------------------------------------------------------------------------*/
// == Segmentation VPC3+ ===============================================================
#ifdef DP_VPC3_4KB_MODE
    // 16 Byte segmentation
    #define SEG_OFFSET              ((u8)0x0F)
    #define SEG_MULDIV              ((u8)0x04)
    #define SEG_ADDBYTE             ((u16)0xFFF0)
    #define SEG_ADDWORD             ((u16)0xFFF0)
#else
    // 8 Byte segmentation
    #define SEG_OFFSET              ((u8)0x07)
    #define SEG_MULDIV              ((u8)0x03)
    #define SEG_ADDBYTE             ((u16)0xF8)
    #define SEG_ADDWORD             ((u16)0xFFF8)
#endif

// == DPV1/DPV2 ========================================================================
//#define DP_C1_USE_RD_WR_SAP         0x01
//#define DP_C1_USE_ALARM_SAP         0x04


//#if (DP_C1_SAPS & DP_C1_USE_RD_WR_SAP)
//    #define DP_TEMP_X1              1
//#else
 //   #define DP_TEMP_X1              0
//#endif

//#if (DP_C1_SAPS & DP_C1_USE_ALARM_SAP )
  // #define DP_TEMP_X2               1
 //  #define SAP_50
//#else
 //  #define DP_TEMP_X2               0
//#endif



#define DP_ORG_LENGTH               0x40 // organizational parameter


#ifdef DP_VPC3_4KB_MODE
    #define ASIC_RAM_LENGTH         0x1000
    #define ASIC_USER_RAM           (ASIC_RAM_LENGTH - DP_ORG_LENGTH )
#else
    #define ASIC_RAM_LENGTH         0x800
    #define ASIC_USER_RAM           (ASIC_RAM_LENGTH - DP_ORG_LENGTH )
#endif

#define ISR_ENABLE_VPC3_INT_MAC_RESET               0
#define ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX        0
#define ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT         0
#define ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT           0
#define ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK        0
#define ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR          0
#define ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA        0
#define ISR_ENABLE_VPC3_INT_DXB_OUT                 0
#define ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND          0
#define ISR_ENABLE_VPC3_INT_NEW_SSA_DATA            0
#define ISR_ENABLE_VPC3_INT_NEW_CFG_DATA            0
#define ISR_ENABLE_VPC3_INT_NEW_PRM_DATA            0
#define ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED        0
#define ISR_ENABLE_VPC3_INT_DX_OUT                  0
#define ISR_ENABLE_VPC3_INT_RESERVED                0
#define ISR_ENABLE_VPC3_INT_SERVICE_ERROR           0


#if( INIT_VPC3_IND_L & 0x01 )
    #undef  ISR_ENABLE_VPC3_INT_MAC_RESET
    #define ISR_ENABLE_VPC3_INT_MAC_RESET           1
#endif
#if( INIT_VPC3_IND_L & 0x02 )
    #undef  ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX
    #define ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX    1
#endif
#if( INIT_VPC3_IND_L & 0x04 )
    #undef  ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT
    #define ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT     1
#endif
#if( INIT_VPC3_IND_L & 0x08 )
    #undef  ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT
    #define ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT       1
#endif
#if( INIT_VPC3_IND_L & 0x10 )
    #undef  ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK
    #define ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK    1
#endif
#if( INIT_VPC3_IND_L & 0x20 )
    #undef  ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR
    #define ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR      1
#endif
#if( INIT_VPC3_IND_L & 0x40 )
    #undef  ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA
    #define ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA    1
#endif
#if( INIT_VPC3_IND_L & 0x80 )
    #undef  ISR_ENABLE_VPC3_INT_DXB_OUT
    #define ISR_ENABLE_VPC3_INT_DXB_OUT             1
#endif


#if( INIT_VPC3_IND_H & 0x01 )
    #undef  ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND
    #define ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND      1
#endif
#if( INIT_VPC3_IND_H & 0x02 )
    #undef  ISR_ENABLE_VPC3_INT_NEW_SSA_DATA
    #define ISR_ENABLE_VPC3_INT_NEW_SSA_DATA        1
#endif
#if( INIT_VPC3_IND_H & 0x04 )
    #undef  ISR_ENABLE_VPC3_INT_NEW_CFG_DATA
    #define ISR_ENABLE_VPC3_INT_NEW_CFG_DATA        1
#endif
#if( INIT_VPC3_IND_H & 0x08 )
    #undef  ISR_ENABLE_VPC3_INT_NEW_PRM_DATA
    #define ISR_ENABLE_VPC3_INT_NEW_PRM_DATA        1
#endif
#if( INIT_VPC3_IND_H & 0x10 )
    #undef  ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED
    #define ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED    1
#endif
#if( INIT_VPC3_IND_H & 0x20 )
    #undef  ISR_ENABLE_VPC3_INT_DX_OUT
    #define ISR_ENABLE_VPC3_INT_DX_OUT              1
#endif
#if( INIT_VPC3_IND_H & 0x40 )
    #undef  ISR_ENABLE_VPC3_INT_POLL_END_IND
    #define ISR_ENABLE_VPC3_INT_POLL_END_IND        1
#endif
#if( INIT_VPC3_IND_H & 0x80 )
    #undef  ISR_ENABLE_VPC3_INT_FDL_IND
    #define ISR_ENABLE_VPC3_INT_FDL_IND             1
#endif


/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DP_CFG.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2004. Confidential.                         */
/*****************************************************************************/
