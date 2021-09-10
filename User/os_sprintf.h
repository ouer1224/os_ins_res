
#ifndef __OS_SPRINTF_H__
#define __OS_SPRINTF_H__


#ifndef uint8_t 
typedef unsigned char uint8_t;
#endif

#ifndef uint32_t 
typedef unsigned int uint32_t;
#endif


uint32_t os_printf(uint8_t *str , ...);


#define msg_out	//os_printf
#define exception os_printf

#endif
