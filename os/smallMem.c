


#undef assert		
#define assert(val) while(!val)


#if 0
#include "stdlib.h"
#include "stdio.h"
#define debug_out(formate,...)	printf(formate, ##__VA_ARGS__)
#else
#define debug_out(formate,...)
#endif


#ifndef uint32_t

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef char int8_t;

#endif


#ifndef null
#define null ((void *)0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


#define code_ok 		1
#define code_err 		0xffffffff
#define code_prnone		0


#define pradd(a,b)	(((uint32_t)(a)+(uint32_t)(b)))
#define prsub(a,b)	((uint32_t)(a)-(uint32_t)(b))
#define prxor(a,b)	((uint32_t)(a)^(uint32_t)(b))



#define fun_prv		static
#define fun_glo		extern 

#define min_block_size	4


//!����ʹ��xor������,���Խ�Լ1��ָ��Ŀռ�.����1/3���˷�
struct memCB
{
	uint32_t size;
	struct memCB *xor;//!xor=pre^next.ê���preΪ0,�����1���ڵ��nextΪ0.
};

//!header���洢�κ���Ϣ,��������Ϊ�����ê��.�����Ľڵ�����ڴ洢��Ϣ.
//�������xorȥʵ��˫������,���ǲ����ػ�����.
//!��˲�����ֿսڵ�,��������1��header.
static struct memCB s_header_noused={0,null};
static struct memCB * pr_header_noused=&s_header_noused;
static uint32_t s_mem_end_address=0,s_mem_start_address=0;


#define getNextNode(cur,pre)	((void *)prxor((cur)->xor,(pre)))
#define getPreNode(cur,next)	((void *)prxor((cur)->xor,(next)))



fun_prv uint32_t os_memcpy(uint8_t *dest,uint8_t *src,uint32_t len)
{
	if((dest==NULL)||(src==NULL)||(len==0))
	{
		return code_prnone;
	}

	while (len!=0)
	{
		len--;
		*dest=*src;
		dest++;
		src++;
	}
	
	return code_ok;
}




/***********************************************
 *fun     :��С�ڴ�Ĺ���ʽ,��ʼ��mem,��СblockΪmin_block_size(4)
			ֻ����1��noused������
 *name    :
 *var     :
 *return  :
 ************************************************/
fun_glo uint32_t init_mem(void* pool,uint32_t size)
{
	struct memCB *prmem=null;

	if(pool==NULL)
	{
		return code_prnone;
	}

	prmem=pool;
	prmem->size=size-sizeof(struct memCB);
	prmem->xor=(void*)prxor(pr_header_noused,NULL);

	s_header_noused.size=0;
	s_header_noused.xor=(void *)prxor(NULL,prmem);

	s_mem_start_address=(uint32_t)pool;
	s_mem_end_address=pradd(pool,size);

	return code_ok;
}

fun_prv void * __os_malloc(uint32_t size)
{
	struct memCB *prmem=NULL;
	uint32_t leftsize=0;

	struct memCB *pre=NULL;
	struct memCB *next =NULL;
	struct memCB *cur=NULL;

	size=size/min_block_size*min_block_size+(((size%min_block_size)==0)?0:min_block_size);

	cur=pr_header_noused;
	pre=NULL;
	next=NULL;
	prmem=getNextNode(cur,pre);
	pre=pr_header_noused;
	cur=prmem;
	while(cur!=NULL)
	{

		leftsize=cur->size;
		if(leftsize>=size)
		{
			if(leftsize<(size+sizeof(struct memCB)+4))
			{
				prmem=cur;
				//�����noused������ɾ��
				next=getNextNode(cur,pre);
				if(next!=NULL)
				{
					//pre->xor=pre->xor^cur^next;
					//next->xor=next->xor^cur^pre;
					pre->xor=(void*)prxor(pre->xor,cur);
					pre->xor=(void*)prxor(pre->xor,next);

					next->xor=(void*)prxor(next->xor,cur);
					next->xor=(void*)prxor(next->xor,pre);
				}
				else
				{
					pre->xor=(void*)prxor(pre->xor,cur);
					pre->xor=(void*)prxor(pre->xor,next);
				}

			}
			else//!�Ӹߵ�ַȡblock,�ṩ��user
			{
				prmem=(void *)pradd(cur,leftsize+sizeof(struct memCB));
				debug_out("==pr=%p\n",prmem);
				prmem=(void *)prsub(prmem,size+sizeof(struct memCB));
				debug_out("==prmem=%p\n",prmem);
				prmem->size=size;
				prmem->xor=prmem;

				cur->size=leftsize-size-sizeof(struct memCB);
			}

			break;
		}
		prmem=getNextNode(cur,pre);
		pre=cur;
		cur=prmem;

	}

	return prmem;
}


fun_glo void * os_malloc(uint32_t size)
{
	void *pr=NULL;

	if(size==0)
	{
		return NULL;
	}

	pr=__os_malloc(size);
	
	pr=(void *)pradd(pr,sizeof(struct memCB));

	return pr;
}

/***********************************************
 *fun     :�������block�ĵ�ַ�Ƿ���������.
 *name    :
 *var     :
 *return  :code_ok:����,code_err:������.code_prnone:��ָ��
 ************************************************/
uint32_t check_block_consecutive(struct memCB *cur,struct memCB *tar)
{
	uint32_t sizea=0;
	uint32_t sizeb=0;

	if((cur==NULL)||(tar==NULL))
	{
		return code_prnone;
	}

	sizea=cur->size+sizeof(struct memCB);
	sizeb=tar->size+sizeof(struct memCB);

	if((prsub(tar,cur)==sizea)||(prsub(cur,tar)==sizeb))
	{
		debug_out("--can merge\n");
		debug_out("a=%p sizea=%d b=%p sizeb=%d  diff=%d \n",cur,cur->size,tar,tar->size,prsub(tar,cur));
		return code_ok;;
	}

	return code_err;
}

/***********************************************
 *fun     :�����ڴ�ϲ�,�ϲ��ɹ���,���غϲ����block,ʧ����ֱ�ӽ�cur���뵽noused����,������NULL
 *name    :
 *var     :
 *return  :
 ************************************************/
struct memCB * try_memory_merge(struct memCB *head,struct memCB *tar)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;	
	uint32_t size=0;

	cur=head;
	pre=NULL;
	next=getNextNode(cur,pre);
	//head�ǲ�Я����Ϣ��,�ұض�����,ֻ������ê��.�����Ľڵ��вŻ�Я����Ϣ.
	while(next!=NULL)
	{
		pre=cur;
		cur=next;
		next=getNextNode(cur,pre);

		if(check_block_consecutive(cur,tar)==code_ok)
		{
			//��cur��������ɾ��,Ȼ���ٺ�tar�ϲ�.�ٽ��ϲ����block����.��ʱ�������봦��.	
			debug_out("merg pre=%p cur=%p next=%p\n",pre,cur,next);

			pre->xor=(void*)prxor(pre->xor,cur);
			pre->xor=(void*)prxor(pre->xor,next);

			if(next!=NULL)
			{
				next->xor=(void*)prxor(next->xor,cur);
				next->xor=(void*)prxor(next->xor,pre);
			}
			

			if((uint32_t)cur<(uint32_t)tar)
			{
				cur->size=cur->size+tar->size+sizeof(struct memCB);
				return cur;
			}
			else
			{
				tar->size=cur->size+tar->size+sizeof(struct memCB);
				return tar;
			}


		}


	}


	return NULL;
}


/***********************************************
 *fun     :���ڴ���ͷŻ�ȥ.������noused��,���غ���Ҫ��֤�ڴ�鰴�մ�С,��С�����������.
 *name    :
 *var     :
 *return  :
 ************************************************/
fun_prv uint32_t __os_free(struct memCB *prmem)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;	

	uint32_t size=0;
	
	if(prmem==NULL)
	{
		return code_prnone;
	}

	do
	{
		cur=try_memory_merge(pr_header_noused,prmem);
		if(cur!=NULL)
		{
			prmem=cur;
			debug_out("--merge=%p size=%d\n",prmem,prmem->size);
		}
	}while(cur!=NULL);

	cur=pr_header_noused;
	pre=NULL;
	next=getNextNode(cur,pre);

	if(next==NULL)
	{
		cur->xor=(void*)prxor(NULL,prmem);
		prmem->xor=(void *)prxor(cur,NULL);
	}

	while(next!=NULL)
	{
		pre=cur;
		cur=next;
		next=getNextNode(cur,pre);

		debug_out("cur=%p pre=%p next=%p\n",cur,pre,next);
		debug_out("mem.size=%d cur.size=%d\n",prmem->size,cur->size);
		size=cur->size;
		if(prmem->size<=size)//!��prmem���뵽cur��ǰ��
		{
			pre->xor=(void*)prxor(pre->xor,cur);
			pre->xor=(void*)prxor(pre->xor,prmem);
			prmem->xor=(void*)prxor(pre,cur);

			cur->xor=(void*)prxor(prmem,next);
			debug_out("inser---cur=%p pre=%p next=%p\n",cur,pre,next);
			debug_out("inser--xor=%p\n",prmem->xor);
			debug_out("inser--pre.xor=%p\n",pre->xor);

			break;
		}
	}

	return code_ok;
}


fun_glo uint32_t os_free(void *pr)
{
	struct memCB *prmem=NULL;

	prmem=(void*)prsub(pr,sizeof(struct memCB));
	if(((uint32_t)prmem<(uint32_t)s_mem_start_address)||((uint32_t)prmem>=s_mem_end_address))
	{
		return code_prnone;
	}
	if(prmem->size==0)
	{
		assert(0);
	}

	return __os_free(prmem);

}

void readMemMap(void)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;
	struct memCB *pr=NULL;

	cur=pr_header_noused;
	next=getNextNode(cur,pre);
	while(next!=NULL)
	{
		pre=cur;
		cur=next;
		next=getNextNode(cur,pre);
		debug_out("cur size=%d addr=%p\n",cur->size,cur);
		debug_out("next=%p\n",next);
	}
}


#undef assert







#if 0

#define buf_size	1024
static uint8_t s_buf[buf_size];

uint32_t main(void)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;
	//struct memCB *pr[48];
	void *pr[48];
	int seed=0;
	int size=0;
	
	uint32_t i=0;
	uint32_t st=code_err;

	srand(seed);

	st=init_mem(s_buf,buf_size);
	cur=s_buf;
	debug_out("size=%d\n",cur->size);
	debug_out("buf st=%x startaddr = %p ,endaddr=%p\n",st,s_buf,s_buf+buf_size);


	readMemMap();

	for(i=1;i<24;i++)
	{
		debug_out("\n-------i=%d--------------\n",i);
		size=rand()%25;
		debug_out("request size=%d\n",size);
		//pr[i]=__os_malloc(size);
		pr[i]=os_malloc(size);
		//debug_out("ml add=%p,size=%d\n",pr[i],pr[i]->size);
		readMemMap();
		debug_out("\n~~~~~~~~~~~~~~~~~~~~~\n");
	}



	debug_out("\n\n----------free----------------\n\n");
	for(i=1;i<24;i++)
	{
		debug_out("\n-------free i=%d--------------\n",i);
		//debug_out("free %p size=%d\n",pr[i],pr[i]->size);
		//__os_free(pr[i]);
		os_free(pr[i]);
		readMemMap();
		debug_out("\n~~~~~~~~~~~~~~~~~~~~~\n");
	}


	debug_out("\n========================\n");
	
	return 0;
}


#endif





