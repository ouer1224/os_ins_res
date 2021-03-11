/**************************  Filename: dp_user.c  ****************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstra遝 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Tel: ++49-9132-744-200  */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax: ++49-9132-744-204  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:      Demo for PROFICHIP Extension Board AT89C5132               */
/*                This example simulates a modular profibus device           */
/*                with max 6 Modules.                                       */
/*                                                                           */
/*                Module  1: dip-switch PORT1                                */
/*                Module  2: dip-switch PORT2                                */
/*                Module  3: led PORT1                                       */
/*                Module  4: led PORT2                                       */
/*                Module  5: dio32                                           */
/*                Module  6: counter                                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Hardware requirements: ProfiChip Evaluation Board AT89C5132  (PA006101)   */
/*                        ProfiChip Evaluation Board VPC3+/C    (PA006103)   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/* memory:  0000H ... 7FFFH: RAM                                             */
/*          8000H ... 8FFFH: VPC3+                                           */
/*          9000H ... 9FFFH: Reserved                                        */
/*          A000H ... AFFFH: FPGA                                            */
/*          B000H ... BFFFH: RTC                                             */
/*          C000H ... CFFFH: LCD                                             */
/*          D000H ... DFFFH: I/O Port 0                                      */
/*          E000H ... EFFFH: I/O Port 1                                      */
/*          F000H ... FFFFH: I/O Port 2                                      */
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

  - function prototypes
  - data structures
  - internal functions

*/
/*****************************************************************************/
/* include hierarchy */

#include "Hardwaredef.h"
#include "dp_inc.h"
//#include "debug.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
// -- defines for user alarm state
#define USER_AL_STATE_CLOSED        ((u8)0x00)
#define USER_AL_STATE_OPEN          ((u8)0x01)
// -- defines for user state
#define USER_STATE_CLEAR            ((u8)0x00)
#define USER_STATE_RUN              ((u8)0x01)
// -- defines for diagnostics
#define USER_TYPE_DPV0              ((u8)0xFA)
#define USER_TYPE_PRM_OK            ((u8)0xFB)
#define USER_TYPE_PRM_NOK           ((u8)0xFC)
#define USER_TYPE_CFG_OK            ((u8)0xFD)
#define USER_TYPE_CFG_NOK           ((u8)0xFE)
#define USER_TYPE_APPL_RDY          ((u8)0xFF)

// -- status message data (coded as device diagnosis) -------------------------
// -- Modulstatus -------------------------------------------------------------
typedef struct
{
    u8   sign_len;       // fix to 0x07
    u8   status_type;    // fix to 0x82
    u8   slot_number;    // fix to 0x00
    u8   specifier;      // fix to 0x00
    u8   user_data[2];
} tStatusDiag;
#define sizeof_StatusDiag 6

tStatusDiag status_diag;

const u8 DefStatusDiag[] = { 0x06, 0x82, 0x00, 0x00, 0xAA, 0x0A };

#define szUsrDiagnosticAlarmData    ((u8)0x05)
#define szDiagnosticAlarm           (szAlarmHeader + szUsrDiagnosticAlarmData)

#define MaxAlarmLength  ((u8)szUsrDiagnosticAlarmData)

typedef struct
{
    u8                       state;
    u8                       user_diag_active;
    u16                       old_diag;
    u8                       user_diag[60];

    u16                       event;
    u8                       application_ready;
    CFG_STRUCT                  real_cfg;   //real cfg data


}USER_STRUC;

extern ModPrmData   ModulePrm;
extern u8  UOMonFlag,SCAlarmFlag;
extern u8  ChannelStatus[2];
extern u8  ChConfigStatus[2];
extern u16  InputLength,OutputLength;
extern u16  IOwithDiag;
extern STRUC_PRM    PrmHeader;


extern u16  UOStatus,DispUOStatus;
extern u16  UIStatus,DispUIStatus;
extern u8 DispSCAlarmFlag;
extern u8 Txcounter,Rxcounter;
extern u32 SysTick_count;
extern u32  LastTickCount;
extern u8 TxBuffer[20];
enum{DIAGLENGTH,DIAGALARM,DIAGOPSTATUS,DIAGENCODERTYPE,DIAGSREVHHI,DIAGSREVHLO,
	DIAGSREVLHI,DIAGSREVLLO,DIAGMULTIHI,DIAGMULTILO,DIAGADALARM}Diagbufferlocation;

extern u8  DispCHStatus[POFIBUS_BYTE];
extern u8 pofibus_buff_in[POFIBUS_BYTE];
extern u8 pofibus_buff_out[POFIBUS_BYTE];
extern u8 profibu_read_bit;

 u8   gEventFlag=0xff;
 u32  gData;
 extern u8  PAddress;
 #define   BAUDDETECTFLAG    0x1
 #define   PRMFINISHEDFLAG   0x2
 #define   CFGFINISHEDFLAG    0x3
 #define   OUTPUTFLAG       0x4
 #define   CFGERRORFLAG     0x5
 #define   GOLEAVEFLAG      0x6
 

 #define   NOTHING                    0xff
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/
VPC3_STRUC_ERRCB                vpc3_errcb; // error structure
USER_STRUC                      user;       // user system structure
extern VPC3_SYSTEM_STRUC               dp_sys;     // global system structure

/*---------------------------------------------------------------------------*/
/* defines, structures and variables for our demo application                */
/*---------------------------------------------------------------------------*/

typedef struct
{
   u32   Input;
   u32   Output;
} STRUC_SYSTEM;

STRUC_SYSTEM sSystem;

CFG_STRUCT   cfg;
u8 SetZeroFlag;
u32 OldOutputValue;
u16       gAddTime=0;

 u8 Diagbuffer[60];
 u8 VPC3RDFlag;


u8 ConvertData(u8*   invalue);
void Delay(__IO uint32_t nTime);
u16 CheckInput(void);
void ReadChannelIn(u8* invalue);
u16 BuildDiagBuffer(void);
u16 CheckDiag(void);
void SetModuleLed(void);
u8     SimReadEMIMUX16(u16 address);
void  SimWriteEMIMUX16(u8 value, u16 address);
u32   CalCurTimeGap(u32 BeginTime);
extern void delay_us(u32 nTime);

//extern unsigned long  ScaleFactor;

//#define  DEBUGSIM

extern u8 PAddress;
extern u8 bReadFlag;



extern u8 OutSCAlarmFlag,OutUIStatus,OutUOStatus;



u32  CntTimer=0;
//#define PROFIBUSDEBUG 1
u8 AlarmSeq=0;
void ReadPrmData(void);

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

u8   AlarmFlag,DispAlarmFlag;
/*---------------------------------------------------------------------------*/
/* function: user_alarm ( is also called from alarm state machine !!!! )     */
/*---------------------------------------------------------------------------*/
u8 user_alarm( u8 alarm_type, u8 seq_nr, ALARM_STATUS_PDU_PTR alarm_ptr, u8 check_diag_flag )
{


VPC3_UNION_DIAG_PTR tmp_diag;
u8               ret_value;
u8               ext_diag;
u8               len_diag;
u8               error;
u16               diag;

    ret_value = 0x00;
   // return  0;
    diag = ( (u16)(alarm_type << 8) | ((u16)seq_nr) );

    //don't send diagnostic twice!
    if(    ( diag  != user.old_diag         )
        && ( FALSE == user.user_diag_active )
      )
    {
      
       tmp_diag.byte_ptr = user.user_diag;

        ext_diag = 0x00;
        len_diag = 0x00;

        switch( alarm_type )
        {
            case USER_TYPE_CFG_OK:
            {
               
		            
               ext_diag = STAT_DIAG_SET;
 
               			
                break;
            }//case USER_TYPE_CFG_OK:

            case USER_TYPE_CFG_NOK:
            {
                //modul_status diagnose
       


                ext_diag = 0;
                len_diag = 0;
                break;
            }//case USER_TYPE_APPL_RDY:

            case USER_TYPE_DPV0:      
                //ext_diag = STAT_DIAG_SET;                 
			 
					
                break;
            

            case USER_TYPE_APPL_RDY:            	

				 len_diag=0;
				 ext_diag=0;

				 break;
            case USER_TYPE_PRM_NOK:
            case USER_TYPE_PRM_OK:
            default:
            {
                ext_diag = 0;
				
				len_diag = 0;
                break;
            }//default:
        }//switch(user[vpc3_channel].diag_byte)

       // ext_diag = 0x00;
        error = set_diagnosis( tmp_diag, len_diag, ext_diag, check_diag_flag  );
       // print_string("DIAG=");
      //  print_hexbyte(len_diag);
       // start_serial_communication();   
      //  WaitSerialPort();


        if( error == DP_OK )
        {
            user.old_diag = diag;

            if( alarm_type >= USER_TYPE_PRM_OK )
            {
                user.user_diag_active = FALSE;
            }//if( alarm_type >= USER_TYPE_PRM_OK )
            ret_value = DP_OK;
        }//if( error == DP_OK )
        else
        {
            user.user_diag_active = FALSE;           


            ret_value = error;
        }//else of if( error == DP_OK )
    }//if(     ( diag  != user.old_diag         ) ...

    return ret_value;
}//u8 user_alarm( u8 diag_event, u8 crc_byte, ALARM_STATUS_PDU_PTR alarm_ptr )





/*--------------------------------------------------------------------------*/
/* function: read_input_data       	//输出数据32字节                                         */
/*--------------------------------------------------------------------------*/
void read_input_data( void )
{
  u16  input_buf_ptr;  // pointer to input buffer

    // write DIN data to VPC3
 u16 i;
    input_buf_ptr=vpc3_get_dinbufptr ();
	//输出48字节
    if(input_buf_ptr )            // get pointer to DIN data
    {	
	    for(i=0;i<POFIBUS_BYTE;i++)
		{
           SimWriteEMIMUX16(DispCHStatus[i],input_buf_ptr);//	  
		   input_buf_ptr++;
		}
        // the user makes a new Din-Buffer available in the state N
        dp_sys.vpc3_con = VPC3_INPUT_UPDATE(); 

    }//if(input_buf_ptr = vpc3_get_dinbufptr ())
}//void read_input_data( void )

void InitOutput(void)
{
   u16 value,i;
   u8 temp;


    temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_dout_buffer_sm);
  //  switch (VPC3->ctrl_reg.rd.dout_buffer_sm & 0x30)    // locate user Dout Buffer
 
    switch(temp&0x30)
    {
        case 0x10:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr0);
        	   value=value<<SEG_MULDIV;
           // p_doutbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->dout_buf_ptr[0] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        case 0x20:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr1);
        	   value=value<<SEG_MULDIV;        	
           // p_doutbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->dout_buf_ptr[1] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        case 0x30:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr2);
        	   value=value<<SEG_MULDIV;        	
           // p_doutbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->dout_buf_ptr[2] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        default:
        {
        	  value=0;
           // p_doutbuf = NULL_PTR;
            break;
        }
    }//switch (p_Vpc3->ctrl_reg.rd.dout_buffer_sm & 0x30)    // locate user Dout Buffer
    for(i=0;i<4;i++)
    {
        SimWriteEMIMUX16(0,value);
		value++;
    }
	//SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_next_dout_buf_cmd);

}


/*--------------------------------------------------------------------------*/
/* function: application_ready                                              */
/*--------------------------------------------------------------------------*/
void application_ready( void )
{
    //todo
    //make here your own initialization

    //read input data
    CheckInput();
    read_input_data();
    //reset Diag.Stat  USER_TYPE_CFG_OK  USER_TYPE_APPL_RDY
    user_alarm( USER_TYPE_APPL_RDY, 0, (ALARM_STATUS_PDU_PTR) 0, FALSE );
    user.application_ready = TRUE;
    user.state=USER_STATE_RUN;
	bReadFlag=0;
//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}//void application_ready( void )

/*---------------------------------------------------------------------------*/
/* function: user_main                                                       */
/*---------------------------------------------------------------------------*/
u8 ClearWDFlag =1;
u32 WDBegTick;
static u8 pofibus_in[POFIBUS_BYTE];
void user_main( void )
{

DP_ERROR_CODE       error;
u8               outp_state;     // state of output data
u8               ReadInValue[4];
u8               diaglength;
u16 			 output_buf_ptr;
u8 				 i;
static u8 s=0;
static u32 ERROR_CUNT=0;
    /*-----------------------------------------------------------------------*/
    /* init user data                                                        */
    /*-----------------------------------------------------------------------*/
    memset( &user, 0, sizeof(user) );

    /* TRUE  deactivates diagnosis handling ! */
    /* FALSE activates   diagnosis handling ! */
    user.user_diag_active  = FALSE;
    user.application_ready = FALSE;
    user.state = USER_STATE_CLEAR;

    /*-----------------------------------------------------------------------*/
    /* init System                                                           */
    /*-----------------------------------------------------------------------*/
    memset( &sSystem, 0, sizeof(STRUC_SYSTEM) );

    /*-----------------------------------------------------------------------*/
    /* initialize VPC3                                                       */
    /*-----------------------------------------------------------------------*/

    //todo:
    //insert your real configuration data here
     
   DISVPRESET
   LastTickCount=SysTick_count;
   while(1)
   {
      if(CalCurTimeGap(LastTickCount)>100)
	  	break;
   }
    cfg.length=3;cfg.cfg_data[0]=0xc0 ;
	cfg.cfg_data[1]=0xdf,cfg.cfg_data[2]=0xdf;
	__disable_irq();   
	//读取地址 	   //	PAddress=10;   	ReadSW();
	//地址设定
	PAddress = read_hc165();	
    error = vpc3_initialization(PAddress, cfg );				  // address of slave
    if( DP_OK == error )
    {
        // set microcontroller interrupt to falling edge
    //    SET_FALLING_EDGE__;
        // enable microcontroller interrupt
       // ENABLE_VPC3_INT__;
       __enable_irq(); 

        //todo: before startup the VPC3+, make here your own initialzations
        VPC3RDFlag=1;
        START_VPC3();
    }
    else
    {
        VPC3RDFlag=0;
        vpc3_errcb.error_code = error;
     //   fatal_error( _DP_USER, __LINE__, &vpc3_errcb );
    }//if( DP_OK == error )
   
    Diagbuffer[DIAGLENGTH]=0xa;
	Diagbuffer[DIAGOPSTATUS]=0x2;  
	user_alarm( USER_TYPE_DPV0, 0, (ALARM_STATUS_PDU_PTR) 0, FALSE );

    /*-----------------------------------------------------------------------*/
    /* endless demo operation loop                                           */
    /*-----------------------------------------------------------------------*/

	read_input_data();
	LastTickCount=SysTick_count;

	__enable_irq();	  //中断使能
	start_stop_tim2(ON); //定时器打开
    while( 1 )
    {

		Power_check();				  	//电源检测
		sys_proc();						//系统运行
        /*-------------------------------------------------------------------*/
        /* trigger watchdogs                                                 */
        /*-------------------------------------------------------------------*/
        // toggle user watchdog
        VPC3_RESET_USER_WD();   // toggle user watchdog
        /*-------------------------------------------------------------------*/
        /* internal state machine                                            */
        /*-------------------------------------------------------------------*/
        if( user.state == USER_STATE_CLEAR )
        {
            // clear data
            sSystem.Input=0;
            sSystem.Output=0;
	        user_alarm( USER_TYPE_APPL_RDY, 0, (ALARM_STATUS_PDU_PTR) 0, FALSE );
			InitOutput();
            user.state = USER_STATE_RUN;
        }//if( user.state == USER_STATE_CLEAR )
        /*-------------------------------------------------------------------*/
        /* dpv1 statemachines                                                */
        /*-------------------------------------------------------------------*/
  


        /*-------------------------------------------------------------------*/
        /* VPC3+ DP-state                                                    */
        /*-------------------------------------------------------------------*/
        switch(VPC3_GET_DP_STATE())
        {
            case WAIT_PRM:
            case WAIT_CFG:	   	
						ERROR_CUNT++;
						if(ERROR_CUNT>15000)while(1);		 //	自己加入防止掉网
            case DP_ERROR:
            {
                if( user.event & VPC3_EV_DX_OUT )			 //PLC传数据标志			 
                {
                    user.event &= ~VPC3_EV_DX_OUT;       // clear event
                }
				
                break;
            }
            case DATA_EX:
            {
			  if(CheckDiag())			
			  {
				  AlarmSeq++;
				  AlarmSeq=AlarmSeq&(MAX_SEQ_NR - 1);			  
			  }  
			 user_alarm(USER_TYPE_DPV0, AlarmSeq, (ALARM_STATUS_PDU_PTR) 0,TRUE);
              if( user.event & VPC3_EV_DX_OUT )			//接收数据
              {
            	
				user.event &= ~VPC3_EV_DX_OUT;       // clear event
				output_buf_ptr = vpc3_get_doutbufptr (&outp_state);

				for(i=0;i<POFIBUS_BYTE;i++)									   //接受32个数据
				{
					pofibus_buff_in[i] = SimReadEMIMUX16(output_buf_ptr);
					output_buf_ptr++ ;
				}	 
				/* 	*/

				for(i=0;i<POFIBUS_BYTE;i++)
				{
					if(pofibus_buff_in[i]!=	pofibus_in[i])
					{   	
						pofibus_in[i]=	pofibus_buff_in[i] ;
						profibu_read_bit = 1;
					}
				}  
				
              }
			  if(CheckInput())
			  	 user.event=user.event|VPC3_EV_NEW_INPUT_DATA;
              if( user.event & VPC3_EV_NEW_INPUT_DATA )
              {
                     read_input_data();						  //写
                     user.event &= ~VPC3_EV_NEW_INPUT_DATA;
              }
                break;
                //case DATA_EX:
            }
            default:
            {
                vpc3_errcb.error_code = VPC3_GET_DP_STATE();
                break;
            }
        }//switch(VPC3_GET_DP_STATE())
        if( user.event & VPC3_EV_NEW_CFG_DATA )
        {
        //  Delay(10000);
            application_ready();
         //   print_string("ready");
         //  start_serial_communication();
            user.event &= ~VPC3_EV_NEW_CFG_DATA;    // clear event
        }
        //Delay(1);
    }//while(1)
}//void user_main( void )

/*---------------------------------------------------------------------------*/
/* function: delay                                                           */
/*---------------------------------------------------------------------------*/
void delay( void )
{
u8 i,j;

    for(i = 0; i < 255; i++)
    {
        for(j = 0; j < 255; j++);
    }
}//void delay( void )




/*---------------------------------------------------------------------------*/
/* function: user_isr_new_prm_data                                           */
/*                                                                           */
/* todo  : check the prm-data                                                */
/* return: DP_OK  - prm-data OK                                              */
/*         DP_NOK - prm-data not OK                                          */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )
u8 user_chk_new_prm_data( u16 prm_ptr, u8 len )
{
typedef struct
{
    u8 len;
    u8 slot_nr;
    u8 user_data;
}STRUC_MODULE_PRM_BLOCK;
#define VPC3_STRUC_MODULE_PRM_BLOCK_PTR  STRUC_MODULE_PRM_BLOCK  *



u8                           ret_value;
u8*                ptr;


    ret_value = DP_OK;

  // if(len==27)   // if( len == 27 )// 27 is elco gsd prm data length. 17 is kuebler encoder prm data
   if(len==28)  // 10 is kuebler 5868 prm length and tr
   {	
         
       ret_value=DP_OK;	
     
     }//if( len == 0x13 )
    else
    {
        ret_value = DP_PRM_LEN_ERROR;
    }//else of if( len == 0x13 )
    
    if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( ret_value == DP_OK ) )
    {
        //don't send diagnostic here
    }//if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( ret_value == DP_OK ) )
    else
    {

        user_alarm( USER_TYPE_PRM_OK, 0, (ALARM_STATUS_PDU_PTR)0, FALSE );
    }//else of if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( ret_value == DP_OK ) )

    return ret_value;

   
}//u8 user_chk_new_prm_data(VPC3_UNSIGNED8_PTR prm_ptr, u8 len)
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_new_ext_prm_data                                       */
/*                                                                           */
/* todo  : check the prm-data                                                */
/* return: DP_OK  - prm-data OK                                              */
/*         DP_NOK - prm-data not OK                                          */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )
u8 user_chk_new_ext_prm_data( VPC3_STRUC_PRM_BLOCK_PTR prmblock_ptr, u8 len )
{
u8 ret_value;

    ret_value = DP_OK;
    len = len;

    if(    ( prmblock_ptr->command == PBC_USER_PRM_DATA )
        && ( prmblock_ptr->len     == 0x0C )
        && ( prmblock_ptr->slot_nr == 0x00 )
        && ( prmblock_ptr->index   == 0x00 )
      )
    {
        //copy counter parameter data
        memcpy( &sSystem.prm, &prmblock_ptr->user_data+1, 7 );

        //check parameter of counter module
        if(    ( sSystem.prm.alarm     &  0xFC )        //check alarm
            || ( sSystem.prm.mode      >  0x01 )        //check counter mode
            || ( sSystem.prm.time_base >  0x05 )        //check counter timebase
          )
        {
            //error
            ret_value = DP_PRM_ENTRY_ERROR;
        }//if(    ( counter.prm.len != 9 )  ...
    }
    else
    {
        ret_value = DP_PRM_BLOCK_ERROR;
    }

    return ret_value;
}//u8 user_chk_new_ext_prm_data( VPC3_STRUC_PRM_BLOCK_PTR prmblock_ptr, u8 len )
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_new_cfg_data                                           */
/*                                                                           */
/* todo  : check cfg data                                                    */
/*                                                                           */
/* return:  VPC3_CFG_OK      0                                               */
/*          VPC3_CFG_FAULT   1                                               */
/*          VPC3_CFG_UPDATE  2                                               */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )
u8 user_chk_new_cfg_data( void )
{
    u16  cfg_ptr;   // pointer check config buffer

   u16 i;
   u8  length, ret_value;

   u8*   ptr;

    cfg_ptr  = VPC3_GET_CFG_BUF_PTR();
    length = VPC3_GET_CFG_LEN();
    
    if(length == 3)
    {
        if(length!=cfg.length)
			ret_value=VPC3_CFG_UPDATE;
		else
			ret_value=VPC3_CFG_OK;

		cfg.length=length;
	    ptr=(u8*)cfg.cfg_data;
		for(i=0;i<cfg.length;i++)
		{
		   *ptr=SimReadEMIMUX16(cfg_ptr);
		   cfg_ptr++;
		   ptr++;
		 }
        

    }//if( cfg_len != real_cfg_len )
    else
    {
        ret_value = VPC3_CFG_FAULT;
    }//if( cfg_len != real_cfg_len )


    if((ret_value == VPC3_CFG_OK)||(ret_value==VPC3_CFG_UPDATE))
    {

    }
    if(( ret_value == VPC3_CFG_OK)||(ret_value==VPC3_CFG_UPDATE) )
	{

       
    	
        user_alarm( USER_TYPE_CFG_OK, 0, (ALARM_STATUS_PDU_PTR) 0, FALSE );
        user.event |= VPC3_EV_NEW_CFG_DATA;
    }//if( ret_value == VPC3_CFG_UPDATE )
    else
    {
      
        user_alarm( USER_TYPE_CFG_NOK, status_diag.user_data[ 0 ], (ALARM_STATUS_PDU_PTR) 0, FALSE );
    }//else of if( ret_value == VPC3_CFG_UPDATE )
   
    return ret_value;
}//u8 user_chk_new_cfg_data( void )
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_go_leave_data_ex                                       */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )
void user_isr_go_leave_data_ex( void )
{
    if( VPC3_GET_DP_STATE() != DATA_EX )
    {
        user.state = USER_STATE_CLEAR;
		gEventFlag=GOLEAVEFLAG;
        user.application_ready = FALSE;
    }//if( VPC3_GET_DP_STATE() != DATA_EX )
}//void user_isr_go_leave_data_ex( void )
#endif//#if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_dx_out                                                 */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )
void user_isr_dx_out( void )
{
    user.event |= VPC3_EV_DX_OUT;
}//void user_isr_dx_out( void )
#endif//#if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_diag_buf_changed                                       */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )
void user_isr_diag_buf_changed( void )
{
    // diagnosis buffer has been changed
    user.user_diag_active = FALSE;
    // Fetch new diagnosis buffer
    //dp_sys.diag_buf_ptr.byte_ptr = vpc3_get_diagbufptr();
    dp_sys.diag_ptr = vpc3_get_diagbufptr();
}//void user_isr_diag_buf_changed( void )
#endif//#if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_new_wd_timeout                                         */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )
void user_isr_new_wd_dp_timeout( void )
{
    reset_diagnostic_buffer();
}//void user_isr_new_wd_dp_timeout( void )
#endif//#if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_mac_reset                                              */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )
void user_isr_mac_reset( void )
{
    //not used in our application
}//void user_isr_mac_reset( void )
#endif//#if( ISR_ENABLE_VPC3_INT_MAC_RESET == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_baudrate_detect                                        */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )
void user_isr_baudrate_detect( void )
{
	  gEventFlag=BAUDDETECTFLAG;
	  gData=0x5;
    
}//void user_isr_baudrate_detect( void )
#endif//#if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_new_gc_command                                         */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )
void user_isr_new_gc_command( void )
{
    //not used in our application
}//void user_isr_new_gc_command( void )
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_new_ssa_data                                           */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )
void user_isr_new_ssa_data( void )
{
    //not used in our application

VPC3_STRUC_SSA_BLOCK_PTR ssa_ptr;

    ssa_ptr = (VPC3_STRUC_SSA_BLOCK_PTR) VPC3_GET_SSA_BUF_PTR();
    //store the new address and the bit real_no_add_chg for the next startup
    /*
    print_string("\r\nNewAddr: ");
    print_hexbyte(ssa_ptr->address);
    print_hexbyte(ssa_ptr->no_add_chg);
    */
}
#endif//#if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_user_timer_clock (10ms)                                */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )
void user_isr_user_timer_clock( void )
{
    //simulation for counter module

}//void user_isr_user_timer_clock( void )
#endif//#if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )

#ifdef DP_SUBSCRIBER
/*---------------------------------------------------------------------------*/
/* function: user_isr_dxb_out                                                */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 )
void user_isr_dxb_out( void )
{
    user.event |= VPC3_EV_DXB_OUT;
}//void user_isr_dxb_out( void )
#endif//#if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 )

/*---------------------------------------------------------------------------*/
/* function: user_isr_dxb_link_error                                         */
/*---------------------------------------------------------------------------*/
#if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 )
void user_isr_dxb_link_error( void )
{
    user.event |= VPC3_EV_DXB_LINK_ERROR;
}//void user_isr_dxb_link_error( void )
#endif//#if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 )

#endif//#ifdef DP_SUBSCRIBER


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2004. Confidential.                         */
/*****************************************************************************/
