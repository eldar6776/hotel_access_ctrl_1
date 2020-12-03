/**
 ******************************************************************************
 * File Name          : tft_lcd.c
 * Date               : 28/02/2016 23:16:19
 * Description        : tft lcd display software modul
 ******************************************************************************
 *
 *
 *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
#include "dma.h"
#include "dio.h"
#include "eeprom.h"
#include "signal.h"
#include "tft_lcd.h"
#include "rs485.h"
#include "rc522.h"
#include "display.h"


/* Variables  ----------------------------------------------------------------*/
uint16_t BACK_COLOR, POINT_COLOR;
uint8_t bit_byte, font_size;
uint8_t spi_buff[8];
uint8_t asc_1608[1520];
uint16_t asc_1220[1900];
uint16_t asc_1626[2470];


/* Defines    ----------------------------------------------------------------*/
/* Function Prototypes  -----------------------------------------------------*/
static void LCD_WriteByte(uint8_t data);
static void LCD_WriteInt(uint16_t data);


/* Program code   ------------------------------------------------------------*/
void LCD_Init(void)
{
	HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
    LCD_RST_SetHigh();
    DIO_SetOuput();
    HAL_Delay(5U);
    LCD_RST_SetLow();
    DIO_SetOuput();
    HAL_Delay(50U);
    LCD_RST_SetHigh();
    DIO_SetOuput();
    HAL_Delay(200U);

    LCD_WriteRegister(0xefU);
    LCD_WriteByte(0x03U); 
    LCD_WriteByte(0x80U); 
    LCD_WriteByte(0x02U);

    LCD_WriteRegister(0xcfU);
    LCD_WriteByte(0x00U); 
    LCD_WriteByte(0xc1U); 
    LCD_WriteByte(0x30U);
    
    LCD_WriteRegister(0xedU);
    LCD_WriteByte(0x64U); 
    LCD_WriteByte(0x03U); 
    LCD_WriteByte(0x12U);
    LCD_WriteByte(0x81U);
    
    LCD_WriteRegister(0xe8U);
    LCD_WriteByte(0x85U); 
    LCD_WriteByte(0x00U); 
    LCD_WriteByte(0x78U);
    
    LCD_WriteRegister(0xcbU);
    LCD_WriteByte(0x39U); 
    LCD_WriteByte(0x2cU); 
    LCD_WriteByte(0x00U);
    LCD_WriteByte(0x34U); 
    LCD_WriteByte(0x02U);
	    
    LCD_WriteRegister(0xf7U);
    LCD_WriteByte(0x20U);
        
    LCD_WriteRegister(0xeaU);
    LCD_WriteByte(0x00U); 
    LCD_WriteByte(0x00U);

    LCD_WriteRegister(ILI9341_POWER_CTRL_1);
    LCD_WriteByte(0x23U);
    
    LCD_WriteRegister(ILI9341_POWER_CTRL_2);
    LCD_WriteByte(0x10U);   
    
    LCD_WriteRegister(ILI9341_VCOM_CTRL_1);
    LCD_WriteByte(0x3eU); 
    LCD_WriteByte(0x28U);
        
    LCD_WriteRegister(ILI9341_VCOM_CTRL_2);
    LCD_WriteByte(0x86U);    
        
    LCD_WriteRegister(ILI9341_MEMORY_ACCESS_CONTROL);
    LCD_WriteByte(0xe8U);    
        
    LCD_WriteRegister(ILI9341_PIXEL_FORMAT_SET);
    LCD_WriteByte(0x55U);
    
    LCD_WriteRegister(ILI9341_FRAME_RATE_CTRL);
    LCD_WriteByte(0x00U); 
    LCD_WriteByte(0x18U);
   
    LCD_WriteRegister(ILI9341_DISP_FUNCTION_CTRL);
    LCD_WriteByte(0x08U); 
    LCD_WriteByte(0x82U); 
    LCD_WriteByte(0x27U);
   
    LCD_WriteRegister(0xf2U);
    LCD_WriteByte(0x00U);
    
    LCD_WriteRegister(ILI9341_GAMMA_SET);
    LCD_WriteByte(0x01U);
    
    LCD_WriteRegister(ILI9341_POSITIVE_GAMMA_CORRECTION);
    LCD_WriteByte(0x0fU);
    LCD_WriteByte(0x31U);
    LCD_WriteByte(0x2bU);
    LCD_WriteByte(0x0cU); 
    LCD_WriteByte(0x0eU);
    LCD_WriteByte(0x08U);
    LCD_WriteByte(0x4eU);
    LCD_WriteByte(0xf1U);
    LCD_WriteByte(0x37U);
    LCD_WriteByte(0x07U);
    LCD_WriteByte(0x10U);
    LCD_WriteByte(0x03U);
    LCD_WriteByte(0x0eU);
    LCD_WriteByte(0x09U);
    LCD_WriteByte(0x00U);

    LCD_WriteRegister(ILI9341_NEGATIVE_GAMMA_CORRECTION);
    LCD_WriteByte(0x00U);
    LCD_WriteByte(0x0eU);
    LCD_WriteByte(0x14U);
    LCD_WriteByte(0x03U);
    LCD_WriteByte(0x11U);
    LCD_WriteByte(0x07U);
    LCD_WriteByte(0x31U);
    LCD_WriteByte(0xc1U); 
    LCD_WriteByte(0x48U);
    LCD_WriteByte(0x08U);    
    LCD_WriteByte(0x0fU);
    LCD_WriteByte(0x0cU);
    LCD_WriteByte(0x31U);
    LCD_WriteByte(0x36U); 
    LCD_WriteByte(0x0fU);  
        
    LCD_WriteRegister(0xb1U);
    LCD_WriteByte(0x00U); 
    LCD_WriteByte(0x10U);
    
    LCD_WriteRegister(ILI9341_SLEEP_OUT); 
    HAL_Delay(200U);
    LCD_WriteRegister(ILI9341_DISP_ON);
}


void LCD_Clear(uint16_t color)
{
	uint32_t i = 640U;
    uint8_t dsp_buff[640];

    LCD_AddressSet(0U, 0U, LCD_W - 1U, LCD_H - 1U);
	LCD_WriteRegister(ILI9341_MEMORY_WRITE);
    HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    
    do
    {
        --i;
        dsp_buff[i] = (color >> 8U);
        --i;
        dsp_buff[i] = (color & 0xFFU);
    } 
    while (i != 0U);
    
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
    i = LCD_H;
    
    do
    {
        if (HAL_SPI_Transmit(&hspi2, dsp_buff, 640U, SPI_FLASH_TIMEOUT) != HAL_OK) Error_Handler();
        --i;
    }
    while(i != 0U);
    
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
    LCD_WriteRegister(ILI9341_NOP);
}


uint8_t LCD_ReadCommand(uint8_t command, uint8_t index) 
{
    spi_buff[0] = 0U;
    LCD_WriteRegister(0xD9U);
    LCD_WriteByte(0x10U + index);
    LCD_WriteRegister(command);
    HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, spi_buff, 1U, 10U);
    HAL_SPI_Receive(&hspi2, spi_buff, 1U, 10U);
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
    return (spi_buff[0]);
}


void LCD_AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_WriteRegister(ILI9341_COLUMN_ADDRESS_SET);
    LCD_WriteInt(x1);
    LCD_WriteInt(x2);
    LCD_WriteRegister(ILI9341_PAGE_ADDRESS_SET);
    LCD_WriteInt(y1);
    LCD_WriteInt(y2);
}


void LCD_WriteRegister(uint8_t data)
{
    HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi2, &data, 1U, SPI_FLASH_TIMEOUT) != HAL_OK) Error_Handler();
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET); 
}


void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num)
{
    uint16_t temp;
    uint8_t pos, t;
    uint16_t colortemp = POINT_COLOR;

    num = num - ' ';
    
    if(font_size == SMALL_SIZE_FONT)
	{
		if ((x > LCD_W - 8) || (y > LCD_H - 16)) return;			
		LCD_AddressSet(x, y, (x + 8 - 1), (y  + 16 - 1));
		LCD_WriteRegister(ILI9341_MEMORY_WRITE);
		
        for (pos = 0; pos < 16; pos++)
		{
            temp = asc_1608[(uint16_t)num * 16 + pos];		 
    
            for (t = 0; t < 8; t++)
			{  
				if (temp & 0x01) POINT_COLOR = colortemp;
                else POINT_COLOR = BACK_COLOR;
                LCD_WriteInt(POINT_COLOR);    
                temp >>= 1;  
            }    
        }
    }
	else if(font_size == MIDDLE_SIZE_FONT)
	{
		if ((x > LCD_W - 12) || (y > LCD_H - 20))return;
		LCD_AddressSet(x, y, (x + 12 - 1), (y  + 20 - 1));  
		LCD_WriteRegister(ILI9341_MEMORY_WRITE);

        for(pos = 0; pos < 20; pos++)
		{
            temp = asc_1220[(uint16_t)num * 20 + pos];
            
            for(t = 0; t < 12; t++)
			{   
				if(temp & 0x8000) POINT_COLOR = colortemp;			
				else POINT_COLOR = BACK_COLOR;
				LCD_WriteInt(POINT_COLOR);    
                temp <<= 1;                  
            }
        }		
    }
	else if(font_size == BIG_SIZE_FONT)
	{
		if ((x > LCD_W - 16) || (y > LCD_H - 26))return;
		LCD_AddressSet(x, y, (x + 16 - 1), (y  + 26 - 1)); 
		LCD_WriteRegister(ILI9341_MEMORY_WRITE);
		
        for(pos = 0; pos < 26; pos++)
		{
            temp = asc_1626[(uint16_t)num * 26 + pos];
            
            for(t = 0; t < 16; t++)
			{
				if(temp & 0x8000) POINT_COLOR = colortemp;			
				else POINT_COLOR = BACK_COLOR;
				LCD_WriteInt(POINT_COLOR);    
                temp <<= 1;                 
            }
        }		
    }
    
    POINT_COLOR = colortemp;	


//    uint32_t temp, pos, n, t;
//    uint16_t colortemp = POINT_COLOR;
//    uint32_t font_cnt, font_i, font_x, font_y, bcnt1, bcnt2;
//    
//    n = num - ' ';

//    
//    if(font_size == SMALL_SIZE_FONT)
//	{
//        font_x = 8U;
//        font_y = 16U;
//        font_cnt = 16U;
//        bcnt1 = 0xFFFFU;
//        bcnt2 = 0xFFFFU;
//    }
//    else if(font_size == MIDDLE_SIZE_FONT)
//	{		
//        bcnt1 = 19U;
//        bcnt2 = 3U;
//        font_x = 12U;
//        font_y = 20U;
//        font_cnt = 20U;
//    }
//    else if(font_size == BIG_SIZE_FONT)
//	{
//        font_x = 16U;
//        font_y = 26U;
//        font_cnt = 26U;
//        bcnt1 = 0xFFFFU;
//        bcnt2 = 0xFFFFU;
//    }
//    
//    LCD_AddressSet(x, y, (x + font_x - 1U), (y  + font_y - 1U));
//    LCD_WriteRegister(ILI9341_MEMORY_WRITE);
//		
//    for (pos = 0U; pos < font_cnt; pos++)
//    {
//        font_i = (n * font_cnt + pos);
//        
//        if(font_size == 1U)         temp = asc_1608[font_i];
//        else if(font_size == 2U)    temp = asc_1220[font_i];
//        else if(font_size == 3U)    temp = asc_1626[font_i];
//        
//        t = font_x;
//        
//        do
//        {   
//            --t;
//            if(temp & (1U << t)) POINT_COLOR = colortemp;			
//            else POINT_COLOR = BACK_COLOR;
//            LCD_WriteInt(POINT_COLOR);          // write pixels for all 32 bits if bcnt1 and bcnt2 not set
//            if(t == bcnt1) t = 15U;             // write pixels for first 12 bits, skeep 4 empty bits,
//            else if(t == bcnt2) t = 0U;         // write pixels for next 12 bits and break loop
//        }
//        while(t != 0);  
//    }
//    
//    POINT_COLOR = colortemp;
}



void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len)
{
    uint32_t t, temp, enshow = 0U;
    
	for(t = 0U; t < len; t++)
	{
		temp =( num / BaseToPower(10U, len - t - 1U)) %10U;
        
		if((enshow == 0U) && (t < (len - 1)))
		{
			if(temp == 0U)
			{
				LCD_ShowChar(( x + 8U * t), y, ' ');
				continue;
			}
			else 
			{
                enshow = 1U;
            }
		}
        
        if(font_size == SMALL_SIZE_FONT) LCD_ShowChar((x + 8U * t), y, (temp + '0'));
		else if(font_size == MIDDLE_SIZE_FONT) LCD_ShowChar((x + 12U * t), y, (temp + '0'));
		else if(font_size == BIG_SIZE_FONT) LCD_ShowChar((x + 16U * t), y, (temp + '0'));
	}
}


void LCD_ShowString(uint16_t x, uint16_t y, const char *p)
{            
    while(*p != '\0')
	{        
		if(font_size == SMALL_SIZE_FONT)
		{
			if(x > (LCD_W - 8U))
			{
				x = 0U;
				y += 16U;
			}

			if(y > (LCD_H - 16U)) y = x = 0U;
			LCD_ShowChar(x, y, *p);
			x += 8U;
			p++;			
		}
		else if(font_size == MIDDLE_SIZE_FONT)
		{
			if(x > (LCD_W - 12U))
			{
				x = 0U;
				y += 20U;
			}

			if(y > (LCD_H - 20U)) y = x = 0U;
			LCD_ShowChar(x, y, *p);
			x += 12U;
			p++;
		} 
		else if(font_size == BIG_SIZE_FONT)
		{
			if(x > (LCD_W - 16U))
			{
				x = 0U;
				y += 26U;
			}

			if(y > (LCD_H - 26U)) y = x = 0U;
			LCD_ShowChar(x, y, *p);
			x += 16U;
			p++;
		}        
    }
}


static void LCD_WriteByte(uint8_t data)
{
	HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi2, &data, 1U, SPI_FLASH_TIMEOUT) != HAL_OK) Error_Handler();
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
}


static void LCD_WriteInt(uint16_t data)
{
	HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    spi_buff[0] = data >> 8U;
    spi_buff[1] = data & 0xFFU;
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi2, spi_buff, 2U, SPI_FLASH_TIMEOUT) != HAL_OK) Error_Handler();
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
}

/******************************   END OF FILE  **********************************/