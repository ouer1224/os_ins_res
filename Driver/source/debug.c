/*---------------------------------------------------------------------------*/
/* functions                                                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void print_string__(UBYTE *p)                                           */
/*                                                                           */
/*   function:     print string                                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#include "debug.h"
#include "Hardwaredef.h"
extern u8 TxBuffer[20];
extern u8 RxBuffer[20];
extern u8 Txcounter,Rxcounter;
extern u8 bReadFlag;
static const u8 asccii[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
	                          0x41,0x42,0x43,0x44,0x45,0x46};
static const u8  decascii[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};



void print_string__( u8 *p )
{
 
    while ( *p )
    {
        TxBuffer [Txcounter++] = *p++;
        //Txcounter++;
    }
   // start_serial_communication();
}//void print_string__( UBYTE *p )


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   UBYTE print_hexbyte__(UBYTE b)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_hexbyte__(u8 b)
{
    //TxBuffer[Txlocation++] = asccii[(b >> 4)];
    TxBuffer[Txcounter++]=asccii[(b>>4)];
    //Txcounter++;
    TxBuffer[Txcounter++] = asccii[(b & 0x0F)];
   // Txcounter++;
   // start_serial_communication();
}//void print_hexbyte__(UBYTE b)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   UBYTE print_hexword__(UWORD w)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_hexword__(u16 w)
{
    TxBuffer[Txcounter++] = asccii[(u8)((w >> 12) & 0x000F)];
    //Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((w >> 8) & 0x000F)];
    //Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((w >> 4) & 0x000F)];
    //Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)(w & 0x000F)];
    //Txcounter++;
   // start_serial_communication();
}//void print_hexword__(UWORD w)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   UBYTE print_hexlong__(ULONG dw)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_declong__(u32 dw)
{
    u32 temp,tempmod;
	temp=dw/10000000;
	tempmod=dw%10000000;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];
	
    temp=tempmod/1000000;
	tempmod=tempmod%1000000;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];
	
    temp=tempmod/100000;
	tempmod=tempmod%100000;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];

	
    temp=tempmod/10000;
	tempmod=tempmod%10000;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];

    temp=tempmod/1000;
	tempmod=tempmod%1000;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];

    temp=tempmod/100;
	tempmod=tempmod%100;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];	

    temp=tempmod/10;
	tempmod=tempmod%10;
    TxBuffer[Txcounter++] = asccii[(u8)(temp & 0x000F)];	

	TxBuffer[Txcounter++]= asccii[(u8)(tempmod &0xf)];
	

    //Txcounter++;
    //start_serial_communication();
}//void print_hexlong__(ULONG dw)


void print_decimal__(u16 x)
{
	   u8     value;
	   u16   temp;
	   value=x/1000;
	   TxBuffer[Txcounter++]=asccii[value];
	   temp=x%1000;
	   value=temp/100;
	   TxBuffer[Txcounter++]=asccii[value];
	   temp=temp%100;
	   value=temp/10;
	   TxBuffer[Txcounter++]=asccii[value];
	   temp=temp%10;
	   TxBuffer[Txcounter++]=asccii[temp];
	
}

void print_hexlong__(u32 dw)
{
    TxBuffer[Txcounter++] = asccii[(u8)((dw >> 28) & 0x000F)];
   // Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((dw >> 24) & 0x000F)];
   // Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((dw >> 20) & 0x000F)];
   // Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((dw >> 16) & 0x000F)];
    //Txcounter++;
    TxBuffer[Txcounter++] = asccii[(u8)((dw >> 12) & 0x000F)];
    //Txcounter++;
    TxBuffer [Txcounter++] = asccii[(u8)((dw >> 8) & 0x000F)];
    //Txcounter++;
    TxBuffer [Txcounter++] = asccii[(u8)((dw >> 4) & 0x000F)];
    //Txcounter++;
    TxBuffer [Txcounter++] = asccii[(u8)(dw & 0x000F)];
    //Txcounter++;
    //start_serial_communication();
}//void print_hexlong__(ULONG dw)



/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void start_serial_communication(void)                                   */
/*                                                                           */
/*   function:     starts serial communication                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void start_serial_communication( void )
{
	  
   DMA1_Channel4->CCR=DMACMDDISABLE;
   DMA1_Channel4->CPAR=(u32)UART1_DR_Address;
   DMA1_Channel4->CMAR=(u32)(TxBuffer);
   DMA1_Channel4->CNDTR=Txcounter; 
   DMA_ClearFlag(DMA1_FLAG_TC4);   
   DMA1_Channel4->CCR=DMACMDENABLE;
   Txcounter=0;
   bReadFlag=FALSE;
   Rxcounter=0;
}//void start_serial_communication( void )

void  WaitSerialPort(void)
{

	    while(1)
	    {
	    	  if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET)break;
	    }
        DMA_ClearFlag(DMA1_FLAG_TC4);   		
	    Txcounter=0;
}

void InitSerialPortDMA(void)
{
  DMA_InitTypeDef  DMA_InitStructure;	
  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)UART1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = Txcounter;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);

 
}

void init_serio__(void  )
{
		USART_InitTypeDef USART_InitStructure;
		 
  USART_InitStructure.USART_BaudRate = 38400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);

	
}//void init_serio__ (UWORD baudrate)


