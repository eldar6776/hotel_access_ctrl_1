/**
  * @file    GPIO.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.6.9
  * @brief   GPIO Driver
  */
#include "Gpio.h"

/**
  * @brief  初始化LED管脚
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED_GPIO_PORT,LED1_GPIO_PIN|LED2_GPIO_PIN);
}

/**
  * @brief  翻转端口电平
  * @param  选择相应LED 1-LED1 2-LED2
  * @retval None
  */
void LED_Toggle(uint8_t led)
{
  if (led == 1) {
    LED_GPIO_PORT->ODR ^= LED1_GPIO_PIN;	            //GPIOD->ODR端口输出数据寄存器
  } else if (led == 2) {
    LED_GPIO_PORT->ODR ^= LED2_GPIO_PIN; 
  }
}

/**
  * @brief  初始化蜂鸣器控制管脚PD3
  * @param  None
  * @retval None
  */
void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);

}

/**
  * @brief  初始化按键检测引脚PA0、PD11
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                   //中断分组2,占4个抢占优先级和4个响应优先级

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                          //按键S2
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                          //按键S3
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;          
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;               //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
