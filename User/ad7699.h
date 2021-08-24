#ifndef __AD7699_H__
#define __AD7699_H__

#include "../os/os_typedef.h"


#define Select_7699_0()		GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define DisSelect_7699_0()	GPIO_ResetBits(GPIOC, GPIO_Pin_6)	

#define Select_7699_1()		GPIO_SetBits(GPIOC, GPIO_Pin_7)	
#define DisSelect_7699_1()	GPIO_ResetBits(GPIOC, GPIO_Pin_7)


fnDri uint8_t init_ad7699(void);
fnDri uint8_t start_conver_ad7699(void);
fnDri uint16_t LoopReadVal_7699(uint8_t id );
fnDri uint8_t init7699SelectIO(void);
fnDri uint8_t selectWhich7699(uint8_t id,uint8_t command);

#endif
