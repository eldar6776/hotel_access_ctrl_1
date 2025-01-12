/**
  ******************************************************************************
  * @file    rtc.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Header for rtc module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_RTC_H
#define __APP_RTC_H

/* Includes ------------------------------------------------------------------*/
//#include "global_includes.h"
#include <stm32f4xx.h>
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_HSE
//#define RTC_CLOCK_SOURCE_LSI
extern uint8_t RTC_HandlerFlag;
extern int8_t  RTC_Error;
extern RTC_TimeTypeDef   RTC_Time;
extern RTC_DateTypeDef   RTC_Date;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t RTC_Config(void);
void RTC_HttpCgiDateTimeUpdate(uint8_t *ibuff);

#endif /* __APP_RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
