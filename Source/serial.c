#include "serial.h"

volatile uint8_t TxBuffer[USART_BUFFERSIZE];
volatile uint8_t RxBuffer[USART_BUFFERSIZE];

uint32_t RxBufferPos = 0;
volatile uint16_t TxBufferTail = 0;
volatile uint16_t TxBufferHead = 0;

volatile uint8_t TxDmaCallEnabled = 0;

void InitSerial(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);
  USART_StructInit(&USART_InitStructure);
  DMA_StructInit(&DMA_InitStructure);
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,  ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	

	
	// Configure USART Tx and Rx as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin = USART_TX_PIN | USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
	// GPIO Pins set to alternate functions
	GPIO_PinAFConfig(USART_GPIO_PORT,USART_TX_SOURCE,GPIO_AF_USART2);
	GPIO_PinAFConfig(USART_GPIO_PORT,USART_RX_SOURCE,GPIO_AF_USART2);
  
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
	
	//Configure DMA TX IRQ
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	// Configure USART
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2,&USART_InitStructure);
	
  // Configure RX DMA
	DMA_DeInit(USART_DMA_RX_STREAM);
	
  DMA_InitStructure.DMA_Channel = USART_DMA_CH ;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART2->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)RxBuffer;
	DMA_InitStructure.DMA_BufferSize = USART_BUFFERSIZE;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  
  DMA_Init(USART_DMA_RX_STREAM,&DMA_InitStructure);
	
	DMA_Cmd(USART_DMA_RX_STREAM, ENABLE);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx, ENABLE);
	
	RxBufferPos = DMA_GetCurrDataCounter(USART_DMA_RX_STREAM);
	
  // Configure TX DMA
	DMA_DeInit(USART_DMA_TX_STREAM);
	
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)TxBuffer; 
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;


  DMA_Init(USART_DMA_TX_STREAM,&DMA_InitStructure);
	
	DMA_SetCurrDataCounter(USART_DMA_TX_STREAM,0);
	
	DMA_ITConfig(USART_DMA_TX_STREAM, DMA_IT_TC, ENABLE);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
         
  // Enable USART
  USART_Cmd(USART2, ENABLE);
	
}


void DMA1_Stream6_IRQHandler(void)
{
	DMA_ClearITPendingBit(USART_DMA_TX_STREAM, DMA_IT_TCIF6);
	
	TxDmaCallEnabled = 0;
	
	TransmittData();
	
}

void TransmittData(void)
{
		if((TxDmaCallEnabled == 1) || (TxBufferHead == TxBufferTail))
			return;
		
		USART_DMA_TX_STREAM->M0AR = (uint32_t) &TxBuffer[TxBufferTail];
		if(TxBufferHead > TxBufferTail)
		{
			DMA_SetCurrDataCounter(USART_DMA_TX_STREAM, TxBufferHead - TxBufferTail);
			TxBufferTail = TxBufferHead;
		}
		else
		{
			DMA_SetCurrDataCounter(USART_DMA_TX_STREAM, USART_BUFFERSIZE - TxBufferTail);
			TxBufferTail = 0;
		}
		TxDmaCallEnabled = 1;
		
		DMA_Cmd(USART_DMA_TX_STREAM, ENABLE);
}

uint16_t SerialAvailable(void)
{
	int32_t count = RxBufferPos - DMA_GetCurrDataCounter(USART_DMA_RX_STREAM);
	if(count >= 0)
		return (uint16_t) count;
	else
		return (uint16_t) (USART_BUFFERSIZE + count);
}

uint8_t SerialReadByte(void)
{
	uint8_t data = RxBuffer[USART_BUFFERSIZE - RxBufferPos];
	// go back around the buffer
	if(--RxBufferPos == 0)
		RxBufferPos = USART_BUFFERSIZE;
	
	return data;
}

void SerialWriteByte(uint8_t data)
{
	TxBuffer[TxBufferHead] = data;
	TxBufferHead = (TxBufferHead +1) % USART_BUFFERSIZE;
	
	TransmittData();
}

void SerialWriteBytes(uint8_t* buffer, uint8_t length)
{
	for(uint8_t i = 0; i<length; i++)
	{
		SerialWriteByte(buffer[i]);
	}
}
// Printf retarget to USART
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;


int fputc(int c, FILE *f) 
{
	SerialWriteByte(c);
  return c;
}


