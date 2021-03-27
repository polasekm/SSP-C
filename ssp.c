/*
 * usart.c
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin
 */
//------------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------------
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

//------------------------------------------------------------------------------
void spp_init(spp_t *spp, uint8_t *buff, uint16_t size)
{
  spp->buff = buff;
  spp->size = size;

  spp->write = buff;
  spp->magic = 0;
  spp->state = SPP_STATE_WAIT;
  spp->len = 0;

  spp->err_cnt = 0;
  spp->pck_cnt = 0;
}
//------------------------------------------------------------------------------
uint8_t spp_receive(spp_t *spp, uint8_t byte)
{
  if(spp->magic == 1)
  {
    spp->magic = 0;

    if(byte != SPP_MAGIC)
    {
      spp->write = spp->buff;

      spp->state = SPP_STATE_LEN2;
      spp->len = byte;

      spp->cc = 0;
      spp->ccc = 0;

      return 0;
    }
  }
  else
  {
    if(byte == SPP_MAGIC)
    {
      spp->magic = 1;
      return 0;
    }
  }

  switch(spp->state)
  {
    case SPP_STATE_LEN2:
      spp->len += (uint16_t)byte << 8;
      spp->state = SPP_STATE_DATA;
      break;

    case SPP_STATE_DATA:
      *spp->write++ = byte;
      spp->ccc += byte;

      if(spp->write - spp->buff >= spp->len)
      {
        spp->state = SPP_STATE_CC1;
      }
      else if(spp->write - spp->buff >= spp->size)
      {
        spp->err_cnt++;
        spp->state = SPP_STATE_WAIT;
        return 0;
      }
      break;

    case SPP_STATE_CC1:
      spp->cc = byte;
      spp->state = SPP_STATE_CC2;
      break;

    case SPP_STATE_CC2:
      spp->cc += (uint16_t)byte << 8;

      if(spp->cc == spp->ccc)
      {
        spp->pck_cnt++;
        spp->state = SPP_STATE_WAIT;

        return 1;
      }
      else
      {
        spp->err_cnt++;
      }

      spp->state = SPP_STATE_WAIT;
      break;

    default:
      // WAITING....
      break;
  }

  return 0;
}
//------------------------------------------------------------------------------
uint16_t spp_create(uint8_t *buff, uint16_t buff_len, uint8_t *data, uint16_t data_len)
{
  uint16_t i, cc;
  uint8_t byte;
  uint8_t *buff_p;

  if(buff_len < data_len + 1 + 2 + 2) return 0;

  buff_p = buff;

  *buff++ = SPP_MAGIC;

  *buff++ = data_len & 0xFF;
  *buff++ = data_len >> 8;

  cc = 0;

  for(i = 0; i < data_len; i++)
  {
    byte = *data++;

    cc += byte;
    *buff++ = byte;
    if(byte == SPP_MAGIC) *buff++ = byte;


    if(buff_len < buff - buff_p + 2) return 0;
  }

  byte = cc & 0xFF;
  *buff++ = byte;
  if(byte == SPP_MAGIC) *buff++ = byte;

  byte = cc >> 8;
  *buff++ = byte;
  if(byte == SPP_MAGIC) *buff++ = byte;

  return buff - buff_p;
}
//------------------------------------------------------------------------------
