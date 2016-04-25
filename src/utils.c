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

void warn(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void die(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  exit(EXIT_FAILURE);
}

void print_bin(uint8_t *data, size_t size) {
  size_t byte_count = 18;

  if (size < 18) {
    byte_count = size;
  }

  for (int i = 0; i < byte_count; i++) {
    printf("0x%02X ", *(data + i));
  }
  puts("");
}

void print_bin_end(uint8_t *data, size_t size) {
  size_t byte_count = 18;

  if (size < 18) {
    byte_count = size;
  }

  for (int i = 0; i < byte_count; i++) {
    printf("0x%02X ", *(
      (data + (size - 1) - (byte_count - i))
    ));
  }
  puts("");
}

/* vi: set et ts=2 sw=2: */

