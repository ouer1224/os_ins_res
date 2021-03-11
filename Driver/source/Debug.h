#ifndef   DEBUG_H
#define   DEBUG_H

#include "stm32f10x.h"


#define print_hexbyte( _b )         print_hexbyte__(_b)
#define print_hexword( _w )         print_hexword__(_w)
#define print_hexlong( _dw )        print_hexlong__(_dw)
#define print_string( _p )          print_string__(_p)
#define print_decimal(_x)           print_decimal__(_x)
#define print_declong(_x)           print_declong__(_x)

extern void print_hexbyte__ ( u8 b );
extern void print_hexword__ ( u16 w );
extern void print_hexlong__ ( u32 dw );
extern void print_declong__(u32 dw);

extern void print_string__  ( u8 *p );
extern void start_serial_communication();
void print_declong__(u32 dw);
void init_serio__(void  );
//void  WaitSerialPort(void);


#endif

