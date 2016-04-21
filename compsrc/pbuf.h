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

#ifndef PBUF_H__
#define PBUF_H__

typedef struct pbuf_s {
  buf_t buf;
  cmp_ctx_t cmp;
} pbuf_t;

void    pbuf_init(pbuf_t *pbuf);
void    pbuf_init_with_capacity(pbuf_t *pbuf, size_t capacity);
pbuf_t* pbuf_new(void);
pbuf_t* pbuf_new_with_capacity(size_t capacity);

#define pbuf_get_capacity(pb)             buf_get_capacity(&(pb)->buf)
#define pbuf_get_size(pb)                 buf_get_size(&(pb)->buf)
#define pbuf_get_cursor(pb)               buf_get_cursor(&(pb)->buf)
#define pbuf_get_buffer(pb)               buf_get_buffer(&(pb)->buf)
#define pbuf_get_data(pb)                 buf_get_data(&(pb)->buf)
#define pbuf_get_data_at_cursor(pb)       buf_get_data_at_cursor(&(pb)->buf)
#define pbuf_ensure_capacity(pb, s)       buf_ensure_capacity(&(pb)->buf, s)
#define pbuf_ensure_total_capacity(pb, s) \
  buf_ensure_total_capacity(&(pb)->buf, s)
#define pbuf_copy(dst, src)               buf_copy(&(dst)->buf, &(src)->buf)
#define pbuf_set_data(pb, d, s)           buf_set_data(&(pb)->buf, d, s)
#define pbuf_seek(pb, pos)                buf_seek(&(pb)->buf, pos)
#define pbuf_seek_backward(pb, c)         buf_seek_backward(&(pb)->buf, c)
#define pbuf_seek_forward(pb, c)          buf_seek_forward(&(pb)->buf, c)
#define pbuf_zero(pb)                     buf_zero(&(pb)->buf)
#define pbuf_clear(pb)                    buf_clear(&(pb)->buf)
#define pbuf_free(pb)                     buf_free(&(pb)->buf)

#endif

/* vi: set et ts=2 sw=2: */

