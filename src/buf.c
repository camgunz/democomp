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

#include "buf.h"

buf_t* buf_new(void) {
  buf_t *buf = calloc(1, sizeof(buf_t));

  if (!buf) {
    die("buf_new: Calloc returned NULL.");
  }

  buf_init(buf);

  return buf;
}

buf_t* buf_new_with_capacity(size_t capacity) {
  buf_t *buf = calloc(1, sizeof(buf_t));

  if (!buf) {
    die("buf_new_with_capacity: calloc returned NULL");
  }

  buf_init_with_capacity(buf, capacity);

  return buf;
}

void buf_init(buf_t *buf) {
  buf->size     = 0;
  buf->capacity = 0;
  buf->cursor   = 0;
  buf->data     = NULL;
}

void buf_init_with_capacity(buf_t *buf, size_t capacity) {
  buf_init(buf);
  buf_ensure_capacity(buf, capacity);
}

size_t buf_get_capacity(buf_t *buf) {
  return buf->capacity;
}

size_t buf_get_size(buf_t *buf) {
  return buf->size;
}

size_t buf_get_cursor(buf_t *buf) {
  return buf->cursor;
}

char* buf_get_data(buf_t *buf) {
  return buf->data;
}

char* buf_get_data_at_cursor(buf_t *buf) {
  return buf->data + buf->cursor;
}

void buf_ensure_capacity(buf_t *buf, size_t capacity) {
  size_t needed_capacity = buf->cursor + capacity;

  if (buf->capacity < needed_capacity) {
    buf->data = realloc(buf->data, needed_capacity * sizeof(char));

    if (buf->data == NULL)
      die("buf_ensure_capacity: Reallocating buffer data failed");

    memset(buf->data + buf->capacity, 0, needed_capacity - buf->capacity);
    buf->capacity = needed_capacity;
  }
}

void buf_ensure_total_capacity(buf_t *buf, size_t capacity) {
  if (buf->capacity < capacity) {
    size_t old_capacity = buf->capacity;

    buf->capacity = capacity;
    buf->data = realloc(buf->data, buf->capacity * sizeof(char));

    if (buf->data == NULL)
      die("buf_ensure_total_capacity: Allocating buffer data failed");

    memset(buf->data + old_capacity, 0, buf->capacity - old_capacity);
  }
}

void buf_copy(buf_t *dst, buf_t *src) {
  buf_set_data(dst, buf_get_data(src), buf_get_size(src));
}

void buf_cursor_copy(buf_t *dst, buf_t *src) {
  buf_write(
    dst,
    buf_get_data_at_cursor(src),
    buf_get_size(src) - (buf_get_cursor(src) - 1)
  );
}

void buf_set_data(buf_t *buf, const void *data, size_t size) {
  buf_clear(buf);
  buf_ensure_total_capacity(buf, size);
  buf_write(buf, data, size);
}

bool buf_seek(buf_t *buf, size_t pos) {
  if (pos > buf->size) {
    return false;
  }

  buf->cursor = pos;
  return true;
}

bool buf_seek_backward(buf_t *buf, size_t count) {
  if (count > buf->cursor) {
    return false;
  }

  buf->cursor -= count;
  return true;
}

bool buf_seek_forward(buf_t *buf, size_t count) {
  if (buf->cursor + count > buf->size) {
    return false;
  }

  buf->cursor += count;
  return true;
}

char buf_peek(buf_t *buf) {
  return *(buf->data + buf->cursor);
}

void buf_write(buf_t *buf, const void *data, size_t size) {
  buf_ensure_capacity(buf, size);
  memcpy(buf->data + buf->cursor, data, size);
  buf->cursor += size;
  if (buf->cursor > buf->size)
    buf->size = buf->cursor;
}

bool buf_read(buf_t *buf, void *data, size_t size) {
  if (buf->cursor + size > buf->size)
    return false;

  if (size == 1)
    *((char *)data) = *(buf->data + buf->cursor);
  else
    memcpy(data, buf->data + buf->cursor, size);

  buf->cursor += size;

  return true;
}

void buf_sprintf(buf_t *buf, const char *fmt, ...) {
  size_t size;
  size_t buf_size = buf_get_size(buf);
  va_list args;
  va_list args_copy;

  va_start(args, fmt);
  va_copy(args_copy, args);

  size = vsnprintf(buf_get_data(buf), buf_size, fmt, args_copy);

  if (size >= buf_size) {
    buf_ensure_total_capacity(buf, size + 1);
    vsnprintf(buf_get_data(buf), size + 1, fmt, args);
  }
}

void buf_zero(buf_t *buf) {
  memset(buf->data, 0, buf->capacity);
}

void buf_clear(buf_t *buf) {
  buf->size = 0;
  buf->cursor = 0;
  buf_zero(buf);
}

void buf_free(buf_t *buf) {
  free(buf->data);
  memset(buf, 0, sizeof(buf_t));
  buf->data = NULL;
}

/* vi: set et ts=2 sw=2: */

