#include "motor.h"

void InitMotor(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIO Configuration */
	GPIO_InitStructure.GPIO_Pin = MOTOR_FRONT_PIN | MOTOR_BACK_PIN | MOTOR_LEFT_PIN | MOTOR_RIGHT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);
	
	/* Connect TIM4 pins to AF2 */  
	GPIO_PinAFConfig(GPIOB, MOTOR_FRONT_PIN_SOURCE, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, MOTOR_BACK_PIN_SOURCE, GPIO_AF_TIM4); 
  GPIO_PinAFConfig(GPIOB, MOTOR_LEFT_PIN_SOURCE, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, MOTOR_RIGHT_PIN_SOURCE, GPIO_AF_TIM4);
	
	  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 2040;
  TIM_TimeBaseStructure.TIM_Prescaler = 83;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = MOTOR_IDLE_VALUE;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = MOTOR_IDLE_VALUE;

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = MOTOR_IDLE_VALUE;

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = MOTOR_IDLE_VALUE;

  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

void MotorUpdate(void)
{
	if(status.armed == true)
	{
		// PWM1 Mode configuration: Channel1 PB6 nærmest mpu hvit
		TIM4->CCR1= motor.front;
		// PWM1 Mode configuration: Channel2 PB7 Rød
		TIM4->CCR2= motor.back;
		// PWM1 Mode configuration: Channel3 PB8 Oransj
		TIM4->CCR3= motor.left;
		// PWM1 Mode configuration: Channel4 PB9 Brun
		TIM4->CCR4= motor.right;
	}
	else
	{
		TIM4->CCR1 = MOTOR_IDLE_VALUE;
		TIM4->CCR2 = MOTOR_IDLE_VALUE;
		TIM4->CCR3 = MOTOR_IDLE_VALUE;
		TIM4->CCR4 = MOTOR_IDLE_VALUE;
	}
}
