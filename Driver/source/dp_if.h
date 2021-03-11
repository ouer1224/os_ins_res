/************************** Filename: dp_if.h ********************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraße 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Tel: ++49-9132-744-200  */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax: ++49-9132-744-204  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Description: VPC3+ structure.                                             */
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

    1.0 VPC3+
    1.1     - register structure of VPC3+
    1.2     - literals for interrupt register
    1.3     - literals for status register
    1.4     - literals for mode register 0
    1.5     - literals for mode register 1
    1.6     - literals for mode register 2
    1.7     - basic macros for VPC3+

    2.0 - prm data
    2.1     - structure of prm data
    2.2     - literals for DPV1 Status 1
    2.3     - literals for DPV1 Status 2
    2.4     - literals for DPV1 Status 3
    2.5     - general defines for prm data
    2.6     - literals for structured prm data
    2.7     - returncodes for prm data
    2.8     - macros for prm data

    3.0 - cfg data
    3.1     - standard format of cfg byte
    3.2     - special format of cfg byte
    3.3     - literals for cfg byte
    3.4     - returncodes for cfg data
    3.5     - literals for chk config data
    3.6     - macros for cfg data
    3.7     - structure for real cfg data

    4.0 - input / output
    4.1     - structure for calculated input-/output-length
    4.2     - states for output buffer
    4.3     - macros for input buffer

    5.0 - set slave address
    5.1     - structure
    5.2     - macros

    6.0 - global control telegram
    6.1     - defines for GLOBAL CONTROL
    6.2     - macros for global control

    7.0 - diagnostic telegram
    7.1     - defines
    7.2     - structures
    7.3     - macros

    8.0 - subscriber
    8.1     - structures
    8.2     - macros

    9.0 - isochron mode
    9.1     - defines
    9.2     - structures

   10.0 - fdl layer
   10.1     - SAPs (service access points) for MSAC1
   10.2     - SAPs (service access points) for MSAC2
   10.3     - structure of a FDL-indication-response-buffer-head
   10.4     - structure of a FDL-sap-entry
   10.5     - structure of a sap-control-block
   10.6     - structure of a indication- or response-buffer
   10.7     - structure of the immediate-response-pdu of the FDL-RM-SAP
   10.8     - global structure of FDL state machine
   10.9     - function-codes
   10.A     - returncodes of FDL-services
   10.B     - helpful macros

   11.0 - DPV1
   11.1     - return codes for DPV1 services
   11.2     - errorcodes of DPV1 for ERROR_DECODE
   11.3     - errorcodes of DPV1 for ERROR_CODE_1
   11.4     - coding of abort
   11.5     - Function Codes for DPV1
   11.6     - general defines for DPV1
   11.7     - structures for DPV1
   11.8     - defines for ALARM
   11.9     - structure of ALARM

   12.0 - global system structure

   13.0 - ERROR defines

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_IF_H
#define DP_IF_H

#include "Dpl_list.h"

/*-----------------------------------------------------------------------------------------------------------*/
/* 1.0 vpc3+                                                                                                 */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 1.1 register structure of vpc3+                                           */
/*---------------------------------------------------------------------------*/
//typedef struct
//{
                                        	// address  register
                                        	// -------------------------------------------------------------------
   // u8 int_req1;                     	// 000H     Interrupt request register 1
   // u8 int_req2;                     	// 001H     Interrupt request register 2
    #define VPC3_STRUC_int_req1   0x0
    #define VPC3_STRUC_int_req2   0x1
    //union                               	
    //{                                   	
     //   struct                          	
      //  {                               	// [read]
      //      u8 int_reg1;             	// 002H     Interrupt register 1
     //       u8 int_reg2;             	// 003H     Interrupt register 2
     //       u8 status_L;             	// 004H     status register b0..b7
    //        u8 status_H;             	// 005H     status register b8..b15
    #define VPC3_STRUC_isreg_rd_int_reg1  0x2
    #define VPC3_STRUC_isreg_rd_int_reg2  0x3
    #define VPC3_STRUC_isreg_rd_status_L  0x4
    #define VPC3_STRUC_isreg_rd_status_H  0x5
       // }rd;                            	
                                        	
      //  struct                          	
      //  {                               	// [write]
  //          u8 int_ack1;             	// 002H     Interrupt acknowledge register 1
  //          u8 int_ack2;             	// 003H     Interrupt acknowledge register 2
 //           u8 int_mask_L;           	// 004H     Interrupt mask register b0..b7
  //          u8 int_mask_H;           	// 005H     Interrupt mask register b8..b15
       // }wr;                            	
   // } isreg;                            	
    #define  VPC3_STRUC_isreg_wr_int_ack1  0x2
    #define  VPC3_STRUC_isreg_wr_int_ack2  0x3
    #define  VPC3_STRUC_isreg_wr_int_mask_L  0x4
    #define  VPC3_STRUC_isreg_wr_int_mask_H  0x5
                                        	
  //  u8 mode_reg0_L;                  	// 006H     mode register0 b0..b7
  //  u8 mode_reg0_H;                  	// 007H     mode register0 b8..b15

    #define  VPC3_STRUC_mode_reg0_L  0x6
    #define  VPC3_STRUC_mode_reg0_H  0x7
    
                                        	
   // union                               	
   // {                                   	
      //  struct                          	
      //  {                               	//          [read]
       //     u8 din_buffer_sm;        	// 008H     buffer assignment of the DP_DIN_BUFFER state machine
       //     u8 new_din_buf_cmd;      	// 009H     the user makes a new DP_DIN_BUFFER available in the N state
       //     u8 dout_buffer_sm;       	// 00AH     buffer assignment of the DP_DOUT_BUFFER state machine
       //     u8 next_dout_buf_cmd;    	// 00BH     the user fetches the last DP_DOUT_BUFFER from the N state
      //      u8 diag_buffer_sm;       	// 00CH     buffer assignment for DP_DIAG_BUFFER state machine
     //       u8 new_diag_buf_cmd;     	// 00DH     the user makes a new DP_DIAG_BUFFER available to the VPC3+
    //       u8 user_prm_data_ok;     	// 00EH     positive acknowledge for received user parameter data
    //        u8 user_prm_data_nok;    	// 00FH     negative acknowledge for received user parameter data
    //        u8 user_cfg_data_ok;     	// 010H     positive acknowledge for received config data
    //        u8 user_cfg_data_nok;    	// 011H     negative acknowledge for received config data
   //         u8 dxbout_buffer_sm;     	// 012H     buffer assignment of the DXB_OUT_BUFFER state machine
    //        u8 next_dxb_buf_cmd;     	// 013H     the user fetches the last DXB_OUT_BUFFER
    //        u8 ssa_buf_free_cmd;     	// 014H     the user has fetched data from ssa buffer and enables buffer again
   //         u8 mode_reg1;            	// 015H     current value of mode_reg1
       // } rd;                           	

       #define VPC3_STRUC_ctrl_reg_rd_din_buffer_sm 0x8
       #define VPC3_STRUC_ctrl_reg_rd_new_din_buf_cmd   0x9
       #define VPC3_STRUC_ctrl_reg_rd_dout_buffer_sm  0xa
       #define VPC3_STRUC_ctrl_reg_rd_next_dout_buf_cmd  0xb
       #define VPC3_STRUC_ctrl_reg_rd_diag_buffer_sm     0xc
       #define VPC3_STRUC_ctrl_reg_rd_new_diag_buf_cmd    0xd
       #define VPC3_STRUC_ctrl_reg_rd_user_prm_data_ok   0xe
       #define VPC3_STRUC_ctrl_reg_rd_user_prm_data_nok  0xf
       #define VPC3_STRUC_ctrl_reg_rd_user_cfg_data_ok   0x10
       #define VPC3_STRUC_ctrl_reg_rd_user_cfg_data_nok  0x11
       #define VPC3_STRUC_ctrl_reg_rd_dxbout_buffer_sm   0x12
       #define VPC3_STRUC_ctrl_reg_rd_next_dxb_buf_cmd   0x13
       #define VPC3_STRUC_ctrl_reg_rd_ssa_buf_free_cmd   0x14
       #define VPC3_STRUC_ctrl_reg_rd_mode_reg1          0x15
      //  struct                          	
      //  {                               	//          [write]
         //   u8 mode_reg1_s;          	// 008H     set b0..b7 in mode_reg1
       //     u8 mode_reg1_r;          	// 009H     reset b0..b7 in mode_reg1
     //       u8 wd_baud_ctrl;         	// 00AH     root value for baud rate monitoring
    //        u8 mintsdr;              	// 00BH     MinTsdr time
   //         u8 mode_reg2;            	// 00CH     set b0..b7 in mode_reg2
    //        u8 sync_pw_reg;          	// 00DH     sync pulse width register
     //       u8 sync_mode;            	// 00EH
      //      u8 sync_group;           	// 00FH
       //     u8 controlbyte_mask;     	// 010H
      //      u8 groupselect_mask;     	// 011H
     //       u8 reserved_12;          	// 012H
    //        u8 reserved_13;          	// 013H
   //         u8 reserved_14;          	// 014H
    //        u8 reserved_15;          	// 015H
    
       #define VPC3_STRUC_ctrl_reg_wr_mode_reg1_s  0x8
       #define VPC3_STRUC_ctrl_reg_wr_mode_reg1_r  0x9
       #define VPC3_STRUC_ctrl_reg_wr_wd_baud_ctrl  0xa
       #define VPC3_STRUC_ctrl_reg_wr_mintsdr       0xb
       #define VPC3_STRUC_ctrl_reg_wr_mode_reg2     0xc
       #define VPC3_STRUC_ctrl_reg_wr_sync_pw_reg   0xd
       #define VPC3_STRUC_ctrl_reg_wr_sync_mode     0xe
       #define VPC3_STRUC_ctrl_reg_wr_sync_group    0xf
       #define VPC3_STRUC_ctrl_reg_wr_controlbyte_mask 0x10
       #define VPC3_STRUC_ctrl_reg_wr_groupselect_mask   0x11
       #define VPC3_STRUC_ctrl_reg_wr_reserved_12       0x12
       #define VPC3_STRUC_ctrl_reg_wr_reserved_13       0x13
       #define VPC3_STRUC_ctrl_reg_wr_reserved_14       0x14
       #define VPC3_STRUC_ctrl_reg_wr_reserved_15       0x15
       // } wr;                           	
   // }ctrl_reg;                          	
                                        	
   // u8 slave_addr;                   	// 016H     setup VPC3+ station address
   // u8 fdl_sap_list_ptr;             	// 017H     pointer fdl_sap_list
   // u8 user_wd_value[2];             	// 018H     user watchdog value b0..b7
                                        	// 019H     user watchdog value b8..b15
     #define VPC3_STRUC_slave_addr  0x16
     #define VPC3_STRUC_fdl_sap_list_ptr  0x17
     #define VPC3_STRUC_user_wd_value0     0x18
     #define VPC3_STRUC_user_wd_value1     0x19
                                        	
  //  u8 len_dout_buf;                 	// 01AH     length of dout buffers
  //  u8 dout_buf_ptr[3];              	// 01BH     segment base address of dout_buffer [0]
                                        	// 01CH     segment base address of dout_buffer [1]
                                        	// 01DH     segment base address of dout_buffer [2]
    #define  VPC3_STRUC_len_dout_buf  0x1a
    #define  VPC3_STRUC_dout_buf_ptr0 0x1b
    #define  VPC3_STRUC_dout_buf_ptr1  0x1c
    #define  VPC3_STRUC_dout_buf_ptr2  0x1d
                                        	
    //u8 len_din_buf;                  	// 01EH     length of din buffers
    //u8 din_buf_ptr[3];               	// 01FH     segment base address of din_buffer [0]
                                        	// 020H     segment base address of din_buffer [1]
                                        	// 021H     segment base address of din_buffer [2]
    #define  VPC3_STRUC_len_din_buf   0x1e
    #define  VPC3_STRUC_din_buf_ptr0  0x1f
    #define  VPC3_STRUC_din_buf_ptr1  0x20
    #define  VPC3_STRUC_din_buf_ptr2  0x21
    
 //   u8 len_dxb_out_buf;              	// 022H     length of dxb buffers
//    u8 dxb_out_buf_ptr1;             	// 023H     segment base address of dxbout_buffer1
 //   u8 len_diag_buf[2];              	// 024H     length of diag buffers [0]
                                        	// 025H     length of diag buffers [1]

    #define  VPC3_STRUC_len_dxb_out_buf  0x22
    #define  VPC3_STRUC_dx_out_bufer_ptr1  0x23
    #define  VPC3_STRUC_len_diag_buf0      0x24
    #define  VPC3_STRUC_len_diag_buf1      0x25

    
  //  u8 diag_buf_ptr[2];              	// 026H     segment base address of diag_buffer [0]
                                        	// 027H     segment base address of diag_buffer [1]
    #define  VPC3_STRUC_diag_buf_ptr0  0x26
    #define  VPC3_STRUC_diag_buf_ptr1  0x27

    
   // u8 len_ctrl_buf[2];              	// 028H     length of aux buffer 1
                                        	// 029H     length of aux buffer 2
    #define VPC3_STRUC_len_ctrl_buf0   0x28
    #define VPC3_STRUC_len_ctrl_buf1   0x29

    
    //u8 aux_buf_sel;                  	// 02AH     assignment for aux buffers 1/2
  //  u8 aux_buf_ptr[2];               	// 02BH     segment base address of aux buffer 1
                                        	// 02CH     segment base address of aux buffer 2
    #define VPC3_STRUC_aux_buf_sel    0x2a
    #define VPC3_STRUC_aux_buf_ptr0   0x2b
    #define VPC3_STRUC_aux_buf_ptr1   0x2c
    
   // u8 len_ssa_buf;                  	// 02DH     length of SET_SLAVE_ADDRESS buffer
  //  u8 ssa_buf_ptr;                  	// 02EH     segment base address of SET_SLAVE_ADDRESS buffer
   // u8 len_prm_data;                 	// 02FH     max. length of input data in SET_PRM buffer
  //  u8 prm_buf_ptr;                  	// 030H     segment base address of SET_PRM buffer
 //   u8 len_cfg_data;                 	// 031H     length of input data in the CHECK_CONFIG buffer
 //   u8 cfg_buf_ptr;                  	// 032H     segment base address of CHECK_CONFIG buffer
 //   u8 len_read_cfg_data;            	// 033H     length of input data in the GET_CONFIG buffer
 //   u8 read_cfg_buf_ptr;             	// 034H     segment base address of GET_CONFIG buffer

    #define VPC3_STRUC_len_ssa_buf    0x2d
    #define VPC3_STRUC_ssa_buf_ptr    0x2e
    #define VPC3_STRUC_len_prm_data   0x2f
    #define VPC3_STRUC_prm_buf_ptr    0x30
    #define VPC3_STRUC_len_cfg_data       0x31
    #define VPC3_STRUC_cfg_buf_ptr    0x32
    #define VPC3_STRUC_len_read_cfg_data  0x33
    #define VPC3_STRUC_read_cfg_buf_ptr   0x34
    
 //   u8 len_dxb_link_table_buf;       	// 035H     length of dxb link table buffer
 //   u8 dxb_link_table_buf_ptr;       	// 036H     segment base address of dxb link table buffer
 //   u8 len_dxb_link_status_buf;      	// 037H     length of dxb link status buffer
  //  u8 dxb_link_status_buf_ptr;      	// 038H     segment base address of dxb link status buffer
 //   u8 real_no_add_change;           	// 039H     address changes
 //   u8 ident_low;                    	// 03AH     IDENT_LOW
 //   u8 ident_high;                   	// 03BH     IDENT_HIGH

    #define VPC3_STRUC_len_dxb_link_table_buf  0x35
    #define VPC3_STRUC_dxb_link_table_buf_ptr  0x36
    #define VPC3_STRUC_len_dxb_link_status_buf  0x37
    #define VPC3_STRUC_dxb_link_status_buf_ptr  0x38
    #define VPC3_STRUC_real_no_add_change       0x39
    #define VPC3_STRUC_ident_low                0x3a
    #define VPC3_STRUC_ident_high               0x3b
    
  //  u8 gc_command;                   	// 03CH     last global control command
  //  u8 len_spec_prm_buf;             	// 03DH     length of SPEC_PRM buffer
  //  u8 dxb_out_buf_ptr2;             	// 03EH     segment base address of dxbout_buffer2
   // u8 dxb_out_buf_ptr3;             	// 03FH     segment base address of dxbout_buffer3

    #define VPC3_STRUC_gc_command     0x3c
    #define VPC3_STRUC_len_spec_prm_buf   0x3d
    #define VPC3_STRUC_dxb_out_buf_ptr2   0x3e
    #define VPC3_STRUC_dxb_out_buf_ptr3   0x3f
                                        	
  //  u8 sap_ctrl_list[SAP_LENGTH];    	// 040H     SAP CONTROL BLOCK LIST
                                        	
 //   u8 dpbuffer[ASIC_USER_RAM];      	// VPC3: 040H...7F0H: ram area for dp buffers
                                        	// SPC3: 040H...5F0H
    #define VPC3_STRUC_dpbuffer    0x40

//}  VPC3_STRUC;
//#define VPC3_STRUC_PTR    VPC3_STRUC VPC3_PTR

//extern VPC3_STRUC_PTR           p_Vpc3;     // structure for vpc3+
//#define  VPC3         ((VPC3_STRUC_PTR)(VPC3_ASIC_ADDRESS))

   

/*---------------------------------------------------------------------------*/
/* 1.2 literals for interrupt register                                       */
/*---------------------------------------------------------------------------*/
//low Byte
#define VPC3_INT_MAC_RESET              ((u8)0x01)  // VPC3 in offline state
#define VPC3_INT_GO_LEAVE_DATA_EX       ((u8)0x02)  // VPC3 has entered or left DATA_EX
#define VPC3_INT_BAUDRATE_DETECT        ((u8)0x04)  // VPC3 has detected Baudrate
#define VPC3_INT_WD_DP_TIMEOUT          ((u8)0x08)  // DP watchdog timeout
#define VPC3_INT_USER_TIMER_CLOCK       ((u8)0x10)  // time base for USER_TIMER_CLOCKS has run out
#define VPC3_INT_DXB_LINK_ERROR         ((u8)0x20)  // subscriber link error
#define VPC3_INT_NEW_EXT_PRM_DATA       ((u8)0x40)  // New EXT_PRM_DATA received
#define VPC3_INT_DXB_OUT                ((u8)0x80)  // NEW subscriber data received

//high Byte
#define VPC3_INT_NEW_GC_COMMAND         ((u8)0x01)  // New Global control command received
#define VPC3_INT_NEW_SSA_DATA           ((u8)0x02)  // New SSA_DATA received
#define VPC3_INT_NEW_CFG_DATA           ((u8)0x04)  // New check_config received
#define VPC3_INT_NEW_PRM_DATA           ((u8)0x08)  // New prm_data received
#define VPC3_INT_DIAG_BUF_CHANGED       ((u8)0x10)  // diag_buffer has been changed
#define VPC3_INT_DX_OUT                 ((u8)0x20)  // New DOut_data received
#define VPC3_INT_POLL_END_IND           ((u8)0x40)  // DPV1
#define VPC3_INT_FDL_IND                ((u8)0x80)  // DPV1

// -- events for interrupt event handler --------------------------------------
#define VPC3_EV_CLOCK_SYNCHRONISATION   ((u16)0x0001)
#define VPC3_EV_MAC_RESET               ((u16)0x0001)
#define VPC3_EV_GO_LEAVE_DATA_EX        ((u16)0x0002)
#define VPC3_EV_BAUDRATE_DETECT         ((u16)0x0004)
#define VPC3_EV_WD_DP_TIMEOUT           ((u16)0x0008)
#define VPC3_EV_USER_TIMER_CLOCK        ((u16)0x0010)
#define VPC3_EV_DXB_LINK_ERROR          ((u16)0x0020)
#define VPC3_EV_NEW_EXT_PRM_DATA        ((u16)0x0040)
#define VPC3_EV_DXB_OUT                 ((u16)0x0080)
#define VPC3_EV_NEW_GC_COMMAND          ((u16)0x0100)
#define VPC3_EV_NEW_SSA_DATA            ((u16)0x0200)
#define VPC3_EV_NEW_CFG_DATA            ((u16)0x0400)
#define VPC3_EV_NEW_PRM_DATA            ((u16)0x0800)
#define VPC3_EV_DIAG_BUF_CHANGED        ((u16)0x1000)
#define VPC3_EV_DX_OUT                  ((u16)0x2000)
#define VPC3_EV_RESERVED                ((u16)0x4000)
#define VPC3_EV_NEW_INPUT_DATA          ((u16)0x8000)

/*---------------------------------------------------------------------------*/
/* 1.3 literals for status register                                          */
/*---------------------------------------------------------------------------*/
#define VPC3_PASS_IDLE                  ((u8)0x01)
#define VPC3_DIAG_FLAG                  ((u8)0x04)
#define WAIT_PRM                        ((u8)0x00)
#define WAIT_CFG                        ((u8)0x10)
#define DATA_EX                         ((u8)0x20)
#define DP_ERROR                        ((u8)0x30)
#define BAUD_SEARCH                     ((u8)0x00)
#define BAUD_CONTROL                    ((u8)0x40)
#define DP_MODE                         ((u8)0x80)
#define WD_ERROR                        ((u8)0xC0)

#define BAUDRATE_MASK                   ((u8)0x0F)
#define BAUDRATE_12MBAUD                ((u8)0x00)
#define BAUDRATE_6MBAUD                 ((u8)0x01)
#define BAUDRATE_3MBAUD                 ((u8)0x02)
#define BAUDRATE_1_5MBAUD               ((u8)0x03)
#define BAUDRATE_500KBAUD               ((u8)0x04)
#define BAUDRATE_187_50KBAUD            ((u8)0x05)
#define BAUDRATE_93_75KBAUD             ((u8)0x06)
#define BAUDRATE_45_45KBAUD             ((u8)0x07)
#define BAUDRATE_19_20KBAUD             ((u8)0x08)
#define BAUDRATE_9_60KBAUD              ((u8)0x09)
#define BAUDRATE_AFTER_RESET            ((u8)0xFF)

#define AT_MASK                         ((u8)0xF0)
#define AT_VPC3                         ((u8)0x00)
#define AT_VPC3B                        ((u8)0xB0)
#define AT_VPC3C                        ((u8)0xC0)
#define AT_MPI12X                       ((u8)0xD0)

/*---------------------------------------------------------------------------*/
/* 1.4 literals for mode register 0                                          */
/*---------------------------------------------------------------------------*/
//low Byte
#define VPC3_DIS_START_CTRL             ((u8)0x01)
#define VPC3_DIS_STOP_CTRL              ((u8)0x02)
#define VPC3_FDL_DBB                    ((u8)0x04)
#define VPC3_MINTSDR                    ((u8)0x08)
#define VPC3_INT_POL_HIGH               ((u8)0x10)
#define VPC3_EARLY_RDY                  ((u8)0x20)
#define VPC3_SYNC_SUPP                  ((u8)0x40)
#define VPC3_FREEZE_SUPP                ((u8)0x80)

//high Byte
#define VPC3_DP_MODE                    ((u8)0x01)
#define VPC3_EOI_TBASE_1ms              ((u8)0x02)
#define VPC3_USR_TBASE_10ms             ((u8)0x04)
#define VPC3_WD_TEST                    ((u8)0x08)
#define VPC3_SPEC_PRM_BUF_MODE          ((u8)0x10)
#define VPC3_SPEC_CLR_MODE              ((u8)0x20)

/*---------------------------------------------------------------------------*/
/* 1.5 literals for mode register 1                                          */
/*---------------------------------------------------------------------------*/
#define VPC3_START                      ((u8)0x01)
#define VPC3_EOI                        ((u8)0x02)
#define VPC3_GO_OFFLINE                 ((u8)0x04)
#define VPC3_USER_LEAVE_MASTER          ((u8)0x08)
#define VPC3_EN_CHG_CFG_BUFFER          ((u8)0x10)
#define VPC3_RES_USER_WD                ((u8)0x20)
#define VPC3_EN_CHK_SSAP                ((u8)0x80)

/*---------------------------------------------------------------------------*/
/* 1.6 literals for mode register 2                                          */
/*---------------------------------------------------------------------------*/
#define VPC3_GC_INT_MODE                ((u8)0x01)
#define VPC3_NO_CHECK_GC_RESERVED       ((u8)0x02)
#define VPC3_DX_INT_MODE                ((u8)0x04)
#define VPC3_DX_INT_PORT                ((u8)0x08)
#define VPC3_SYNC_ENA                   ((u8)0x10)
#define VPC3_SYNC_POL                   ((u8)0x20)
#define VPC3_NO_CHECK_PRM_RESERVED      ((u8)0x40)
#define VPC3_4KB_MODE                   ((u8)0x80)

/*---------------------------------------------------------------------------*/
/* 1.7 basic macros for VPC3+                                                */
/*---------------------------------------------------------------------------*/
// Startaddress of User-Area in VPC3-Format -----------------------------------
//#define VPC3_DP_BUF_START (u8)(((VPC3_ADR)&VPC3->dpbuffer - (VPC3_ADR)VPC3_ASIC_ADDRESS) >> SEG_MULDIV)
#define VPC3_DP_BUF_START  (u8)(0x40>>SEG_MULDIV)

// Startaddress of FDL-List-Area in VPC3-Format -------------------------------
//#define VPC3_SAP_CTRL_LIST_START (u8)(((VPC3_ADR)&VPC3->sap_ctrl_list - (VPC3_ADR)VPC3_ASIC_ADDRESS) >> SEG_MULDIV)

// set HW-Mode ----------------------------------------------------------------
//#define VPC3_SET_HW_MODE_HIGH(MODE_HIGH) VPC3->mode_reg0_H = MODE_HIGH
//#define VPC3_SET_HW_MODE_LOW(MODE_LOW)   VPC3->mode_reg0_L = MODE_LOW

#define VPC3_SET_HW_MODE_HIGH(MODE_HIGH) SimWriteEMIMUX16(MODE_HIGH,\
	               VPC3_STRUC_mode_reg0_H)
#define VPC3_SET_HW_MODE_LOW(MODE_LOW)  SimWriteEMIMUX16(MODE_LOW,\
	                VPC3_STRUC_mode_reg0_L)

// set interrupt indications --------------------------------------------------
//#define VPC3_SET_INDICATIONS_HIGH(IND_HIGH) VPC3->isreg.wr.int_mask_H = IND_HIGH
//#define VPC3_SET_INDICATIONS_LOW(IND_LOW)   VPC3->isreg.wr.int_mask_L = IND_LOW

#define VPC3_SET_INDICATIONS_HIGH(IND_HIGH) SimWriteEMIMUX16(IND_HIGH,\
	               VPC3_STRUC_isreg_wr_int_mask_H)
#define VPC3_SET_INDICATIONS_LOW(IND_LOW)   SimWriteEMIMUX16(IND_LOW,\
	               VPC3_STRUC_isreg_wr_int_mask_L)


// -- Set Slave-Adresse -------------------------------------------------------
//#define VPC3_SET_STATION_ADDRESS(ADDRESS) VPC3->slave_addr = ADDRESS
//#define VPC3_GET_STATION_ADDRESS()        VPC3->slave_addr

#define VPC3_SET_STATION_ADDRESS(ADDRESS) SimWriteEMIMUX16(ADDRESS,\
	                VPC3_STRUC_slave_addr)
#define VPC3_GET_STATION_ADDRESS()        SimReadEMIMUX16(VPC3_STRUC_slave_addr)


// -- Set min_TSDR ------------------------------------------------------------
//#define VPC3_SET_MINTSDR(MINTSDR) VPC3->ctrl_reg.wr.mintsdr = MINTSDR

#define VPC3_SET_MINTSDR(MINTSDR) SimWriteEMIMUX16(MINTSDR,\
	               VPC3_STRUC_ctrl_reg_wr_mintsdr)

// -- Set Baud Control --------------------------------------------------------
//#define VPC3_SET_BAUD_CNTRL(VALUE) VPC3->ctrl_reg.wr.wd_baud_ctrl = VALUE

#define VPC3_SET_BAUD_CNTRL(VALUE) SimWriteEMIMUX16(VALUE,\
	               VPC3_STRUC_ctrl_reg_wr_wd_baud_ctrl)


//-- Ident-Nummer -------------------------------------------------------------
//-- Set Ident-Number ---------------------------------------------------------
//#define VPC3_SET_IDENT_NUMBER_LOW(NR) VPC3->ident_low = NR
//#define VPC3_SET_IDENT_NUMBER_HIGH(NR) VPC3->ident_high = NR

#define VPC3_SET_IDENT_NUMBER_LOW(NR) SimWriteEMIMUX16(NR,\
	               VPC3_STRUC_ident_low)
#define VPC3_SET_IDENT_NUMBER_HIGH(NR) SimWriteEMIMUX16(NR,\
	               VPC3_STRUC_ident_high)



// -- address change service --------------------------------------------------
//#define VPC3_SET_ADD_CHG_DISABLE() VPC3->real_no_add_change  = 0xFF
//#define VPC3_SET_ADD_CHG_ENABLE()  VPC3->real_no_add_change  = 0x00

#define VPC3_SET_ADD_CHG_DISABLE() SimWriteEMIMUX16(0xFF,\
	                 VPC3_STRUC_real_no_add_change)
#define VPC3_SET_ADD_CHG_ENABLE()  SimWriteEMIMUX16(0x00,\
	                 VPC3_STRUC_real_no_add_change)


//-- start VPC3 ---------------------------------------------------------------
//#define START_VPC3__() VPC3->ctrl_reg.wr.mode_reg1_s = VPC3_START
//#define RESET_START_VPC3() VPC3->ctrl_reg.wr.mode_reg1_r = VPC3_START

#define START_VPC3__() SimWriteEMIMUX16(VPC3_START,\
	                   VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)
#define RESET_START_VPC3() SimWriteEMIMUX16(VPC3_START,\
	                   VPC3_STRUC_ctrl_reg_wr_mode_reg1_r)


//-- set VPC3 offline ---------------------------------------------------------
//#define GO_OFFLINE_VPC3()  VPC3->ctrl_reg.wr.mode_reg1_s = VPC3_GO_OFFLINE
#define GO_OFFLINE_VPC3()  SimWriteEMIMUX16(VPC3_GO_OFFLINE,\
	                   VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)

// -- read State of DP-State Machine ------------------------------------------
//#define VPC3_GET_DP_STATE() (VPC3->isreg.rd.status_L & 0x30)
#define VPC3_GET_DP_STATE() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_L) & 0x30)

// -- read WD-State Machine ---------------------------------------------------
//#define VPC3_GET_WD_STATE() (VPC3->isreg.rd.status_L & 0xC0)
#define VPC3_GET_WD_STATE() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_L)& 0xC0)


// -- read Baud-Rate ----------------------------------------------------------
//#define VPC3_GET_BAUDRATE() (VPC3->isreg.rd.status_H & BAUDRATE_MASK)
#define VPC3_GET_BAUDRATE() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_H)& BAUDRATE_MASK)


// -- read ASIC-Type ----------------------------------------------------------
//#define VPC3_GET_ASIC_TYPE() (VPC3->isreg.rd.status_H & AT_MASK)
#define VPC3_GET_ASIC_TYPE() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_H)& AT_MASK)


// -- read VPC3-Offline/Passiv-Idle-State -------------------------------------
//#define VPC3_GET_OFF_PASS()  (VPC3->isreg.rd.status_L & VPC3_PASS_IDLE)
#define VPC3_GET_OFF_PASS()  (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_L)& VPC3_PASS_IDLE)


// -- set DP StateMachine to WAIT_PRM -----------------------------------------
//#define VPC3_SET_USER_LEAVE_MASTER() VPC3->ctrl_reg.wr.mode_reg1_s = VPC3_USER_LEAVE_MASTER
#define VPC3_SET_USER_LEAVE_MASTER() SimWriteEMIMUX16(VPC3_USER_LEAVE_MASTER,\
	                 VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)


// -- User-Watchdog -----------------------------------------------------------
//#define VPC3_SET_USER_WD_VALUE_HIGH(VALUE_HIGH) VPC3->user_wd_value[1] = VALUE_HIGH
//#define VPC3_SET_USER_WD_VALUE_LOW(VALUE_LOW)   VPC3->user_wd_value[0] = VALUE_LOW
#define VPC3_SET_USER_WD_VALUE_HIGH(VALUE_HIGH) SimWriteEMIMUX16(VALUE_HIGH,\
	             VPC3_STRUC_user_wd_value1)
#define VPC3_SET_USER_WD_VALUE_LOW(VALUE_LOW)   SimWriteEMIMUX16(VALUE_LOW,\
	             VPC3_STRUC_user_wd_value0)


// -- Reset User-Watchdog -----------------------------------------------------
//#define VPC3_RESET_USER_WD() VPC3->ctrl_reg.wr.mode_reg1_s = VPC3_RES_USER_WD
//-- VPC3_SET_EN_CHK_SSAP -----------------------------------------------------
//#define VPC3_SET_EN_CHK_SSAP() VPC3->ctrl_reg.wr.mode_reg1_s = VPC3_EN_CHK_SSAP
//#define VPC3_RESET_EN_CHK_SSAP() VPC3->ctrl_reg.wr.mode_reg1_r = VPC3_EN_CHK_SSAP

// -- Reset User-Watchdog -----------------------------------------------------
#define VPC3_RESET_USER_WD() SimWriteEMIMUX16(VPC3_RES_USER_WD,\
	             VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)
//-- VPC3_SET_EN_CHK_SSAP -----------------------------------------------------
#define VPC3_SET_EN_CHK_SSAP() SimWriteEMIMUX16(VPC3_EN_CHK_SSAP,\
	             VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)
#define VPC3_RESET_EN_CHK_SSAP() SimWriteEMIMUX16(VPC3_EN_CHK_SSAP,\
	             VPC3_STRUC_ctrl_reg_wr_mode_reg1_r)


// -- read out reasons for indications ( vpc3_isr() ) --------------------------
//#define VPC3_GET_IND_MAC_RESET()        (VPC3->isreg.rd.int_reg1 & VPC3_INT_MAC_RESET)
//#define VPC3_GET_IND_GO_LEAVE_DATA_EX() (VPC3->isreg.rd.int_reg1 & VPC3_INT_GO_LEAVE_DATA_EX)
//#define VPC3_GET_IND_BAUDRATE_DETECT()  (VPC3->isreg.rd.int_reg1 & VPC3_INT_BAUDRATE_DETECT)
//#define VPC3_GET_IND_WD_DP_TIMEOUT()    (VPC3->isreg.rd.int_reg1 & VPC3_INT_WD_DP_TIMEOUT)
//#define VPC3_GET_IND_USER_TIMER_CLOCK() (VPC3->isreg.rd.int_reg1 & VPC3_INT_USER_TIMER_CLOCK)
//#define VPC3_GET_IND_DXB_LINK_ERROR()   (VPC3->isreg.rd.int_reg1 & VPC3_INT_DXB_LINK_ERROR)
//#define VPC3_GET_IND_NEW_EXT_PRM_DATA() (VPC3->isreg.rd.int_reg1 & VPC3_INT_NEW_EXT_PRM_DATA)
//#define VPC3_GET_IND_DXB_OUT()          (VPC3->isreg.rd.int_reg1 & VPC3_INT_DXB_OUT)


#define VPC3_GET_IND_MAC_RESET()        (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1)& VPC3_INT_MAC_RESET)
#define VPC3_GET_IND_GO_LEAVE_DATA_EX() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_GO_LEAVE_DATA_EX)
#define VPC3_GET_IND_BAUDRATE_DETECT()  (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_BAUDRATE_DETECT)
#define VPC3_GET_IND_WD_DP_TIMEOUT()    (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_WD_DP_TIMEOUT)
#define VPC3_GET_IND_USER_TIMER_CLOCK() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_USER_TIMER_CLOCK)
#define VPC3_GET_IND_DXB_LINK_ERROR()   (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_DXB_LINK_ERROR)
#define VPC3_GET_IND_NEW_EXT_PRM_DATA() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_NEW_EXT_PRM_DATA)
#define VPC3_GET_IND_DXB_OUT()          (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg1) & VPC3_INT_DXB_OUT)


//#define VPC3_GET_IND_NEW_GC_COMMAND()   (VPC3->isreg.rd.int_reg2 & VPC3_INT_NEW_GC_COMMAND)
//#define VPC3_GET_IND_NEW_SSA_DATA()     (VPC3->isreg.rd.int_reg2 & VPC3_INT_NEW_SSA_DATA)
//#define VPC3_GET_IND_NEW_CFG_DATA()     (VPC3->isreg.rd.int_reg2 & VPC3_INT_NEW_CFG_DATA)
//#define VPC3_GET_IND_NEW_PRM_DATA()     (VPC3->isreg.rd.int_reg2 & VPC3_INT_NEW_PRM_DATA)
//#define VPC3_GET_IND_DIAG_BUF_CHANGED() (VPC3->isreg.rd.int_reg2 & VPC3_INT_DIAG_BUF_CHANGED)
//#define VPC3_GET_IND_DX_OUT()           (VPC3->isreg.rd.int_reg2 & VPC3_INT_DX_OUT)
//#define VPC3_GET_IND_POLL_END_IND()     (VPC3->isreg.rd.int_reg2 & VPC3_INT_POLL_END_IND)
//#define VPC3_GET_IND_FDL_IND()          (VPC3->isreg.rd.int_reg2 & VPC3_INT_FDL_IND)

#define VPC3_GET_IND_NEW_GC_COMMAND()   (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2)& VPC3_INT_NEW_GC_COMMAND)
#define VPC3_GET_IND_NEW_SSA_DATA()     (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_NEW_SSA_DATA)
#define VPC3_GET_IND_NEW_CFG_DATA()     (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_NEW_CFG_DATA)
#define VPC3_GET_IND_NEW_PRM_DATA()     (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_NEW_PRM_DATA)
#define VPC3_GET_IND_DIAG_BUF_CHANGED() (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_DIAG_BUF_CHANGED)
#define VPC3_GET_IND_DX_OUT()           (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_DX_OUT)
#define VPC3_GET_IND_POLL_END_IND()     (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_POLL_END_IND)
#define VPC3_GET_IND_FDL_IND()          (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_int_reg2) & VPC3_INT_FDL_IND)



// -- Acknowledging the indication -------------------------------------------
//define VPC3_CON_IND_MAC_RESET()        VPC3->isreg.wr.int_ack1 = VPC3_INT_MAC_RESET
//#define VPC3_CON_IND_GO_LEAVE_DATA_EX() VPC3->isreg.wr.int_ack1 = VPC3_INT_GO_LEAVE_DATA_EX
//#define VPC3_CON_IND_BAUDRATE_DETECT()  VPC3->isreg.wr.int_ack1 = VPC3_INT_BAUDRATE_DETECT
//#define VPC3_CON_IND_WD_DP_TIMEOUT()    VPC3->isreg.wr.int_ack1 = VPC3_INT_WD_DP_TIMEOUT
//#define VPC3_CON_IND_USER_TIMER_CLOCK() VPC3->isreg.wr.int_ack1 = VPC3_INT_USER_TIMER_CLOCK
//#define VPC3_CON_IND_DXB_LINK_ERROR()   VPC3->isreg.wr.int_ack1 = VPC3_INT_DXB_LINK_ERROR
//#define VPC3_CON_IND_NEW_EXT_PRM_DATA() VPC3->isreg.wr.int_ack1 = VPC3_INT_NEW_EXT_PRM_DATA
//#define VPC3_CON_IND_DXB_OUT()          VPC3->isreg.wr.int_ack1 = VPC3_INT_DXB_OUT

#define VPC3_CON_IND_MAC_RESET()        SimWriteEMIMUX16(VPC3_INT_MAC_RESET,\
	                 VPC3_STRUC_isreg_wr_int_ack1)                 
#define VPC3_CON_IND_GO_LEAVE_DATA_EX() SimWriteEMIMUX16(VPC3_INT_GO_LEAVE_DATA_EX,\
	                 VPC3_STRUC_isreg_wr_int_ack1) 
#define VPC3_CON_IND_BAUDRATE_DETECT()  SimWriteEMIMUX16(VPC3_INT_BAUDRATE_DETECT,\
	                 VPC3_STRUC_isreg_wr_int_ack1) 	
#define VPC3_CON_IND_WD_DP_TIMEOUT()    SimWriteEMIMUX16(VPC3_INT_WD_DP_TIMEOUT,\
                  VPC3_STRUC_isreg_wr_int_ack1) 
#define VPC3_CON_IND_USER_TIMER_CLOCK() SimWriteEMIMUX16(VPC3_INT_USER_TIMER_CLOCK,\
	                  VPC3_STRUC_isreg_wr_int_ack1)
#define VPC3_CON_IND_DXB_LINK_ERROR()   SimWriteEMIMUX16(VPC3_INT_DXB_LINK_ERROR,\
	                  VPC3_STRUC_isreg_wr_int_ack1)
#define VPC3_CON_IND_NEW_EXT_PRM_DATA() SimWriteEMIMUX16(VPC3_INT_NEW_EXT_PRM_DATA,\
	                  VPC3_STRUC_isreg_wr_int_ack1)
#define VPC3_CON_IND_DXB_OUT()          SimWriteEMIMUX16(VPC3_INT_DXB_OUT,\
	                  VPC3_STRUC_isreg_wr_int_ack1)


//#define VPC3_CON_IND_NEW_GC_COMMAND()   VPC3->isreg.wr.int_ack2 = VPC3_INT_NEW_GC_COMMAND
//#define VPC3_CON_IND_NEW_SSA_DATA()     VPC3->isreg.wr.int_ack2 = VPC3_INT_NEW_SSA_DATA
//#define VPC3_CON_IND_DIAG_BUF_CHANGED() VPC3->isreg.wr.int_ack2 = VPC3_INT_DIAG_BUF_CHANGED
//#define VPC3_CON_IND_DX_OUT()           VPC3->isreg.wr.int_ack2 = VPC3_INT_DX_OUT
//#define VPC3_CON_IND_POLL_END_IND()     VPC3->isreg.wr.int_ack2 = VPC3_INT_POLL_END_IND
//#define VPC3_CON_IND_FDL_IND()          VPC3->isreg.wr.int_ack2 = VPC3_INT_FDL_IND

#define VPC3_CON_IND_NEW_GC_COMMAND()   SimWriteEMIMUX16(VPC3_INT_NEW_GC_COMMAND,\
	                 VPC3_STRUC_isreg_wr_int_ack2)
#define VPC3_CON_IND_NEW_SSA_DATA()     SimWriteEMIMUX16(VPC3_INT_NEW_SSA_DATA,\
		                 VPC3_STRUC_isreg_wr_int_ack2)
#define VPC3_CON_IND_DIAG_BUF_CHANGED() SimWriteEMIMUX16(VPC3_INT_DIAG_BUF_CHANGED,\
		                 VPC3_STRUC_isreg_wr_int_ack2)
#define VPC3_CON_IND_DX_OUT()           SimWriteEMIMUX16(VPC3_INT_DX_OUT,\
		                 VPC3_STRUC_isreg_wr_int_ack2)
#define VPC3_CON_IND_POLL_END_IND()     SimWriteEMIMUX16(VPC3_INT_POLL_END_IND,\
		                 VPC3_STRUC_isreg_wr_int_ack2)
#define VPC3_CON_IND_FDL_IND()          SimWriteEMIMUX16(VPC3_INT_FDL_IND,\
		                 VPC3_STRUC_isreg_wr_int_ack2)



// -- Ending the Indication ---------------------------------------------------
//#define VPC3_SET_EOI() VPC3->ctrl_reg.wr.mode_reg1_s  = VPC3_EOI

#define VPC3_SET_EOI() SimWriteEMIMUX16(VPC3_EOI,\
	                   VPC3_STRUC_ctrl_reg_wr_mode_reg1_s)


// poll read out reasons for indications --------------------------------------
//#define VPC3_POLL_IND_MAC_RESET()           (VPC3->int_req1 & VPC3_INT_MAC_RESET)
//#define VPC3_POLL_IND_BAUDRATE_DETECT()     (VPC3->int_req1 & VPC3_INT_BAUDRATE_DETECT)
//#define VPC3_POLL_IND_USER_TIMER_CLOCK()    (VPC3->int_req1 & VPC3_INT_USER_TIMER_CLOCK)
//#define VPC3_POLL_IND_GO_LEAVE_DATA_EX()    (VPC3->int_req1 & VPC3_INT_GO_LEAVE_DATA_EX)
//#define VPC3_POLL_IND_WD_DP_MODE_TIMEOUT()  (VPC3->int_req1 & VPC3_INT_WD_DP_MODE_TIMEOUT)
//#define VPC3_POLL_IND_NEW_EXT_PRM_DATA()    (VPC3->int_req1 & VPC3_INT_NEW_EXT_PRM_DATA)

#define VPC3_POLL_IND_MAC_RESET()           (SimReadEMIMUX16(VPC3_STRUC_int_req1)& VPC3_INT_MAC_RESET)
#define VPC3_POLL_IND_BAUDRATE_DETECT()     (SimReadEMIMUX16(VPC3_STRUC_int_req1) & VPC3_INT_BAUDRATE_DETECT)
#define VPC3_POLL_IND_USER_TIMER_CLOCK()    (SimReadEMIMUX16(VPC3_STRUC_int_req1) & VPC3_INT_USER_TIMER_CLOCK)
#define VPC3_POLL_IND_GO_LEAVE_DATA_EX()    (SimReadEMIMUX16(VPC3_STRUC_int_req1) & VPC3_INT_GO_LEAVE_DATA_EX)
//#define VPC3_POLL_IND_WD_DP_MODE_TIMEOUT()  (VPC3->int_req1 & VPC3_INT_WD_DP_MODE_TIMEOUT)
#define VPC3_POLL_IND_NEW_EXT_PRM_DATA()    (SimReadEMIMUX16(VPC3_STRUC_int_req1) & VPC3_INT_NEW_EXT_PRM_DATA)


//#define VPC3_POLL_IND_NEW_GC_COMMAND()      (VPC3->int_req2 & VPC3_INT_NEW_GC_COMMAND)
//#define VPC3_POLL_IND_NEW_SSA_DATA()        (VPC3->int_req2 & VPC3_INT_NEW_SSA_DATA)
//#define VPC3_POLL_IND_NEW_CFG_DATA()        (VPC3->int_req2 & VPC3_INT_NEW_CFG_DATA)
//#define VPC3_POLL_IND_NEW_PRM_DATA()        (VPC3->int_req2 & VPC3_INT_NEW_PRM_DATA)
//#define VPC3_POLL_IND_DIAG_BUFFER_CHANGED() (VPC3->int_req2 & VPC3_INT_DIAG_BUF_CHANGED)
//#define VPC3_POLL_IND_DX_OUT()              (VPC3->int_req2 & VPC3_INT_DX_OUT)
//#define VPC3_POLL_IND_POLL_END_IND()        (VPC3->int_req2 & VPC3_INT_POLL_END_IND)
//#define VPC3_POLL_IND_FDL_IND()             (VPC3->int_req2 & VPC3_INT_FDL_IND)

#define VPC3_POLL_IND_NEW_GC_COMMAND()      (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_NEW_GC_COMMAND)
#define VPC3_POLL_IND_NEW_SSA_DATA()        (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_NEW_SSA_DATA)
#define VPC3_POLL_IND_NEW_CFG_DATA()        (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_NEW_CFG_DATA)
#define VPC3_POLL_IND_NEW_PRM_DATA()        (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_NEW_PRM_DATA)
#define VPC3_POLL_IND_DIAG_BUFFER_CHANGED() (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_DIAG_BUF_CHANGED)
#define VPC3_POLL_IND_DX_OUT()              (SimReadEMIMUX16(VPC3_STRUC_int_req2)& VPC3_INT_DX_OUT)
#define VPC3_POLL_IND_POLL_END_IND()        (SimReadEMIMUX16(VPC3_STRUC_int_req2)& VPC3_INT_POLL_END_IND)
#define VPC3_POLL_IND_FDL_IND()             (SimReadEMIMUX16(VPC3_STRUC_int_req2) & VPC3_INT_FDL_IND)



//#define VPC3_SET_IRR_1(IRR_BYTE)            (VPC3->int_req1 |= IRR_BYTE)
//#define VPC3_SET_IRR_2(IRR_BYTE)            (VPC3->int_req2 |= IRR_BYTE)

//#define VPC3_SET_IRR_1(IRR_BYTE)            (VPC3->int_req1 |= IRR_BYTE)
//#define VPC3_SET_IRR_2(IRR_BYTE)            (VPC3->int_req2 |= IRR_BYTE)



//#define DP_LOCK_IND()                                                            \
//{                                                                                \
 //     VPC3->isreg.wr.int_mask_H = 0xFF;                                        \
 //                                                                                \
 //     VPC3->isreg.wr.int_mask_L = 0xFF;                                        \
//}

#define DP_LOCK_IND()                                                            \
{                                                                                \
     SimWriteEMIMUX16(0xFF,VPC3_STRUC_isreg_wr_int_mask_H);                    \
                                                                                 \
     SimWriteEMIMUX16(0xFF,VPC3_STRUC_isreg_wr_int_mask_L);               \
}



//#define DP_UNLOCK_IND()                                                          \
//{                                                                                \
    //  VPC3->isreg.wr.int_mask_H = dp_sys.ind_high;                             \
    //                                                                             \
   //   VPC3->isreg.wr.int_mask_L = dp_sys.ind_low;                              \
//}

#define DP_UNLOCK_IND()                                                          \
{                                                                                \
      SimWriteEMIMUX16(dp_sys.ind_high,VPC3_STRUC_isreg_wr_int_mask_H);         \
                                                                                 \
      SimWriteEMIMUX16(dp_sys.ind_low,VPC3_STRUC_isreg_wr_int_mask_L);          \
}



/*-----------------------------------------------------------------------------------------------------------*/
/* 2.0 parameter telegram                                                                                    */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 2.1 structure of prm data                                                 */
/*---------------------------------------------------------------------------*/

/*
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|      7       |      6     |       5       |           4           |      3     |     2     |         1        |          0        |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|   Lock_Req   | Unlock_Req |   Sync_Req    |       Freeze_Req      |    WD_On   |     0     |         0        |          0        |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| WD_Fact_1 (1 bis 255)                                                                                                             |
|-----------------------------------------------------------------------------------------------------------------------------------|
| WD_Fact_2 (1 bis 255)                                                                                                             |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Min. Station Delay Responder (min Tsdr)                                                                                           |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Ident_Number (high)                                                                                                               |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Ident_Number (low)                                                                                                                |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Group_Ident                                                                                                                       |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| DPV1_Enable  | Fail_Safe  | Publisher_En  |       reserved        |  res       |  WD_Base  | Dis_Stop_Control | Dis_Start_Control |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| En_Pull_Plug | En_Process | En_Diagnostic | En_Manufacturer_Spec. | En_Status  | En_Update |    reserviert    | Chk_Cfg_Mode      |
|    Alarm     |    Alarm   |    Alarm      |         Alarm         |   Alarm    |   Alarm   |                  |                   |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|   PrmCmd     |  reserved  |   reserved    |       IsoM_Req        | Prm_Struct |                 Alarm_Mode                       |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|                                                            User_Prm_Data                                                          |
|-----------------------------------------------------------------------------------------------------------------------------------|
*/




#pragma pack(1)
typedef struct

{
    u8 station_state;
    u8 wd_fact_1;
    u8 wd_fact_2;
    u8 mintsdr;
    u8 pno_ident_high;
    u8 pno_ident_low;
    u8 group_ident;
    u8 dpv1_status_1;
    u8 dpv1_status_2;
    u8 dpv1_status_3;

   
}STRUC_PRM;

#pragma pack()

#define VPC3_STRUC_PRM_PTR  STRUC_PRM*


/*---------------------------------------------------------------------------*/
/* 2.1 literals for Station State                                            */
/*---------------------------------------------------------------------------*/
#define STATION_STATE_WD_ON             ((u8)0x08)

/*---------------------------------------------------------------------------*/
/* 2.2 literals for DPV1 Status 1                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_1_RESERVED_1        ((u8)0x03)
#define DPV1_STATUS_1_WD_BASE_1MS       ((u8)0x04)
#define DPV1_STATUS_1_RESERVED_2        ((u8)0x08)
#define DPV1_STATUS_1_RESERVED_3        ((u8)0x10)
#define DPV1_STATUS_1_PUBLISHER_ENABLE  ((u8)0x20)
#define DPV1_STATUS_1_FAIL_SAFE         ((u8)0x40)
#define DPV1_STATUS_1_DPV1_ENABLE       ((u8)0x80)

/*---------------------------------------------------------------------------*/
/* 2.3 literals for DPV1 Status 2                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_2_CHECK_CFG_MODE    ((u8)0x01)
#define CHECK_CFG                       ((u8)0x00)
#define NO_CHECK_CFG                    ((u8)0x01)
#define DPV1_STATUS_2_RESERVED_4        ((u8)0x02)

#define ALARM_TYPE_UPDATE_VALUE         ((u8)0x04)
#define ALARM_TYPE_STATUS_VALUE         ((u8)0x08)
#define ALARM_TYPE_MANUFACTURER_VALUE   ((u8)0x10)
#define ALARM_TYPE_DIAGNOSTIC_VALUE     ((u8)0x20)
#define ALARM_TYPE_PROCESS_VALUE        ((u8)0x40)
#define ALARM_TYPE_PULLPLUG_VALUE       ((u8)0x80)

#define DPV1_STATUS_2_ALARM_TYPE_MASK   (   ALARM_TYPE_UPDATE_VALUE       \
                                          | ALARM_TYPE_STATUS_VALUE       \
                                          | ALARM_TYPE_MANUFACTURER_VALUE \
                                          | ALARM_TYPE_DIAGNOSTIC_VALUE   \
                                          | ALARM_TYPE_PROCESS_VALUE      \
                                          | ALARM_TYPE_PULLPLUG_VALUE     \
                                        )
#define ALARM_TYPE_NONE_VALUE           ((u8)0x00)
#define ALARM_TYPE_ALL_VALUE            ((u8)0xFF)

/*---------------------------------------------------------------------------*/
/* 2.4 literals for DPV1 Status 3                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_3_ALARM_MODE_MASK   ((u8)0x07)
#define DPV1_STATUS_3_PRM_STRUCTURE     ((u8)0x08)
#define DPV1_STATUS_3_ISOM_REQUEST      ((u8)0x10)
#define DPV1_STATUS_3_RESERVED_UNUSED5  ((u8)0x70)
#define DPV1_STATUS_3_PRM_CMD           ((u8)0x80)

/*---------------------------------------------------------------------------*/
/* 2.5 general defines for prm data                                          */
/*---------------------------------------------------------------------------*/
#define PRM_LEN_NORM                    ((u8)0x07)
#define PRM_LEN_DPV1                    ((u8)0x0A)

/*---------------------------------------------------------------------------*/
/* 2.6 literals for structured prm data                                      */
/*---------------------------------------------------------------------------*/
#define PBC_PRMCMD                      ((u8)0x02)
#define PBC_DXB_LINKTABLE               ((u8)0x03)
#define PBC_ISOCHRON                    ((u8)0x04)
#define PBC_DXB_SUBSCRIBERTABLE         ((u8)0x07)
#define PBC_TIME_AR                     ((u8)0x08)
#define PBC_USER_PRM_DATA               ((u8)0x81)

typedef struct
{
    u8 len;
    u8 command;
    u8 slot_nr;
    u8 index;
    u8 user_data;
}STRUC_PRM_BLOCK;
#define VPC3_STRUC_PRM_BLOCK_PTR  STRUC_PRM_BLOCK PTR_ATTR *

/*---------------------------------------------------------------------------*/
/* 2.7 returncodes prm data                                                  */
/*---------------------------------------------------------------------------*/
#define VPC3_PRM_FINISHED               ((u8)0x00)
#define VPC3_PRM_CONFLICT               ((u8)0x01)
#define VPC3_PRM_NOT_ALLOWED            ((u8)0x03)

/*---------------------------------------------------------------------------*/
/* 2.8 macros for prm data                                                   */
/*---------------------------------------------------------------------------*/
// read length of prm-Data --------------------------------------------------
//#define VPC3_GET_PRM_LEN()  VPC3->len_prm_data
#define VPC3_GET_PRM_LEN()  SimReadEMIMUX16(VPC3_STRUC_len_prm_data)

// get pointer to prm-buffer ------------------------------------------------
//#define VPC3_GET_PRM_BUF_PTR() (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->prm_buf_ptr << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
#define VPC3_GET_PRM_BUF_PTR() (((u16)SimReadEMIMUX16(VPC3_STRUC_prm_buf_ptr)) << SEG_MULDIV)

// acknowledge prm-data -----------------------------------------------------
//#define VPC3_SET_PRM_DATA_OK()      VPC3->ctrl_reg.rd.user_prm_data_ok
//#define VPC3_SET_PRM_DATA_NOT_OK()  VPC3->ctrl_reg.rd.user_prm_data_nok

#define VPC3_SET_PRM_DATA_OK()     SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_user_prm_data_ok)
#define VPC3_SET_PRM_DATA_NOT_OK()  SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_user_prm_data_nok)


/*-----------------------------------------------------------------------------------------------------------*/
/* 3.0 configuration telegram                                                                                */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 3.1 standard format of cfg byte                                           */
/*---------------------------------------------------------------------------*/
//
//        MSB                         LSB
//       +---+---+---+---+---+---+---+---+
//Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//       +---+---+---+---+---+---+---+---+
//         |   |   |   |   |   |   |   |      meaning:
//         |   |   |   |   |   |   |   |
//         |   |   |   |   +---+---+---+----  length of data
//         |   |   |   |                      00 1 byte resp. 1 word
//         |   |   |   |                      .
//         |   |   |   |                      .
//         |   |   |   |                      15 16 byte resp. 16 words
//         |   |   |   |
//         |   |   |   |
//         |   |   +---+--------------------  input/output
//         |   |                              00 specific identifier formats
//         |   |                              01 input
//         |   |                              10 output
//         |   |                              11 input-output
//         |   |
//         |   +----------------------------  length format 1)
//         |                                   0 byte byte structure
//         |                                   1 word word structure
//         |
//         +--------------------------------  consistency over
//                                             0 byte or word
//                                             1 whole length
//

/*---------------------------------------------------------------------------*/
/* 3.2 special format of cfg byte                                            */
/*---------------------------------------------------------------------------*/
//
//        MSB                         LSB
//       +---+---+---+---+---+---+---+---+
//Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//       +---+---+---+---+---+---+---+---+
//         |   |   |   |   |   |   |   |      meaning:
//         |   |   |   |   |   |   |   |
//         |   |   |   |   |   |   |   |
//         |   |   |   |   +---+---+---+----- length of manufacturer
//         |   |   |   |                      specific data 2)
//         |   |   |   |
//         |   |   +---+--------------------- fixed to 00
//         |   |
//         +---+----------------------------- input/output
//                                            00 free place
//                                            01 it follows 1 length byte for inputs
//                                            10 it follows 1 length byte for outputs
//                                            11 it follows - 1 length byte for outputs
//                                                          - 1 length byte for inputs
//
// -----------------------------------------------------------------------------
// 2) The length information of the manufacturer specific data shall be interpreted as follows:
//
// In case of Chk_Cfg:
//
//   0             No manufacturer specific data follow; no data in Real_Cfg_Data.
//   1 to 14       Manufacturer specific data of specified length follow;
//                 these shall be identical with the data in Real_Cfg_Data.
//   15            No manufacturer specific data follow;
//                 the verification can be omitted
//
// In case of DDLM_Get_Cfg :
//
//   0             No manufacturer specific data follow
//   1 to 14       Manufacturer specific data with specified length follow
//   15            Not allowed

// The structure of the length bytes is as follows:
//
//        MSB                         LSB
//       +---+---+---+---+---+---+---+---+
//Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//       +---+---+---+---+---+---+---+---+
//         |   |   |   |   |   |   |   |      meaning:
//         |   |   |   |   |   |   |   |
//         |   |   +---+---+---+---+---+----- length of inputs/outputs
//         |   |                              00 1 byte resp. 1 word
//         |   |                              .
//         |   |                              .
//         |   |                              63 64 byte resp. 64 words
//         |   |
//         |   +----------------------------- Length format 3)
//         |                                   0 byte byte structure
//         |                                   1 word word structure
//         |
//         +--------------------------------- consistency over
//                                             0 byte or word
//                                             1 whole length

/*---------------------------------------------------------------------------*/
/* 3.3 literals for cfg-bytes                                                */
/*---------------------------------------------------------------------------*/
#define VPC3_CFG_IS_BYTE_FORMAT         ((u8)0x30)
#define VPC3_CFG_BF_LENGTH              ((u8)0x0f)
#define VPC3_CFG_LENGTH_IS_WORD_FORMAT  ((u8)0x40)
#define VPC3_CFG_BF_INP_EXIST           ((u8)0x10)
#define VPC3_CFG_BF_OUTP_EXIST          ((u8)0x20)
#define VPC3_CFG_SF_OUTP_EXIST          ((u8)0x80)
#define VPC3_CFG_SF_INP_EXIST           ((u8)0x40)
#define VPC3_CFG_SF_LENGTH              ((u8)0x3f)

/*---------------------------------------------------------------------------*/
/* 3.4 returncodes cfg data                                                  */
/*---------------------------------------------------------------------------*/
#define VPC3_CFG_FINISHED               ((u8)0x00)
#define VPC3_CFG_CONFLICT               ((u8)0x01)
#define VPC3_CFG_NOT_ALLOWED            ((u8)0x03)

/*---------------------------------------------------------------------------*/
/* 3.5 literals for chk config data                                          */
/*---------------------------------------------------------------------------*/
#define VPC3_CFG_OK                     ((u8)0x00)
#define VPC3_CFG_FAULT                  ((u8)0x01)
#define VPC3_CFG_UPDATE                 ((u8)0x02)

/*---------------------------------------------------------------------------*/
/* 3.6 macros for cfg data                                                   */
/*---------------------------------------------------------------------------*/
// read length of cfg-buffers ----------------------------------------------
//#define VPC3_GET_READ_CFG_LEN()   VPC3->len_read_cfg_data
//#define VPC3_GET_CFG_LEN()        VPC3->len_cfg_data

#define VPC3_GET_READ_CFG_LEN()   SimReadEMIMUX16(VPC3_STRUC_len_read_cfg_data)
#define VPC3_GET_CFG_LEN()        SimReadEMIMUX16(VPC3_STRUC_len_cfg_data)

// set length of cfg-data --------------------------------------------------
//#define VPC3_SET_READ_CFG_LEN(LEN)  VPC3->len_read_cfg_data = LEN

#define VPC3_SET_READ_CFG_LEN(LEN)  SimWriteEMIMUX16(LEN,VPC3_STRUC_len_read_cfg_data)

// get pointer to cfg-buffers ----------------------------------------------
//#define VPC3_GET_READ_CFG_BUF_PTR() (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->read_cfg_buf_ptr << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
//#define VPC3_GET_CFG_BUF_PTR()      (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->cfg_buf_ptr << SEG_MULDIV     )+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
#define VPC3_GET_READ_CFG_BUF_PTR() (((u16)SimReadEMIMUX16(VPC3_STRUC_read_cfg_buf_ptr)) << SEG_MULDIV)
#define VPC3_GET_CFG_BUF_PTR()      (((u16)SimReadEMIMUX16(VPC3_STRUC_cfg_buf_ptr)) << SEG_MULDIV)


// acknowledge cfg-data ----------------------------------------------------
//#define VPC3_SET_CFG_DATA_OK()     vpc3_cfg_data_ok()
//#define VPC3_SET_CFG_DATA_UPDATE() vpc3_cfg_data_update()
//#define VPC3_SET_CFG_DATA_NOT_OK() vpc3_cfg_data_not_ok()

#define VPC3_SET_CFG_DATA_OK()     vpc3_cfg_data_ok()
#define VPC3_SET_CFG_DATA_UPDATE() vpc3_cfg_data_update()
#define VPC3_SET_CFG_DATA_NOT_OK() vpc3_cfg_data_not_ok()

/*---------------------------------------------------------------------------*/
/* 3.7 structure for real cfg data                                           */
/*---------------------------------------------------------------------------*/
// -- cfg structure -----------------------------------------------------------
typedef struct
{
    u8 length;
    u8 cfg_data[6];
} CFG_STRUCT;


/*-----------------------------------------------------------------------------------------------------------*/
/* 4.0 input / output                                                                                        */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 4.1 structure for calculated input-/output-length                         */
/*---------------------------------------------------------------------------*/
typedef struct
{
   u8 inp_data_len;               // calculated input length  (VPC3->DP-Master)
   u8 outp_data_len;              // calculated output length (DP-Master->VPC3)
}  VPC3_IO_DATA_LEN;

/*---------------------------------------------------------------------------*/
/* 4.2 states for output buffer                                              */
/*---------------------------------------------------------------------------*/
#define VPC3_NEW_DOUT_BUF               ((u8)0x04)
#define VPC3_DOUT_BUF_CLEARED           ((u8)0x08)

/*---------------------------------------------------------------------------*/
/* 4.3 macros for input buffer                                               */
/*---------------------------------------------------------------------------*/
//#define VPC3_INPUT_UPDATE()             VPC3->ctrl_reg.rd.new_din_buf_cmd
#define VPC3_INPUT_UPDATE()             SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_new_din_buf_cmd)

/*-----------------------------------------------------------------------------------------------------------*/
/* 5.0 set slave address                                                                                     */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 5.1 structure                                                             */
/*---------------------------------------------------------------------------*/
typedef struct
{
    u8 address;
    u8 ident_high;
    u8 ident_low;
    u8 no_add_chg;
    u8 user_data;
}STRUC_SSA_BLOCK;
#define VPC3_STRUC_SSA_BLOCK_PTR  STRUC_SSA_BLOCK*

/*---------------------------------------------------------------------------*/
/* 5.2 macros                                                                */
/*---------------------------------------------------------------------------*/
// -- read length of set-slave-address ----------------------------------------
//#define VPC3_GET_SSA_LEN()  VPC3->len_ssa_buf
#define VPC3_GET_SSA_LEN()  SimReadEMIMUX16(VPC3_STRUC_len_ssa_buf)

// -- get pointer to ssa buffer -----------------------------------------------
//#define VPC3_GET_SSA_BUF_PTR() (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->ssa_buf_ptr << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
#define VPC3_GET_SSA_BUF_PTR() (((u16)SimReadEMIMUX16(VPC3_STRUC_ssa_buf_ptr) )<< SEG_MULDIV)

// -- acknowledge ssa commando ------------------------------------------------
//#define VPC3_FREE_SSA_BUF() VPC3->ctrl_reg.rd.ssa_buf_free_cmd
#define VPC3_FREE_SSA_BUF()  SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_ssa_buf_free_cmd)

/*-----------------------------------------------------------------------------------------------------------*/
/* 6.0 global control telegram                                                                               */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 6.1 defines for GLOBAL CONTROL                                            */
/*---------------------------------------------------------------------------*/
#define GC_OPERATE                      ((u8)0x00)
#define GC_CLEAR                        ((u8)0x02)

/*---------------------------------------------------------------------------*/
/* 6.2 macros for global control                                             */
/*---------------------------------------------------------------------------*/
// -- read Global-Control-Command ---------------------------------------------
//#define VPC3_GET_GC_COMMAND() VPC3->gc_command
#define VPC3_GET_GC_COMMAND() SimReadEMIMUX16(VPC3_STRUC_gc_command)

/*-----------------------------------------------------------------------------------------------------------*/
/* 7.0 diagnostic telegram                                                                                   */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 7.1 defines                                                               */
/*---------------------------------------------------------------------------*/
#define EXT_DIAG_RESET                  ((u8)0x00)
#define EXT_DIAG_SET                    ((u8)0x01)
#define STAT_DIAG_RESET                 ((u8)0x00)
#define STAT_DIAG_SET                   ((u8)0x02)
#define EXT_STAT_DIAG_SET               ((u8)0x03)
#define DIAG_WAIT_FOR_ALARM             ((u8)0x80)

#define DIAG_REV_SIGN                   ((u8)0xC0)
#define DIAG_KEN_SIGN                   ((u8)0x40)
#define DIAG_CHN_SIGN                   ((u8)0x80)
#define DIAG_DEV_SIGN                   ((u8)0x00)

#define DIAG_TYPE_MASK                  ((u8)0xC0)
#define DIAG_TYPE_DEV                   ((u8)0x00)
#define DIAG_TYPE_KEN                   ((u8)0x40)
#define DIAG_TYPE_CHN                   ((u8)0x80)
#define DIAG_TYPE_REV                   ((u8)0xC0)

#define DIAG_NORM_DIAG_SIZE             ((u8)0x06)
#define DIAG_TYPE_CHN_SIZE              ((u8)0x03)
#define DIAG_TYPE_REV_SIZE              ((u8)0x01)

#define STATUS_DIAG_HEAD_SIZE           ((u8)0x04)
#define STATUS_TYPE_SIGN                ((u8)0x80)
#define STATUS_TYPE_STATUS_MESSAGE      ((u8)0x01)
#define STATUS_TYPE_MODUL_STATUS        ((u8)0x02)
#define STATUS_TYPE_PRM_COMMAND_ACK     ((u8)0x1E)
#define STATUS_TYPE_H_STATUS_MESSAGE    ((u8)0x1F)
#define STATUS_TYPE_MANU_MIN            ((u8)0x20)
#define STATUS_TYPE_MANU_MAX            ((u8)0x7E)

#define ALARM_TYPE_SIGN                 ((u8)0x00)
#define ALARM_DIAG_HEAD_SIZE            ((u8)0x04)

/*---------------------------------------------------------------------------*/
/* 7.2 structures                                                            */
/*---------------------------------------------------------------------------*/
// -- revision diagnosis data -------------------------------------------------
typedef struct
{
    u8   sign_revision;
} STRUC_REV_DIAG;

// -- slot signed diagnosis data ----------------------------------------------
typedef struct
{
    u8   sign_len;
    u8   slots[1];
} STRUC_KEN_DIAG;

//defines for channel related diagnostic
//        MSB                         LSB
//       +---+---+---+---+---+---+---+---+
//Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//       +---+---+---+---+---+---+---+---+
//         |   |   |   |   |   |   |   |    meaning:
//         |   |   |   |   |   |   |   |
//         |   |   |   +---+---+---+---+--- error type
//         |   |   |
//         +---+---+----------------------- channel type

//channel type of statuscode
#define STATUS_1BIT                     ((u8)0x20)
#define STATUS_2BIT                     ((u8)0x40)
#define STATUS_4BIT                     ((u8)0x60)
#define STATUS_BYTE                     ((u8)0x80)
#define STATUS_WORD                     ((u8)0xA0)
#define STATUS_DWORD                    ((u8)0xC0)

//error type of statuscode (PROFIBUS NORM)
#define STATUS_SHORT_CIRCUIT            ((u8)0x01)
#define STATUS_UNDERVOLTAGE             ((u8)0x02)
#define STATUS_OVERVOLTAGE              ((u8)0x03)
#define STATUS_OVERLOAD                 ((u8)0x04)
#define STATUS_OVERTEMPERATURE          ((u8)0x05)
#define STATUS_LINE_BREAK               ((u8)0x06)
#define STATUS_UPPER_LIMIT_VALUE        ((u8)0x07)
#define STATUS_LOWER_LIMIT_VALUE        ((u8)0x08)
#define STATUS_ERROR                    ((u8)0x09)
#define STATUS_RESERVED_10              ((u8)0x0A)
#define STATUS_RESERVED_11              ((u8)0x0B)
#define STATUS_RESERVED_12              ((u8)0x0C)
#define STATUS_RESERVED_13              ((u8)0x0D)
#define STATUS_RESERVED_14              ((u8)0x0E)
#define STATUS_RESERVED_15              ((u8)0x0F)

//manufacturer specific between 16 and 31
//Siemens Error Codes
#define STATUS_PRM_ERROR                ((u8)0x10)
#define STATUS_VOLTAGE                  ((u8)0x11)
#define STATUS_FUSE_ERROR               ((u8)0x12)
#define STATUS_GND_ERROR                ((u8)0x13)
#define STATUS_REF_ERROR                ((u8)0x14)
#define STATUS_UNKNOWN_ERROR            ((u8)0x1B)

//defines for channel number in/out
#define CH_NUMBER_INOUT                 ((u8)0xC0)
#define CH_NUMBER_OUT                   ((u8)0x80)
#define CH_NUMBER_IN                    ((u8)0x40)

// -- channel diagnosis data --------------------------------------------------
typedef struct
{
    u8   sign_ident;
    u8   number;
    u8   status_code;
} STRUC_CHN_DIAG;

// -- device diagnosis data ---------------------------------------------------
typedef struct
{
    u8   sign_len;
    u8   user_data[9];
} STRUC_DEV_DIAG;

// -- status message data (coded as device diagnosis (see above) --------------
typedef struct
{
    u8   sign_len;           // equal to DIAG_DEV_SIGN
    u8   status_type;
    u8   slot_number;
    u8   specifier;
    u8   user_data;
} STRUC_STATUS_DIAG;

// alarm data (coded as device diagnosis (see above) --------------------------
typedef struct
{
    u8   sign_len;           // equal to DIAG_DEV_SIGN */
    u8   alarm_type;
    u8   slot_number;
    u8   specifier;
    u8   user_data;
} STRUC_ALARM_DIAG;

typedef union
{
    STRUC_REV_DIAG     rev;
    STRUC_KEN_DIAG     ken;
    STRUC_CHN_DIAG     chn;
    STRUC_DEV_DIAG     dev;
    STRUC_STATUS_DIAG  status;
    STRUC_ALARM_DIAG   alarm;
} UNION_DIAG;
#define VPC3_DIAG_PTR   UNION_DIAG  *

typedef union
{
    VPC3_DIAG_PTR       struc_ptr;
   // VPC3_UNSIGNED8_PTR  byte_ptr;
    u8*               byte_ptr;

} VPC3_UNION_DIAG_PTR;

/*---------------------------------------------------------------------------*/
/* 7.3 macros                                                                */
/*---------------------------------------------------------------------------*/
// get pointer to diag-buffers ----------------------------------------------
//#define VPC3_GET_DIAG1_PTR() (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->diag_buf_ptr[0] << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
//#define VPC3_GET_DIAG2_PTR() (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->diag_buf_ptr[1] << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))

#define VPC3_GET_DIAG1_PTR() (((u16)SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0)) << SEG_MULDIV)
#define VPC3_GET_DIAG2_PTR() (((u16)SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1) )<< SEG_MULDIV)

//-- Control diagnostics buffer ------------

//#define VPC3_GET_DIAG_FLAG()  (VPC3->isreg.rd.status_L & VPC3_DIAG_FLAG)
#define VPC3_GET_DIAG_FLAG()  (SimReadEMIMUX16(VPC3_STRUC_isreg_rd_status_L)& VPC3_DIAG_FLAG)

/*-----------------------------------------------------------------------------------------------------------*/
/* 8.0 subscriber                                                                                            */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 8.1 structures                                                            */
/*---------------------------------------------------------------------------*/
#ifdef DP_SUBSCRIBER
typedef struct
{
    u8       publisher_addr;
    u8       link_status;
} DXB_STATUS;

typedef struct
{
    u8       header;
    u8       status_type;
    u8       slot_number;
    u8       status_specifier;
    DXB_STATUS  link_status[MAX_LINK_SUPPORTED];
} VPC3_DXB_LINK_STATUS;
#define VPC3_DXB_LINK_STATUS_PTR  VPC3_DXB_LINK_STATUS  *

typedef struct
{
    u8       publisher_addr;
    u8       publisher_length;
    u8       sample_offset;
    u8       sample_length;
} DXB_LINK_TABLE;

typedef struct
{
    DXB_LINK_TABLE  link_table_entry[MAX_LINK_SUPPORTED];
} VPC3_DXB_LINK_TABLE;
#define VPC3_DXB_LINK_TABLE_PTR  VPC3_DXB_LINK_TABLE  *

typedef struct
{
    u8       publisher_addr;
    u8       publisher_length;
    u8       sample_offset;
    u8       dest_slot_number;
    u8       offset_data_area;
    u8       sample_length;
} DXB_SUBSCRIBER_TABLE;

typedef struct
{
    u8                   version;
    DXB_LINK_TABLE          link_table_entry[MAX_LINK_SUPPORTED];
} PRM_DXB_LINK_TABLE;
#define PRM_DXB_LINK_TABLE_PTR  PRM_DXB_LINK_TABLE  *

typedef struct
{
    u8                   version;
    DXB_SUBSCRIBER_TABLE    subscriber_table_entry[MAX_LINK_SUPPORTED];
} PRM_SUBSCRIBER_TABLE;
#define PRM_SUBSCRIBER_TABLE_PTR  PRM_SUBSCRIBER_TABLE  *

typedef struct
{
    u8       publisher_addr;
    u8       sample_length;
    u8       sample_data;
} VPC3_DXB_OUT;
#define VPC3_DXB_OUT_PTR  VPC3_DXB_OUT  *
#endif//#ifdef DP_SUBSCRIBER

/*---------------------------------------------------------------------------*/
/* 8.2 macros                                                                */
/*---------------------------------------------------------------------------*/
#define VPC3_GET_DXB_LINK_STATUS_BUF_PTR()  (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->dxb_link_status_buf_ptr << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
#define VPC3_GET_DXB_LINK_STATUS_LEN()      VPC3->len_dxb_link_status_buf
#define VPC3_SET_DXB_LINK_STATUS_LEN(value) VPC3->len_dxb_link_status_buf = value

#define VPC3_GET_DXB_LINK_TABLE_BUF_PTR()   (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->dxb_link_table_buf_ptr << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))
#define VPC3_GET_DXB_LINK_TABLE_LEN()       VPC3->len_dxb_link_table_buf
#define VPC3_SET_DXB_LINK_TABLE_LEN(value)  VPC3->len_dxb_link_table_buf = value


/*-----------------------------------------------------------------------------------------------------------*/
/* 9.0 isochron mode                                                                                         */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 9.1 defines                                                               */
/*---------------------------------------------------------------------------*/
#define PLL_OFFLINE                     ((u8)0x01)
#define PLL_WAIT_SYNCH                  ((u8)0x02)
#define PLL_WAIT_DELAY                  ((u8)0x04)
#define PLL_RUN                         ((u8)0x08)
#define PLL_RUN_PLL                     ((u8)0x10)

/*---------------------------------------------------------------------------*/
/* 9.2 structures                                                            */
/*---------------------------------------------------------------------------*/
typedef struct
{
    u8   T_IMPLICIT;
    u32   T_BASE_DP;
    u16   T_DP;
    u8   T_MAPC;
    u32   T_BASE_IO;
    u16   T_I;
    u16   T_O;
    u32   T_DX;
    u16   T_PLL_W;
    u16   T_PLL_D;
} T_ISOCHRON;

#define sizeof_T_ISOCHRON               ((u8)0x18)

/*-----------------------------------------------------------------------------------------------------------*/
/* 9.3 time stamp                                                                                            */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 9.4 defines                                                               */
/*---------------------------------------------------------------------------*/
// defines for command
#define CS_CM_STOP_CLOCK_SYNC           	((u8)0x01)
#define CS_CM_IGNORE_CYCLIC_STATE_MACHINE	((u8)0x02)
#define CS_CM_CLOCK_VALUE_CHECK_ENABLE      ((u8)0x04)

/*---------------------------------------------------------------------------*/
/* 9.5 structures                                                            */
/*---------------------------------------------------------------------------*/
typedef struct
{
    struct
    {
   		u8	status;
   		u8 	command;
   		u8 	cv_status1;			
   		u8 	cv_status2;
   		u8 	cv_time_event[8];
   		u8 	rd_time[4];			
   		u8 	cv_prev_time_event[8];
    }clock;	
    
	u8 		cs_interval[2];
	u8       cs_delay_interval[8];
} T_TIMESTAMP;

/*-----------------------------------------------------------------------------------------------------------*/
/* 10.0 fdl layer                                                                                            */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 10.1 SAPs (service access points) for MSAC1                               */
/*---------------------------------------------------------------------------*/
#define DP_C1_ALARM_SAP_NR              ((u8)0x32)
#define DP_C1_RD_WR_SAP_NR              ((u8)0x33)

/*---------------------------------------------------------------------------*/
/* 10.2 SAPs (service access points) for MSAC2                               */
/*---------------------------------------------------------------------------*/
#define MSAC_C2_MAX_INPUT_ITEMS         ((u8)((2*DP_C2_NUM_SAPS)+1)) // number of elements in the input queue
#define MSAC_C2_SAP_NR_LOW              ((u8)0x00)                   // lowest possible SAP for MSAC_C2
#define MSAC_C2_SAP_NR_HIGH             ((u8)0x30)                   // highest possible SAP for MSAC_C2 (dp_mode =1)

/*---------------------------------------------------------------------------*/
/* 10.3 structure of a FDL-indication-response-buffer-head                   */
/*---------------------------------------------------------------------------*/
// -- defines for buffer-control-byte ----------------------------------------
#define PCNTRL_USER                     ((u8)0x80)  // 1: buffer in user
#define PCNTRL_IND                      ((u8)0x40)  // 1: indication occured
#define PCNTRL_RESP                     ((u8)0x20)  // 1: response provided
#define PCNTRL_INUSE                    ((u8)0x10)  // 1: buffer in use

// -- structure of a FDL-indication-response-buffer-head ---------------------
typedef struct
{
    u8 cntrl;        // buffer-control
    u8 max_len;      // max ind-buffer-length
    u8 len;          // actual ind-response-length
    u8 fc;           // ind-response-function-code
} FDL_BUF_HEAD;
#define FDL_BUF_HEAD_PTR FDL_BUF_HEAD  *
#define sizeof_FDL_BUF_HEAD             ((u8)0x04)

/*---------------------------------------------------------------------------*/
/* 10.4 structure of a FDL-sap-entry                                         */
/*---------------------------------------------------------------------------*/
typedef struct
{
    u8  resp_s__sap_nr;
    u8  req_sa;
    u8  access__req_ssap;
    u8  event__serv_sup;
    u8  ind_ptr[2];
    u8  resp_ptr;
} FDL_SAP_CNTRL;
#define FDL_SAP_CNTRL_PTR FDL_SAP_CNTRL  *
#define sizeof_FDL_SAP_CNTRL   ((u8)0x07)

/*---------------------------------------------------------------------------*/
/* 10.5 structure of a sap-control-block                                     */
/*---------------------------------------------------------------------------*/
// -- defines for FDL SAP TYPE -----------------------------------------------
typedef enum
{
    FDL_RM_SAP      = ((u8)0x00),
    FDL_MSAC_C1_SAP = ((u8)0x01),
    FDL_MSAC_C2_SAP = ((u8)0x02)
} FDL_SAP_TYPE;

// -- defines for FDL STATE --------------------------------------------------
typedef enum
{
    ENABLED          =  ((u8)0x00),
    DISABLED         =  ((u8)0x01),
    DISABLED_FOR_RM  =  ((u8)0x02),
    DISABLED_IN_RM   =  ((u8)0x03)
} FDL_STATE;

typedef struct
{
    FDL_BUF_HEAD_PTR  ind_ptr[2];       // ind-buffer-address uP-formatted
    FDL_BUF_HEAD_PTR  resp_ptr;         // resp-buffer-address uP-formatted

    FDL_SAP_CNTRL_PTR sap_ptr;          // pointer to VPC3-SAP uP-formatted
    FDL_SAP_CNTRL     sap;              // content of VPC3-sap

    u8             vpc3_ind_ptr[2];  // ind-buffer-address VPC3-formatted
    FDL_STATE         state;

    u8             sap_nr;
    u8             req_ssap;
    FDL_SAP_TYPE      sap_type;
    u8             ind_buf_len;
    u8             resp_buf_len;
    u8             serv_sup;
    u8             req_sa;
} FDL_SAP_HEADER;
#define FDL_SAP_HEADER_PTR  FDL_SAP_HEADER PTR_ATTR *

/*---------------------------------------------------------------------------*/
/* 10.6 structure of a indication- or response-buffer                        */
/*---------------------------------------------------------------------------*/
typedef struct
{
    u8 data_len;       // length of netto-data
    u8 fc;             // function-code
    u8 user_data[246]; // netto-data
} IND_RESP_BUF;
#define IND_RESP_BUF_PTR        IND_RESP_BUF  *
#define MSAC_C2_DATA_BUF        IND_RESP_BUF
#define MSAC_C2_DATA_BUF_PTR    MSAC_C2_DATA_BUF  *

/*---------------------------------------------------------------------------*/
/* 10.7 structure of the immediate-response-pdu of the FDL-RM-SAP            */
/*---------------------------------------------------------------------------*/
typedef struct
{
    u8 opcode;
    u8 sap_nr;
    u8 poll_to_high;
    u8 poll_to_low;
} rm_answer;

/*---------------------------------------------------------------------------*/
/* 10.8 global structure of FDL state machine                                */
/*---------------------------------------------------------------------------*/
// -- defines for resource manager state -------------------------------------
typedef enum
{
    RM_LOCKED       = ((u8)0x01),    // do not treat rm-sap (channel not opened)
    RM_LOADABLE     = ((u8)0x02),    // load rm-sap (immediate response was sent or channel was opened)
    RM_LOADED       = ((u8)0x03)     // do not load rm-sap (immediate-response not sent yet)
} FDL_RM_STATE;

#ifdef DP_FDL
typedef struct
{
    u8                   dpv2_open;
    // saps for use of rm
    FDL_RM_STATE            rm_state;
    rm_answer       *rm_buffer;
    // list for fast sap-finding
    u8                   search_key[0x41]; /* alle SAPs, Index 0x40 = Def.-SAP */
    // sap-list with any sap-information
    FDL_SAP_HEADER          sap_header[FDL_SAP_MAX];
}  FDL_STRUC;
#endif//#ifdef DP_FDL

#define FDL_RM_BUF_LENGTH           ((u8)0x04)
#define FDL_DPV2_SEC_BUF_LENGTH     ((u8)0x04)

/*---------------------------------------------------------------------------*/
/* 10.9 function-codes                                                       */
/*---------------------------------------------------------------------------*/
#define SS_ALL                      ((u8)0x00)
#define SS_SDN_L                    ((u8)0x01)
#define SS_SDN_H                    ((u8)0x02)
#define SS_SDN_LH                   ((u8)0x03)
#define SS_SDN_SRD_LH               ((u8)0x04)
#define SS_SDA_L                    ((u8)0x05)
#define SS_SDA_H                    ((u8)0x06)
#define SS_SDA_LH                   ((u8)0x07)
#define SS_SRD_DDB                  ((u8)0x08)
#define SS_SRD_L                    ((u8)0x09)
#define SS_SRD_H                    ((u8)0x0A)
#define SS_SRD_LH                   ((u8)0x0B)
#define SS_DDB_REQ                  ((u8)0x0C)
#define SS_DDB_RES_L                ((u8)0x0D)
#define SS_DDB_RES_H                ((u8)0x0E)
#define SS_DDB_RES_LH               ((u8)0x0F)

#define FC_RESP_L                   ((u8)0x08)
#define FC_RESP_H                   ((u8)0x0A)

#define DEFAULT_SAP                 ((u8)0x40)
#define SAP_ALL                     ((u8)0x7F)
#define SA_ALL                      ((u8)0x7F)

/*---------------------------------------------------------------------------*/
/* 10.A returncodes of FDL-services                                          */
/*---------------------------------------------------------------------------*/
#define FDL_OK                      ((u8)0x00)
#define FDL_SNI                     ((u8)0x01)
#define FDL_IUSE                    ((u8)0x02)
#define FDL_IVP                     ((u8)0x03)
#define FDL_ACT                     ((u8)0x04)
#define FDL_DACT                    ((u8)0x05)
#define FDL_SND                     ((u8)0x06)
#define FDL_NUIB                    ((u8)0x07)
#define FDL_UBLEN                   ((u8)0x08)
#define FDL_FPTR                    ((u8)0x09)
#define FDL_NRB                     ((u8)0x0A)
#define FDL_NURB                    ((u8)0x0B)
#define FDL_SBNI                    ((u8)0x0C)
#define FDL_RLEN                    ((u8)0x0D)
#define FDL_SBUF                    ((u8)0x0E)
#define FDL_SDA                     ((u8)0x0F)
#define FDL_NRD                     ((u8)0x40)
#define FDL_OPEN_ERR                ((u8)0xAA)

#define FDL_RR                      ((u8)0x10)    // do not change
#define FDL_RS                      ((u8)0x20)    // do not change

#define FDL_PRIMARY_BUF             ((u8)0x01)
#define FDL_SECONDARY_BUF           ((u8)0x02)

/*---------------------------------------------------------------------------*/
/* 10.B helpful macros                                                       */
/*---------------------------------------------------------------------------*/
#define _INIT_FDL_BUF_HEAD(FDL_PTR, LEN)                                \
{                                                                       \
  (FDL_PTR)->cntrl   = 0;                                               \
  (FDL_PTR)->len     = 0;                                               \
  (FDL_PTR)->fc      = 0;                                               \
  (FDL_PTR)->max_len = (LEN);                                           \
}//#define _INIT_FDL_BUF_HEAD(FDL_PTR, LEN)

#define copy_sap_parameter__(_dest, _src)                               \
{                                                                       \
  FDL_SAP_CNTRL  * dest = (_dest);                              \
  FDL_SAP_CNTRL  * src =  (_src);                               \
                                                                        \
  dest->resp_s__sap_nr    =  src->resp_s__sap_nr;                       \
  dest->req_sa            =  src->req_sa;                               \
  dest->access__req_ssap  =  src->access__req_ssap;                     \
  dest->event__serv_sup   =  src->event__serv_sup;                      \
  dest->ind_ptr[0]        =  src->ind_ptr[0];                           \
  dest->ind_ptr[1]        =  src->ind_ptr[1];                           \
  dest->resp_ptr          =  src->resp_ptr;                             \
}//#define copy_sap_parameter__(_dest, _src)

#define incr_vpc3_up_ptr__(_buf_size)                                   \
{                                                                       \
  vpc3_seg_adr += (u8)((_buf_size)>>SEG_MULDIV);                     \
  vpc3_ptr = &vpc3_ptr[(_buf_size)];                                    \
}//#define incr_vpc3_up_ptr__(_buf_size)

#define SWAP_WORD(w) ((((w)&0x00ff)*0x100) + (((w)>>8)&0x00ff))


/*-----------------------------------------------------------------------------------------------------------*/
/* 11.0 DPV1                                                                                                 */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 11.1 return codes for DPV1 services                                       */
/*---------------------------------------------------------------------------*/
typedef enum
{
    DPV1_OK     = ((u8)0x00),      //must be the same as for return no_error
    DPV1_NOK    = ((u8)0x81),      //negative response
    DPV1_DELAY  = ((u8)0x82),      //user give pdu later back
    DPV1_ABORT  = ((u8)0x83),      //user want abort the connection
    DPV1_INIT   = ((u8)0xFF)
} DPV1_RET_VAL;

/*----------------------------------------------------------------------------*/
/* 11.2 errorcodes of DPV1 for ERROR_DECODE                                   */
/*----------------------------------------------------------------------------*/
#define DPV1_ERRDC_DPV1                 ((u8)0x80) // Error_decode, DPV1 fixed value
#define DPV1_ERRDC_FMS                  ((u8)0xFE) // Error_decode, FMS  fixed value
#define DPV1_ERRDC_HART                 ((u8)0xFF) // Error_decode, HART fixed value

/*----------------------------------------------------------------------------*/
/* 11.3 errorcodes of DPV1 for ERROR_CODE_1                                   */
/*----------------------------------------------------------------------------*/
// error_class
#define DPV1_ERRCL_APPLICATION          ((u8)0xA0)
#define DPV1_ERRCL_ACCESS               ((u8)0xB0)
#define DPV1_ERRCL_RESSOURCE            ((u8)0xC0)
#define DPV1_ERRCL_USER                 ((u8)0xD0)
// error_code for  DPV1_ERRCL_APPLICATION
#define DPV1_ERRCL_APP_READ             ((u8)0x00)
#define DPV1_ERRCL_APP_WRITE            ((u8)0x01)
#define DPV1_ERRCL_APP_MODULE           ((u8)0x02)
#define DPV1_ERRCL_APP_VERSION          ((u8)0x08)
#define DPV1_ERRCL_APP_NOTSUPP          ((u8)0x09)
#define DPV1_ERRCL_APP_USER             ((u8)0x0A)
// error_code for  DPV1_ERRCL_ACCESS
#define DPV1_ERRCL_ACC_INV_INDEX        ((u8)0x00)
#define DPV1_ERRCL_ACC_WRITE_LEN        ((u8)0x01)
#define DPV1_ERRCL_ACC_INV_SLOT         ((u8)0x02)
#define DPV1_ERRCL_ACC_TYPE             ((u8)0x03)
#define DPV1_ERRCL_ACC_INV_AREA         ((u8)0x04)
#define DPV1_ERRCL_ACC_STATE            ((u8)0x05)
#define DPV1_ERRCL_ACC_ACCESS           ((u8)0x06)
#define DPV1_ERRCL_ACC_INV_RANGE        ((u8)0x07)
#define DPV1_ERRCL_ACC_INV_PARAM        ((u8)0x08)
#define DPV1_ERRCL_ACC_INV_TYPE         ((u8)0x09)
#define DPV1_ERRCL_ACC_USER             ((u8)0x0A)
// error_code for  DPV1_ERRCL_RESOURCE
#define DPV1_ERRCL_RES_READ_CONSTRAIN   ((u8)0x00)
#define DPV1_ERRCL_RES_WRITE_CONSTRAIN  ((u8)0x01)
#define DPV1_ERRCL_RES_BUSY             ((u8)0x02)
#define DPV1_ERRCL_RES_UNAVAIL          ((u8)0x03)
#define DPV1_ERRCL_RES_USER             ((u8)0x08)
// error_code for  msac_c1_transmit_delay
#define DPV1_NO_VALID_SAP               ((u8)0x01)
#define DPV1_ERR_INV_PARAM              ((u8)0x02)
#define DPV1_ERR_NO_JOB                 ((u8)0x03)

/*----------------------------------------------------------------------------*/
/* 11.4 coding of abort                                                       */
/*----------------------------------------------------------------------------*/
/* subnet */
#define MSAC_C2_SUBNET_NO               ((u8)0x00)
#define MSAC_C2_SUBNET_LOCAL            ((u8)0x01)
#define MSAC_C2_SUBNET_REMOTE           ((u8)0x02)
/* instance */
#define MSAC_C2_INSTANCE_FDL            ((u8)0x00)
#define MSAC_C2_INSTANCE_MSAC_C2        ((u8)0x10)
#define MSAC_C2_INSTANCE_USER           ((u8)0x20)
#define MSAC_C2_INSTANCE_RESERVED       ((u8)0x30)
/* reason */
#define MSAC_C2_ABT_SE                  ((u8)0x01)   // sequence error
#define MSAC_C2_ABT_FE                  ((u8)0x02)   // invalid request PDU received
#define MSAC_C2_ABT_TO                  ((u8)0x03)   // timeout of the connection
#define MSAC_C2_ABT_RE                  ((u8)0x04)   // invalid response PDU received
#define MSAC_C2_ABT_IV                  ((u8)0x05)   // invalid service from USER
#define MSAC_C2_ABT_STO                 ((u8)0x06)   // Send_Timeout requested was too small
#define MSAC_C2_ABT_IA                  ((u8)0x07)   // invalid additional address information
#define MSAC_C2_ABT_OC                  ((u8)0x08)   // waiting for FDL_DATA_REPLY.con
#define MSAC_C2_ABT_RES                 ((u8)0x0F)   // resource error

/*----------------------------------------------------------------------------*/
/* 11.5 Function Codes for DPV1                                               */
/*----------------------------------------------------------------------------*/
#define DPV1_FC_ERROR                   ((u8)0x80)
#define DPV1_FC_INITIATE                ((u8)0x57)
#define DPV1_FC_ABORT                   ((u8)0x58)
#define DPV1_FC_READ                    ((u8)0x5E)
#define DPV1_FC_WRITE                   ((u8)0x5F)
#define DPV1_FC_ALARM_ACK               ((u8)0x5C)
#define DPV1_FC_IDLE                    ((u8)0x48)
#define DPV1_FC_TRANSPORT               ((u8)0x51)
#define DPV1_FC_RM                      ((u8)0x56)
// mask for function code
#define MSAC_C2_FN_MASK                 ((u8)0x1F)
#define MSAC_C2_FN_CONNECT              ((u8)0x17)
#define MSAC_C2_FN_DATA                 ((u8)0x11)
#define MSAC_C2_FN_DISCONNECT           ((u8)0x18)
#define MSAC_C2_FN_IDLE                 ((u8)0x08)
#define MSAC_C2_FN_DS_READ              ((u8)0x1E)
#define MSAC_C2_FN_DS_WRITE             ((u8)0x1F)

/*----------------------------------------------------------------------------*/
/* 11.6 general defines for DPV1                                              */
/*----------------------------------------------------------------------------*/
#define MSAC_C2_TIMEOUT_12MBAUD         ((u16)0x0064)
#define MSAC_C2_TIMEOUT_6MBAUD          ((u16)0x0064)
#define MSAC_C2_TIMEOUT_3MBAUD          ((u16)0x0064)
#define MSAC_C2_TIMEOUT_1_5MBAUD        ((u16)0x0064)
#define MSAC_C2_TIMEOUT_500KBAUD        ((u16)0x00C8)
#define MSAC_C2_TIMEOUT_187_50KBAUD     ((u16)0x0190)

#define MSAC_C2_MIN_POLL_TIMEOUT        ((u16)0x0001)
#define MSAC_C2_MAX_POLL_TIMEOUT        ((u16)0x7FFF)

#define MSAC_C2_MIN_PDU_LEN             ((u8)0x04)
#define MSAC_C2_LEN_DISCONNECT_PDU      ((u8)0x04)
#define MSAC_C2_LEN_IDLE_PDU            ((u8)0x02)
#define MSAC_C2_LEN_RESSOURCE_PDU       ((u8)0x04)
#define MSAC_C2_MIN_CC_PDU_SIZE         ((u8)0x10)
#define MSAC_C2_MAX_PDU_SIZE            ((u8)0xF0)
#define MSAC_C2_MIN_CR_PDU_SIZE         ((u8)0x14)

#define MSAC_C2_MAX_PDU                 ((u8)0xF4)   // PDU with DPV1-Header (4Byte) + 240 User Data
#define MSAC_C2_LEN_INITIATE_RES_HEAD   ((u8)0x0C)   // Initiate response, no addr_data[]
#define DPV1_LEN_NEG_RESPONSE           ((u8)0x04)   // Negative Response PDU
#define DPV1_LEN_HEAD_DATA              ((u8)0x04)   // Fct,Slot,Index,Len

#define MSAC_C2_FUNCTION_NUM_EXTENSION  ((u8)0x40)
#define MSAC_C2_ERROR_FRAME             ((u8)0xC0)
#define DPV1_ERROR_BIT_RESPONSE         ((u8)0x80)

#define MSAC_C2_IND_RECV                ((u8)0x01)   // indication receive
#define MSAC_C2_RESP_SENT               ((u8)0x02)   // response sent
#define MSAC_C2_IND_DISABLE_DONE        ((u8)0x10)   // ind_disable_done

#define MSAC_C2_FC_SRD_L                ((u8)0x0C)
#define VPC3_MSAC_C1_CLOSE_FAILED       ((u8)0x90)

typedef enum
{
    MSAC_C2_RESP_DIRECTION = ((u8)0x01),
    MSAC_C2_REQ_DIRECTION  = ((u8)0x02)
} MSAC2_DIRECTION;

/*----------------------------------------------------------------------------*/
/* 11.7 structures for DPV1                                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    u8 function_num;
    u8 slot_num;
    u8 index;
    u8 length;
    u8 pdu_data[1];
}DPV1_PDU;

typedef struct
{
    u8 function_num;
    u8 slot_num;
    u8 index;
    u8 length;
    u8 pdu_data[1];
}DPV1_READ_PDU;

typedef struct
{
    u8 function_num;
    u8 slot_num;
    u8 index;
    u8 length;
    u8 pdu_data[1];
}DPV1_WRITE_PDU;

typedef struct
{
    u8 function_num;
    u8 err_decode;
    u8 err_code1;
    u8 err_code2;
}DPV1_NEG_RES_PDU;

typedef struct _MSAC_C2_TRANSPORT_PDU
{
    u8  function_num;
    u8  slot_num;
    u8  index;
    u8  length;
    u8  pdu_data[1];
}MSAC_C2_TRANSPORT_PDU;

typedef struct
{
    u8  function_num;
    u8  reserved1;
    u8  reserved2;
    u8  reserved3;
    u16  send_timeout;
    u8  features_supported1;
    u8  features_supported2;
    u8  profile_features_supported1;
    u8  profile_features_supported2;
    u16  profile_ident_nummer;
    u8  s_type;
    u8  s_len;
    u8  d_type;
    u8  d_len;
    u8  addr_data[20];  /* len is defined by S_len or D_len !! */
}MSAC_C2_INITIATE_REQ_PDU;
#define MSAC_C2_INITIATE_REQ_PDU_PTR   MSAC_C2_INITIATE_REQ_PDU  *

typedef struct _MSAC_C2_INITIATE_SUB_PARAM
{
    u8  api;
    u8  reserved;
    u8  net_addr[6];
    u8  mac_addr[1]; /* len is defined by S_len or D_len !! */
}MSAC_C2_INITIATE_SUB_PARAM;

typedef struct _MSAC_C2_INITIATE_RES_PDU
{
    u8  function_num;
    u8  max_len_data_unit;
    u8  features_supported1;
    u8  features_supported2;
    u8  profile_features_supported1;
    u8  profile_features_supported2;
    u16  profile_ident_nummer;
    u8  s_type;
    u8  s_len;
    u8  d_type;
    u8  d_len;
    u8  addr_data[1]; /* len is defined by S_len and D_len !! */
}MSAC_C2_INITIATE_RES_PDU;

typedef struct _MSAC_C2_ABORT_PDU
{
    u8  function_num;
    u8  subnet;
    u8  instance_reason;
    u8  reserved; /* MSAC_C2 + 1 Byte, no additional Detail */
}MSAC_C2_ABORT_PDU;

typedef struct
{
    u8 function_num;
    u8 slot_num;
    u8 alarm_type;
    u8 specifier;
}MSAC_C1_ALARM_ACK_PDU;
#define ALARM_ACK_REQ_PTR MSAC_C1_ALARM_ACK_PDU  *

typedef union
{
    u8                     ubyte_ptr;
    DPV1_READ_PDU             read;
    DPV1_WRITE_PDU            write;
    MSAC_C1_ALARM_ACK_PDU     alarm;
    MSAC_C2_TRANSPORT_PDU     transport;
    MSAC_C2_INITIATE_REQ_PDU  initiate_req;
    MSAC_C2_ABORT_PDU         abort;
    MSAC_C2_INITIATE_RES_PDU  initiate_res;
    DPV1_NEG_RES_PDU          neg;
    DPV1_PDU                  response;
}DPV1_UNION_PDU;

#define DPV1_PTR   DPV1_UNION_PDU  *

typedef struct
{
  u8                       opcode;
  u8                       reserved1;
  u16                       reserved2;
  u16                       poll_time_out;
  u16                       features_supported;
  u16                       profile_features_supported;
  u16                       profile_ident_number;
}MSAC_C2_CR_BUF;
#define MSAC_C2_CR_BUF_PTR            MSAC_C2_CR_BUF  *

typedef struct
{
  u8                       opcode;
  u8                       max_pdu_size;
  u16                       features_supported;
  u16                       profile_features_supported;
  u16                       profile_ident_number;
}MSAC_C2_CC_BUF;
#define MSAC_C2_CC_BUF_PTR            MSAC_C2_CC_BUF  *

typedef struct
{
  u8                       opcode;
  u8                       location;
  u8                       reason_code;
  u8                       reserved;
}MSAC_C2_DR_BUF;
#define MSAC_C2_DR_BUF_PTR            MSAC_C2_DR_BUF  *

typedef struct
{
  u8                       opcode;
  u8                       slot;
  u8                       index;
  u8                       length;
}MSAC_C2_DE_BUF;
#define MSAC_C2_DE_BUF_PTR            MSAC_C2_DE_BUF  *

typedef struct
{
  u8                       opcode;
  u8                       reserved;
}MSAC_C2_WAIT_BUF;
#define MSAC_C2_WAIT_BUF_PTR      MSAC_C2_WAIT_BUF  *

// input queue element
typedef struct
{
  MSAC_C2_DATA_BUF_PTR  buf_ptr;
  u8                 ret_value;
  u8                 sap_nr;
  u8                 fdl_code;
}MSAC_C2_REC_QUEUE;
#define MSAC_C2_REC_QUEUE_PTR    MSAC_C2_REC_QUEUE  *

// reserve queue for ind disable buffer
typedef struct
{
  u8                 data_len;                              // length of netto-data
  u8                 fc;                                    // function-code
  u8                 user_data[MSAC_C2_LEN_DISCONNECT_PDU]; // netto-data
}MSAC_C2_RES_IND_QUEUE;

//defines for MSAC_C2 connection state
typedef enum
{
    MSAC_C2_CS_CLOSE_CHANNEL    = ((u8)0x01),    // initialization state
    MSAC_C2_CS_AWAIT_CONNECT    = ((u8)0x04),    // first state
    MSAC_C2_CS_AWAIT_IND        = ((u8)0x05),    // state after successfull connect
    MSAC_C2_CS_PROVIDE_IND      = ((u8)0x06),
    MSAC_C2_CS_PROVIDE_RESP     = ((u8)0x07),
    MSAC_C2_CS_DISABLE_CONNECT  = ((u8)0x08)
} MSAC2_CONN_STATE;

//defines for MSAC_C2 error codes
typedef enum
{
    MSAC_C2_EC_OK               = ((u8)0x00),    //  without errors
    MSAC_C2_EC_USER_ERR         = ((u8)0x01),    //  invalid response data
    MSAC_C2_EC_IUSE				= ((u8)0x02),	//  inuse bit set //V504
    MSAC_C2_EC_INV_S_D_LEN_ERR  = ((u8)0x03),    //  S,D-len error
    MSAC_C2_EC_REQ_TIMEOUT      = ((u8)0x05),    //  req_time_out
    MSAC_C2_EC_DC_BY_USER       = ((u8)0x0D),    //  user response is disconnect
    MSAC_C2_EC_DC_BY_MASTER     = ((u8)0x0E),    //  master request is disconnect
    MSAC_C2_EC_REMOTE_ERROR     = ((u8)0x10)     //  invalid pdu from the master
} MSAC2_ERROR_CODE;

//defines for MSAC_C2 user_break
typedef enum
{
    MSAC_C2_UB_NO_BREAK         = ((u8)0x00),
    MSAC_C2_UB_BREAK            = ((u8)0x01)
} MSAC2_USER_BREAK;

// MSAC_C2 connections
typedef struct
{
  MSAC_C2_DATA_BUF_PTR  buf_ptr;             /* data_ptr */
  MSAC_C2_DATA_BUF_PTR  buf_ptr_res;         /* reserve data_ptr */
  MSAC_C2_DATA_BUF_PTR  wait_buf_ptr;        /* buffer_ptr for wait_pdu */
  u16                 poll_time_out;       /* poll_time_out for the connection */
  u8                 timer_id;            /* id for used timer */
  u8                 sap_nr;              /* sap_nr FDL */
  u8                 cn_id;               /* connection id */
  u8                 opcode;              /* fc of actual pdu */
  MSAC2_CONN_STATE      state;               /* connection state */
  MSAC2_ERROR_CODE      error_code;
  u8                 dr_reason_code;      /* save reason_code of MSAC_C2_disconnect() */
  u8                 dr_location;         /* save location of MSAC_C2_disconnect() */
  MSAC2_USER_BREAK      user_break;          /* user with disconnect request */
  u8                 timer_break;         /* timeout may be ignored */
  u8                 timer_active;        /* timer has started */
}MSAC_C2_CONNECT_ITEM;
#define MSAC_C2_CONNECT_ITEM_PTR  MSAC_C2_CONNECT_ITEM  *

// -- defines for MSAC2 - Timer -----------------------------------------------------
#define MSAC_C2_TIMER_OK                ((u8)10)
#define MSAC_C2_TIMER_ALREADY_STOPPED   ((u8)20)
#define MSAC_C2_TIMER_ALREADY_RUNS      ((u8)30)

typedef struct
{
  u16  time_act;
  u8  running;
} MSAC_C2_TIMER;

#define MSAC_C1_IND_DS_READ             ((u8)0x01)
#define MSAC_C1_IND_DS_WRITE            ((u8)0x02)
#define MSAC_C1_IND_ALARM_QUIT          ((u8)0x04)
#define MSAC_C1_IND_ALARM_QUIT_SAP50    ((u8)0x08)

/*----------------------------------------------------------------------------*/
/* 11.8 defines for ALARM                                                     */
/*----------------------------------------------------------------------------*/
// max length of alarm-fifo
#define ALARM_MAX_FIFO                  ((u8)0x20)
// defines for Status-Types
#define STATUS_MESSAGE                  ((u8)0x81)
#define MODUL_STATUS                    ((u8)0x82)

#define SET_ALARM_AL_STATE_CLOSED       ((u8)0x01)
#define SET_ALARM_SEQ_NR_ERROR          ((u8)0x02)
#define SET_ALARM_SPECIFIER_ERROR       ((u8)0x03)
#define SET_ALARM_LIMIT_EXPIRED         ((u8)0x04)
#define SET_ALARM_PENDING               ((u8)0x05)
#define SET_ALARM_ALARMTYPE_NOTSUPP     ((u8)0x06)
#define SET_ALARM_OK                    ((u8)0x00)

#define SET_STATUS_NOT_ENABLED          ((u8)0x01)
#define SET_STATUS_OK                   ((u8)0x00)

// defines for alarm-type
#define ALARM_TYPE_MAX                  ((u8)0x07)

#define ALARM_TYPE_DIAGNOSTIC           ((u8)0x01)
#define ALARM_TYPE_PROCESS              ((u8)0x02)
#define ALARM_TYPE_PULL                 ((u8)0x03)
#define ALARM_TYPE_PLUG                 ((u8)0x04)
#define ALARM_TYPE_STATUS               ((u8)0x05)
#define ALARM_TYPE_UPDATE               ((u8)0x06)
#define ALARM_TYPE_MANU_MIN             ((u8)0x20)
#define ALARM_TYPE_MANU_MAX             ((u8)0x7E)

// defines for sequence_mode
#define SEQC_MODE_TOTAL_00              ((u8)0x00)
#define SEQC_MODE_OFF                   ((u8)0x01)
#define SEQC_MODE_TOTAL_02              ((u8)0x02)
#define SEQC_MODE_TOTAL_04              ((u8)0x04)
#define SEQC_MODE_TOTAL_08              ((u8)0x08)
#define SEQC_MODE_TOTAL_12              ((u8)0x0C)
#define SEQC_MODE_TOTAL_16              ((u8)0x10)
#define SEQC_MODE_TOTAL_24              ((u8)0x18)
#define SEQC_MODE_TOTAL_32              ((u8)0x20)

// defines for specifier
#define SPEC_GENERAL                    ((u8)0x00)
#define SPEC_APPEARS                    ((u8)0x01)
#define SPEC_DISAPPEARS                 ((u8)0x02)

#define SPEC_MASK                       ((u8)0x03)
#define SPEC_SEQ_START                  ((u8)0x03)   //use it to shift the sequence number down to bit 0 or up to bit 3
#define MAX_SEQ_NR                      ((u8)0x20)
#define SEQUENCE_NUMBER_ALL             ((u8)0xFF)
#define SEQ_NR_MASK                     ((u8)0xF8)

#define SPEC_ADD_ACK                    ((u8)0x04)

#define szAlarmHeader                   ((u8)0x04)

/*----------------------------------------------------------------------------*/
/* 11.9 structure of ALARM                                                    */
/*----------------------------------------------------------------------------*/
typedef struct
{
  DPL_STRUC_LIST_CB list_head;

  u8                 header;
  u8                 type;
  u8                 slot;
  u8                 spec;
  u8                 user_data_len;
  u8                 callback;
  u8*    user_data_ptr;
}ALARM_STATUS_PDU;
#define ALARM_STATUS_PDU_PTR   ALARM_STATUS_PDU  *

typedef union
{
    DPL_STRUC_LIST_CB_PTR list_ptr;
    ALARM_STATUS_PDU_PTR  data_ptr;

}ALARM_UNION_ALARM;


/*-----------------------------------------------------------------------------------------------------------*/
/* 12.0 global system structure                                                                              */
/*-----------------------------------------------------------------------------------------------------------*/
//defines for MSAC1 Operation Mode
typedef enum
{
    DP_OPMODE_V0        = ((u8)0x00),
    DP_OPMODE_V1        = ((u8)0x01)
} MSAC1_OPMODE;

//defines for MSAC1 Start State
typedef enum
{
    DP_SS_IDLE          = ((u8)0x00),
    DP_SS_STOP          = ((u8)0x01),
    DP_SS_START_AGAIN   = ((u8)0x02),
    DP_SS_RUN           = ((u8)0x03)
} MSAC1_START_STATE;

// -- structures --------------------------------------------------------------
typedef struct
{
    u8               DX_Entered;
    u8               Fdl_Closing;
    MSAC1_OPMODE        Operation_Mode;             // DP/DPV1
    u8               active_Job;
    MSAC1_START_STATE   Start_State;
    u8               event;
} C1_STRUC;

#define AL_STATE_CLOSED                 ((u8)0x10)
#define AL_STATE_OPEN                   ((u8)0x11)

#define AL_TYPE_MAX                     ((u8)0x07)   // dpv1 draft specification - do not use cast !
#define AL_SEQUENCE_MAX                 ((u8)0x20)   // dpv1 draft specification - do not use cast !

//defines for sequence_status
#define AL_SEQUENCE_STATUS_SIZE         ((u8)(((ALARM_TYPE_MAX * MAX_SEQ_NR)+7)/8))

#define AL_ALARM_STATUS_ACTION_SET      ((u8)0x11)
#define AL_ALARM_STATUS_ACTION_CHECK    ((u8)0x49)
#define AL_ALARM_STATUS_ACTION_RESET    ((u8)0x57)

typedef struct
{
    DPL_STRUC_LIST_CB queue_new;
    DPL_STRUC_LIST_CB queue_ack;

    u8       state;
    u8       enabled;    //DPV1_STATUS 2, indicates the type of alarms
    u8       mode;       //DPV1_STATUS 3

    u8       type_status;
    u8       sequence_status[ AL_SEQUENCE_STATUS_SIZE ];

    u8       sequence;   //FALSE: only one alarm of a specific ALARM_TYPE can be active at one time
                            //TRUE : several alarms (2 to 32) of the same or different ALARM_TYPE can be
                            //       active at one time
    u8       limit;      //contains the maximum number of alarms
                            //allowed by the actual master-slave connection
    u8       count;      //contains the number of alarms, which have been sent
} AL_STRUC;

// -- dp system structure -----------------------------------------------------
typedef struct
{
    u8               outp_data_len;              // calculated output length (data from DP-Master to VPC3)
    u8               inp_data_len;               // calculated input length  (data from VPC3 to DP-Master)
    u8               din_bufsize;                // Length of the DIn Buffer (Data Slave to Master)                0..244
    u8               dout_bufsize;               // Length of the DOut Buffer     (Data Master to Slave)           0..244
    u8               prm_bufsize;                // Length of the Parameter Buffer                                 7..244
    u8               diag_bufsize;               // Length of the Diagnosis Buffer                                 6..244
    u8               cfg_bufsize;                // Length of the Configuration Buffer                             1..244
    u8               ssa_bufsize;                // Length of the Input Data in the Set_Slave_Address-Buffer 0 and 4..244

  
    u16     vpc3_used_dp_buf_mem;
    u8               vpc3_con;                   // dummy
    VPC3_UNION_DIAG_PTR diag_buf_ptr;               // pointer to actual diagnosis buffer
    u16    diag_ptr;
} VPC3_SYSTEM_STRUC;
#define VPC3_SYSTEM_STRUC_PTR    VPC3_SYSTEM_STRUC   *

/*---------------------------------------------------------------------------*/
/* 13.0 defines, structure for IM                                            */
/*---------------------------------------------------------------------------*/
// structure for I&M CALL
#define IM_FN_CALL      ((u8)0x08)

#define IM0_INDEX_0     ((u16)0xFDE8)
#define IM0_INDEX_1     ((u16)0xFDE9)
#define IM0_INDEX_2     ((u16)0xFDEA)
#define IM0_INDEX_3     ((u16)0xFDEB)
#define IM0_INDEX_4     ((u16)0xFDEC)

typedef struct
{
    u8   bExtendedFunctionNumber;
    u8   bReserved;
    u16   wIndex;
    u8   abData[1];
} sIMCALL;
#define psIMCALL sIMCALL PTR_ATTR *

//defines for I&M supported
#define IM00_SUPPORTED		((u16)0x0001)
#define IM01_SUPPORTED		((u16)0x0002)
#define IM02_SUPPORTED		((u16)0x0004)
#define IM03_SUPPORTED		((u16)0x0008)
#define IM04_SUPPORTED		((u16)0x0010)
#define IM05_SUPPORTED		((u16)0x0020)
#define IM06_SUPPORTED		((u16)0x0040)
#define IM07_SUPPORTED		((u16)0x0080)
#define IM08_SUPPORTED		((u16)0x0100)
#define IM09_SUPPORTED		((u16)0x0200)
#define IM10_SUPPORTED		((u16)0x0400)
#define IM11_SUPPORTED		((u16)0x0800)
#define IM12_SUPPORTED		((u16)0x1000)
#define IM13_SUPPORTED		((u16)0x2000)
#define IM14_SUPPORTED		((u16)0x4000)
#define IM15_SUPPORTED		((u16)0x8000)

// structure for I&M0 (mandatory)
typedef struct
{
    u8   abHeader[10];
    u16   wManufacturerID;
    u8   abOrderID[20];
    u8   abSerialNumber[16];
    u16   wHardwareRevision;
    u8   abSoftwareRevision[4];
    u16   wRevCounter;
    u16   wProfileID;
    u16   wProfileSpecificType;
    u8   abIMVersion[2];
    u16   wIMSupported;
} sIM0;

#define SizeOfIM0 ((u8)0x40)

// structure for I&M1 (optional)
typedef struct
{
    u8   abHeader[10];
    u8   abTagFunction[32];
    u8   abTagLocation[22];
} sIM1;

#define SizeOfIM1 ((u8)0x40)

// structure for I&M2 (optional)
typedef struct
{
    u8   abHeader[10];
    u8   abInstallationDate[16];
    u8   abReserved[38];
} sIM2;

#define SizeOfIM2 ((u8)0x40)

// structure for I&M3 (optional)
typedef struct
{
    u8   abHeader[10];
    u8   abDescriptor[54];
} sIM3;

#define SizeOfIM3 ((u8)0x40)

// structure for I&M4 (optional)
typedef struct
{
    u8   abHeader[10];
    u8   abSignature[54];
} sIM4;

#define SizeOfIM4 ((u8)0x40)

/*---------------------------------------------------------------------------*/
/* 14.0 defines, structure for sDP_VERSION                                   */
/*---------------------------------------------------------------------------*/
#define DP_COMP_INSTALLED_MSAC1_IFA ((u16)0x0001)
#define DP_COMP_INSTALLED_SUB_AL    ((u16)0x0002)
#define DP_COMP_INSTALLED_SUB_AL_50 ((u16)0x0004)

#define DP_COMP_INSTALLED_MSAC2_IFA ((u16)0x0100)

#define DP_COMP_INSTALLED_IM        ((u16)0x0200)

#define DP_COMP_INSTALLED_VPC3_B    ((u16)0x1000)
#define DP_COMP_INSTALLED_VPC3_C    ((u16)0x2000)
#define DP_COMP_INSTALLED_VPC3_D    ((u16)0x4000)

typedef struct
{
    u16 wComponentsInstalled;

    u8 bMainInterface;
    u8 bFunction;
    u8 bBugfix;
} sDP_VERSION;
#define psDP_VERSION sDP_VERSION  VPC3_PTR

/*-----------------------------------------------------------------------------------------------------------*/
/* 15.0 ERROR defines                                                                                        */
/*-----------------------------------------------------------------------------------------------------------*/
typedef enum
{
    DP_FATAL_ERROR                  = 0x00, // fatal error

    DP_OK                           = 0x01, // OK
    DP_NOK                          = 0x02, // OK

    DP_NOT_OFFLINE_ERROR            = 0x10, // VPC3 is not in OFFLINE state
    DP_ADDRESS_ERROR                = 0x11, // Slave Address Error
    DP_CALCULATE_IO_ERROR           = 0x12,

    DP_DOUT_LEN_ERROR               = 0x13,
    DP_DIN_LEN_ERROR                = 0x14,
    DP_DIAG_LEN_ERROR               = 0x15,
    DP_PRM_LEN_ERROR                = 0x16,
    DP_SSA_LEN_ERROR                = 0x17,
    DP_CFG_LEN_ERROR                = 0x18,
    DP_LESS_MEM_ERROR               = 0x19,
    DP_LESS_MEM_FDL_ERROR           = 0x1A,

    DP_PRM_ENTRY_ERROR              = 0x20,
    DP_PRM_DXB_MAX_LINK_ERROR       = 0x21,
    DP_PRM_DXB_ERROR                = 0x22,
    DP_CFG_ENTRY_ERROR              = 0x23,
    DP_CFG_UPDATE_ERROR             = 0x24,
    DP_PRM_DPV1_STATUS              = 0x25,
    DP_PRM_BLOCK_ERROR              = 0x26,
    DP_PRM_ALARM_ERROR              = 0x27,
    DP_PRMCMD_LEN_ERROR             = 0x28,
    DP_PRM_SOLL_IST_ERROR           = 0x29,
    DP_PRM_DXB_WD_ERROR             = 0x2A,

    DP_DIAG_BUFFER_ERROR            = 0x30,
    DP_DIAG_SEQUENCE_ERROR          = 0x31,
    DP_DIAG_OLD_DIAG_NOT_SEND_ERROR = 0x32,
    DP_DIAG_NOT_POSSIBLE_ERROR      = 0x33,
    DP_DIAG_NO_BUFFER_ERROR         = 0x34,
    DP_DIAG_BUFFER_LENGTH_ERROR     = 0x35,
    DP_DIAG_CONTROL_BYTE_ERROR      = 0x36,

    C2_DATA_LEN_ERROR               = 0x80,
    C2_DATA_POLL_TIMEOUT_ERROR      = 0x81,
    C2_DATA_SAP_ERROR               = 0x82,
    C2_NO_CONN_RESOURCE             = 0x83,
    C2_INV_LOWER_LAYER              = 0x84,
    C2_ENABLED_ERROR                = 0x85,
    C2_RESOURCE_ERROR               = 0x86,
    C2_INV_CN_ID                    = 0x87,
    C2_USER_ERR                     = 0x88,

    SSC_MAX_DATA_PER_LINK           = 0x90,

    DP_EEPROM_ERROR                 = 0xF1, // Hardware errors
    DP_VPC3_ERROR                   = 0xF4,
    DP_SRAM_ERROR                   = 0xFF

}   DP_ERROR_CODE;

typedef enum
{
    _DP_USER    = 0x10,
    _DP_IF      = 0x20,
    _DP_ISR     = 0x30,
    _DP_FDL     = 0x40,
    _DP_C1      = 0x50,
    _DP_C2      = 0x60

}   DP_ERROR_FILE;


typedef struct
{
    u8                   function;
    u8                   error_code;
    u16                   detail;
    u8                   cn_id;
}   VPC3_STRUC_ERRCB;
#define VPC3_ERRCB_PTR   VPC3_STRUC_ERRCB  *
extern VPC3_STRUC_ERRCB   vpc3_errcb;


/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DP_IF.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2001. Confidential.                         */
/*****************************************************************************/
