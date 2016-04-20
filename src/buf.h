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

#ifndef BUF_H__
#define BUF_H__

typedef struct buf_s {
  size_t capacity;
  size_t size;
  size_t cursor;
  char *data;
} buf_t;

buf_t* buf_new(void);
buf_t* buf_new_with_capacity(size_t capacity);
void   buf_init(buf_t *buf);
void   buf_init_with_capacity(buf_t *buf, size_t capacity);

size_t buf_get_capacity(buf_t *buf);
size_t buf_get_size(buf_t *buf);
size_t buf_get_cursor(buf_t *buf);
char*  buf_get_data(buf_t *buf);
char*  buf_get_data_at_cursor(buf_t *buf);

void buf_ensure_capacity(buf_t *buf, size_t capacity);
void buf_ensure_total_capacity(buf_t *buf, size_t capacity);

void buf_copy(buf_t *dst, buf_t *src);
void buf_cursor_copy(buf_t *dst, buf_t *src);
bool buf_move(buf_t *buf, size_t dpos, size_t spos, size_t count);

void buf_set_data(buf_t *buf, const void *data, size_t size);

bool buf_seek(buf_t *buf, size_t pos);
bool buf_seek_backward(buf_t *buf, size_t count);
bool buf_seek_forward(buf_t *buf, size_t count);

char buf_peek(buf_t *buf);
void buf_write(buf_t *buf, const void *data, size_t size);
bool buf_read(buf_t *buf, void *data, size_t size);

void buf_sprintf(buf_t *buf, const char *fmt, ...);

void buf_zero(buf_t *buf);
void buf_clear(buf_t *buf);
void buf_free(buf_t *buf);

#endif

/* vi: set et ts=2 sw=2: */

