#include"dma.h"
extern u8 *DMA_DR_BASE;
extern u8 *DMA_DT_BASE; 
extern u16 rx_size;	
extern u16 tx_size;
/*
 * ��������DMA_Config
 * ����  ��DMA ���ڵĳ�ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
uint8_t SendBuff[SENDBUFF_SIZE];

void DMA_Config_Channel4(u8 size)
{
    DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//����DMAʱ��
	NVIC_Config();	   			//����DMA�ж�
  	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   
	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_DR_BASE;
	/*���򣺴��ڴ浽����*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = size;
	/*�����ַ����*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	/*�������ݵ�λ*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*�ڴ����ݵ�λ 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	/*DMAģʽ��һ�δ��䣬ѭ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	/*���ȼ�����*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/*����DMA1��4ͨ��*/		   
    DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
	DMA_Cmd (DMA1_Channel4,ENABLE);					//ʹ��DMA
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�

}

/*
 * ��������NVIC_Config
 * ����  ��DMA �ж�����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    /* Enable the DMAChannel5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
