/*****************************************************************************/
/* DEMOCOMP                                                                  */
/*                                                                           */
/* Copyright (C) 2016: D2K and PrBoom+ authors (see COPYRIGHT file)          */
/*                                                                           */
/* This file is part of DEMOCOMP.                                            */
/*                                                                           */
/* DEMOCOMP is free software: you can redistribute it and/or modify it under */
/* the terms of the GNU General Public License as published by the Free      */
/* Software Foundation, either version 2 of the License, or (at your option) */
/* any later version.                                                        */
/*                                                                           */
/* DEMOCOMP is distributed in the hope that it will be useful, but WITHOUT   */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for */
/* more details.                                                             */
/*                                                                           */
/* You should have received a copy of the GNU General Public License along   */
/* with DEMOCOMP.  If not, see <http://www.gnu.org/licenses/>.               */
/*                                                                           */
/*****************************************************************************/

#include "utils.h"

#include "cmp.h"

#include "buf.h"
#include "pbuf.h"

#define DUMP_READ_DATA 0

static bool pbuf_read(cmp_ctx_t *ctx, void *data, size_t limit) {
  buf_t *buf = (buf_t *)ctx->buf;
  size_t pos = buf_get_cursor(buf);
  bool res = buf_read(buf, data, limit);

#if DUMP_READ_DATA
  {
    puts("Reading data:");
    for (size_t i = 0; i < limit; i++) {
      if ((i > 0) && (i % 21) == 0) {
        printf("0x%02X\n", ((uint8_t *)data)[i]);
      }
      else {
        printf("0x%02X ", ((uint8_t *)data)[i]);
      }
    }
    puts("\nDone\n");
  }
#endif

  if (res && (buf_get_cursor(buf) - pos) == limit) {
    return true;
  }

  printf("res, buf_get_cursor(buf), pos, limit: %d, %lu, %lu, %lu\n",
    res, buf_get_cursor(buf), pos, limit
  );

  return false;
}

static size_t pbuf_write(cmp_ctx_t *ctx, const void *data, size_t count) {
  buf_t *buf = (buf_t *)ctx->buf;
  size_t pos = buf_get_cursor(buf);

  buf_write(buf, data, count);

  return buf_get_cursor(buf) - pos;
}

void pbuf_init(pbuf_t *pbuf) {
  buf_init(&pbuf->buf);
  cmp_init(&pbuf->cmp, &pbuf->buf, &pbuf_read, &pbuf_write);
}

void pbuf_init_with_capacity(pbuf_t *pbuf, size_t capacity) {
  buf_init_with_capacity(&pbuf->buf, capacity);
  cmp_init(&pbuf->cmp, &pbuf->buf, &pbuf_read, &pbuf_write);
}

pbuf_t* pbuf_new(void) {
  pbuf_t *pbuf = malloc(sizeof(pbuf_t));

  if (pbuf == NULL) {
    die("pbuf_new: Error allocating new pbuf");
  }

  pbuf_init(pbuf);

  return pbuf;
}

pbuf_t* pbuf_new_with_capacity(size_t capacity) {
  pbuf_t *pbuf = malloc(sizeof(pbuf_t));

  if (!pbuf) {
    die("M_PBufNewWithCapacity: Error allocating new pbuf");
  }

  pbuf_init_with_capacity(pbuf, capacity);

  return pbuf;
}

/* vi: set et ts=2 sw=2: */

