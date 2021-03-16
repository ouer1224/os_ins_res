


#include <stdarg.h>
#include "os_sprintf.h"


#define MaxStringLen	128

static uint8_t s_buf[MaxStringLen];

extern void USART1_SendByte(unsigned char temp);


uint8_t os_puts(uint8_t *str,uint32_t len)
{
	while(len>0)
	{
		USART1_SendByte(*str);
		str++;
		len--;
	}
	
	return len;
}



uint32_t os_sprintf_x(uint32_t d,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	tmp=d;
	do
	{
		i++;
		tmp=tmp>>4;
	}while(tmp>0);

	len=i;
	do
	{
		i--;
		tmp=(d&0x0f);
		if(tmp<10)
		{
			buf[i]='0'+tmp;
		}
		else
		{
			buf[i]=tmp-10+'a';
		}
		d=d>>4;
	}while(d>0);

	return len;
}

uint32_t os_sprintf_d(uint32_t d,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	tmp=d;
	do
	{
		i++;
		tmp=tmp/10;
	}while(tmp>0);

	len=i;
		
	do
	{
		i--;
		tmp=(d%10);
		if(tmp<10)
		{
			buf[i]='0'+tmp;
		}
		else
		{
			buf[i]=tmp-10+'a';
		}
		
		
		d=d/10;
	}while(d>0);

	return len;
}





uint32_t os_sprintf_f(double f,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	
	tmp=f;
	len=os_sprintf_d(tmp,buf);
	f=f-tmp;
	tmp=f*1000;
	buf[len]='.';
	len++;
	len=len+os_sprintf_d(tmp,buf+len);

	return len;
}


uint32_t os_printf(uint8_t *str , ...)
{
	va_list ap;
	uint32_t count=0;
	uint32_t len=0;
	uint32_t val_d=0;
	double val_f=0;
	uint8_t *pr_buf=s_buf;
	
	va_start(ap,str);
	while(count<MaxStringLen)
	{
		count++;
		if(*str==0)
		{
			goto label_exit;
		}
		*pr_buf=*str;

		if(*str=='%')
		{
			
			str++;	
			switch(*str)
			{
				case 'd':
				{
					val_d=va_arg(ap,int);
					//printf("arg= d =%d\n",val_d);
					len=os_sprintf_d(val_d,pr_buf);
					pr_buf+=len-1;
				}
				break;
				
				case 'x':
				case 'X':
				{
					val_d=va_arg(ap,int);
					//printf("arg= d =0x%x\n",val_d);
					len=os_sprintf_x(val_d,pr_buf);
					pr_buf+=len-1;
				}
				break;
				case 'f':
				{
					val_f=va_arg(ap,double);
					//printf("arg= d =%f\n",val_f);
					len=os_sprintf_f(val_f,pr_buf);
					pr_buf+=len-1;
				}
				break;
				
				default:
				break;
		
			}			
		}
		
		pr_buf++;
		str++;

	}

label_exit:
	va_end(ap);
	
	len=(uint32_t)pr_buf-(uint32_t)s_buf;
	s_buf[len]=0;
	
	
	os_puts(s_buf,len);
	
	return len;
}






