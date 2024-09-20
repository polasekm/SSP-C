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

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  SSP structure definition
  */
typedef struct
{
  uint8_t *buff;        //buffer pointer

  uint8_t *write;       //zapisovaci ukazatel
  uint16_t size;        //buffer size

  uint8_t magic;        //
  uint8_t state;        //state
  uint16_t len;         //packet length
  uint16_t cc;          //packet CC
  uint16_t ccc;         //receive data CC

  uint32_t err_cnt;     //error counter
  uint32_t pck_cnt;     //packet counter

} spp_t;

/* Exported functions ------------------------------------------------------- */

/* Defines -------------------------------------------------------------------*/

#define SPP_MAGIC           0xA3

#define SPP_STATE_WAIT      0
#define SPP_STATE_LEN1      1
#define SPP_STATE_LEN2      2
#define SPP_STATE_DATA      3
#define SPP_STATE_CC1       4
#define SPP_STATE_CC2       5

// Functions -------------------------------------------------------------------
void spp_init(spp_t *spp, uint8_t *buff, uint16_t size);

uint8_t spp_receive(spp_t *spp, uint8_t byte);
uint16_t spp_create(uint8_t *buff, uint16_t buff_len, uint8_t *data, uint16_t data_len);
//------------------------------------------------------------------------------
#endif /* SPP_H_INCLUDED */
