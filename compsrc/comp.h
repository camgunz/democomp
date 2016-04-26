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

#ifndef COMP_H__
#define COMP_H__

typedef struct {
  int states_read;
  pbuf_t dump_pbuf1;
  pbuf_t dump_pbuf2;
  pbuf_t state_pbuf1;
  pbuf_t state_pbuf2;
  pbuf_t delta_pbuf;
  buf_t delta_buf;
  buf_t name_buf;
  int gametic;
  int numsectors;
  int numlines;
  int numactors;
  int numprclass;
  int markpointnum;
  int playeringame[MAXPLAYERS];
  bool found_desync;
} comp_ctx_t;

void comp_load_first_states(comp_ctx_t *comp);
void comp_load_next_states(comp_ctx_t *comp);
void comp_compare(comp_ctx_t *comp);
bool comp_eof(comp_ctx_t *comp);

#endif

/* vi: set et ts=2 sw=2: */

