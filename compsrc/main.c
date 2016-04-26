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
#include "doom.h"

#include "buf.h"
#include "pbuf.h"
#include "delta.h"

#include "comp.h"
#include "sha256.h"

int main(int argc, char **argv) {
  FILE       *dump_fobj1;
  FILE       *dump_fobj2;
  long        dump_file_size1;
  long        dump_file_size2;
  int         rv;
  comp_ctx_t  comp = { 0 };

  if (argc != 3) {
    fprintf(stderr, "\nUsage: %s [dump file 1] [dump file 2]\n\n", argv[0]);
    return EXIT_FAILURE;
  }

  dump_fobj1 = fopen(argv[1], "rb");

  if (!dump_fobj1) {
    perror("Error opening 1st dump file");
    return EXIT_FAILURE;
  }

  dump_fobj2 = fopen(argv[2], "rb");

  if (!dump_fobj2) {
    perror("Error opening 2nd dump file");
    return EXIT_FAILURE;
  }

  fseek(dump_fobj1, 0, SEEK_END);
  dump_file_size1 = ftell(dump_fobj1);
  fseek(dump_fobj1, 0, SEEK_SET);

  if (dump_file_size1 < 0) {
    perror("Error getting the size of 1st dump file");
    return EXIT_FAILURE;
  }

  fseek(dump_fobj2, 0, SEEK_END);
  dump_file_size2 = ftell(dump_fobj2);
  fseek(dump_fobj2, 0, SEEK_SET);

  if (dump_file_size2 < 0) {
    perror("Error getting the size of 2nd dump file");
    return EXIT_FAILURE;
  }

  /*
   * Check size
   *   - Match:
   *     - Check hash
   *       - Match:
   *         - return EXIT_SUCCESS
   */

  pbuf_init_with_capacity(&comp.dump_pbuf1, dump_file_size1);
  pbuf_init_with_capacity(&comp.dump_pbuf2, dump_file_size2);

  rv = fread(comp.dump_pbuf1.buf.data, 1, dump_file_size1, dump_fobj1);

  if (rv != dump_file_size1) {
    printf("%d\n", rv);
    perror("Error reading 1st dump file's data");
    return EXIT_FAILURE;
  }

  comp.dump_pbuf1.buf.size = dump_file_size1;

  rv = fread(comp.dump_pbuf2.buf.data, 1, dump_file_size2, dump_fobj2);

  if (rv != dump_file_size2) {
    perror("Error reading 2nd dump file's data");
    return EXIT_FAILURE;
  }

  comp.dump_pbuf2.buf.size = dump_file_size2;

  if (dump_file_size1 == dump_file_size2) {
    mbedtls_sha256_context sha2561;
    mbedtls_sha256_context sha2562;
    unsigned char output1[32];
    unsigned char output2[32];

    mbedtls_sha256_init(&sha2561);
    mbedtls_sha256_starts(&sha2561, 0);
    mbedtls_sha256_update(&sha2561,
                          (unsigned char *)pbuf_get_data(&comp.dump_pbuf1),
                          pbuf_get_size(&comp.dump_pbuf1));
    mbedtls_sha256_finish(&sha2561, output1);

    mbedtls_sha256_init(&sha2562);
    mbedtls_sha256_starts(&sha2562, 0);
    mbedtls_sha256_update(&sha2562,
                          (unsigned char *)pbuf_get_data(&comp.dump_pbuf2),
                          pbuf_get_size(&comp.dump_pbuf2));
    mbedtls_sha256_finish(&sha2562, output2);

    if (memcmp(output1, output2, 32) == 0) {
      return EXIT_SUCCESS;
    }
  }

  delta_init();

  comp.states_read = 0;
  pbuf_init(&comp.state_pbuf1);
  pbuf_init(&comp.state_pbuf2);
  pbuf_init(&comp.delta_pbuf);
  buf_init(&comp.delta_buf);
  buf_init(&comp.name_buf);

  while (true) {
    if (comp_eof(&comp)) {
      break;
    }

    if (comp.found_desync) {
      return EXIT_FAILURE;
    }

    comp_load_next_states(&comp);
    comp_compare(&comp);
  }

  return EXIT_SUCCESS;
}

/* vi: set et ts=2 sw=2: */
