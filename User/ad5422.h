#ifndef __AD5422_H__
#define __AD5422_H__

fnDri uint8_t AD5410xWriteReg_chain(uint8_t id, uint8_t RegAddr, uint16_t value);
fnDri uint8_t init_ad5422_chain(void);
fnDri uint8_t set5422VolOut_chain(uint8_t id,uint32_t vol);






#endif
