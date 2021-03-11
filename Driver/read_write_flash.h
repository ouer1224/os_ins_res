#ifndef __read_write_flash_h
#define __read_write_flash_h


//扇区大小
#define SECTORSIZE	1024

//#define FLASHAddress  0x08008000						//定义FLASH写入起始地址
////参数保存地址
//#define PARAM_ADDR	FLASH_PARAM_BASE
////存储器分配和映射
//#define FLASH_BASE_ADDR		0x08000000
//#define FLASH_BACKUP_BASE	0x08008000	//备份FLASH区域地址---用于大批量数据存储以及程序下载的备份
//#define FLASH_BACKUP_LENGTH	0x00008000	//备份FLASH区域长度
//#define FLASH_PARAM_BASE	0x0800F000	//参数区基地址


#define FLASHAddress  0x08018000						//定义FLASH写入起始地址
//参数保存地址
#define PARAM_ADDR	FLASH_PARAM_BASE
//存储器分配和映射
#define FLASH_BASE_ADDR		0x08003000
#define FLASH_BACKUP_BASE	0x08018000	//备份FLASH区域地址---用于大批量数据存储以及程序下载的备份
#define FLASH_BACKUP_LENGTH	0x00008000	//备份FLASH区域长度
#define FLASH_PARAM_BASE	0x0801F000	//参数区基地址
#define DP_FLASH_ADDRESS	  FLASHAddress+4
#define DP_FLASH_NADDRESS	  FLASHAddress+6

u32  ReaddataFlash(u32 Addrse);
u8 flash_erase(u32 start, u32 length);
void run_flash_copy_in_ram(u32 length);
void flash_copy(u32 length);
u8 flash_erase(u32 start, u32 length);
u8 flash_write(u8 *buf, u32 start, u32 length);

void save_param(void);
void save_load_default(void);
void save_init(void);

#endif
