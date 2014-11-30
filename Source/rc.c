#include "rc.h"

rc_t rc;
int16_t rcData[NUM_RC_CHANS];

void InitRC(void)
{
	// Common typedefs
	NVIC_InitTypeDef NVIC_InitStructure;

	// Input Capture
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 83;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	GPIO_InitStructure.GPIO_Pin =  RC_PPM_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RC_PPM_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(RC_PPM_PORT, RC_PPM_PIN_SOURCE, GPIO_AF_TIM3);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable capture*/
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; /* input pins directly connected */
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
 /* Enable TIM3 */
 TIM_Cmd(TIM3, ENABLE);

 /* Enable CC1-4 interrupt */
 TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);

 /* Clear CC1-4 Flag*/
 TIM_ClearFlag(TIM3, TIM_FLAG_CC1);	
}

void ComputeRC(void)
{
	rc.roll = 		rcData[ROLL];
	rc.pitch = 		rcData[PITCH];
	rc.yaw = 			rcData[YAW];
	rc.throttle = rcData[THROTTLE];
	rc.aux1 = 		rcData[AUX1];
	rc.aux2 = 		rcData[AUX2];
	rc.aux3 = 		rcData[AUX3];
	rc.aux4 = 		rcData[AUX4];
}

void TIM3_IRQHandler(void)
{
    uint16_t diff;
    static uint16_t now;
    static uint16_t last = 0;
    static uint8_t  chan = 0;

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
    {
        last = now;
        now = TIM_GetCapture4(TIM3);
    }

    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

    diff = now - last;

    if (diff > 2700 * 2)   // Per http://www.rcgroups.com/forums/showpost.php?p=21996147&postcount=3960
    {                      // "So, if you use 2.5ms or higher as being the reset for the PPM stream start,
        chan = 0;          // you will be fine. I use 2.7ms just to be safe."
    }
    else
    {
        if (diff > 750 * 2 && diff < 2250 * 2 && chan < 8)    // 750 to 2250 ms is our 'valid' channel range
        {
            rcData[chan] = diff;
        }
        chan++;
    }	
}
