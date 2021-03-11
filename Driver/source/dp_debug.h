/*********************** Filename: dp_debug.h ********************************/
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
/* Description: Print routines for serio.c.                                  */
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

    - debug prototypes

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_DEBUG_H
#define DP_DEBUG_H

/*---------------------------------------------------------------------------*/
/* prototypes                                                                */
/*---------------------------------------------------------------------------*/
extern void print_help              ( void );
extern void print_org_para          ( void );
extern void print_prm_data          ( void );
extern void print_cfg_data          ( void );
extern void print_sap_list          ( void );
extern void print_serial_inputs     ( void );
extern void fatal_error             ( DP_ERROR_FILE file, UWORD line, VPC3_ERRCB_PTR errcb_ptr );

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DP_DEBUG.h is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2004. Confidential.                         */
/*****************************************************************************/