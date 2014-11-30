#include "spi.h"

void InitSPI(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = SPI_SCK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(SPI_GPIO_PORT, SPI_SCK_PIN_SOURCE,  GPIO_AF_SPI1);
  GPIO_PinAFConfig(SPI_GPIO_PORT, SPI_MISO_PIN_SOURCE, GPIO_AF_SPI1);
  GPIO_PinAFConfig(SPI_GPIO_PORT, SPI_MOSI_PIN_SOURCE, GPIO_AF_SPI1);
	
	GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN);
	
	SPI_StructInit(&SPI_InitStructure);
	
	SPI_I2S_DeInit(SPI1);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

void WriteByteSPI(uint8_t reg, uint8_t data)
{
	SetCSPin();
	Transfer(reg);
	Transfer(data);
	ResetCSPin();
}

uint8_t ReadByteSPI(uint8_t reg)
{
	uint8_t returnValue;
	uint8_t addr = reg|0x80;
	
	SetCSPin();
	Transfer(addr);
	returnValue = Transfer(0x00);
	ResetCSPin();
	return returnValue;
}

void ReadBytesSPI(uint8_t* buffer, uint8_t length, uint8_t reg)
{
	uint8_t addr = reg | 0x80;
	SetCSPin();
	Transfer(addr);
	for(uint8_t i = 0; i<length; i++)
	{
		buffer[i] = Transfer(0x00);
	}
	ResetCSPin();
}

uint8_t Transfer(uint8_t data)
{
	uint16_t timeout = 0x1000;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        if ((timeout--) == 0)
            return(0);

    SPI_I2S_SendData(SPI1, data);

    timeout = 0x1000;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
        if ((timeout--) == 0)
    	    return(0);

    return((uint8_t)SPI_I2S_ReceiveData(SPI1));
}				

void SetSPIClockDivider(uint16_t divider)
{
	uint16_t BR_CLEAR_MASK = 0xFFC7;
	uint16_t tempRegister;
	SPI_Cmd(SPI1,DISABLE);
	
	tempRegister = SPI1->CR1;
	
    switch (divider)
    {
        case 2:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_2;
            break;

        case 4:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_4;
            break;

        case 8:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_8;
            break;

        case 16:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_16;
            break;

        case 32:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_32;
            break;

        case 64:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_64;
            break;

        case 128:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_128;
            break;

        case 256:
            tempRegister &= BR_CLEAR_MASK;
            tempRegister |= SPI_BaudRatePrescaler_256;
            break;
    }

    SPI1->CR1 = tempRegister;

    SPI_Cmd(SPI1, ENABLE);
}

