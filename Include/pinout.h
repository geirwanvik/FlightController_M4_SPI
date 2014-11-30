#ifndef PINOUT_H
#define PINOUT_H

#define PUBLIC
#define PRIVATE static


// LEDS
#define LED_GREEN_PIN								GPIO_Pin_12
#define LED_ORANGE_PIN              GPIO_Pin_13
#define LED_RED_PIN                 GPIO_Pin_14
#define LED_BLUE_PIN                GPIO_Pin_15
#define LED_GPIO_PORT          			GPIOD
// LEDS

// PUSH BUTTON
#define PB_PIN                			GPIO_Pin_0
#define PB_GPIO_PORT          			GPIOA

#define PB_EXTI_LINE          			EXTI_Line0
#define PB_EXTI_PORT_SOURCE   			EXTI_PortSourceGPIOA
#define PB_EXTI_PIN_SOURCE    			EXTI_PinSource0
// PUSH BUTTON

// SERIAL - USART 2
#define USART_TX_PIN								GPIO_Pin_5
#define USART_TX_SOURCE							GPIO_PinSource5
#define USART_RX_PIN								GPIO_Pin_6
#define USART_RX_SOURCE							GPIO_PinSource6
#define USART_GPIO_PORT							GPIOD

#define USART_BAUDRATE							115200
#define USART_BUFFERSIZE						512

#define USART_DMA_CH								DMA_Channel_4
#define USART_DMA_TX_STREAM					DMA1_Stream6
#define USART_DMA_RX_STREAM					DMA1_Stream7
// SERIAL

// SPI - SPI 1
#define SPI_SCK_PIN          				GPIO_Pin_3
#define SPI_SCK_PIN_SOURCE   				GPIO_PinSource3
#define SPI_MISO_PIN         				GPIO_Pin_4
#define SPI_MISO_PIN_SOURCE  				GPIO_PinSource4
#define SPI_MOSI_PIN         				GPIO_Pin_5
#define SPI_MOSI_PIN_SOURCE  				GPIO_PinSource5
#define SPI_GPIO_PORT           		GPIOB

#define SPI_CS_PIN									GPIO_Pin_7
#define SPI_CS_PORT									GPIOE
// SPI

// MOTOR - TIMER 4
#define MOTOR_FRONT_PIN							GPIO_Pin_6
#define MOTOR_FRONT_PIN_SOURCE			GPIO_PinSource6
#define MOTOR_BACK_PIN							GPIO_Pin_7
#define MOTOR_BACK_PIN_SOURCE				GPIO_PinSource7
#define MOTOR_LEFT_PIN							GPIO_Pin_8
#define MOTOR_LEFT_PIN_SOURCE				GPIO_PinSource8
#define MOTOR_RIGHT_PIN							GPIO_Pin_9
#define MOTOR_RIGHT_PIN_SOURCE			GPIO_PinSource9
#define MOTOR_GPIO_PORT							GPIOB

#define MOTOR_IDLE_VALUE						950
// MOTOR

// RC - TIMER 3
#define RC_PPM_PIN									GPIO_Pin_6
#define RC_PPM_PIN_SOURCE						GPIO_PinSource6
#define RC_PPM_PORT									GPIOA
// RC

#endif
