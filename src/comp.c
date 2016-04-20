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

#include "comp.h"
#include "delta.h"

#define compare(name)                                                         \
  do {                                                                        \
    compare_next_object(comp, name, NULL);                                    \
  } while (0);

#define read_compare(name, val)                                               \
  do {                                                                        \
    compare_next_object(comp, name, val);                                     \
  } while (0);

#define read_iter_compare(name, i, val)                                       \
  do {                                                                        \
    buf_zero(&comp->name_buf);                                                \
    buf_ensure_total_capacity(&comp->name_buf, strlen(name) + 15);            \
    sprintf(comp->name_buf.data, "%s %d", name, i);                           \
    buf_seek(&comp->name_buf, strlen(comp->name_buf.data));                   \
    compare_next_object(comp, comp->name_buf.data, val);                      \
  } while (0);

#define read_iter_compare2(name, i, j, val)                                   \
  do {                                                                        \
    buf_zero(&comp->name_buf);                                                \
    buf_ensure_total_capacity(&comp->name_buf, strlen(name) + 15);            \
    sprintf(comp->name_buf.data, "%s %d", name, i);                           \
    buf_seek(&comp->name_buf, strlen(comp->name_buf.data));                   \
    compare_next_object(comp, comp->name_buf.data, val);                      \
  } while (0);

#define iter_compare(name, i)                                                 \
  do {                                                                        \
    buf_zero(&comp->name_buf);                                                \
    buf_ensure_total_capacity(&comp->name_buf, strlen(name) + 15);            \
    sprintf(comp->name_buf.data, "%s %d", name, i);                           \
    buf_seek(&comp->name_buf, strlen(comp->name_buf.data));                   \
    compare_next_object(comp, comp->name_buf.data, NULL);                     \
  } while (0);

#define iter_compare2(name, i, j)                                             \
  do {                                                                        \
    buf_zero(&comp->name_buf);                                                \
    buf_ensure_total_capacity(&comp->name_buf, strlen(name) + 30);            \
    sprintf(comp->name_buf.data, "%s %d %d", name, i, j);                     \
    buf_seek(&comp->name_buf, strlen(comp->name_buf.data));                   \
    compare_next_object(comp, comp->name_buf.data, NULL);                     \
  } while (0);

const char* cmp_object_type_to_string(int type) {
  switch (type) {
    case CMP_TYPE_POSITIVE_FIXNUM:
      return "positive fixnum";
    case CMP_TYPE_FIXMAP:
      return "fixmap";
    case CMP_TYPE_FIXARRAY:
      return "fixarray";
    case CMP_TYPE_FIXSTR:
      return "fixstr";
    case CMP_TYPE_NIL:
      return "nil";
    case CMP_TYPE_BOOLEAN:
      return "bool";
    case CMP_TYPE_BIN8:
      return "b8";
    case CMP_TYPE_BIN16:
      return "b16";
    case CMP_TYPE_BIN32:
      return "b32";
    case CMP_TYPE_FLOAT:
      return "float";
    case CMP_TYPE_DOUBLE:
      return "double";
    case CMP_TYPE_UINT8:
      return "u8";
    case CMP_TYPE_UINT16:
      return "u16";
    case CMP_TYPE_UINT32:
      return "u32";
    case CMP_TYPE_UINT64:
      return "u64";
    case CMP_TYPE_SINT8:
      return "s8";
    case CMP_TYPE_SINT16:
      return "s16";
    case CMP_TYPE_SINT32:
      return "s32";
    case CMP_TYPE_SINT64:
      return "s64";
    case CMP_TYPE_STR8:
      return "string8";
    case CMP_TYPE_STR16:
      return "string16";
    case CMP_TYPE_STR32:
      return "string32";
    case CMP_TYPE_NEGATIVE_FIXNUM:
      return "negative fixnum";
    default:
      die("Unsupported MessagePack type %d\n", type);
      break;
  }

  return "unknown";
}

const char* cmp_object_to_string(cmp_object_t *obj) {
  static buf_t str;
  static bool str_initialized = false;

  if (!str_initialized) {
    buf_init(&str);
  }

  switch (obj->type) {
    case CMP_TYPE_POSITIVE_FIXNUM:
      buf_sprintf(&str, "pfix [%u]", obj->as.u8);
      break;
    case CMP_TYPE_FIXMAP:
      buf_sprintf(&str, "fixmap [%u]", obj->as.map_size);
      break;
    case CMP_TYPE_FIXARRAY:
      buf_sprintf(&str, "fixarray [%u]", obj->as.array_size);
      break;
    case CMP_TYPE_FIXSTR:
      buf_sprintf(&str, "fixstr [%u]", obj->as.str_size);
      break;
    case CMP_TYPE_NIL:
      buf_sprintf(&str, "nil");
      break;
    case CMP_TYPE_BOOLEAN:
      buf_sprintf(&str, "bool [%s]", obj->as.boolean ? "true" : "false");
      break;
    case CMP_TYPE_BIN8:
      buf_sprintf(&str, "b8 [%u]", obj->as.bin_size);
      break;
    case CMP_TYPE_BIN16:
      buf_sprintf(&str, "b16 [%u]", obj->as.bin_size);
      break;
    case CMP_TYPE_BIN32:
      buf_sprintf(&str, "b32 [%u]", obj->as.bin_size);
      break;
    case CMP_TYPE_FLOAT:
      buf_sprintf(&str, "float [%f]", obj->as.flt);
      break;
    case CMP_TYPE_DOUBLE:
      buf_sprintf(&str, "double [%f]", obj->as.dbl);
      break;
    case CMP_TYPE_UINT8:
      buf_sprintf(&str, "u8 [%u]", obj->as.u8);
      break;
    case CMP_TYPE_UINT16:
      buf_sprintf(&str, "u16 [%u]", obj->as.u16);
      break;
    case CMP_TYPE_UINT32:
      buf_sprintf(&str, "u32 [%u]", obj->as.u32);
      break;
    case CMP_TYPE_UINT64:
      buf_sprintf(&str, "u64 [%" PRIu64 "]", obj->as.u64);
      break;
    case CMP_TYPE_SINT8:
      buf_sprintf(&str, "s8 [%u]", obj->as.s8);
      break;
    case CMP_TYPE_SINT16:
      buf_sprintf(&str, "s16 [%u]", obj->as.s16);
      break;
    case CMP_TYPE_SINT32:
      buf_sprintf(&str, "s32 [%u]", obj->as.s32);
      break;
    case CMP_TYPE_SINT64:
      buf_sprintf(&str, "s64 [%" PRId64 "]", obj->as.s64);
      break;
    case CMP_TYPE_STR8:
      buf_sprintf(&str, "str8 [%u]", obj->as.str_size);
      break;
    case CMP_TYPE_STR16:
      buf_sprintf(&str, "str16 [%u]", obj->as.str_size);
      break;
    case CMP_TYPE_STR32:
      buf_sprintf(&str, "str32 [%u]", obj->as.str_size);
      break;
    case CMP_TYPE_NEGATIVE_FIXNUM:
      buf_sprintf(&str, "nfix [%d]", obj->as.s8);
      break;
    default:
      die("Unsupported MessagePack type %d\n", obj->type);
      break;
  }

  return buf_get_data(&str);
}

void compare_next_object(comp_ctx_t *comp, const char *name, int *res) {
  int rv;
  cmp_object_t obj1;
  cmp_object_t obj2;
  size_t cursor1 = pbuf_get_cursor(&comp->state_pbuf1);
  size_t cursor2 = pbuf_get_cursor(&comp->state_pbuf2);

  cmp_read_object(&comp->state_pbuf1.cmp, &obj1);
  cmp_read_object(&comp->state_pbuf2.cmp, &obj2);

  if (obj1.type != obj2.type) {
    fprintf(stderr,
      "%s at gametic %d (byte %lu/%lu) does not have the same type (%s != %s)\n",
      name,
      comp->gametic,
      cursor1,
      cursor2,
      cmp_object_type_to_string(obj1.type),
      cmp_object_type_to_string(obj2.type)
    );
    fprintf(stderr, "Values: %s, %s\n",
      cmp_object_to_string(&obj1),
      cmp_object_to_string(&obj2)
    );
    die("Exiting on mismatch\n");
  }

  switch (obj1.type) {
    case CMP_TYPE_BOOLEAN:
      if (obj1.as.boolean != obj2.as.boolean) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%s != %s)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.boolean ? "true" : "false",
          obj2.as.boolean ? "true" : "false"
        );
      }
      break;
    case CMP_TYPE_BIN8:
    case CMP_TYPE_BIN16:
    case CMP_TYPE_BIN32:
      if (obj1.as.bin_size != obj2.as.bin_size) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%u != %u)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.bin_size,
          obj2.as.bin_size
        );
      }
      rv = memcmp(
        pbuf_get_data_at_cursor(&comp->state_pbuf1),
        pbuf_get_data_at_cursor(&comp->state_pbuf2),
        obj1.as.bin_size
      );
      if (rv != 0) {
        die("%s at gametic %d (byte %lu/%lu) does not match (binary data)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2)
        );
      }
      pbuf_seek_forward(&comp->state_pbuf1, obj1.as.bin_size);
      pbuf_seek_forward(&comp->state_pbuf2, obj2.as.bin_size);
      break;
    case CMP_TYPE_FLOAT:
      if (obj1.as.flt != obj2.as.flt) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%f != %f)\n",
          name,
          comp->gametic,
          obj1.as.flt,
          obj2.as.flt
        );
      }
      break;
    case CMP_TYPE_DOUBLE:
      if (obj1.as.dbl != obj2.as.dbl) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%f != %f)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.dbl,
          obj2.as.dbl
        );
      }
      break;
    case CMP_TYPE_POSITIVE_FIXNUM:
    case CMP_TYPE_UINT8:
      if (obj1.as.u8 != obj2.as.u8) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%u != %u)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.u8,
          obj2.as.u8
        );
      }
      if (res) {
        *res = obj1.as.u8;
      }
      break;
    case CMP_TYPE_UINT16:
      if (obj1.as.u16 != obj2.as.u16) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%u != %u)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.u16,
          obj2.as.u16
        );
      }
      if (res) {
        *res = obj1.as.u16;
      }
      break;
    case CMP_TYPE_UINT32:
      if (obj1.as.u32 != obj2.as.u32) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%u != %u)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.u32,
          obj2.as.u32
        );
      }
      if (res) {
        *res = obj1.as.u32;
      }
      break;
    case CMP_TYPE_UINT64:
      if (obj1.as.u64 != obj2.as.u64) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%" PRIu64 " != %" PRIu64 ")\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.u64,
          obj2.as.u64
        );
      }
      break;
    case CMP_TYPE_NEGATIVE_FIXNUM:
    case CMP_TYPE_SINT8:
      if (obj1.as.s8 != obj2.as.s8) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%d != %d)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.s8,
          obj2.as.s8
        );
      }
      if (res) {
        *res = obj1.as.s8;
      }
      break;
    case CMP_TYPE_SINT16:
      if (obj1.as.s16 != obj2.as.s16) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%d != %d)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.s16,
          obj2.as.s16
        );
      }
      if (res) {
        *res = obj1.as.s16;
      }
      break;
    case CMP_TYPE_SINT32:
      if (obj1.as.s32 != obj2.as.s32) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%d != %d)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.s32,
          obj2.as.s32
        );
      }
      if (res) {
        *res = obj1.as.s32;
      }
      break;
    case CMP_TYPE_SINT64:
      if (obj1.as.s64 != obj2.as.s64) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%" PRId64 " != %" PRId64 ")\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.s64,
          obj2.as.s64
        );
      }
      break;
    case CMP_TYPE_FIXSTR:
    case CMP_TYPE_STR8:
    case CMP_TYPE_STR16:
    case CMP_TYPE_STR32:
      if (obj1.as.str_size != obj2.as.str_size) {
        die("%s at gametic %d (byte %lu/%lu) does not match (%u != %u)\n",
          name,
          comp->gametic,
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          obj1.as.str_size,
          obj2.as.str_size
        );
      }
      rv = strncmp(
        pbuf_get_data_at_cursor(&comp->state_pbuf1),
        pbuf_get_data_at_cursor(&comp->state_pbuf2),
        obj1.as.str_size
      );
      if (rv != 0) {
        die("%s at gametic %d (byte %lu/%lu) does not match ([%s] != [%s])\n",
          name,
          comp->gametic, 
          pbuf_get_cursor(&comp->state_pbuf1),
          pbuf_get_cursor(&comp->state_pbuf2),
          pbuf_get_data_at_cursor(&comp->state_pbuf1),
          pbuf_get_data_at_cursor(&comp->state_pbuf2)
        );
      }
      pbuf_seek_forward(&comp->state_pbuf1, obj1.as.str_size);
      pbuf_seek_forward(&comp->state_pbuf2, obj2.as.str_size);
      break;
    default:
      die("Unsupported MessagePack type %d\n", obj1.type);
      break;
  }
}

void comp_clear(comp_ctx_t *comp) {
  comp->numsectors = 0;
  comp->numlines = 0;
  comp->numactors = 0;
  comp->numprclass = 0;
  comp->markpointnum = 0;
  for (int i = 0; i < MAXPLAYERS; i++) {
    comp->playeringame[i] = 0;
  }
}

void comp_load_first_states(comp_ctx_t *comp) {
}

void comp_load_next_states(comp_ctx_t *comp) {
  uint32_t size1;
  uint32_t size2;

  if (comp->states_read == 0) {

    if (!cmp_read_uint(&comp->dump_pbuf1.cmp, &size1)) {
      die("Error reading size of 1st state: %s\n",
        cmp_strerror(&comp->dump_pbuf1.cmp)
      );
    }

    if (!cmp_read_uint(&comp->dump_pbuf2.cmp, &size2)) {
      die("Error reading size of 2nd state: %s\n",
        cmp_strerror(&comp->dump_pbuf2.cmp)
      );
    }

    pbuf_set_data(
      &comp->state_pbuf1, pbuf_get_data_at_cursor(&comp->dump_pbuf1), size1
    );

    pbuf_set_data(
      &comp->state_pbuf2, pbuf_get_data_at_cursor(&comp->dump_pbuf2), size2
    );
  }
  else {
    if (!cmp_read_uint(&comp->dump_pbuf1.cmp, &size1)) {
      die("Error reading size of 1st state: %s\n",
        cmp_strerror(&comp->dump_pbuf1.cmp)
      );
    }

    if (!cmp_read_uint(&comp->dump_pbuf2.cmp, &size2)) {
      die("Error reading size of 2nd state: %s\n",
        cmp_strerror(&comp->dump_pbuf2.cmp)
      );
    }

    buf_set_data(
      &comp->delta_buf,
      pbuf_get_data_at_cursor(&comp->dump_pbuf1),
      size1
    );

    pbuf_copy(&comp->delta_pbuf, &comp->state_pbuf1);
    pbuf_clear(&comp->state_pbuf1);

    if (!delta_apply(&comp->delta_pbuf, &comp->state_pbuf1, &comp->delta_buf)) {
      die("Error applying delta\n");
    }

    buf_set_data(
      &comp->delta_buf,
      buf_get_data_at_cursor(&comp->dump_pbuf2.buf),
      size2
    );

    pbuf_copy(&comp->delta_pbuf, &comp->state_pbuf2);
    pbuf_clear(&comp->state_pbuf2);

    if (!delta_apply(&comp->delta_pbuf, &comp->state_pbuf2, &comp->delta_buf)) {
      die("Error applying delta\n");
    }
  }

  pbuf_seek(&comp->state_pbuf1, 0);
  pbuf_seek(&comp->state_pbuf2, 0);

  comp->states_read++;

  comp_clear(comp);

  pbuf_seek_forward(&comp->dump_pbuf1, size1);
  pbuf_seek_forward(&comp->dump_pbuf2, size2);
}

void comp_compare(comp_ctx_t *comp) {
  int s = 0;
  int tc;
  int version;

  /** Config **/

  // printf("(%d) Reading config\n", comp->gametic);

  read_compare("version", &version);

  switch (version) {
    case 106:
    case 109:
    case 110:
    case 111:
      compare("gameskill");
      compare("gameepisode");
      compare("gamemap");
      compare("deathmatch");
      compare("respawnparm");
      compare("fastparm");
      compare("nomonsters");
      compare("consoleplayer");
      break;
    case 201:
    case 202:
      compare("longtics");
    case 203:
    case 210:
    case 211:
    case 212:
    case 213:
    case 214:
      compare("gameskill");
      compare("gameepisode");
      compare("gamemap");
      compare("deathmatch");
      compare("consoleplayer");
      compare("monsters_remember");
      compare("variable_friction");
      compare("weapon_recoil");
      compare("allow_pushers");
      compare("player_bobbing");
      compare("respawnparm");
      compare("fastparm");
      compare("nomonsters");
      compare("demo_insurance");
      compare("rngseed");
      compare("monster_infighting");
      compare("dogs");
      compare("distfriend");
      compare("monster_backing");
      compare("monster_avoid_hazards");
      compare("monster_friction");
      compare("help_friends");
      compare("dog_jumping");
      compare("monkeys");
      for (int i = 0; i < COMP_TOTAL; i++) {
        iter_compare("comp", i);
      }

      compare("forceOldBsp");
      break;
    default:
      die("Unknown version %d\n", version);
      break;
  }

  for (int i = 0; i < COMPERR_NUM; i++) {
    iter_compare("comperr", i);
  }

  /** Misc **/

  read_compare("gametic", &comp->gametic);

  // printf("(%d) Reading misc\n", comp->gametic);

  compare("leveltime");
  compare("totalleveltimes");
  compare("basetic");

  /** RNG **/

  // printf("(%d) Reading RNG\n", comp->gametic);

  compare("rng.rndindex");
  compare("rng.prndindex");

  /** Players **/

  // printf("(%d) Reading players\n", comp->gametic);

  for (int i = 0; i < MAXPLAYERS; i++) {
    read_iter_compare("playeringame[i]", i, &comp->playeringame[i]);
  }

  for (int i = MAXPLAYERS; i < MIN_MAXPLAYERS; i++) {
    iter_compare("playeringame[i]", i);
  }

  for (int i = 0; i < MAXPLAYERS; i++) {
    if (comp->playeringame[i]) {
      iter_compare("player->playerstate", i);
      iter_compare("player->cmd.forwardmove", i);
      iter_compare("player->cmd.sidemove", i);
      iter_compare("player->cmd.angleturn", i);
      iter_compare("player->cmd.consistancy", i);
      iter_compare("player->cmd.chatchar", i);
      iter_compare("player->cmd.buttons", i);
      iter_compare("player->viewz", i);
      iter_compare("player->viewheight", i);
      iter_compare("player->deltaviewheight", i);
      iter_compare("player->bob", i);
      iter_compare("player->health", i);
      iter_compare("player->armorpoints", i);
      iter_compare("player->armortype", i);
      for (int j = 0; j < NUMPOWERS; j++)
        iter_compare2("player->powers[i]", i, j);
      for (int j = 0; j < NUMCARDS; j++)
        iter_compare2("player->cards[i]", i, j);
      iter_compare("player->backpack", i);
      for (int j = 0; j < MAXPLAYERS; j++)
        iter_compare2("player->frags[i]", i, j);
      iter_compare("player->readyweapon", i);
      iter_compare("player->pendingweapon", i);
      for (int j = 0; j < NUMWEAPONS; j++)
        iter_compare2("player->weaponowned[i]", i, j);
      for (int j = 0; j < NUMAMMO; j++)
        iter_compare2("player->ammo[i]", i, j);
      for (int j = 0; j < NUMAMMO; j++)
        iter_compare2("player->maxammo[i]", i, j);
      iter_compare("player->attackdown", i);
      iter_compare("player->usedown", i);
      iter_compare("player->cheats", i);
      iter_compare("player->refire", i);
      iter_compare("player->killcount", i);
      iter_compare("player->itemcount", i);
      iter_compare("player->secretcount", i);
      iter_compare("player->damagecount", i);
      iter_compare("player->bonuscount", i);
      iter_compare("player->extralight", i);
      iter_compare("player->fixedcolormap", i);
      iter_compare("player->colormap", i);
      for (int j = 0; j < NUMPSPRITES; j++) {
        iter_compare2("state_index", i, j);
        iter_compare2("player->psprites[i].tics", i, j);
        iter_compare2("player->psprites[i].sx", i, j);
        iter_compare2("player->psprites[i].sy", i, j);
      }
      iter_compare("player->didsecret)", i);
      iter_compare("player->momx)", i);
      iter_compare("player->momy)", i);
      iter_compare("player->resurectedkillcount", i);
      // iter_compare("player->jumpTics)", i);
    }
  }

  /** World **/

  // printf("(%d) Reading world\n", comp->gametic);

  compare("numspechit");
  read_compare("numsectors", &comp->numsectors);
  read_compare("numlines", &comp->numlines);

  for (int i = 0; i < comp->numsectors; i++) {
    iter_compare("sec->floorheight", i);
    iter_compare("sec->ceilingheight", i);
    iter_compare("sec->floorpic", i);
    iter_compare("sec->ceilingpic", i);
    iter_compare("sec->lightlevel", i);
    iter_compare("sec->special", i);
    iter_compare("sec->tag", i);
    iter_compare("sec->soundorg.x", i);
    iter_compare("sec->soundorg.y", i);
    iter_compare("sec->soundorg.z", i);
  }

  for (int i = 0; i < comp->numlines; i++) {
    // iter_compare("li->flags", i);
    iter_compare("li->special", i);
    iter_compare("li->tag", i);

    for (int j = 0; j < 2; j++) {
      int has_side;

      iter_compare2("line side", i, j);
      read_iter_compare2("line has side", i, j, &has_side);

      if (has_side) {
        iter_compare2("si->textureoffset", i, j);
        iter_compare2("si->rowoffset", i, j);
        iter_compare2("si->toptexture", i, j);
        iter_compare2("si->bottomtexture", i, j);
        iter_compare2("si->midtexture", i, j);
      }
    }

    iter_compare("li->soundorg.x", i);
    iter_compare("li->soundorg.y", i);
    iter_compare("li->soundorg.z", i);
  }

  compare("musinfo.current_item");

  /** Actors **/

  // printf("(%d) Reading actors\n", comp->gametic);

  compare("max ident ID");
  compare("brain.easy");
  compare("brain.targeton");

  read_compare("actor_count", &comp->numactors);

  for (int i = 0; i < comp->numactors; i++) {
    iter_compare("mobj->type", i);
    iter_compare("mobj->index", i);
    iter_compare("mobj->x", i);
    iter_compare("mobj->y", i);
    iter_compare("mobj->z", i);
    iter_compare("mobj->angle", i);
    iter_compare("mobj->pitch", i);
    iter_compare("mobj->id", i);
    iter_compare("mobj->tics", i);
    iter_compare("mobj->flags", i);
    iter_compare("mobj->floorz", i);
    iter_compare("mobj->ceilingz", i);
    iter_compare("mobj->dropoffz", i);
    iter_compare("mobj->momx", i);
    iter_compare("mobj->momy", i);
    iter_compare("mobj->momz", i);
    iter_compare("mobj->validcount", i);
    iter_compare("mobj->intflags", i);
    iter_compare("mobj->health", i);
    iter_compare("mobj->movedir", i);
    iter_compare("mobj->movecount", i);
    iter_compare("mobj->strafecount", i);
    iter_compare("mobj->reactiontime", i);
    iter_compare("mobj->threshold", i);
    iter_compare("mobj->pursuecount", i);
    iter_compare("mobj->gear", i);
    iter_compare("mobj->lastlook", i);
    iter_compare("mobj->spawnpoint.x", i);
    iter_compare("mobj->spawnpoint.y", i);
    iter_compare("mobj->spawnpoint.angle", i);
    iter_compare("mobj->spawnpoint.type", i);
    iter_compare("mobj->spawnpoint.options", i);
    iter_compare("mobj->friction", i);
    iter_compare("mobj->movefactor", i);
    // iter_compare("mobj->patch_width", i);
    iter_compare("mobj->iden_nums", i);
    iter_compare("mobj->state", i);
    iter_compare("mobj->target", i);
    iter_compare("mobj->player", i);
    iter_compare("mobj->tracer", i);
    iter_compare("mobj->lastenemy", i);
  }

  for (int i = 0; i < comp->numsectors; i++) {
    iter_compare("sound target", i);
  }

  /** Specials **/

  // printf("(%d) Reading specials\n", comp->gametic);

  read_compare("thinker class", &tc);

  while (tc != tc_endspecials) {
    switch (tc) {
      case tc_ceiling:
        iter_compare("ceiling->type", s);
        iter_compare("ceiling->sector", s);
        iter_compare("ceiling->bottomheight", s);
        iter_compare("ceiling->topheight", s);
        iter_compare("ceiling->speed", s);
        iter_compare("ceiling->oldspeed", s);
        iter_compare("ceiling->crush", s);
        iter_compare("ceiling->newspecial", s);
        iter_compare("ceiling->oldspecial", s);
        iter_compare("ceiling->texture", s);
        iter_compare("ceiling->direction", s);
        iter_compare("ceiling->tag", s);
        iter_compare("ceiling->olddirection", s);
        break;
      case tc_door:
        iter_compare("door->type", s);
        iter_compare("door->sector", s);
        iter_compare("door->topheight", s);
        iter_compare("door->speed", s);
        iter_compare("door->direction", s);
        iter_compare("door->topwait", s);
        iter_compare("door->topcountdown", s);
        iter_compare("door->line", s);
        iter_compare("door->lighttag", s);
        break;
      case tc_floor:
        iter_compare("floor->type", s);
        iter_compare("floor->crush", s);
        iter_compare("floor->sector", s);
        iter_compare("floor->direction", s);
        iter_compare("floor->newspecial", s);
        iter_compare("floor->oldspecial", s);
        iter_compare("floor->texture", s);
        iter_compare("floor->floordestheight", s);
        iter_compare("floor->speed", s);
        break;
      case tc_plat:
        iter_compare("plat->sector", s);
        iter_compare("plat->speed", s);
        iter_compare("plat->low", s);
        iter_compare("plat->high", s);
        iter_compare("plat->wait", s);
        iter_compare("plat->count", s);
        iter_compare("plat->status", s);
        iter_compare("plat->oldstatus", s);
        iter_compare("plat->crush", s);
        iter_compare("plat->tag", s);
        iter_compare("plat->type", s);
        break;
      case tc_flash:
        iter_compare("flash->sector", s);
        iter_compare("flash->count", s);
        iter_compare("flash->maxlight", s);
        iter_compare("flash->minlight", s);
        iter_compare("flash->maxtime", s);
        iter_compare("flash->mintime", s);
        break;
      case tc_strobe:
        iter_compare("strobe->sector", s);
        iter_compare("strobe->count", s);
        iter_compare("strobe->minlight", s);
        iter_compare("strobe->maxlight", s);
        iter_compare("strobe->darktime", s);
        iter_compare("strobe->brighttime", s);
        break;
      case tc_glow:
        iter_compare("glow->sector", s);
        iter_compare("glow->maxlight", s);
        iter_compare("glow->minlight", s);
        iter_compare("glow->direction", s);
        break;
      case tc_flicker:
        iter_compare("flicker->sector", s);
        iter_compare("flicker->count", s);
        iter_compare("flicker->maxlight", s);
        iter_compare("flicker->minlight", s);
        break;
      case tc_elevator:
        iter_compare("elevator->type", s);
        iter_compare("elevator->sector", s);
        iter_compare("elevator->direction", s);
        iter_compare("elevator->floordestheight", s);
        iter_compare("elevator->ceilingdestheight", s);
        iter_compare("elevator->speed", s);
        break;
      case tc_scroll:
        iter_compare("scroll->dx", s);
        iter_compare("scroll->dy", s);
        iter_compare("scroll->affectee", s);
        iter_compare("scroll->control", s);
        iter_compare("scroll->last_height", s);
        iter_compare("scroll->vdx", s);
        iter_compare("scroll->vdy", s);
        iter_compare("scroll->accel", s);
        iter_compare("scroll->type", s);
        break;
      case tc_pusher:
        iter_compare("pusher->type", s);
        iter_compare("pusher->x_mag", s);
        iter_compare("pusher->y_mag", s);
        iter_compare("pusher->magnitude", s);
        iter_compare("pusher->radius", s);
        iter_compare("pusher->x", s);
        iter_compare("pusher->y", s);
        iter_compare("pusher->affectee", s);
        break;
      default:
        die("Unknown thinker class %d in savegame", tc);
        break;
    }
    s++;
    read_iter_compare("thinker class", s, &tc);
  }

  /** AutoMap **/

  // printf("(%d) Reading AutoMap\n", comp->gametic);

  compare("automapmode");
  read_compare("markpointnum", &comp->markpointnum);

  for (int i = 0; i < comp->markpointnum; i++) {
    iter_compare("markpoints[i].x", i);
    iter_compare("markpoints[i].y", i);
    iter_compare("markpoints[i].w", i);
    iter_compare("markpoints[i].h", i);

    for (int j = 0; j < 16; j++) {
      iter_compare2("markpoints[i].label[j]", i, j);
    }

    for (int j = 0; j < 16; j++) {
      iter_compare2("markpoints[i].widths[j]", i, j);
    }
  }
}

bool comp_eof(comp_ctx_t *comp) {
  if (pbuf_get_cursor(&comp->dump_pbuf1) >= pbuf_get_size(&comp->dump_pbuf1)) {
    return true;
  }

  if (pbuf_get_cursor(&comp->dump_pbuf2) >= pbuf_get_size(&comp->dump_pbuf2)) {
    return true;
  }

  return false;
}

/* vi: set et ts=2 sw=2: */

