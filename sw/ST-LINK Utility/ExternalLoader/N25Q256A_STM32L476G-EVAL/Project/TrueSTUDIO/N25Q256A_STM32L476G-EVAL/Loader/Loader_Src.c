/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_quadspi.h"
#include "stm32l4xx_rcc.h"
#include "stm32l4xx_conf.h"
#include "stm32l4xx_syscfg.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define QSPI_DDRMODE
#define QSPI_PAGESIZE                                 256
#define IO_READ_DDR_QUAD_CMD                          0xED
#define IO_READ_QUAD_CMD                              0xEB
#define WRITE_ENABLE_CMD                              0x06
#define READ_STATUS_REGISTER_CMD                      0x05
#define FAST_PROGRAM_CMD                              0x32
#define SECTOR_ERASE_CMD                              0xD8



/* Description :																			*/
/* Configuration of the QUADSPI		*/
/* Inputs 	:																				*/
/* 				 None 																		*/
/* outputs 	:																				*/
/* 				 None 																		*/


void QUADSPI_Config(void)
{
  QUADSPI_InitTypeDef QUADSPI_InitStructure;
  QUADSPI_DeInit();

  QUADSPI_InitStructure.QUADSPI_SShift = QUADSPI_SShift_NoShift;
  QUADSPI_InitStructure.QUADSPI_Prescaler = 0x6;
  QUADSPI_InitStructure.QUADSPI_CKMode = QUADSPI_CKMode_Mode0;
  QUADSPI_InitStructure.QUADSPI_CSHTime = QUADSPI_CSHTime_1Cycle;
  QUADSPI_InitStructure.QUADSPI_FSize = 0x1F;
  QUADSPI_Init(&QUADSPI_InitStructure); 
	
  QUADSPI_Cmd(ENABLE);
}


/* Description :																			*/
/* QUADSPI Memory Mapped mode configuration 		                */
/* Inputs 	:																				*/
/* 				 None 																		*/
/* outputs 	:																				*/
/* 				 None 																		*/

void QUADSPI_MappedMode()
{
 QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
	
#ifdef QSPI_DDRMODE
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Enable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = IO_READ_DDR_QUAD_CMD ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles  = 8;
#else
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = IO_READ_QUAD_CMD
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles  = 10;
#endif /* QSPI_DDRMODE */

  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Memory_Mapped;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_4Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_4Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line;
	
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 

  /* Remap QUADSPI on zero address */
  SYSCFG_ExternalMemorySwapping(ENABLE);
}


/* Description :																			*/
/* Clock and GPIO configuration 		                */
/* Inputs 	:																				*/
/* 				 None 																		*/
/* outputs 	:																				*/
/* 				 None 																		*/
void GPIO_Clock_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
   /* Enable QUADSPI Clock*/
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_QSPI,ENABLE);

  /* Enable GPIO AHB2 clock */
   RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA|RCC_AHB2Periph_GPIOB|RCC_AHB2Periph_GPIOE
                          |RCC_AHB2Periph_GPIOG,ENABLE);
   
   /* Enable APB2 clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
   
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_QSPI);*/
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_QSPI);*/
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_QSPI);*/
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_QSPI);*/
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_QSPI);*/
/*   GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_QSPI);*/
/*
/*   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;*/
/*   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;*/
/*   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;*/
/*   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;*/
/*
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);*/
/*
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);*/
/*
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);*/
/*
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);*/
/*
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);*/
/*   */
/*   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;*/
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;*/
/*   GPIO_Init(GPIOE,& GPIO_InitStructure);  */
 
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_QSPI);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_QSPI);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_QSPI);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_QSPI);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_QSPI);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_QSPI);
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_Init(GPIOA,& GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_Init(GPIOA,& GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_Init(GPIOB,& GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_Init(GPIOB,& GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOB,& GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_Init(GPIOB,& GPIO_InitStructure);
}


/* Description :																			*/
/* Enable de QSPI_Write in the indirect and Auto_Polling mode  		*/
/* Inputs 	:																				*/
/* 				 None 																		*/
/* outputs 	:																				*/
/* 				 None                                     */
void QSPI_WriteEnable(void)
{
  QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
	
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line;
  
  
  QUADSPI_SetFIFOThreshold(0);
  QUADSPI_SetDataLength(0);
  
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = WRITE_ENABLE_CMD ; 
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
  
  QUADSPI_AutoPollingMode_Config(0x02,0x02,QUADSPI_PMM_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0x00);

  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = READ_STATUS_REGISTER_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_SM)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  QUADSPI_ClearFlag(QUADSPI_FLAG_TC);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);

}



/* Description :																			*/
/* send command to the nor flash  		*/
/* Inputs 	:																				*/
/* 			Instruction  																		*/
/* outputs 	:																				*/
/* 				 None 																		*/
void QSPI_SendInstruction(uint8_t Instruction)
{
 QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
	
  QSPI_WriteEnable();
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0 ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line ;
  
  QUADSPI_SetFIFOThreshold(0);
  QUADSPI_SetDataLength(0);
  
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = Instruction ;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure);
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);

}

 


/* Description :																			*/
/* Write Page in the indirect and Auto_Polling mode  		*/
/* Inputs :																					*/
/* 				Address 	: Write location  										*/
/* 				Size 		: Length in bytes 										*/
/* 				buffer 		: Address where to get the data to write		*/
/* outputs 	:																				*/
/* 				 None                                     */
void QSPI_WritePage(uint32_t Address, uint32_t Size , uint8_t* buffer)
{
 QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
	
  if (Size == 0)
  return;
 /* Enable the write access to the NOR_FLASH */	
  QSPI_WriteEnable();
  
   /* Writing Sequence -------------------------------------------------------*/

  QUADSPI_SetDataLength(Size - 1);	
  QUADSPI_SetAddress(Address); 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0 ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_32bit; /* Enable 4 Byte Addressing Mode */
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_4Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = FAST_PROGRAM_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); /* Write Buffer to the Flash */
	
  while(Size--)
  {
    if(QUADSPI_GetFlagStatus(QUADSPI_FLAG_FT))
	QUADSPI_SendData8(* buffer++);
  }
	
  Size = 0;
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_TC)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_TC);
  
  QUADSPI_SetDataLength(0x00);
  QUADSPI_AutoPollingMode_Config(0x00,0x01,QUADSPI_PMM_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0);

  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = READ_STATUS_REGISTER_CMD;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_SM)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
}





/* Description :																			*/
/* Write data to the device	 														*/
/* Inputs :																					*/
/* 				Address 	: Write location  										*/
/* 				Size 		: Length in bytes 										*/
/* 				buffer 		: Address where to get the data to write		*/
/* outputs :																				*/
/* 				"1" 			: Operation succeeded								*/
/* Info :																						*/
/* Align and memory size (32/16/8 bits) is handled in this function 	*/
/* Note : Mandatory for all types except SRAM and PSRAM			*/	

int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
	
  uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  uint32_t   QSPI_DataNum = 0;
  
  Addr = Address % QSPI_PAGESIZE;
  count = QSPI_PAGESIZE - Addr;
  NumOfPage =  Size / QSPI_PAGESIZE;
  NumOfSingle = Size % QSPI_PAGESIZE;
  
  QSPI_SendInstruction(0xB7);/* Enable 4 Byte Addressing Mode */

  if (Addr == 0) /*!< Address is QSPI_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < QSPI_PAGESIZE */
    {
      QSPI_DataNum = Size;      
      QSPI_WritePage(Address, QSPI_DataNum, buffer);
    }
    else /*!< Size > QSPI_PAGESIZE */
    {
      while (NumOfPage--)
      {
        QSPI_DataNum = QSPI_PAGESIZE;
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
        Address +=  QSPI_PAGESIZE;
        buffer += QSPI_PAGESIZE;
      }
      
      QSPI_DataNum = NumOfSingle;
      QSPI_WritePage(Address, QSPI_DataNum, buffer);
    }
  }
  else /*!< Address is not QSPI_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< Size < QSPI_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (Size + Address) > QSPI_PAGESIZE */
      {
        temp = NumOfSingle - count;
        QSPI_DataNum = count;
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
        Address +=  count;
        buffer += count;
        
        QSPI_DataNum = temp;
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
      }
      else
      {
        QSPI_DataNum = Size; 
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
      }
    }
    else /*!< Size > QSPI_PAGESIZE */
    {
      Size -= count;
      NumOfPage =  Size / QSPI_PAGESIZE;
      NumOfSingle = Size % QSPI_PAGESIZE;

      QSPI_DataNum = count;
        
      QSPI_WritePage(Address, QSPI_DataNum, buffer);
      Address +=  count;
      buffer += count;

      while (NumOfPage--)
      {
        QSPI_DataNum = QSPI_PAGESIZE;
        
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
        Address +=  QSPI_PAGESIZE;
        buffer += QSPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        QSPI_DataNum = NumOfSingle;
        
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
      }
    }
  }
  
  QSPI_SendInstruction(0xE9);/* Disable 4 Byte Addressing Mode */
	return 1;
}






/* Description :															*/
/* Write data to the device	 													*/
/* Inputs :																*/
/* 		                EraseStartAddress  									        	*/
/* 				EraseEndAddress 									              	*/
/* outputs :										        					*/
/* 				"1" 			: Operation succeeded						*/
/* Note : Mandatory for all types except SRAM and PSRAM			*/

int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{   

  QUADSPI_ComConfig_InitTypeDef QUADSPI_ComConfig_InitStructure;
  QSPI_WriteEnable();
  
  /* Send Erase Sector Command */
  QUADSPI_SetDataLength(0); 
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DummyCycles = 0 ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_24bit;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Read;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_1Line ;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = SECTOR_ERASE_CMD;
  
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); 
  /* Set sector address to erase */
  QUADSPI_SetAddress(EraseStartAddress);
  
  /* Reconfigure Quadspi to automatic polling mode to wait for end of erase */
	QSPI_WriteEnable();
  QUADSPI_SetDataLength(0x00);
  QUADSPI_AutoPollingMode_Config(0x00,0x01,QUADSPI_PMM_AND);
  QUADSPI_AutoPollingModeStopCmd(ENABLE);
  QUADSPI_SetDataLength(0);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Auto_Polling;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_1Line;
  QUADSPI_ComConfig_InitStructure.QUADSPI_ComConfig_Ins = READ_STATUS_REGISTER_CMD ;
  QUADSPI_ComConfig_Init(&QUADSPI_ComConfig_InitStructure); /* Read SR1 Command */
  
  /* Wait for end of Erase */
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_SM)==RESET);
  QUADSPI_ClearFlag(QUADSPI_FLAG_SM);
  while(QUADSPI_GetFlagStatus(QUADSPI_FLAG_BUSY)==SET);
  
  return 1;	
}





/* Description :																			*/
/* Read data from the device	 														*/
/* Inputs :																					*/
/* 				Address 	: Write location  										*/
/* 				Size 		: Length in bytes 										*/
/* 				buffer 		: Address where to get the data to write		*/
/* outputs :																				*/
/* 				"1" 			: Operation succeeded								*/
/* 				"0" 			: Operation failure										*/
/* Note : Not Mandatory                                		                                	*/	
int Read (uint32_t Address, uint32_t Size, uint8_t* Buffer)
{     	int i = 0;
        QUADSPI_MappedMode();
        for (i=0; i < Size;i++)
        { 
          *(uint8_t*)Buffer++ = *(uint8_t*)Address;
          Address ++;
        }
          
      return 1;
       
} 


/* Description :																			*/
/* Verify the data 	 														*/
/* Inputs :																					*/
/* 				MemoryAddr 	:   Write location  										*/
/* 				RAMBufferAddr 	: RAM Address		*/
/* 				Size 		: Length in bytes 										*/
/* outputs :																				*/
/* 				"0" 			: Operation succeeded								*/
/* Info :																						*/
/* Align and memory size (32/16/8 bits) is handled in this function 	*/
/* Note : Not Mandatory                                         			*/	
int Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size)
{ 
        uint32_t StartAddresse = 0x90000000;
        uint32_t VerifiedData = 0;
        Size*=4;
        
   QUADSPI_MappedMode() ;
   while (Size>VerifiedData)
  {
  if ( *(uint8_t*)StartAddresse++ != *((uint8_t*)RAMBufferAddr+VerifiedData))
  return MemoryAddr+VerifiedData;  
  VerifiedData++;  
        }
        
        return 0;
}






/* Description :																			*/
/* device and initilize the QUADSPI with the chosen configuration 		*/
/* Inputs 	:																				*/
/* 				 None 																		*/
/* outputs 	:																				*/
/* 				"1" 			: Operation succeeded								*/
/* 				"0" 			: Operation failure										*/


int Init (void)
{  
	GPIO_Clock_Config();
        QUADSPI_Config();
	return 1;


}








