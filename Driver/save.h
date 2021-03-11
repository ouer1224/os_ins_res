 //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 参数保存 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//*文件名称:save.c

//*文件作用:参数保存

//*文件作者:翟  鹏

//*创建日期:2008年10月
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



#ifndef SAVE_H
#define SAVE_H


 #define DEBUG_RECEIVE_CHAR 	   read_usart3_char
//参数保存数据结构类型
typedef struct
{
	u8 sn;//序列号
	u8 route_addr;//路由地址
			
	//校验和			
	u16 crc;
}saveparam_t;
extern saveparam_t saveparam;



extern void save_init(void);
extern void save_load_default(void);
extern void save_param(void);
void download(void);
static u8 receive(u8 *ch);
static u8 wiat_char(u8 ch);
void  SavaParamter(void);
void downloadYmodem(void);
#endif


