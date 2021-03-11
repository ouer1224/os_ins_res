/***********************  Filename: dp_if.c  *********************************/
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
/* Function:       interface service routines for VPC3+ (dp-protocol)        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/* history                                                                   */
/* ========================================================================= */
/* 08.09.2005 [V5.00]  Urversion                                             */
/* 22.10.2005 [V5.01]  BugFix:                                               */
/*                     Dp_cfg.h:                                             */
/*                     Missing bracket:                                      */
/*                     #define ASIC_USER_RAM (ASIC_RAM_LENGTH -              */
/*                                              DP_ORG_LENGTH - SAP_LENGTH)  */
/*                     DP_VERSION_BUGFIX changed to 0x01                     */
/* 09.01.2006 [V5.02]  BugFix:                                               */
/*                     Problem with SIEMENS PLC 318 and Diag.cfg_Fault       */
/*                     The VPC3+/C does not set the default master address   */
/*                     back in the diagnostic telegram to 0xFF.              */
/*                     Software solution in dp_user.c                        */
/*                     (named with BugFix 502).                              */ 
/*                     DP_VERSION_BUGFIX changed to 0x02                     */
/* 21.02.2006 [V5.03]  BugFix:                                               */
/*                     I&M Functions: Return Codes of NRS-PDUs changed.      */
/*                     profichip has now it's own MANUFACTURER_ID (0x01BC).  */
/*                     BugFix:                                               */
/*                     The definition of LITTLE_ENDIAN and BIG_ENDIAN is     */
/*                     now correct.                                          */
/*                     BugFix:                                               */
/*                     MSAC_C2-connection: Certification Error Ifak -        */
/*                     ILL31 transistion implemented.                        */
/*                     After disconnecting profibus cable, an open MSAC-C2   */
/*                     connection will be closed after timeout (2*F-timer).  */
/*                     Function acls in dp_msac_c1 optimized.                */
/*                     (named with BugFix 503).                              */ 
/*                     DP_VERSION_BUGFIX changed to 0x03                     */
/* 13.11.2006 [V5.04]  Certification problem ( itm ):						 */
/*					   The DPV1-alarms will be checked now in the function   */
/*                     user_chk_new_prm_data().                              */ 
/*                     Certification problem ( procentec ):                  */ 
/*                     The master class 2 sends an initiate.req with out     */
/*                     data and the slave answers with next free SAP.        */
/*                     After then the master class 2 send an initiate.req    */
/*                     with data, the slave should now answer with the same  */
/*                     SAP and not with new SAP.                             */
/*                     (named with BugFix 504).                              */ 
/*                     DP_VERSION_BUGFIX changed to 0x04                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
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
/* version                                                                   */
/*---------------------------------------------------------------------------*/
#define DP_VERSION_MAIN_INTERFACE   ((u8)0x05)
#define DP_VERSION_FUNCTION         ((u8)0x00)
#define DP_VERSION_BUGFIX           ((u8)0x04)

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE   vpc3_set_constants  ( u8 slave_address );
DP_ERROR_CODE   ssc_init            ( void );
DP_ERROR_CODE   vpc3_buf_init       ( void );
u16 			assign_aux_buf		( u16 *pwLens, u8 bCount, u8*pbAssign, u16 *pwAuxLen );
//void      WaitSerialPort();
u8     SimReadEMIMUX16(u16 address);
void  SimWriteEMIMUX16(u8 value, u16 address);

//unsigned char  xdata  *vpc3_ptr;
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/
// VPC3_STRUC          VPC3_PTR    p_Vpc3;     // pointer to Vpc3 structure
VPC3_SYSTEM_STRUC               dp_sys;     // global system structure


/*---------------------------------------------------------------------------*/
/* defines                                                                   */
/*---------------------------------------------------------------------------*/
/* Literals */
/* Lage der Bits der AUX-Puffer-Zuordnung */
#define RBL_PRM                 0
#define RBL_CFG                 1
#define RBL_SSA                 2





/*---------------------------------------------------------------------------*/
/* function: DP_GetVersion                                                   */
/*---------------------------------------------------------------------------*/
 u8 Errorflag=0;
void DP_GetVersion(  )
{
                               
  //p_Vpc3 = (VPC3_STRUC VPC3_PTR)VPC3_ASIC_ADDRESS;
  
    switch( VPC3_GET_ASIC_TYPE() )
    {
        case AT_VPC3B:
			
        {
//            print_string("ASIC is VPC3B");
			  Errorflag=1;
            break;
        }

        case AT_VPC3C:
        {	   Errorflag=2;
//             print_string("ASIC is VPC3C");
            break;
        }

        case AT_MPI12X:
        {	  Errorflag=3 ;
//            print_string("ASIC is VPC3D");
            break;
        }

        default:
        {	 Errorflag=4;
//           print_string("NOTHIN");
           break;
        }
    }//switch( VPC3_GET_ASIC_TYPE()



}//void DP_GetVersion( psDP_VERSION pVersion )

u16 errorlog;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  u8 vpc3_initialization (void)                                         */
/*                                                                           */
/*   function:     initializing the vpc3+ controller                         */
/*                                                                           */
/*   parameter:    VPC3_INIT, VPC3_DP_INIT                                   */
/*   return value: errorcode (0 = no error)                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE vpc3_initialization( u8 slave_address, CFG_STRUCT cfg )
{
DP_ERROR_CODE       error;
//VPC3_UNSIGNED8_PTR  vpc3_ptr;       // pointer to vpc3 memory area


u16               i,j,rddata;
u8     temp;

    /*-----------------------------------------------------------------------*/
    /* initialize global system structure                                    */
    /*-----------------------------------------------------------------------*/
 
    /*-----------------------------------------------------------------------*/
    /* init vpc3 data                                                        */
    /*-----------------------------------------------------------------------*/
   // p_Vpc3 = (VPC3_STRUC VPC3_PTR)VPC3_ASIC_ADDRESS;

    DISVPRESET
    __no_operation();
   // PLED=0;

    //neccessary, if 4Kbyte mode enabled
    //VPC3->ctrl_reg.wr.mode_reg2 = INIT_VPC3_MODE_REG_2;
    SimWriteEMIMUX16(INIT_VPC3_MODE_REG_2,VPC3_STRUC_ctrl_reg_wr_mode_reg2);
   
  
    /*-----------------------------------------------------------------------*/
    /* test and clear vpc3 ram                                               */
    /*-----------------------------------------------------------------------*/
    error = DP_OK;
    DP_GetVersion();
 //   start_serial_communication();
 
    
    //DISABLE_VPC3_INT__  ;



      j=10;
      for(i = 0x16; i < ASIC_RAM_LENGTH;)
      {
      	   temp=(j>>8)&0xff;
          SimWriteEMIMUX16(temp,i);
          i++;      
          if(j==509)
             errorlog=j;
          temp=j&0xff;
          SimWriteEMIMUX16(temp,i);
          i++;
          j++;
      }
    
      j=10;
      for(i = 0x16; i < ASIC_RAM_LENGTH;)
      {
      	   temp=SimReadEMIMUX16(i);
      	   rddata=(temp<<8);
      	   i++;
      	   temp=SimReadEMIMUX16(i);
      	   rddata=rddata|temp;
           if(j==509)
              errorlog=i;
          if(rddata!=j)
           {
             errorlog=j;
             error=DP_VPC3_ERROR;
             break;
           }
         i++;
         j++;
      }


     j++;	 
    if(DP_OK == error)
    {
        //clear VPC3   
        //memset( &VPC3->slave_addr, 0, (ASIC_RAM_LENGTH-0x16) );
        for(i=0x16;i<ASIC_RAM_LENGTH;i++)
        {
        	  SimWriteEMIMUX16(0,i);
        }
     //   print_string("Test RAM");
    //    start_serial_communication();     
        /*-------------------------------------------------------------------*/
        /* check VPC3 is in OFFLINE                                          */
        /*-------------------------------------------------------------------*/
        if( !VPC3_GET_OFF_PASS() )
        {
            /*---------------------------------------------------------------*/
            /* set constant values                                           */
            /*---------------------------------------------------------------*/
            error = vpc3_set_constants( slave_address );
            if(DP_OK == error)
            {
                /*-----------------------------------------------------------*/
                /* calculate length of input and output data using cfg-data  */
                /*-----------------------------------------------------------*/
                error = vpc3_calculate_inp_outp_len ( &cfg.cfg_data[0], cfg.length);
                if( DP_OK == error )
                {
                    /*-------------------------------------------------------*/
                    /* initialize buffer structure                           */
                    /*-------------------------------------------------------*/
                    error = vpc3_buf_init();
                    if (DP_OK == error)
                    {
                        /*---------------------------------------------------*/
                        /* set real configuration data                       */
                        /*---------------------------------------------------*/
                        VPC3_SET_READ_CFG_LEN(cfg.length);      // set configuration length
                       // memcpy( VPC3_GET_READ_CFG_BUF_PTR(), &cfg.cfg_data[0], cfg.length );

					   j=VPC3_GET_READ_CFG_BUF_PTR();
					   
                        for(i=0;i<cfg.length;i++)
                        	 SimWriteEMIMUX16(cfg.cfg_data[i],j+i);
				
                       // set I/O length
                        vpc3_set_io_data_len();

                        /*---------------------------------------------------*/
                        /* initialize subscriber                             */
                        /*---------------------------------------------------*/


                        /*---------------------------------------------------*/
                        /* initialize fdl_interface                          */
                        /*---------------------------------------------------*/
                    }//if(DP_OK == error)
                }//if(DP_OK == error)
            }//if(DP_OK == error)
        }//if( !VPC3_GET_OFF_PASS() )
        else
        {
            error = DP_NOT_OFFLINE_ERROR;
        }
    }//if(DP_OK == error)
    return error;
}

/*---------------------------------------------------------------------------*/
/* function: START_VPC3                                                      */
/*---------------------------------------------------------------------------*/
void START_VPC3( void )
{
    /*-----------------------------------------------------------------------*/
    /* start vpc3                                                            */
    /*-----------------------------------------------------------------------*/
    START_VPC3__();

    // Fetch the first diagnosis buffer
    //dp_sys.diag_buf_ptr.byte_ptr = vpc3_get_diagbufptr();
    dp_sys.diag_ptr= vpc3_get_diagbufptr();
   // print_string("Diag=");
   // print_hexword(dp_sys.diag_ptr);
   // start_serial_communication();
   // WaitSerialPort();
}//void START_VPC3( void )

/*---------------------------------------------------------------------------*/
/* function: STOP_VPC3                                                       */
/*---------------------------------------------------------------------------*/
//V504
void STOP_VPC3( void )
{
    /*-----------------------------------------------------------------------*/
    /* start vpc3                                                            */
    /*-----------------------------------------------------------------------*/
    GO_OFFLINE_VPC3();
    do
    {
        //wait, for offline
    }while( VPC3_GET_OFF_PASS() );


}//void  STOP_VPC3( void )

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   function:     initializing constant vpc3 values                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE vpc3_set_constants( u8 slave_address )
{
DP_ERROR_CODE error;


    error = DP_OK;


    dp_sys.din_bufsize     = DIN_BUFSIZE;
    dp_sys.dout_bufsize    = DOUT_BUFSIZE;
    dp_sys.prm_bufsize     = PRM_BUFSIZE;
    dp_sys.diag_bufsize    = DIAG_BUFSIZE;
    dp_sys.cfg_bufsize     = CFG_BUFSIZE;
    dp_sys.ssa_bufsize     = SSA_BUFSIZE;
    /*-----------------------------------------------------------------------*/
    /* initialize  control logic                                             */
    /*-----------------------------------------------------------------------*/
  //  VPC3->int_req1                = 0x00;
  //  VPC3->int_req2                = 0x00;
 //   VPC3->isreg.wr.int_ack1       = 0xFF;
 //   VPC3->isreg.wr.int_ack2       = 0xFF;
 //   VPC3->ctrl_reg.wr.mode_reg1_r = 0xFF;      // reset mode register 1

    SimWriteEMIMUX16(0x0,VPC3_STRUC_int_req1);
    SimWriteEMIMUX16(0x0,VPC3_STRUC_int_req2);
    SimWriteEMIMUX16(0xff,VPC3_STRUC_isreg_wr_int_ack1);
    SimWriteEMIMUX16(0xff,VPC3_STRUC_isreg_wr_int_ack2);
    SimWriteEMIMUX16(0xff,VPC3_STRUC_ctrl_reg_wr_mode_reg1_r);

    /*-----------------------------------------------------------------------*/
    /* set modes of vpc3                                                     */
    /*-----------------------------------------------------------------------*/
   // VPC3->mode_reg0_H = INIT_VPC3_MODE_REG_H;
 //   VPC3->mode_reg0_L = INIT_VPC3_MODE_REG_L;

   // VPC3->ctrl_reg.wr.mode_reg2 = INIT_VPC3_MODE_REG_2;

    SimWriteEMIMUX16(INIT_VPC3_MODE_REG_H,VPC3_STRUC_mode_reg0_H);
    SimWriteEMIMUX16(INIT_VPC3_MODE_REG_L,VPC3_STRUC_mode_reg0_L);
    SimWriteEMIMUX16(INIT_VPC3_MODE_REG_2,VPC3_STRUC_ctrl_reg_wr_mode_reg2);

    /*-----------------------------------------------------------------------*/
    /* set interrupt triggers                                                */
    /*-----------------------------------------------------------------------*/
   // VPC3->isreg.wr.int_mask_H = ~(INIT_VPC3_IND_H);
   // VPC3->isreg.wr.int_mask_L = ~(INIT_VPC3_IND_L);

    SimWriteEMIMUX16((u8) ~(INIT_VPC3_IND_H),VPC3_STRUC_isreg_wr_int_mask_H);
    SimWriteEMIMUX16( (u8)~(INIT_VPC3_IND_L),VPC3_STRUC_isreg_wr_int_mask_L);
    //dp_sys.ind_high = ~(INIT_VPC3_IND_H);
    //dp_sys.ind_low  = ~(INIT_VPC3_IND_L);

    /*-----------------------------------------------------------------------*/
    /* set time-variables                                                    */
    /*-----------------------------------------------------------------------*/
  //  VPC3->ctrl_reg.wr.wd_baud_ctrl = 0x10;    // WD Timeout
 //   VPC3->ctrl_reg.wr.mintsdr      = 0x0B;    // Min Tsdr 11 BIT (11Bit .. 255Bit)

    SimWriteEMIMUX16(0x10,VPC3_STRUC_ctrl_reg_wr_wd_baud_ctrl);
    SimWriteEMIMUX16(0xB,VPC3_STRUC_ctrl_reg_wr_mintsdr);
    /*-----------------------------------------------------------------------*/
    /* set variables for synch-mode                                          */
    /*-----------------------------------------------------------------------*/


    /*-----------------------------------------------------------------------*/
    /* set user watchdog (dataexchange telegram counter)                     */
    /*-----------------------------------------------------------------------*/

  //  VPC3->user_wd_value[1] = (u8)(USER_WD >> 8);
  //  VPC3->user_wd_value[0] = (u8)(USER_WD);

    SimWriteEMIMUX16((u8)(USER_WD >> 8),VPC3_STRUC_user_wd_value1);
    SimWriteEMIMUX16((u8)(USER_WD),VPC3_STRUC_user_wd_value0);

    /*-----------------------------------------------------------------------*/
    /* set pointer to FF                                                     */
    /*-----------------------------------------------------------------------*/
//    p_Vpc3->fdl_sap_list_ptr = VPC3_SAP_CTRL_LIST_START;
   // p_Vpc3->sap_ctrl_list[0] = 0xFF;

    /*-----------------------------------------------------------------------*/
    /* ssa support                                                           */
    /*-----------------------------------------------------------------------*/
    //p_Vpc3->real_no_add_change = ( dp_sys.ssa_bufsize == 0 ) ? 0xFF : 0x00;
    //VPC3->real_no_add_change=0x0; // no ssa support
    SimWriteEMIMUX16(0x0,VPC3_STRUC_real_no_add_change);
      
    /*-----------------------------------------------------------------------*/
    /* set profibus ident number                                             */
    /*-----------------------------------------------------------------------*/
   // VPC3->ident_high = (u8)(IDENT_NR >> 8);
   // VPC3->ident_low  = (u8)(IDENT_NR);
    SimWriteEMIMUX16((u8)(IDENT_NR >> 8),VPC3_STRUC_ident_high);
    SimWriteEMIMUX16((u8)(IDENT_NR),VPC3_STRUC_ident_low);

    /*-----------------------------------------------------------------------*/
    /* set and check slave address                                           */
    /*-----------------------------------------------------------------------*/
    if( slave_address < 126 && slave_address != 0 )
    {
       //   VPC3->slave_addr = slave_address;
       SimWriteEMIMUX16(slave_address,VPC3_STRUC_slave_addr);
     //  value=SimReadEMIMUX16(VPC3_STRUC_slave_addr);
    //   if(value!=0x4)error=DP_ADDRESS_ERROR;
       
    }
    else
    {
        error = DP_ADDRESS_ERROR;
    }

    return error;
}




/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  DP_ERROR_CODE vpc3_calculate_inp_outp_len (VPC3_UNSIGNED8_PTR cfg_ptr,   */
/*                                             u8              cfg_len )  */
/*                                                                           */
/*   function:      Calculation of Inputs/Outputs.                           */
/*                                                                           */
/*   parameter:     cfg_ptr:  Pointer to configuration buffer.               */
/*                  cfg_len:  Length of configuration data.                  */
/*   return value:  -                                                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE vpc3_calculate_inp_outp_len (u8* cfg_ptr, u8 cfg_len)
{
DP_ERROR_CODE error;
u8         specific_data_length;
u8         temp_outp_data_len;
u8         temp_inp_data_len;
u8         length;
u8         count;

    error = DP_OK;
    temp_outp_data_len = 0;
    temp_inp_data_len  = 0;

    if( (cfg_len > 0)  &&  (cfg_len<=6))//(cfg_len <= dp_sys.cfg_bufsize) )
    {
        for( ; cfg_len > 0; cfg_len -= count)
        {
            count = 0;

            if (*cfg_ptr & VPC3_CFG_IS_BYTE_FORMAT)
            {
                // general identifier format
                count++;
                // cfg_ptr points to "Configurationbyte", CFG_BF means "CFG_IS_BYTE_FORMAT"
                length = (u8)( (*cfg_ptr & VPC3_CFG_BF_LENGTH) + 1 );

                if(*cfg_ptr & VPC3_CFG_LENGTH_IS_WORD_FORMAT)
                {
                    length *= 2;
                }

                if(*cfg_ptr & VPC3_CFG_BF_OUTP_EXIST)
                {
                    temp_outp_data_len += length;
                }

                if(*cfg_ptr & VPC3_CFG_BF_INP_EXIST)
                {
                    temp_inp_data_len += length;
                }

                cfg_ptr++;
            }
            else
            {
                // cfg_ptr points to the headerbyte of "special identifier format
                // CFG_SF means "CFG_IS_SPECIAL_FORMAT"
                if(*cfg_ptr & VPC3_CFG_SF_OUTP_EXIST)
                {
                    count++;    // next u8 contains the length of outp_data
                    length = (u8)((*(cfg_ptr + count) & VPC3_CFG_SF_LENGTH) +1);

                    if(*(cfg_ptr + count) & VPC3_CFG_LENGTH_IS_WORD_FORMAT)
                    {
                        temp_outp_data_len += (2*length);
                    }
                    else
                    {
                        temp_outp_data_len += length;
                    }
                }

                if(*cfg_ptr & VPC3_CFG_SF_INP_EXIST)
                {
                    count++;  // next u8 copntains the length of inp_data
                    length = (u8)((*(cfg_ptr + count) & VPC3_CFG_SF_LENGTH) +1);

                    if(*(cfg_ptr + count) & VPC3_CFG_LENGTH_IS_WORD_FORMAT)
                    {
                        temp_inp_data_len += (2*length);
                    }
                    else
                    {
                        temp_inp_data_len += length;
                    }
                }

                specific_data_length = (u8)(*cfg_ptr & VPC3_CFG_BF_LENGTH);

                if(specific_data_length != 15)
                {
                    count = (u8)(count + 1 + specific_data_length);
                    cfg_ptr = cfg_ptr + count;
                }
                else
                {
                    // specific data length = 15 not allowed
                    dp_sys.inp_data_len  = 0xFF;
                    dp_sys.outp_data_len = 0xFF;

                    error = DP_CALCULATE_IO_ERROR;
                }
            }
        }//for( ; cfg_len > 0; cfg_len -= count)
        if(  ( cfg_len != 0 ) || ( temp_inp_data_len > dp_sys.din_bufsize ) || ( temp_outp_data_len > dp_sys.dout_bufsize ) )
        {
            dp_sys.inp_data_len  = 0xFF;
            dp_sys.outp_data_len = 0xFF;

            error = DP_CALCULATE_IO_ERROR;
        }
        else
        {
            dp_sys.inp_data_len  = temp_inp_data_len;
            dp_sys.outp_data_len = temp_outp_data_len;
            error = DP_OK;
        }

    }//if( (cfg_len > 0)  &&  (cfg_len <= vpc3_dp_init.cfg_buf_len) )
    else
    {
        dp_sys.inp_data_len  = 0xFF;
        dp_sys.outp_data_len = 0xFF;

        error = DP_CALCULATE_IO_ERROR;
    }

    return error;
}//vpc3_calculate_inp_outp_len (VPC3_UNSIGNED8_PTR cfg_ptr, u8 cfg_len)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  DP_ERROR_CODE vpc3_buf_init (void)                                       */
/*                                                                           */
/*   function:      Initializing VPC3 buffers.                               */
/*                                                                           */
/*   parameter:     none                                                     */
/*   return value:  DP_OK        : Buffer length OK                          */
/*                  ERROR        : defined in dp_error.h                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE vpc3_buf_init(void)
{
DP_ERROR_CODE       error;
u16               aux_buf_len[2];      // calculated length of AUX buffer
u16               r_din_dout_buf_len;  // calculated length of real I/O Data length
u16               real_buf_len[3],temp;     // real length of AUX-Buffer
u8               aux_ass;             // Bit array, in which the assignments of AUX1/2 are defined

    error = DP_OK;

    /*-----------------------------------------------------------------------*/
    /* check buffer length                                                   */
    /*-----------------------------------------------------------------------*/


   
    if(error == DP_OK)//else
    {
        /*-------------------------------------------------------------------*/
        /* length of buffers ok, check memory consumption                    */
        /*-------------------------------------------------------------------*/
        //if(VPC3->mode_reg0_H & VPC3_SPEC_PRM_BUF_MODE)
        if(SimReadEMIMUX16(VPC3_STRUC_mode_reg0_H) & VPC3_SPEC_PRM_BUF_MODE)
        {
            real_buf_len[RBL_PRM] = 0; /* prm-data goes directly into prm-buffer */
        }
        else
        {
            real_buf_len[RBL_PRM] = PRM_BUFSIZE; // prm-data goes over AUX-buffer
        }

        // length of cfg and ssa buffer
        real_buf_len[RBL_CFG] = CFG_BUFSIZE;
        real_buf_len[RBL_SSA] = SSA_BUFSIZE;

        /*-------------------------------------------------------------------*/
        /* calculate aux-buffer                                              */
        /*-------------------------------------------------------------------*/
        dp_sys.vpc3_used_dp_buf_mem = assign_aux_buf(&real_buf_len[0], 3, &aux_ass, &aux_buf_len[0]);

        /*-------------------------------------------------------------------*/
        /* check memory consumption                                          */
        /*-------------------------------------------------------------------*/
        r_din_dout_buf_len = ((DOUT_BUFSIZE+ SEG_OFFSET) & SEG_ADDWORD) + ((DIN_BUFSIZE+ SEG_OFFSET) & SEG_ADDWORD);
        // DIn DOut buffer
        dp_sys.vpc3_used_dp_buf_mem += r_din_dout_buf_len * 3;
        // add Read Config Buffer
        dp_sys.vpc3_used_dp_buf_mem += real_buf_len[RBL_CFG];
        // diagnose buffer
        dp_sys.vpc3_used_dp_buf_mem += ((DIAG_BUFSIZE+ SEG_OFFSET) & SEG_ADDBYTE) * 2;
        // prm buffer
       // if(VPC3->mode_reg0_H & VPC3_SPEC_PRM_BUF_MODE)
        if(SimReadEMIMUX16(VPC3_STRUC_mode_reg0_H) & VPC3_SPEC_PRM_BUF_MODE)
        {
            // spec. PRM buffer
            real_buf_len[RBL_PRM] = (PRM_BUFSIZE+ SEG_OFFSET) & SEG_ADDBYTE;
            dp_sys.vpc3_used_dp_buf_mem += real_buf_len[RBL_PRM];
           // VPC3->len_spec_prm_buf = 30;
           SimWriteEMIMUX16(30,VPC3_STRUC_len_spec_prm_buf);
        }
        else
        {
            SimWriteEMIMUX16(0,VPC3_STRUC_len_spec_prm_buf);
           // VPC3->len_spec_prm_buf = 0;
        }

//        if( dp_sys.vpc3_used_dp_buf_mem > sizeof (VPC3->dpbuffer) )
        if( dp_sys.vpc3_used_dp_buf_mem > ASIC_RAM_LENGTH )	
        {
            // Error: user needs too much memory
            dp_sys.vpc3_used_dp_buf_mem = 0;
            error = DP_LESS_MEM_ERROR;
        }
        else
        {
            /*---------------------------------------------------------------*/
            /* set buffer pointer                                            */
            /*---------------------------------------------------------------*/
            //VPC3->aux_buf_sel      = aux_ass;   // AUX-Puffer zuordnen
            SimWriteEMIMUX16(aux_ass,VPC3_STRUC_aux_buf_sel);
            
            //VPC3->dout_buf_ptr[0]  = VPC3_DP_BUF_START;
            SimWriteEMIMUX16(VPC3_DP_BUF_START,VPC3_STRUC_dout_buf_ptr0);
          
           // VPC3->dout_buf_ptr[1]  = VPC3->dout_buf_ptr[0]  + (((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
          //  VPC3->dout_buf_ptr[2]  = VPC3->dout_buf_ptr[1]  + (((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr0);
            temp=temp+(((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_dout_buf_ptr1);
            temp=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr1);
            temp=temp+(((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_dout_buf_ptr2);


        //    VPC3->din_buf_ptr[0]   = VPC3->dout_buf_ptr[2]  + (((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
       //     VPC3->din_buf_ptr[1]   = VPC3->din_buf_ptr[0]   + (((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
        //    VPC3->din_buf_ptr[2]   = VPC3->din_buf_ptr[1]   + (((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_dout_buf_ptr2);
            temp=temp+ (((DOUT_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_din_buf_ptr0);
            temp=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr0);
            temp=temp+(((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_din_buf_ptr1);
            temp=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr1);
            temp=temp+(((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_din_buf_ptr2);            

           // VPC3->diag_buf_ptr[0]  = VPC3->din_buf_ptr[2]   + (((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
         //   VPC3->diag_buf_ptr[1]  = VPC3->diag_buf_ptr[0]  + (((DIAG_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr2);
            temp=temp+ (((DIN_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_diag_buf_ptr0);
            temp=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0);
            temp=temp+ (((DIAG_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_diag_buf_ptr1);
            
         //   VPC3->cfg_buf_ptr      = VPC3->diag_buf_ptr[1]  + (((DIAG_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
          //  VPC3->read_cfg_buf_ptr = VPC3->cfg_buf_ptr      + (real_buf_len[RBL_CFG]>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1);
            temp=temp+(((DIAG_BUFSIZE + SEG_OFFSET) & SEG_ADDBYTE)>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_cfg_buf_ptr);
            temp=SimReadEMIMUX16(VPC3_STRUC_cfg_buf_ptr);
            temp=temp+(real_buf_len[RBL_CFG]>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_read_cfg_buf_ptr);

          //  VPC3->prm_buf_ptr      = VPC3->read_cfg_buf_ptr + (real_buf_len[RBL_CFG]>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_read_cfg_buf_ptr);
            temp=temp+(real_buf_len[RBL_CFG]>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_prm_buf_ptr);


      //      VPC3->aux_buf_ptr[0]   = VPC3->prm_buf_ptr      + (real_buf_len[RBL_PRM]>>SEG_MULDIV);
            temp=SimReadEMIMUX16(VPC3_STRUC_prm_buf_ptr);
            temp=temp+(real_buf_len[RBL_PRM]>>SEG_MULDIV);
            SimWriteEMIMUX16(temp,VPC3_STRUC_aux_buf_ptr0);

            if( aux_buf_len[1] != 0 )
            {
            	   temp=SimReadEMIMUX16(VPC3_STRUC_aux_buf_ptr0);
            	   temp=temp+(aux_buf_len[0]>>SEG_MULDIV);
            	   SimWriteEMIMUX16(temp,VPC3_STRUC_aux_buf_ptr1);
                // VPC3->aux_buf_ptr[1]   = VPC3->aux_buf_ptr[0]   + (aux_buf_len[0]>>SEG_MULDIV);
            }
            else
            {
               // VPC3->aux_buf_ptr[1]   = 0;
               SimWriteEMIMUX16(0,VPC3_STRUC_aux_buf_ptr1);
            }

            if (real_buf_len[RBL_SSA] == 0)
            {
                // no ssa supported
              //  VPC3->ssa_buf_ptr = 0;         /* Dienst sperren */
               SimWriteEMIMUX16(0,VPC3_STRUC_ssa_buf_ptr);
            }
            else
            {
                // ssa supported
                if( aux_buf_len[1] != 0 )
                {
                  //  VPC3->ssa_buf_ptr = VPC3->aux_buf_ptr[1] + (aux_buf_len[1]>>SEG_MULDIV);
                    temp=SimReadEMIMUX16(VPC3_STRUC_aux_buf_ptr1);
                    temp=temp+(aux_buf_len[1]>>SEG_MULDIV);
                    SimWriteEMIMUX16(temp,VPC3_STRUC_ssa_buf_ptr);
                }
                else
                {
                   // VPC3->ssa_buf_ptr = VPC3->aux_buf_ptr[0] + (aux_buf_len[0]>>SEG_MULDIV);
                    temp=SimReadEMIMUX16(VPC3_STRUC_aux_buf_ptr0);
                    temp=temp+(aux_buf_len[0]>>SEG_MULDIV);
                    SimWriteEMIMUX16(temp,VPC3_STRUC_ssa_buf_ptr);
                }
            }

            /*---------------------------------------------------------------*/
            /* set buffer length                                             */
            /*---------------------------------------------------------------*/
            //VPC3->len_dout_buf    = DOUT_BUFSIZE;//dp_sys.outp_data_len;
          //  VPC3->len_din_buf     = DIN_BUFSIZE;//dp_sys.inp_data_len;

            SimWriteEMIMUX16(DOUT_BUFSIZE,VPC3_STRUC_len_dout_buf);
            SimWriteEMIMUX16(DIN_BUFSIZE,VPC3_STRUC_len_din_buf);

          //  VPC3->len_diag_buf[0] = DIAG_BUFSIZE;                // length of Diag Buffers
          //  VPC3->len_diag_buf[1] = DIAG_BUFSIZE;

            SimWriteEMIMUX16(DIAG_BUFSIZE,VPC3_STRUC_len_diag_buf0);
            SimWriteEMIMUX16(DIAG_BUFSIZE,VPC3_STRUC_len_diag_buf1);
            
            if(aux_buf_len[0] >= 244)
            {
            	 SimWriteEMIMUX16(244,VPC3_STRUC_len_ctrl_buf0);
               // VPC3->len_ctrl_buf[0] = 244;      // Length AUX1-Buffer
            }
            else
            {
            	  SimWriteEMIMUX16(aux_buf_len[0],VPC3_STRUC_len_ctrl_buf0);
               // VPC3->len_ctrl_buf[0] = aux_buf_len[0];      // Length AUX1-Buffer
            }

            if(aux_buf_len[1] >= 244)
            {                 
//                VPC3->len_ctrl_buf[1] = 244;      // Length AUX1-Buffer
                SimWriteEMIMUX16(244,VPC3_STRUC_len_ctrl_buf1);
            }
            else
            {
            	   SimWriteEMIMUX16(aux_buf_len[1],VPC3_STRUC_len_ctrl_buf1);
              //  VPC3->len_ctrl_buf[1] = aux_buf_len[1];      // Length AUX1-Buffer
            }

        //    VPC3->len_cfg_data    = CFG_BUFSIZE;
          //  VPC3->len_prm_data    = PRM_BUFSIZE;
           // VPC3->len_ssa_buf     = SSA_BUFSIZE;

            SimWriteEMIMUX16(CFG_BUFSIZE,VPC3_STRUC_len_cfg_data);
            SimWriteEMIMUX16(PRM_BUFSIZE,VPC3_STRUC_len_prm_data);
            SimWriteEMIMUX16(SSA_BUFSIZE,VPC3_STRUC_len_ssa_buf);
        }
    }
    return error;
}//DP_ERROR_CODE vpc3_buf_init(void)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  u8 vpc3_cfg_data_ok(void)                                             */
/*                                                                           */
/*  function:      Set Configuration Data ok                                 */
/*                                                                           */
/*  parameter:     none                                                      */
/*  return value:  VPC3_CFG_FINISHED:  The transferred configuration is OK.  */
/*                 VPC3_CFG_CONFLICT:  Repeat verification of the requested  */
/*                                     configuration.                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
u8 vpc3_cfg_data_ok(void)
{
	   return SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_user_cfg_data_ok);
    //return VPC3->ctrl_reg.rd.user_cfg_data_ok;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  u8 vpc3_cfg_data_update(void)                                         */
/*                                                                           */
/*  function:      Update Configuration Data                                 */
/*                                                                           */
/*  parameter:     none                                                      */
/*  return value:  VPC3_CFG_FINISHED:  The transferred configuration is OK.  */
/*                 VPC3_CFG_CONFLICT:  Repeat verification of the requested  */
/*                                     configuration.                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
u8 vpc3_cfg_data_update(void)
{
    u8 temp;
  //  VPC3->len_read_cfg_data = VPC3->len_cfg_data;
  //  memcpy( VPC3_GET_READ_CFG_BUF_PTR(), VPC3_GET_CFG_BUF_PTR(), VPC3->len_cfg_data);
  //  return VPC3->ctrl_reg.rd.user_cfg_data_ok;
    u16 i,sa,da;
    temp=SimReadEMIMUX16(VPC3_STRUC_len_cfg_data);
    SimWriteEMIMUX16(temp, VPC3_STRUC_len_read_cfg_data);
    da=VPC3_GET_READ_CFG_BUF_PTR();
    sa=VPC3_GET_CFG_BUF_PTR();
    for(i=0;i<temp;i++)
    {
    	  temp=SimReadEMIMUX16(sa);
    	  SimWriteEMIMUX16(temp,da);
    	  da++;
    	  sa++;
    }    
    return SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_user_cfg_data_ok);
}

/*---------------------------------------------------------------------------*/
/*  D e s c r i p t i o n : vpc3_cfg_data_not_ok                             */
/*                                                                           */
/*   function:      Set Configuration Data not ok                            */
/*                                                                           */
/*   parameter:     none                                                     */
/*   return value:  VPC3_CFG_FINISHED:  The transferred configuration is OK. */
/*                  VPC3_CFG_CONFLICT:  Repeat verification of the requested */
/*                                      configuration.                       */
/*---------------------------------------------------------------------------*/
u8 vpc3_cfg_data_not_ok(void)
{
    return SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_user_cfg_data_nok);
  //  return VPC3->ctrl_reg.rd.user_cfg_data_nok;
}

/*---------------------------------------------------------------------------*/
/*  D e s c r i p t i o n : vpc3_get_doutbufptr                              */
/*                                                                           */
/* This function fetches the pointer to the actual output buffer.            */
/*---------------------------------------------------------------------------*/
u16 vpc3_get_doutbufptr (u8 *state_ptr)
{
    u16 value; // pointer to output buffer ( DP-Master -> VPC3+ )
    u8 temp; 
   // *state_ptr = (VPC3->ctrl_reg.rd.next_dout_buf_cmd & (VPC3_NEW_DOUT_BUF | VPC3_DOUT_BUF_CLEARED));

    *state_ptr=(SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_next_dout_buf_cmd)
    	          & (VPC3_NEW_DOUT_BUF | VPC3_DOUT_BUF_CLEARED));

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

    return value;
    //return p_doutbuf;
}

/*---------------------------------------------------------------------------*/
/*  D e s c r i p t i o n : vpc3_get_dinbufptr                               */
/*                                                                           */
/* This function fetches the pointer to the actual input buffer.             */
/*---------------------------------------------------------------------------*/
u16 vpc3_get_dinbufptr (void)
{
//VPC3_UNSIGNED8_PTR p_dinbuf; // pointer to output buffer ( VPC3 -> DP-Master )
    u16 value;
    u8  temp;
 //   switch (VPC3->ctrl_reg.rd.din_buffer_sm & 0x30)         // locate user Din Buffer
     temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_din_buffer_sm);
    switch (temp & 0x30)
    {
        case 0x10:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr0);
        	   value=value<< SEG_MULDIV;
           // p_dinbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->din_buf_ptr[0] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        case 0x20:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr1);
        	   value=value<< SEG_MULDIV;        	
          //  p_dinbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->din_buf_ptr[1] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        case 0x30:
        {
        	   value=SimReadEMIMUX16(VPC3_STRUC_din_buf_ptr2);
        	   value=value<< SEG_MULDIV;        	
           // p_dinbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(VPC3->din_buf_ptr[2] << SEG_MULDIV)+(VPC3_ADR)VPC3_ASIC_ADDRESS);
            break;
        }

        default:
        {
        	  value=0;
          //  p_dinbuf = NULL_PTR;
            break;
        }
    }//switch (p_Vpc3->ctrl_reg.rd.din_buffer_sm & 0x30)

    return value;
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  The function provides the actual prm-master by read out the diag-        */
/*  buffer in vpc3.                                                          */
/*---------------------------------------------------------------------------*/
u8 vpc3_get_master(void)
{
 u8  master;
 u16 temp;
 
    temp=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0);
    temp=temp<< SEG_MULDIV;
    temp=temp+3;
    master=SimReadEMIMUX16(temp);
   // master = *((VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[0] << SEG_MULDIV)+3)+((VPC3_ADR)VPC3_ASIC_ADDRESS))));

    if( master == 0xFF )
    {
    	 temp=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1);
      temp=temp<< SEG_MULDIV;
      temp=temp+3;
      master=SimReadEMIMUX16(temp);
       // master = *((VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[1] << SEG_MULDIV)+3)+((VPC3_ADR)VPC3_ASIC_ADDRESS))));
    }
    return (master);
}

/********************************************************************************
*                                                                               *
*  u16 assign_aux_buf (   u16 PTR_ATTR *lens, u8 count,                  *
*                           u8 PTR_ATTR *assign,                             *
*                           u16 PTR_ATTR *aux_len)                            *
*                                                                              *
*   function:      Calculate length of AUX buffers.                            *
*                                                                              *
*   parameter:     lens    : pointer of list to the buffer length              *
*                  count   : length of list (2..3)                             *
*                  assign  : Bit array, in which the assignments of the        *
*                            AUX-buffers1 and 2 are defined to the control     *
*                            buffers, SSA-Buf, PRM-Buf, Cfg-Buf                *
*                  aux_len : Pointer to a 2 byte field of aux-buffer length.   *
*   return value:  count of bytes used for aux buffers.                        *
*                                                                              *
********************************************************************************/
u16 assign_aux_buf( u16  *pwLens, u8 bCount, u8  *pbAssign, u16  *pwAuxLen )
{
u16   wMinLen  = 0xffff;	// calculated minimal length
u8   bMinStep = 0;       // step by calculated minimal length
u8   i,j;
u8   bPos[3];            // memory for position of length
u16   wLensx[3];          // workarea for calculated length
u8   bStep;              // counter
u16   wTmp;               // tmp-variable for calculated length
u8   bTmp;               // tmp-variable for calculated length
                            // tmp-varaiable for sorted positions

    // check count
    if(( bCount == 2 ) || ( bCount == 3 ))
    {
        for( i = 0; i < bCount; i++ )
        {
            // init memory for position of length
            bPos[i] = i;
            // copy length into workarea for calculated length
            wLensx[i] = pwLens[i];
            // put length to 8 Bit Segmentaddresses
            wLensx[i] = (wLensx[i] + SEG_OFFSET) & SEG_ADDWORD;
        }//for( i = 0; i < bCount; i++ )

        // sort length, highest length to index 0
        for( i = 0; i < bCount-1; i++ )
        {
            for( j = i+1; j < bCount; j++ )
            {
                if( wLensx[i] < wLensx[j] ) 
                {
                    // found higher length
                    //V504
                    wTmp = wLensx[i];          // swap length
                    wLensx[i] = wLensx[j];
                    wLensx[j] = wTmp;

                    bTmp = bPos[i];            // swap position
                    bPos[i] = bPos[j];
                    bPos[j] = bTmp;
                }
            }//for( j = i+1; j < bCount; j++ )
        }//for( i = 0; i < bCount-1; i++ )

        // delete zero length from list lensx
        for( i = bCount; i > 0; i-- )
        {
            if(wLensx[i-1] == 0)
            {
                bCount--;
            }
        }//for( i = bCount; i > 0; i-- )

        if( bCount == 0 )
        {
            wMinLen = 0;     // Error: no defined buffer length
        }

        // gradual length assign to aux-buffer
        for( bStep = 0; bStep < bCount; bStep++ )
        {
            // get length for AUX-Buffer 1
            pwAuxLen[0] = 0;

            for( i = bStep; i < bCount; i++ )
            {
                if( pwAuxLen[0] < wLensx[i] )
                {
                    pwAuxLen[0] = wLensx[i];
                }
            }//for( i = bStep; i < bCount; i++ )

            pwAuxLen[0] = pwAuxLen[0] * (bCount - bStep + 1);

            // get length for AUX-Buffer 2
            pwAuxLen[1] = 0;

            for( i = 0; i < bStep; i++ )
            {
                if( pwAuxLen[1] < wLensx[i] )
                {
                    pwAuxLen[1] = wLensx[i];
                }
            }//for( i = 0; i < bStep; i++ )

            pwAuxLen[1] = pwAuxLen[1] * (bStep + 1);

            if((pwAuxLen[0] + pwAuxLen[1]) < wMinLen )
            {
                // new minimal length found
                wMinLen = pwAuxLen[0] + pwAuxLen[1];
                bMinStep = bStep;
            }
        }//for( bStep = 0; bStep < bCount; bStep++ )

        // calculate length for AUX Buffer 1
        pwAuxLen[0] = 0;

        for( i = bMinStep; i < bCount; i++ )
        {
            if(pwAuxLen[0] < wLensx[i])
            {
                pwAuxLen[0] = wLensx[i];
            }
        }

        // put length, assigned for AUX buffer 1
        for(i = bMinStep; i < bCount; i++)
        {
            pwLens[bPos[i]] = pwAuxLen[0];
        }

        // calculate length of AUX-Buffer 2
        pwAuxLen[1] = 0;

        for( i = 0; i < bMinStep; i++)
        {
            if (pwAuxLen[1] < wLensx[i])
            {
                pwAuxLen[1] = wLensx[i];
            }
        }

        *pbAssign = 0;    // put all buffer to AUX-Buffer 1

        for(i = 0; i < bMinStep; i++)
        {
            pwLens[bPos[i]] = pwAuxLen[1];
            *pbAssign |= 0x1 << bPos[i];
        }

        //check here for VPC3+/B
        //handle Cfg-telegram only over AUX-Buffer 1
        if( *pbAssign & 0x02 )
        {
            bTmp = (*pbAssign);
            *pbAssign = ((~bTmp)&0x07);
            
            wTmp = pwAuxLen[0];
            pwAuxLen[0] = pwAuxLen[1];
            pwAuxLen[1] = wTmp;
        }
    }//if(( bCount == 2 ) || ( bCount == 3 ))
    else
    {
        wMinLen = 0; // error
    }//else of if(( bCount == 2 ) || ( bCount == 3 ))

    return wMinLen;
}//u16 assign_aux_buf( u16 PTR_ATTR *pwLens, u8 bCount, u8 PTR_ATTR *pbAssign, u16 PTR_ATTR *pwAuxLen )

/*---------------------------------------------------------------------------*/
/*  void vpc3_set_io_data_len(void)                                          */
/*---------------------------------------------------------------------------*/
void vpc3_set_io_data_len(void)
{
    // length of buffers OK, set real buffers
   // VPC3->len_dout_buf = DOUT_BUFSIZE;
    //VPC3->len_din_buf  = DIN_BUFSIZE;
    SimWriteEMIMUX16(dp_sys.outp_data_len,VPC3_STRUC_len_dout_buf);
    SimWriteEMIMUX16(dp_sys.inp_data_len,VPC3_STRUC_len_din_buf);
}


/*---------------------------------------------------------------------------*/
/*  D e s c r i p t i o n : vpc3_get_diagbufptr                              */
/*                                                                           */
/* This function fetches the pointer to the actual diagnosis buffer.         */
/*---------------------------------------------------------------------------*/
u16 vpc3_get_diagbufptr (void)
{
//VPC3_UNSIGNED8_PTR p_diagbuf; // pointer to diagnosis buffer
    u16  value;
    u16 temp;
    temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_diag_buffer_sm);
    //if((VPC3->ctrl_reg.rd.diag_buffer_sm & 0x03) == 0x01)           // locate Diag Buffer
    if((temp& 0x03) == 0x01)
    {
        value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0);
        value=value<<SEG_MULDIV;
        
       // p_diagbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)((VPC3->diag_buf_ptr[0] << SEG_MULDIV))+(VPC3_ADR)VPC3_ASIC_ADDRESS);
    }
    else
    {
        if((temp & 0x0C) == 0x04)
        {
        	  value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1);
        	  value=value<<SEG_MULDIV;
            //p_diagbuf = (VPC3_UNSIGNED8_PTR)((VPC3_ADR)((VPC3->diag_buf_ptr[1] << SEG_MULDIV))+(VPC3_ADR)VPC3_ASIC_ADDRESS);
        }
        else
        {
            value=0;
        }
    }
    return value;
}

/*---------------------------------------------------------------------------*/
/* function: vpc3_set_diag_update_all                                        */
/*---------------------------------------------------------------------------*/
u16 vpc3_set_diag_update_all( u8 diag_state, u8 diag_len )
{
u16 value; // pointer to diagnosis buffer
    u8 temp;
    temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_diag_buffer_sm);
    //if((VPC3->ctrl_reg.rd.diag_buffer_sm & 0x03) == 0x01)           // locate Diag Buffer
    if((temp & 0x03) == 0x01)
    {    	   
       // VPC3->len_diag_buf[0] = diag_len; // length of Diag Buffer 1
     //   *((VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->diag_buf_ptr[0] << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))) = diag_state;
        SimWriteEMIMUX16(diag_len,VPC3_STRUC_len_diag_buf0);
        value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0);
        value=value << SEG_MULDIV;
        SimWriteEMIMUX16(diag_state,value);
    }
    else
    {
	       temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_diag_buffer_sm);
	       
      //  if((VPC3->ctrl_reg.rd.diag_buffer_sm & 0x0C) == 0x04)
        if((temp&0x0c)==0x04)
        {
        	 SimWriteEMIMUX16(diag_len,VPC3_STRUC_len_diag_buf1);
        	 value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1);
        	 value=value << SEG_MULDIV;
        	 SimWriteEMIMUX16(diag_state,value);        	   
          //  VPC3->len_diag_buf[1] = diag_len;  // length of Diag Buffer 2
         //   *((VPC3_UNSIGNED8_PTR)(((VPC3_ADR)(VPC3->diag_buf_ptr[1] << SEG_MULDIV)+((VPC3_ADR)VPC3_ASIC_ADDRESS)))) = diag_state;
        }
    }
    temp=SimReadEMIMUX16(VPC3_STRUC_ctrl_reg_rd_new_diag_buf_cmd);
    
//    switch( VPC3->ctrl_reg.rd.new_diag_buf_cmd & 0x03 )
    switch( temp & 0x03 )	
    {
        case 1:   // use buffer 1
        {
	   // p_diagbuf = (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[1] << SEG_MULDIV))+((VPC3_ADR)VPC3_ASIC_ADDRESS)));
          // p_diagbuf = (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[0] << SEG_MULDIV))+((VPC3_ADR)VPC3_ASIC_ADDRESS)));
           value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr0);
           value=value<<SEG_MULDIV;
	         break;
        }
        case 2:   // use buffer 2
        {
	   //p_diagbuf = (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[0] << SEG_MULDIV))+((VPC3_ADR)VPC3_ASIC_ADDRESS)));		
          //  p_diagbuf = (VPC3_UNSIGNED8_PTR)(((VPC3_ADR)((VPC3->diag_buf_ptr[1] << SEG_MULDIV))+((VPC3_ADR)VPC3_ASIC_ADDRESS)));
           value=SimReadEMIMUX16(VPC3_STRUC_diag_buf_ptr1);
           value=value<<SEG_MULDIV;
	         break;
        }
        default:
        {
            // no buffer available
            value=0;
            break;
        }
    }

    return value;
}

/*---------------------------------------------------------------------------*/
/* function: reset_diagnostic_buffer                                         */
/*---------------------------------------------------------------------------*/
void reset_diagnostic_buffer( void )
{
     u16  diag_ptr;

    do
    {
        //fetch new diagnosis buffer
        diag_ptr = vpc3_get_diagbufptr();
    }
    while( diag_ptr == 0 );

    //clear diagnostic buffer
    vpc3_set_diag_update_all( 0, 6 );
}//void reset_diagnostic_buffer( void )

/*--------------------------------------------------------------------------*/
/* set_diagnose ( called by DP, DPV1-ALARM)                                 */
/*--------------------------------------------------------------------------*/
DP_ERROR_CODE set_diagnosis( VPC3_UNION_DIAG_PTR user_diag, u8 user_diag_len, u8 diag_control, u8 check_diag_flag )
{
VPC3_UNION_DIAG_PTR tmp_user_diag;
DP_ERROR_CODE       ret_value;
u8               tmp_user_diag_len;
u8               tmp_len;
u8               dp_state;
u16              value,i;
    ret_value = DP_OK;

    //check available diag buffer
    if( dp_sys.diag_ptr != 0 )
    {
        tmp_user_diag     = user_diag;
        tmp_user_diag_len = user_diag_len;

        dp_state = VPC3_GET_DP_STATE();
        if( (dp_state == DATA_EX) && (check_diag_flag == TRUE) )
        {
            if( VPC3_GET_DIAG_FLAG() )
            {
                //old diagnosis not send
                ret_value = DP_DIAG_OLD_DIAG_NOT_SEND_ERROR;
            }//if( VPC3_GET_DIAG_FLAG() )
        }//if( (dp_state == DATA_EX) && (check_diag_flag == TRUE) )

        // check user_diag_len
        if( user_diag_len > (DIAG_BUFSIZE-6) )
        {
            ret_value = DP_DIAG_BUFFER_LENGTH_ERROR;
        }//if( user_diag_len > (dp_sys.diag_bufsize-6) )

        if( ret_value == DP_OK )
        {
            //check control byte
            switch( diag_control )
            {
                case EXT_DIAG_SET:
                {
                    if( user_diag_len == 0 )
                    {
                        ret_value = DP_DIAG_CONTROL_BYTE_ERROR;
                    }//if( user_diag_len == 0 )
                    break;
                }//case EXT_DIAG_SET:

                default:
                {
                    //do nothing
                    break;
                }//default:
            }//switch( diag_control )


            if( ret_value == DP_OK )
            {
                //copy to diagnosis buffer

             //   value=dp_sys.diag_ptr+DIAG_NORM_DIAG_SIZE;
                value=dp_sys.diag_ptr+DIAG_NORM_DIAG_SIZE;
                for(i=0;i<user_diag_len;i++)
                	{
                	   SimWriteEMIMUX16(user_diag.byte_ptr[i],value++);
                	}                
                //memcpy( dp_sys.diag_buf_ptr.byte_ptr+DIAG_NORM_DIAG_SIZE, user_diag.byte_ptr, user_diag_len );
                dp_sys.diag_ptr = vpc3_set_diag_update_all( diag_control, (DIAG_NORM_DIAG_SIZE + user_diag_len) );
            }//if( ret_value == DP_OK )
        }//if( ret_value == DP_OK )
    }//if( dp_sys.diag_buf_ptr.byte_ptr != NULL_PTR )
    else
    {
        //Fetch new diagnosis buffer
        dp_sys.diag_ptr = vpc3_get_diagbufptr();
        //wait for next free diag_buffer
        ret_value = DP_DIAG_NO_BUFFER_ERROR;
    }//else of if( dp_sys.diag_buf_ptr.byte_ptr != NULL_PTR )
    return ret_value;
}//DP_ERROR_CODE set_diagnosis( VPC3_UNION_DIAG_PTR user_diag, u8 user_diag_len, u8 diag_control, u8 check_diag_flag )

/*-------------------------------------------------------------------*/
/* function: vpc3_get_free_memory                                    */
/*-------------------------------------------------------------------*/
u16 vpc3_get_free_memory( u16 *mem_len )
{
    //return the address and the number of bytes of unused SPC3-ram
    if( dp_sys.vpc3_used_dp_buf_mem < ASIC_USER_RAM )
    {
        *mem_len = ASIC_USER_RAM - dp_sys.vpc3_used_dp_buf_mem;
        return (u16)(dp_sys.vpc3_used_dp_buf_mem +  DP_ORG_LENGTH);

        //return (VPC3_UNSIGNED8_PTR)((VPC3_ADR)(dp_sys.vpc3_used_dp_buf_mem  + DP_ORG_LENGTH)+(VPC3_ADR)VPC3_ASIC_ADDRESS);;
    }
    else
    {
        *mem_len = 0;
        return 0;
    }
}//VPC3_UNSIGNED8_PTR vpc3_get_free_memory( u16 PTR_ATTR *mem_len )

/*-------------------------------------------------------------------*/
/* function: set_alarm                                               */
/*-------------------------------------------------------------------*/
u8 set_alarm( ALARM_STATUS_PDU_PTR user_alarm_ptr, u8 callback  )
{
    #ifdef DP_ALARM
        return set_alarm__( user_alarm_ptr, callback );
    #else
        user_alarm_ptr = user_alarm_ptr;    //avoid warning
        callback       = callback;          //avoid warning
        return SET_ALARM_AL_STATE_CLOSED;
    #endif
}//u8 set_alarm( ALARM_STATUS_PDU_PTR user_alarm_ptr, u8 callback  )

/*-------------------------------------------------------------------*/
/* function: dpv1 statemachines                                      */
/*-------------------------------------------------------------------*/
void vpc3_dpv1_statemachine( void )
{
    #ifdef DP_MSAC_C1
        msac_c1_process();    // state machine MSAC_C1
    #endif

    #ifdef DP_ALARM
        alarm_process();      // state machine ALARM
    #endif

    #ifdef DP_MSAC_C2
        msac_c2_process();    // state machine MSAC_C2
    #endif
}//void vpc3_dpv1_statemachine( void )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2004. Confidential.                         */
/*****************************************************************************/
