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

#ifndef DOOM_H__
#define DOOM_H__

#define MAXPLAYERS      4
#define MIN_MAXPLAYERS 32
#define COMP_TOTAL     32
#define COMPERR_NUM     5

typedef enum
{
  ps_weapon,
  ps_flash,
  NUMPSPRITES
} psprnum_t;

typedef enum {
  it_bluecard,
  it_yellowcard,
  it_redcard,
  it_blueskull,
  it_yellowskull,
  it_redskull,
  NUMCARDS
} card_t;

// The defined weapons, including a marker
// indicating user has not changed weapon.
typedef enum {
  wp_fist,
  wp_pistol,
  wp_shotgun,
  wp_chaingun,
  wp_missile,
  wp_plasma,
  wp_bfg,
  wp_chainsaw,
  wp_supershotgun,

  NUMWEAPONS,
  wp_nochange              // No pending weapon change.
} weapontype_t;

// Ammunition types defined.
typedef enum {
  am_clip,    // Pistol / chaingun ammo.
  am_shell,   // Shotgun / double barreled shotgun.
  am_cell,    // Plasma rifle, BFG.
  am_misl,    // Missile launcher.
  NUMAMMO,
  am_noammo   // Unlimited for chainsaw / fist.
} ammotype_t;

// Power up artifacts.
typedef enum {
  pw_invulnerability,
  pw_strength,
  pw_invisibility,
  pw_ironfeet,
  pw_allmap,
  pw_infrared,
  NUMPOWERS
} powertype_t;

enum {
  tc_ceiling,
  tc_door,
  tc_floor,
  tc_plat,
  tc_flash,
  tc_strobe,
  tc_glow,
  tc_elevator,    //jff 2/22/98 new elevator type thinker
  tc_scroll,      // killough 3/7/98: new scroll effect thinker
  tc_pusher,      // phares 3/22/98:  new push/pull effect thinker
  tc_flicker,     // killough 10/4/98
  tc_endspecials
} specials_e;

typedef enum {
  pr_skullfly,                // #0
  pr_damage,                  // #1
  pr_crush,                   // #2
  pr_genlift,                 // #3
  pr_killtics,                // #4
  pr_damagemobj,              // #5
  pr_painchance,              // #6
  pr_lights,                  // #7
  pr_explode,                 // #8
  pr_respawn,                 // #9
  pr_lastlook,                // #10
  pr_spawnthing,              // #11
  pr_spawnpuff,               // #12
  pr_spawnblood,              // #13
  pr_missile,                 // #14
  pr_shadow,                  // #15
  pr_plats,                   // #16
  pr_punch,                   // #17
  pr_punchangle,              // #18
  pr_saw,                     // #19
  pr_plasma,                  // #20
  pr_gunshot,                 // #21
  pr_misfire,                 // #22
  pr_shotgun,                 // #23
  pr_bfg,                     // #24
  pr_slimehurt,               // #25
  pr_dmspawn,                 // #26
  pr_missrange,               // #27
  pr_trywalk,                 // #28
  pr_newchase,                // #29
  pr_newchasedir,             // #30
  pr_see,                     // #31
  pr_facetarget,              // #32
  pr_posattack,               // #33
  pr_sposattack,              // #34
  pr_cposattack,              // #35
  pr_spidrefire,              // #36
  pr_troopattack,             // #37
  pr_sargattack,              // #38
  pr_headattack,              // #39
  pr_bruisattack,             // #40
  pr_tracer,                  // #41
  pr_skelfist,                // #42
  pr_scream,                  // #43
  pr_brainscream,             // #44
  pr_cposrefire,              // #45
  pr_brainexp,                // #46
  pr_spawnfly,                // #47
  pr_misc,                    // #48
  pr_all_in_one,              // #49
  /* CPhipps - new entries from MBF, mostly unused for now */
  pr_opendoor,                // #50
  pr_targetsearch,            // #51
  pr_friends,                 // #52
  pr_threshold,               // #53
  pr_skiptarget,              // #54
  pr_enemystrafe,             // #55
  pr_avoidcrush,              // #57
  pr_stayonlift,              // #57
  pr_helpfriend,              // #58
  pr_dropoff,                 // #59
  pr_randomjump,              // #60
  pr_defect,                  // #61  // Start new entries -- add new entries below

  // End of new entries
  NUMPRCLASS               // MUST be last item in list
} pr_class_t;

#endif

/* vi: set et ts=2 sw=2: */

