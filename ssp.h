/*-----------------------------------------------------------------------------*/
/*
 * spp.h
 *
 *  Created on: 1.6. 2021
 *      Author: Martin Polasek
 */
/*-----------------------------------------------------------------------------*/
#ifndef SPP_H_INCLUDED
#define SPP_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>

/* Defines -------------------------------------------------------------------*/

#define SSP_MAGIC           0xA3


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  SSP_STATE_WAIT = 0,
  SSP_STATE_LEN1 = 1,
  SSP_STATE_LEN2 = 2,
  SSP_STATE_DATA = 3,
  SSP_STATE_CC1  = 4,
  SSP_STATE_CC2  = 5

} ssp_state_t;

/**
  * @brief  SSP structure definition
  */
typedef struct
{
  uint8_t *buff;        //buffer pointer

  uint8_t *write;       //write pointer
  uint16_t size;        //buffer size

  uint8_t magic;        //
  ssp_state_t state;    //state
  uint16_t type;        //packet type
  uint16_t len;         //packet length
  uint16_t cc;          //packet CC
  uint16_t ccc;         //receive data CC

  uint32_t err_cnt;     //error counter
  uint32_t pck_cnt;     //packet counter

} ssp_t;

/* Exported functions ------------------------------------------------------- */



// Functions -------------------------------------------------------------------
void ssp_init(ssp_t *ssp, uint8_t *buff, uint16_t size);

uint8_t ssp_receive(ssp_t *ssp, uint8_t byte);
uint16_t ssp_create(uint8_t *buff, uint16_t buff_len, uint8_t type, uint8_t *data, uint16_t data_len);
uint16_t ssp_create2(uint8_t *buff, uint16_t buff_len, uint8_t type, uint8_t *data1, uint16_t data1_len, uint8_t *data2, uint16_t data2_len);
//------------------------------------------------------------------------------
#endif /* SSP_H_INCLUDED */
