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

#include <xdiff.h>

#include "cmp.h"
#include "buf.h"
#include "pbuf.h"
#include "delta.h"

#define BLKSIZE 1024

static bool xdiff_initialized = false;

static void* wrap_malloc(void *priv, unsigned int size) {
  return malloc(size);
}

static void wrap_free(void *priv, void *ptr) {
  free(ptr);
}

static void* wrap_realloc(void *priv, void *ptr, unsigned int size) {
  return realloc(ptr, size);
}

static int write_to_buffer(void *priv, mmbuffer_t *mb, int nbuf) {
  int i;
  size_t delta_size;
  buf_t *delta = (buf_t *)priv;

  for (delta_size = 0, i = 0; i < nbuf; i++) {
    delta_size += mb[i].size;
  }

  buf_ensure_capacity(delta, delta_size);

  for (i = 0; i < nbuf; i++) {
    buf_write(delta, (char *)mb[i].ptr, mb[i].size);
  }

  return 0;
}

static void build_mmfile(mmfile_t *mmf, char *data, size_t size) {
  char *blk;

  if ((xdl_init_mmfile(mmf, BLKSIZE, XDL_MMF_ATOMIC)) != 0) {
    die("Error initializing mmfile");
  }

  blk = xdl_mmfile_writeallocate(mmf, size);
  if (!blk) {
    die("Error allocating mmfile buffer");
  }

  memcpy(blk, data, size);
}

void delta_init(void) {
  memallocator_t malt;

  if (xdiff_initialized) {
    return;
  }

  xdiff_initialized = true;

  malt.priv = NULL;
  malt.malloc = wrap_malloc;
  malt.free = wrap_free;
  malt.realloc = wrap_realloc;

  xdl_set_allocator(&malt);
}

void delta_build(pbuf_t *b1, pbuf_t *b2, buf_t *delta) {
  mmbuffer_t mmb1, mmb2;
  xdemitcb_t ecb;
  int res = 0;

  ecb.priv = delta;
  ecb.outf = write_to_buffer;

  mmb1.ptr = (char *)pbuf_get_data(b1);
  mmb1.size = (long)pbuf_get_size(b1);

  mmb2.ptr = (char *)pbuf_get_data(b2);
  mmb2.size = (long)pbuf_get_size(b2);

  buf_clear(delta);

  res = xdl_rabdiff_mb(&mmb1, &mmb2, &ecb);
  if (res != 0) {
    perror("");
    die("delta_build: Error building delta: %d", res);
  }
}

bool delta_apply(pbuf_t *b1, pbuf_t *b2, buf_t *delta) {
  mmfile_t cs, is;
  xdemitcb_t ecb;
  buf_t delta_copy;
  int xdiff_res;
  bool res = false;

  ecb.priv = b2;
  ecb.outf = write_to_buffer;

  build_mmfile(&cs, pbuf_get_data(b1), pbuf_get_size(b1));
  build_mmfile(&is, buf_get_data(delta), buf_get_size(delta));

  buf_init_with_capacity(&delta_copy, buf_get_size(delta));
  buf_copy(&delta_copy, delta);

  xdiff_res = xdl_bpatch(&cs, &is, &ecb);

  if (xdiff_res == 0) {
    res = true;
  }
  else {
    die("Mysterious error applying delta (xdiff_res: %d)\n", xdiff_res);
  }

  buf_free(&delta_copy);

  xdl_free_mmfile(&cs);
  xdl_free_mmfile(&is);

  return res;
}

/* vi: set et ts=2 sw=2: */

