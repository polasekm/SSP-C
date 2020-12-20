/*-----------------------------------------------------------------------------*/
/*
 * usart.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin
 */
/*-----------------------------------------------------------------------------*/
#ifndef SPP_H_INCLUDED
#define SPP_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  Ring buffer structure definition
  */
typedef struct
{
  uint8_t *buff;        //ukazatel na pocatek bufferu

  uint8_t *write;       //zapisovaci ukazatel
  uint16_t size;        //velikost bufferu

  uint8_t magic;        //stav parseru
  uint8_t state;        //stav parseru
  uint16_t len;         //stav parseru
  uint16_t cc;          //stav parseru
  uint16_t ccc;         //stav parseru

  uint32_t err_cnt;     //stav parseru
  uint32_t pck_cnt;     //stav parseru

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
