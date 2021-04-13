

#ifndef __SMALLMEM_H__
#define __SMALLMEM_H__



uint32_t init_mem(void* pool,uint32_t size);
void * os_malloc(uint32_t size);
uint32_t os_free(void *pr);



#endif
