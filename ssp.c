/*
 * ssp.c
 *
 *  Created on: 1.6. 2021
 *      Author: Martin Polasek
 */
//------------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------------
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

//------------------------------------------------------------------------------
void ssp_init(ssp_t *ssp, uint8_t *buff, uint16_t size)
{
  ssp->buff = buff;
  ssp->size = size;

  ssp->write = buff;
  ssp->magic = 0;
  ssp->state = SSP_STATE_WAIT;
  ssp->len = 0;

  ssp->err_cnt = 0;
  ssp->pck_cnt = 0;
}
//------------------------------------------------------------------------------
uint8_t ssp_receive(ssp_t *ssp, uint8_t byte)
{
  if(ssp->magic == 1)
  {
    ssp->magic = 0;

    if(byte != SSP_MAGIC)
    {
      if(ssp->state != SSP_STATE_WAIT) ssp->err_cnt++;

      ssp->state = SSP_STATE_LEN2;
      ssp->write = ssp->buff;
      ssp->len = byte;
      ssp->ccc = byte;
      ssp->cc = 0;

      return 0;
    }
  }
  else
  {
    if(byte == SSP_MAGIC)
    {
      ssp->magic = 1;
      return 0;
    }
  }

  switch(ssp->state)
  {
    case SSP_STATE_LEN2:
      ssp->len += (uint16_t)byte << 8;
      ssp->ccc += byte;
      ssp->state = SSP_STATE_DATA;
      break;

    case SSP_STATE_DATA:
      *ssp->write++ = byte;
      ssp->ccc += byte;

      if(ssp->write - ssp->buff >= ssp->len)
      {
        ssp->state = SSP_STATE_CC1;
      }
      else if(ssp->write - ssp->buff >= ssp->size)
      {
        ssp->err_cnt++;
        ssp->state = SSP_STATE_WAIT;
        return 0;
      }
      break;

    case SSP_STATE_CC1:
      ssp->cc = byte;
      ssp->state = SSP_STATE_CC2;
      break;

    case SSP_STATE_CC2:
      ssp->cc += (uint16_t)byte << 8;

      if(ssp->cc == ssp->ccc)
      {
        ssp->pck_cnt++;
        ssp->state = SSP_STATE_WAIT;

        return 1;
      }
      else
      {
        ssp->err_cnt++;
      }

      ssp->state = SSP_STATE_WAIT;
      break;

    default:
      // WAITING....
      break;
  }

  return 0;
}
//------------------------------------------------------------------------------
uint16_t ssp_create(uint8_t *buff, uint16_t buff_len, uint8_t *data, uint16_t data_len)
{
  uint16_t i, cc;
  uint8_t byte;
  uint8_t *buff_p;

  if(buff_len < data_len + 1 + 2 + 2) return 0;

  buff_p = buff;

  *buff++ = SSP_MAGIC;
  cc = 0;

  byte = data_len & 0xFF;
  cc += byte;
  *buff++ = byte;
  if(byte == SSP_MAGIC) *buff++ = SSP_MAGIC;

  byte = data_len >> 8;
  cc += byte;
  *buff++ = byte;
  if(byte == SSP_MAGIC) *buff++ = SSP_MAGIC;

  for(i = 0; i < data_len; i++)
  {
    byte = *data++;
    cc += byte;
    *buff++ = byte;
    if(byte == SSP_MAGIC) *buff++ = SSP_MAGIC;

    if(buff_len < buff - buff_p + 2) return 0;
  }

  byte = cc & 0xFF;
  *buff++ = byte;
  if(byte == SSP_MAGIC) *buff++ = SSP_MAGIC;

  byte = cc >> 8;
  *buff++ = byte;
  if(byte == SSP_MAGIC) *buff++ = SSP_MAGIC;

  return buff - buff_p;
}
//------------------------------------------------------------------------------
