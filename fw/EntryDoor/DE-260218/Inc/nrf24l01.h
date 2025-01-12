/**
 ******************************************************************************
 * File Name          : nrf24l01.h
 * Date               : 08/05/2016 23:15:16
 * Description        : nrf24l01 radio modul services header
 ******************************************************************************
 *
 *	NRF24L01+	STM32Fxxx	DESCRIPTION
 *
 *	GND			GND			Ground
 * 	VCC			3.3V		3.3V
 *	CE			PB6			RF activated pin
 *	CSN			PB5			Chip select pin
 *	SCK			PB13		SCK pin for SPI2
 *	MOSI		PB15		MOSI pin for SPI2
 *	MISO		PB14		MISO pin for SPI2
 *	IRQ			Not used	Interrupt pin. Goes low when active. Pin functionality is active, but not used in library
 ******************************************************************************
 */
#ifndef NRF24L01_H
#define NRF24L01_H    					101		// version


/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


/* Exported defines    -------------------------------------------------------*/
/* Default SPI used */
#define NRF24L01_SPI				&hspi2
#define RADIO_BUFFER_SIZE			32

/* Interrupt masks */
#define NRF24L01_IRQ_DATA_READY     0x40 /*!< Data ready for receive */
#define NRF24L01_IRQ_TRAN_OK        0x20 /*!< Transmission went OK */
#define NRF24L01_IRQ_MAX_RT         0x10 /*!< Max retransmissions reached, last transmission failed */


/* Exported types    ---------------------------------------------------------*/
typedef union _NRF24L01_IRQ_t 
{
	struct 
	{
		uint8_t reserved0:4;
		uint8_t MaxRT:1;     /*!< Set to 1 if MAX retransmissions flag is set */
		uint8_t DataSent:1;  /*!< Set to 1 if last transmission is OK */
		uint8_t DataReady:1; /*!< Set to 1 if data are ready to be read */
		uint8_t reserved1:1;
		
	} F;
	
	uint8_t Status;          /*!< NRF status register value */
	
} NRF24L01_IRQ_t;

/**
 * @brief  Transmission status enumeration
 */
typedef enum _NRF24L01_Transmit_Status_t 
{
	NRF24L01_Transmit_Status_Lost = 0x00,   /*!< Message is lost, reached maximum number of retransmissions */
	NRF24L01_Transmit_Status_Ok = 0x01,     /*!< Message sent successfully */
	NRF24L01_Transmit_Status_Sending = 0xFF /*!< Message is still sending */
	
} NRF24L01_Transmit_Status_t;

/**
 * @brief  Data rate enumeration
 */
typedef enum _NRF24L01_DataRate_t 
{
	NRF24L01_DataRate_2M = 0x00, /*!< Data rate set to 2Mbps */
	NRF24L01_DataRate_1M,        /*!< Data rate set to 1Mbps */
	NRF24L01_DataRate_250k       /*!< Data rate set to 250kbps */
	
} NRF24L01_DataRate_t;

/**
 * @brief  Output power enumeration
 */
typedef enum _NRF24L01_OutputPower_t 
{
	NRF24L01_OutputPower_M18dBm = 0x00,/*!< Output power set to -18dBm */
	NRF24L01_OutputPower_M12dBm,       /*!< Output power set to -12dBm */
	NRF24L01_OutputPower_M6dBm,        /*!< Output power set to -6dBm */
	NRF24L01_OutputPower_0dBm          /*!< Output power set to 0dBm */
	
} NRF24L01_OutputPower_t;



/* Exported variables  -------------------------------------------------------*/
extern volatile uint32_t radio_flags;
extern volatile uint32_t radio_timer;
extern uint8_t radio_rx_buffer[RADIO_BUFFER_SIZE]; 
extern uint8_t radio_tx_buffer[RADIO_BUFFER_SIZE];


/* Exported macros     -------------------------------------------------------*/
#define RADIO_TxBufferReady()			(radio_flags |= (1 << 0))
#define RADIO_TxBufferNotReady()		(radio_flags &= (~(1 << 0)))
#define IsRADIO_TxBufferReady()			((radio_flags & (1 << 0)) != 0)
#define RADIO_RxBufferReady()			(radio_flags |= (1 << 1))
#define RADIO_RxBufferNotReady()		(radio_flags &= (~(1 << 1)))
#define IsRADIO_RxBufferReady()			((radio_flags & (1 << 1)) != 0)

/* Exported functions  -------------------------------------------------------*/
/**
 * @brief  Initializes NRF24L01+ module
 * @param  channel: channel you will use for communication, from 0 to 125 eg. working frequency from 2.4 to 2.525 GHz
 * @param  payload_size: maximum data to be sent in one packet from one NRF to another.
 * @note   Maximal payload size is 32bytes
 * @retval 1
 */
uint8_t NRF24L01_Init(uint8_t channel, uint8_t payload_size);

/**
 * @brief  Sets own address. This is used for settings own id when communication with other modules
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetMyAddress(uint8_t* adr);

/**
 * @brief  Sets address you will communicate with
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetTxAddress(uint8_t* adr);

/**
 * @brief  Gets number of retransmissions needed in last transmission
 * @param  None
 * @retval Number of retransmissions, between 0 and 15.
 */
uint8_t NRF24L01_GetRetransmissionsCount(void);

/**
 * @brief  Sets NRF24L01+ to TX mode
 * @note   In this mode is NRF able to send data to another NRF module
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpTx(void);

/**
 * @brief  Sets NRF24L01+ to RX mode
 * @note   In this mode is NRF able to receive data from another NRF module.
 *         This is default mode and should be used all the time, except when sending data
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpRx(void);

/**
 * @brief  Sets NRF24L01+ to power down mode
 * @note   In power down mode, you are not able to transmit/receive data.
 *         You can wake up device using @ref NRF24L01_PowerUpTx() or @ref NRF24L01_PowerUpRx() functions
 * @param  None
 * @retval None
 */
void NRF24L01_PowerDown(void);

/**
 * @brief  Gets transmissions status
 * @param  None
 * @retval Transmission status. Return is based on @ref NRF24L01_Transmit_Status_t enumeration
 */
NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void);

/**
 * @brief  Transmits data with NRF24L01+ to another NRF module
 * @param  *data: Pointer to 8-bit array with data.
 *         Maximum length of array can be the same as "payload_size" parameter on initialization
 * @retval None
 */
void NRF24L01_Transmit(uint8_t *data);

/**
 * @brief  Checks if data is ready to be read from NRF24L01+
 * @param  None
 * @retval Data ready status:
 *            - 0: No data available for receive in bufferReturns
 *            - > 0: Data is ready to be collected
 */
uint8_t NRF24L01_DataReady(void);

/**
 * @brief  Gets data from NRF24L01+
 * @param  *data: Pointer to 8-bits array where data from NRF will be saved
 * @retval None
 */
void NRF24L01_GetData(uint8_t *data);

/**
 * @brief  Sets working channel
 * @note   Channel value is just an offset in units MHz from 2.4GHz
 *         For example, if you select channel 65, then operation frequency will be set to 2.465GHz.
 * @param  channel: RF channel where device will operate
 * @retval None 
 */
void NRF24L01_SetChannel(uint8_t channel);

/**
 * @brief  Sets RF parameters for NRF24L01+
 * @param  DataRate: Data rate selection for NRF module. This parameter can be a value of @ref NRF24L01_DataRate_t enumeration
 * @param  OutPwr: Output power selection for NRF module. This parameter can be a value of @ref NRF24L01_OutputPower_t enumeration
 * @retval None
 */
void NRF24L01_SetRF(NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr);

/**
 * @brief  Gets NRLF+ status register value
 * @param  None
 * @retval Status register from NRF
 */
uint8_t NRF24L01_GetStatus(void);

/**
 * @brief  Reads interrupts from NRF 
 * @param  *IRQ: Pointer to @ref NRF24L01_IRQ_t where IRQ status will be saved
 * @retval IRQ status
 *            - 0: No interrupts are active
 *            - > 0: At least one interrupt is active
 */
uint8_t NRF24L01_Read_Interrupts(NRF24L01_IRQ_t* IRQ);

/**
 * @brief  Clears interrupt status
 * @param  None
 * @retval None
 */
void NRF24L01_Clear_Interrupts(void);

/* Private */
void NRF24L01_WriteRegister(uint8_t reg, uint8_t value);


#endif
/******************************   END OF FILE  **********************************/
