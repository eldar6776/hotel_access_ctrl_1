/**
  * @file    sccb_bus.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.11.09
  * @brief   sccb bus
  */
#include "sccb_bus.h"

 uint32_t ntime;

/**
  * @brief  延时nms*1 ms
  * @param  nms:延时长度
  * @retval None
  */
void systick_delay_ms(u16 nms)
{	 		  	  
	ntime = nms;
	SysTick_Config(180000);//1ms产生一次中断
	while(ntime);
	SysTick->CTRL = 0x00;			  	    
}   

/**
  * @brief  延时nus*1 us
  * @param  nus:延时长度
  * @retval None
  */
void systick_delay_us(u32 nus)
{		
	ntime = nus;
	SysTick_Config(180);  //1us产生一次中断
	while(ntime);
	SysTick->CTRL = 0x00;
}

/**
  * @brief  初始化总线
  * @param  None
  * @retval None
  */
void sccb_bus_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  //SCCB_SIC:SCCB_SID
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
  * @brief  设置SCCB_SID为输出
  * @param  None
  * @retval None
  */
void sccb_bus_data_out(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void sccb_bus_data_in(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void sccb_bus_start(void)
{
    SCCB_SID_H();             //数据线高电平
    systick_delay_us(50);
    SCCB_SIC_H();	           //在时钟线高的时候数据线由高至低
    systick_delay_us(50);
    SCCB_SID_L();
    systick_delay_us(50);
    SCCB_SIC_L();	           //数据线恢复低电平，单操作函数必要
    systick_delay_us(50);
}


void sccb_bus_stop(void)
{
    SCCB_SID_L();
    systick_delay_us(50);
    SCCB_SIC_H();	
    systick_delay_us(50);  
    SCCB_SID_H();	
    systick_delay_us(50);  
}


void sccb_bus_send_noack(void)
{	
	SCCB_SID_H();	
	systick_delay_us(50);	
	SCCB_SIC_H();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SID_L();	
	systick_delay_us(50);
}

void sccb_bus_send_ack(void)
{	
	SCCB_SID_L();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SIC_H();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SID_L();	
	systick_delay_us(50);
}

uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++) //循环8次发送数据
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else 
		{
			SCCB_SID_L();
		}
		systick_delay_us(50);
		SCCB_SIC_H();	
		systick_delay_us(50);
		SCCB_SIC_L();	
		systick_delay_us(50);
	}
	systick_delay_us(10);
	SCCB_DATA_IN;
	systick_delay_us(50);
	SCCB_SIC_H();	
	systick_delay_us(100);
	if(SCCB_SID_STATE)
	{
		tem = 0;                //SDA=1发送失败
	}
	else 
	{
		tem = 1;                //SDA=0发送成功，返回1
	}

	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_DATA_OUT;
	return tem;  
}

uint8_t sccb_bus_read_byte(void)
{	
	uint32_t i;
	uint8_t read = 0;
	
	SCCB_DATA_IN;
	systick_delay_us(50);
	for(i = 8; i > 0; i--) //循环8次接收数据
	{		     
		systick_delay_us(50);
		SCCB_SIC_H();
		systick_delay_us(50);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1; 
		}
		SCCB_SIC_L();
		systick_delay_us(50);
	}	
    SCCB_DATA_OUT;
	return read;
}

