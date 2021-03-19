
#ifndef __OS_DEF_H__
#define __OS_DEF_H__

#include "os_errCode.h"
#include "task.h"

#define os_true 			1
#define os_false			0
#define os_null_pr			0xffffffff
#define os_timeout			0xfffffffe

#if 0
#define os_kernel_save_line(member)			(gOS_kernel_err_code.member.line=__LINE__)
#define os_kernel_save_err(member,val)		((gOS_kernel_err_code.member.err=val)?val:0)
#define os_kernel_save_err_line(member,val)	((os_kernel_save_line(member))?((os_kernel_save_err(member,val))):0)
#define os_kernel_save_time(member)			(gOS_kernel_err_code.member.time=get_OS_sys_count())	//此处有问题,当a=0的时候,其表达式返回值是0.就无法通过了
#define os_kernel_err(member,val) 			((os_kernel_save_time(member))?((os_kernel_save_err_line(member,val))):0)
#define os_kernel_val(member,val)			((val==os_true)?os_true:(os_kernel_err(member,val)))
#else
/*优化以后的方式,避免了if(a=0)其实就是if(0)的情况,实际使用情况待测试*/
#define os_kernel_save_line(member)			((gOS_kernel_err_code.member.line=__LINE__)*0)
#define os_kernel_save_err(member,val)		((gOS_kernel_err_code.member.err=val)*0?0:val)
	
#define os_kernel_save_err_line(member,val)	((os_kernel_save_line(member))?0:((os_kernel_save_err(member,val))))
#define os_kernel_save_time(member)			((gOS_kernel_err_code.member.time=get_OS_sys_count())*0)	

#define os_kernel_err(member,val) 			((os_kernel_save_time(member))?0:((os_kernel_save_err_line(member,val))))
#define os_kernel_val(member,val)			((val==os_true)?os_true:(os_kernel_err(member,val)))
#endif

#define __isnbitseted_array(array,N)		((((uint8_t *)array)[(N)/8]&(0x01<<(N%8)))?1:0)				
#define __clearnbit_array(array,N)			(((uint8_t *)array)[(N)/8]&=(~(0x01<<(N%8))))
#define __setnbit_array(array,N)			(((uint8_t *)array)[(N)/8]|=((0x01<<(N%8))))


//#define close_all_interruct() 	do{__asm volatile ("cpsid i" : : : "memory");}while(0)
//#define open_all_interruct() 	do{__asm volatile ("cpsie i" : : : "memory");}while(0)

#define input_critical_area()		__input_critical_area()//do{	if(checkInAppOrInterr()==1){ close_all_interruct();}else{while(1);} }while(0)
#define exit_critical_area()		__exit_critical_area()//do{	if(checkInAppOrInterr()==1){ open_all_interruct();}else{while(1);} }while(0)


#endif








