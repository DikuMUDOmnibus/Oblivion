/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "critical.h"


/* item type list */
const struct item_type		item_table	[]	=
{
	 { 0, ""},
	 {	ITEM_LIGHT,	"light"		},
	 {	ITEM_SCROLL,	"scroll"	},
	 {	ITEM_WAND,	"wand"		},
	 {   ITEM_STAFF,	"staff"		},
	 {   ITEM_WEAPON,	"weapon"	},           /* 5 OLC */
	 { 0, ""},
	 { 0, ""},
	 {   ITEM_TREASURE,	"treasure"	},     /* 8 OLC */
	 {   ITEM_ARMOR,	"armor"		},
	 {	ITEM_POTION,	"potion"	},
	 {	ITEM_CLOTHING,	"clothing"	},
	 {   ITEM_FURNITURE,	"furniture"	},
	 {	ITEM_TRASH,	"trash"		},         /* 13 OLC */
	 { 0, ""},
	 {	ITEM_CONTAINER,	"container"	},
	 { 0, ""},
	 {	ITEM_DRINK_CON, "drink"		},     /* 17 OLC */
	 {	ITEM_KEY,	"key"		},
	 {	ITEM_FOOD,	"food"		},
	 {	ITEM_MONEY,	"money"		},       /* 20 OLC */
	 { 0, ""},
	 {	ITEM_BOAT,	"boat"		},       /* 22 OLC */
	 {	ITEM_CORPSE_NPC,"npc_corpse"	},
	 {	ITEM_CORPSE_PC,	"pc_corpse"	},
	 {   ITEM_FOUNTAIN,	"fountain"	},
	 {	ITEM_PILL,	"pill"		},
	 {	ITEM_PROTECT,	"protect"	},
	 {	ITEM_MAP,	"map"		},
	 {	ITEM_PORTAL,	"portal"	},
	 {	ITEM_WARP_STONE,"warp_stone"	},
	 {	ITEM_ROOM_KEY,	"room_key"	},
	 {	ITEM_GEM,	"gem"		},
	 {	ITEM_JEWELRY,	"jewelry"	},
	 {   ITEM_JUKEBOX,	"jukebox"	},      /* 34 OLC */
	 {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
	{ "exotic", 0, 0, NULL },
	{ "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
	{ "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
	{ "spear",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
	{ "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
	{ "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
	{ "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
	{ "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
   { NULL,	0,				0,	NULL		}
};


 
/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           WIZ_ON,         IM },
   {    "prefix",	WIZ_PREFIX,	IM },
   {    "ticks",        WIZ_TICKS,      IM },
	{    "logins",       WIZ_LOGINS,     IM },
   {    "sites",        WIZ_SITES,      L4 },
   {    "links",        WIZ_LINKS,      L7 },
	{	"newbies",	WIZ_NEWBIE,	IM },
   {	"spam",		WIZ_SPAM,	L5 },
   {    "deaths",       WIZ_DEATHS,     IM },
   {    "resets",       WIZ_RESETS,     L4 },
   {    "mobdeaths",    WIZ_MOBDEATHS,  L4 },
	{    "flags",	WIZ_FLAGS,	L5 },
	{	"penalties",	WIZ_PENALTIES,	L5 },
   {	"saccing",	WIZ_SACCING,	L5 },
   {	"levels",	WIZ_LEVELS,	IM },
   {	"load",		WIZ_LOAD,	L2 },
   {	"restore",	WIZ_RESTORE,	L2 },
   {	"snoops",	WIZ_SNOOPS,	L2 },
   {	"switches",	WIZ_SWITCHES,	L2 },
   {	"secure",	WIZ_SECURE,	L1 },
   {    "rpexp",        WIZ_RPEXP,      L1 },
   {	NULL,		0,		0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[]		=
{
    { 	"none",		"hit",		-1		},  /*  0 */
	 {	"slice",	"slice", 	DAM_SLASH	},
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
	 {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
	 {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
	 {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
	 {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
	 {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
	 {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
	 {   "flame",	"flame",	DAM_FIRE	},
	 {   "chill",	"chill",	DAM_COLD	},
	 {   NULL,		NULL,		0		}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{
/*
    {
	name,		pc_race?,
	act bits,	aff_by bits,	off bits,
	imm,		res,		vuln,
	form,		parts 
    },
*/
	 { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 
	"human",		TRUE,
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"elf",			TRUE,
	0,		AFF_INFRARED,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"dwarf",		TRUE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"half-elf",		TRUE,
	0,		AFF_INFRARED,		0,
	0,		RES_CHARM|RES_POISON,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"gnome",		TRUE,
	0,		AFF_INFRARED,		0,
	0,		RES_POISON|RES_DISEASE|RES_MAGIC,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"halfling",		TRUE,
	0,		AFF_INFRARED,		0,
	0,		RES_POISON|RES_DISEASE|RES_MAGIC,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"half-orc",		TRUE,
	0,		AFF_INFRARED,		0,
	0,		0,						0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"orc",		TRUE,
	0,		AFF_INFRARED,		0,
	0,		0,						0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	"draconian",		TRUE,
	0,		AFF_FLYING,		0,
	0,		RES_FIRE|RES_LIGHTNING|RES_COLD|RES_ACID,	0,
	A|H|M|X,	A|B|C|D|E|F|G|H|I|J|K|L|P|Q|U|V|X
	 },

	 {
	 "drow", TRUE,
	 0, AFF_INFRARED|AFF_DARK_VISION|AFF_DETECT_MAGIC, 0,
	 0, RES_MAGIC, VULN_LIGHT,
	 A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
	 },

	 {
	 "avian", TRUE,
	 0, AFF_FLYING|AFF_DETECT_INVIS, 0,
	 0, RES_PIERCE, VULN_BASH,
	 A|H|M|W, A|B|C|D|E|F|G|H|I|J|K|P
	 },

	 {
	 "gnoll", TRUE,
	 0, 0, 0,
	 0, RES_BASH|RES_COLD|RES_DISEASE, 0,
	 A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
	 },

    {   "dragon",               TRUE,
        0,                      AFF_INFRARED|AFF_FLYING,        0,
        0,                      RES_FIRE|RES_BASH|RES_CHARM,
        VULN_PIERCE|VULN_COLD,
        A|H|Z,          A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    {   "crystal dragon",        TRUE,
        0,                      AFF_INFRARED|AFF_FLYING,        0,
        IMM_LIGHT,              RES_FIRE|RES_BASH|RES_CHARM,
        VULN_PIERCE|VULN_COLD,
        A|H|Z,          A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },



	 {
	"bat",			FALSE,
	0,		AFF_FLYING|AFF_DARK_VISION,	OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
	 },

    {
	"bear",			FALSE,
	0,		0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {
	"cat",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {
	"centipede",		FALSE,
	0,		AFF_DARK_VISION,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
	A|B|G|O,		A|C|K
	 },

    {
	"dog",			FALSE,
	0,		0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
	 },

    {
	"doll",			FALSE,
	0,		0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
	 },

	 {
	"fido",			FALSE,
	0,		0,		OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },		
   
    {
	"fox",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    {
	"goblin",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"hobgoblin",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    {
	"kobold",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {
	"lizard",		FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {
	"modron",		FALSE,
	0,		AFF_INFRARED,		ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

	 {
	"pig",			FALSE,
	0,		0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },	

    {
	"rabbit",		FALSE,
	0,		0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
	 },

    {
	"school monster",	FALSE,
	ACT_NOALIGN,		0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },	

    {
	"snake",		FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },
 
    {
	"song bird",		FALSE,
	0,		AFF_FLYING,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
	 },

    {
	"troll",		FALSE,
	0,		AFF_REGENERATION|AFF_INFRARED|AFF_DETECT_HIDDEN,
	OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    {
	"water fowl",		FALSE,
	0,		AFF_SWIM|AFF_FLYING,	0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },		

	 {
	"wolf",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,	
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {
	"wyvern",		FALSE,
	0,		AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN,
	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
	 },

	{
	"unicorn", FALSE,
	0,  AFF_DETECT_EVIL|AFF_SANCTUARY|AFF_DETECT_HIDDEN|AFF_DETECT_INVIS|AFF_PROTECT_EVIL|AFF_REGENERATION,
	OFF_FAST|OFF_KICK,
	IMM_SUMMON|IMM_CHARM|IMM_DROWNING|IMM_NEGATIVE|IMM_DISEASE,
        A|G|V,          A|C|D|E|F|H|J|K|Q|V
	},

    {
	"unique",		FALSE,
	0,		0,		0,
	0,		0,		0,		
	0,		0
    },


	 {
	NULL, 0, 0, 0, 0, 0, 0
	 }
};

const	struct	pc_race_type	pc_race_table	[]	=
{
      { "null race", "", 0, { 100, 100, 100, 100 },
      { "" }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0 },

/*
	 {
	"race name", 	short name, 	points,	{ class multipliers },
	{ bonus skills },
	{ base stats },		{ max stats },		size,
	"base language"
	 },
*/
	 {
	"human",	"Human ",	0,	{ 100, 100, 100, 100, 100, 100, 100, 100, 150 },
	{ "" },
	{ 5, 0, 0, 0, 0, 0, 5, 0, 0, 0 },	SIZE_MEDIUM,
	"human",10,150,45,60
	 },

	 {
	"elf",		" Elf ",	5,	{ 100, 125,  100, 120, 125, 130, 100, 175, 150 },
	{ "sneak", "hide" },
	{ 0, 15, 15, 5, 0, 0, 5, -20, 5, 0 }, SIZE_SMALL,
        "elven",8,120,40,60
	 },

	 {
	"dwarf",	"Dwarf ",	8,	{ 150, 100, 125, 100, 175, 225, 175, 100, 175 },
	{ "berserk" },
	{ 5, -5, -10, -10, 0, 15, -5, 5, 0, 0 }, SIZE_MEDIUM,
	"dwarven",15,165,35,50
	 },

	 {
	"half-elf",	"Hf-Elf",	4,	{ 100, 100, 100, 100, 100, 100, 100, 125, 150 },
	{ "" },
	{ 5, 10, 10, 0, 0, 5, 5, -10, 0, 0 }, SIZE_MEDIUM,
	"human",10,150,45,60
	 },

	{
	"gnome",	"Gnome ",	5,	{ 150, 145, 100, 100, 200, 175, 130, 200, 200 },
	{ "" },
	{ -5, -5, -5, 15, -10, 5, 0, 5, 10, 5 }, SIZE_SMALL,
	"gnomish",9,120,30,50
	},

	{
	"halfling",	"Hfling",	8,	{ 150, 150, 100, 100, 200, 200, 200, 250, 175 },
	{ "sneak", "hide" },
	{ -20, 10, -15, -5, 0, 15, 15, -10, 0, 0 }, SIZE_SMALL,
	"halfling",8,110,30,50
	 },

	{
	"half-orc",	"Hf-Orc",	6,	{ 150, 125, 100, 100, 200, 175, 175, 100, 200 },
	{ "berserk" },
	{ 5,0,-5,-5,-10,5,0,-5,-5,0 }, SIZE_MEDIUM,
	"human",12,140,50,65
	},

	{
	"orc",	" Orc  ",	8,	{ 150, 125, 100, 100, 200, 175, 175, 100, 225 },
	{ "berserk", "mace", "bash" },
	{ 10,0,-5,-5,-5,10,0,-5,-5,-5 }, SIZE_MEDIUM,
	"orcish",13,220,50,70
	},

	{
	"draconian",	" Drac ",	25, { 125, 125, 100, 100, 125, 175, 100, 150, 200 },
	{ "bash", "berserk" },
	{ 10,5,-5,10,-5,10,-5,-5,5,0 }, SIZE_MEDIUM,
	"human",12,200,50,70
	},

	{
	"drow", " Drow ", 21, { 100, 100, 100, 105, 140, 120, 150, 250, 150 },
	{ "sneak", "hide" },
	{ -5, 10, 5, 5, 5, 0, 10, -15, 0, 5 }, SIZE_SMALL,
	"drow",10,100,45,55
	},

	{
	"avian", "Avian ", 32, { 100, 100, 125, 125, 100, 100, 100, 200, 175 },
	{ "" },
	{ 0, 15, 10, 5, 5, 5, 10, 0, -5, 5}, SIZE_MEDIUM,
	"avian",5,90,45,55
	},

	{
	"gnoll", "Gnoll ", 16, { 150, 100, 125, 100, 125, 175, 175, 100, 225 },
	{ "berserk", "spear", "bash", "polearm" },
	{ 10,0,-5,-5,-5,25,5,-20,0,0 }, SIZE_MEDIUM,
	"gnoll",15,200,50,70
	},

        {
	"dragon", "Dragon", 40, { 200,200,200,200,200,200,200,200,200 },
	{ "" },
	{ 60, -10, 50, 20, 20, 50, -15, -20, 5, 15}, SIZE_GIANT,
	"human",40,200,50,100
	},

	{
        "crystal dragon", "Dragon", 40, { 200,200,200,200,200,200,200,200,200 },
        { "" },
        {60,0,50,20,20,50,25,20,10,20}, SIZE_MEDIUM,
	"human",40,200,50,100
        }

};




/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
	 {
	"mage", "Mag",  {STAT_EM,STAT_RE},  
	{ 3018, 9618 },  75,  20, 6,  2,  5, TRUE,
	"mage basics", "mage default",
        {2,9,7,1,6,8,5,3,4,0}
	 },

	 {
	"cleric", "Cle",  {STAT_IN,STAT_ME}, 
	{ 3003, 9619 },  75,  20, 2,  2, 7, TRUE,
	"cleric basics", "cleric default",
        {4,8,0,7,5,2,1,6,3,9}
	 },

	 {
	"thief", "Thi",  {STAT_QU,STAT_AG}, 
	{ 3028, 9639 },  75,  20,  -4,  2, 8, FALSE,
	"thief basics", "thief default",
        {6,0,1,5,2,7,9,8,3,4}
	 },

	 {
	"warrior", "War",  {STAT_ST,STAT_CO},
	{ 3022, 9633 },  75,  20,  -10,  3, 10, FALSE,
	"warrior basics", "warrior default",
        {5,0,1,6,7,2,8,3,9,4}
	 },

	 {
	"druid", "Dru",  {STAT_IN,STAT_ME},
	{ 3003, 9619 },  75,  20,  0,  2, 7, TRUE,
	"cleric basics", "druid default",
        {8,4,7,9,2,3,5,6,1,0}
	 },

	 {
	"paladin", "Pal",  {STAT_ST,STAT_IN},
	{ 3022, 9633 },  75,  20,  -9,  3, 9, FALSE,
	"warrior basics", "paladin default",
        {0,4,8,5,6,1,2,7,3,9}
	 },

	 {
	"ranger", "Ran",  {STAT_IN,STAT_CO},
	{ 3022, 9633 },  75,  20,  -8,  3, 10, FALSE,
	"warrior basics", "ranger default",
        {5,4,6,7,1,0,8,3,2,9}
	 },

	 {
	 "barbarian", "Bar", {STAT_ST,STAT_CO},
	 { 3022, 9633}, 70, 20, -16, 4, 12, FALSE,
	 "barbarian basics", "barbarian default",
         {0,5,6,1,4,2,3,8,9,7}
	 },
 
         {
         "spellfilcher", "Spf", {STAT_EM,STAT_AG},
         {3028,9639}, 75, 20, 5, 2, 8, TRUE,
         "spellfilcher basics", "spellfilcher default",
         {3,6,1,9,7,0,5,4,2,8}
         }

};

/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
	 { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
	 { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
	 { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
	 { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

	 { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

	 { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
	 { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
	 { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
	 { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
	 { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   }, 
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
	 { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
	 { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

	 { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
	 { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
	 { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

        {
        "abjuration", {1,1,1,1,1,1,1,1,1}, {5,3,0,0,4,4,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0 ,0
        },

        {
        "alteration", {1,1,1,1,1,1,1,1,1}, {6,3,0,0,4,4,4,0,6},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "conjuration", {1,1,1,1,1,1,1,1,1}, {4,2,0,0,3,3,3,0,4},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "illusion", {1,1,1,1,1,1,1,1,1}, {5,0,0,0,0,0,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "phantasm", {1,1,1,1,1,1,1,1,1}, {5,0,0,0,0,0,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "invocation", {1,1,1,1,1,1,1,1,1}, {5,4,0,0,4,4,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "evocation", {1,1,1,1,1,1,1,1,1}, {6,0,0,0,0,0,5,0,6},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "enchantment", {1,1,1,1,1,1,1,1,1}, {4,3,0,0,3,3,3,0,4},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "charm", {1,1,1,1,1,1,1,1,1}, {4,3,0,0,3,3,3,0,4},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "divination", {1,1,1,1,1,1,1,1,1}, {5,4,0,0,3,4,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "necromancy", {1,1,1,1,1,1,1,1,1}, {5,5,0,0,5,5,4,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "wild magic", {1,1,1,1,1,1,1,1,1}, {4,0,0,0,0,0,0,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "earth", {1,1,1,1,1,1,1,1,1}, {2,2,0,0,2,2,2,0,2},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "air", {1,1,1,1,1,1,1,1,1}, {2,2,0,0,2,2,2,0,2},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "fire", {1,1,1,1,1,1,1,1,1}, {2,2,0,0,2,2,2,0,2},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "water", {1,1,1,1,1,1,1,1,1}, {2,2,0,0,2,2,2,0,2},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
        {
        "weather", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "protection", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "plant", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "law", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "sun", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "animal", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "combat", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "creation", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },
         {
        "time", {1,1,1,1,1,1,1,1,1}, {3,3,0,0,3,3,3,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },


/*
 * Magic spells.
 */

	 {
	"reserved",		{ 101,101,101,101,101,101,101,101,101 },	{ 101, 101, 101, 101, 101, 101 ,101, 101,101 },
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		"",
        0, 0
	 },

	 {
	"acid blast",		{ 4, 93, 93, 93, 93, 93, 7, 93,4 }, 	{ 2,  0,  0,  0, 0, 0, 3,0,2},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	25,	12,
	"acid blast",		"!Acid Blast!", "",
        SCHOOL_EVOCATION, 0
	 },

	 {
	"animal essence", { 93, 93, 93, 93, 1, 5, 2, 93,93 },		{ 1, 1, 2, 2, 1, 1, 1, 2,2 },
	spell_animal_essence, 	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(204),	50, 	12,
	"",	"You loose the ability of the eagle.", "",
        SCHOOL_ALTERATION|SCHOOL_DIVINATION,
        SPHERE_ANIMAL
	 },


	 {
	"armor",		{  1,  93, 93, 93, 5, 93, 2,93,1 },     { 1,  1,  1,  1, 1, 1, 1,1,1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	"",
        SCHOOL_CONJURATION,
        0
	 },

	 {
	 "barkskin", {93,3,93,93,2,93,4,93,93}, {1,2,1,1,1,2,1,2,2},
	 spell_barkskin, TAR_CHAR_DEFENSIVE, POS_STANDING,
	 NULL, SLOT(214), 15, 12,
	 "", "Your skin is no longer woody.", "",
         SCHOOL_ALTERATION,
         SPHERE_PROTECTION|SPHERE_PLANT
	 },

	 {
	"bless",		{ 93,  1, 93, 93, 2, 2, 3, 93,93 },     { 1,  1,  2,  2, 1, 1, 1,2,2},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous.",
	"$p's holy aura fades.",
        SCHOOL_CONJURATION|SCHOOL_SUMMONING,
        0
	 },

	 {
	"blindness",		{  93, 2, 93, 93, 3, 3, 4,93,93 },     { 1,  1,  2,2,1,1,2,2,2},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	"",
        SCHOOL_ABJURATION|SCHOOL_NECROMANCY,
        0 
	 },

	 {
	"burning hands",	{  1, 93, 93, 93, 93,93,2,93,1},     { 1,  1,  2,  2,2,2,1,2,1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	"",
        SCHOOL_ALTERATION,
        0
	 },

	 {
	"call lightning",	{ 93, 3, 93, 93,3,33,4,93,93 },     { 1,  1,  2,2,1,1,2,2,1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	"",
        SCHOOL_ALTERATION,
 	SPHERE_WEATHER       
	 },

	{   "calm",	{ 93, 4, 93, 93,3,5,4,93,93 },     { 1,  1,  2,  2,2,2,2,2,2},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	"",
        SCHOOL_CHARM|SCHOOL_ENCHANTMENT,
        SPHERE_LAW
	 },

	 {
	"cancellation",		{ 4, 5, 93, 93,5,6,7,93,4},     { 1,1,2,2,1,1,1,2,1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	"",			"!cancellation!",	"",
        SCHOOL_ABJURATION,
	0
	 },

	 {
	"cause critical",	{ 93, 3, 93, 93,3,4,5,93,93 },     { 1,  1,  2,  2,1,1,2,2,1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	"",
        SCHOOL_NECROMANCY,
        SPHERE_HEALING
	 },

	 {
	"cause light",		{ 93,  1, 93, 93,1,2,2,93,93},     { 1,  1,  2,  2,1,1,1,2,1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	"",
        SCHOOL_NECROMANCY,
        SPHERE_HEALING,
	 },

	 {
	"cause serious",	{ 93,  2, 93, 93,2,3,3,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	"",
        SCHOOL_NECROMANCY,
        SPHERE_HEALING,
	 },

	 {
	"chain lightning",	{ 6, 93, 93, 93,93,93,9,93,6 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	"",
        SCHOOL_EVOCATION,
        0
	 },

	 {
	"change sex",		{ 93, 93, 93, 93,93,93,93,93,93 },     { 1,  1,  2,  2,2,2,2,2,2},
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",	"",
        0,
        0
	 },

	 {
	"charm person",		{ 1, 93, 93, 93,93,93,3,93,1},     { 1,  1,  2,  2,2,2,2,2,1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident.",	"",
        SCHOOL_ENCHANTMENT|SCHOOL_CHARM,
 	0
	 },

	{
	"chaos lace",   { 3, 93, 93, 93,93,93,93,93,4 }, {2,4,4,4,4,4,4,4,2}, 
        spell_chaos_lace,    TAR_OBJ_INV,            POS_STANDING,
        NULL,                   SLOT(300),      100,    24,
        "",                     "!Chaos Lace!",      "",
        SCHOOL_ENCHANTMENT,
        DISC_WILD_MAGIC

	},

	 {
	"chill shield",	 { 4, 93, 93, 93,93,93,9,93,4 },  { 2,4,4,4,4,4,4,4,2},
	spell_chill_shield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_chill_shield,	SLOT(220),	75,	12,
	"icy aura",	"The icy aura around your body dies.", 	"$p's icy aura fades.",
 	SCHOOL_EVOCATION|SCHOOL_ALTERATION,
        DISC_ELEM_WATER
	 },

	 {
	"chill touch",		{  1, 93, 93, 93,93,93,2,93,1},     { 1,  1,  2,  2,2,2,2,2,1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	"",
        SCHOOL_NECROMANCY,
	0
	 },

 /*       {
        "cobble",   { 4, 93, 93, 93,93,93,93,93,5 }, {2,4,4,4,4,4,4,4,2},
        spell_cobble,           TAR_OBJ_INV,            POS_STANDING,
        NULL,                   SLOT(598),      200,    120,
        "",                     "!Cobble!",      "",
        SCHOOL_ALTERATION,
        0
        },
*/

	 {
	"colour spray",		{ 1, 93, 93, 93,93,93,2,93,1 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	"",
        SCHOOL_ALTERATION,
	0
	 },

	 {
	 "cone cold", {5,93,93,93,93,93,5,93,5}, {1,2,2,2,2,2,1,2,1},
	 spell_cone_cold, TAR_IGNORE, POS_FIGHTING,
	 NULL, SLOT(216), 45, 36,
	 "cone of cold", "You feel less chilled.", "",
         SCHOOL_EVOCATION,
         DISC_ELEM_WATER
	 },

	 {
	"continual light",	{  2,  2, 93, 93,2,3,3,93,2 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	"",
        SCHOOL_ALTERATION,
	0
	 },

	 {
	"control weather",	{ 93, 7, 93, 93,5,9,7,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	"",
        SCHOOL_ALTERATION,
        SPHERE_LAW|SPHERE_WEATHER
	},

	 {
	"create buffet",	{ 93, 2, 93, 93,2,3,3,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_create_buffet,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(230),	 50,	12,
	"",			"!Create Buffet!",	"",
        SCHOOL_CONJURATION,
	0
	 },


	 {
	"create food",		{ 93, 1, 93, 93,1,2,2,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	"",
	SCHOOL_CONJURATION,
	0 
	 },

	 {
	"create rose",		{ 3, 3, 93, 93,3,5,5,93,4 }, 	{ 1, 1, 2, 2,1,1,1,2,1 },
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	"",
        SCHOOL_CONJURATION,
	0
	 },

	 {
	"create spring",	{ 3, 1, 93, 93,1,2,2,93,3 },     { 1,  1,  2,  2,1,1,1,2,2},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	"",
	SCHOOL_ALTERATION,
	DISC_ELEM_WATER
	 },

	 {
	"create water",		{ 2,  1, 93, 93,1,1,1,93,2},     { 1,  1,  2,  2,1,1,1,2,1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	"",
	SCHOOL_ALTERATION,
        DISC_ELEM_WATER
	 },

	 {
	"cure blindness",	{ 93,  2, 93, 93,3,3,4,93,93 },     { 1,  1,  2,  2,1,1,2,2,1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	"",
        SCHOOL_ABJURATION|SCHOOL_NECROMANCY,
	0
	 },

	 {
	"cure critical",	{ 93,  3, 93, 93,3,4,5,93,93 },     { 1,  1,  2,  2,1,1,2,2,1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(15),	20,	12,
	"",			"!Cure Critical!",	"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"cure disease",		{ 93, 3, 93, 93,3,4,5,93,93},     { 1,  1,  2,  2,1,1,2,2,1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	"",
	SCHOOL_ABJURATION|SCHOOL_NECROMANCY,
	0
	 },

	 {
	"cure light",		{ 93,  1, 93, 93,1,2,2,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!",		"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"cure poison",		{ 93,  3, 93, 93,3,4,5,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	"",
	SCHOOL_ABJURATION|SCHOOL_NECROMANCY,
	0
	 },

	 {
	"cure serious",		{ 93,  2, 93, 93,2,3,3,93,93 },     { 1,  1,  2,  2,1,1,2,2,1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!Cure Serious!",	"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"curse",		{ 93, 1, 93, 93,1,2,3,93,93 },     { 1,  1,  2,  2,1,1,1,1,1},
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.",
	"$p is no longer impure.",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

	 {
	"detect evil",		{ 1,  1, 93, 93,1,2,2,93,1 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
		  "detect good",          { 1,  1, 93, 93,1,2,2,93,1 }, { 1,  1,  2,  2,1,1,1,1,1},
		  spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
		  NULL,                   SLOT(513),        5,     12,
		  "",                     "The gold in your vision disappears.",	"",
		  SCHOOL_DIVINATION,
		  0
	 },

	 {
	"detect hidden",	{ 3, 2, 93, 93,2,3,3,93,3 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(44),	 5,	12,
	"",			"You feel less aware of your surroundings.",
	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	"detect invis",		{  2,  93, 93, 93,4,93,5,93,2 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	"detect magic",		{  1, 93, 93, 93,1,3,3,93,1 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	"detect poison",	{ 93,  2, 93, 93,1,3,4,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	"",
	SCHOOL_DIVINATION,
	SPHERE_PLANT
	 },

	 {
	"dispel evil",		{ 93, 3, 93, 93,3,5,6,93,93 },     { 1,  1,  2,  2,1,1,2,2,1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	"",
	SCHOOL_ABJURATION,
	0
	 },

	 {
	"dispel good",          { 93, 3, 93, 93,3,5,6,93,93},  {1,1,2,2,1,1,2,2,1},
	spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,                   SLOT(512),      15,     12,
	"dispel good",          "!Dispel Good!",	"",
	SCHOOL_ABJURATION,
	0
	 },

	 {
	"dispel magic",		{3,3,93,93,3,5,6,93,3 },     { 1,  1,  2,  2,1,2,1,2,1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	"",
	SCHOOL_ABJURATION,
	0
	 },


	 {
	"drain blade",	{ 5, 6, 93, 93,93,5,93,93,5 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_drain_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(225),	150,	24,
	"",			"!Drain Blade!",	"$p looks less malevolant.",
	SCHOOL_ENCHANTMENT|SCHOOL_NECROMANCY,
	0	
	 },

	 {
	"earthquake",		{ 93,  5, 93, 93,4,93,93,93,93 },     { 1,  1,  2,  2,1,2,2,2,2},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		"",
	SCHOOL_ALTERATION,
	DISC_ELEM_EARTH
	 },

	 
	 {
	"empower blade",	{ 4, 93, 93, 93,93,5,93,93,4 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_empower_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(226),	100,	24,
	"",			"!Flame Blade!",	"$p looses its electric charge.",
	SCHOOL_ENCHANTMENT|SCHOOL_EVOCATION,
	0
	 },

	 {
	"enchant armor",	{ 3, 93, 93, 93,93,93,93,93,4 },	{ 2,  2,  4,  4,4,4,4,4,2 },
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	"",
	SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	"enchant weapon",	{ 3, 93, 93, 93,93,93,93,93,4 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	"",
	SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	"energy drain",		{ 5, 4, 96, 93,93,93,93,93,5 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!",	"",
	SCHOOL_NECROMANCY,
	0
	 },

        {
        "extension",   { 2, 93, 93, 93,93,93,93,93,4 }, {2,4,4,4,4,4,4,4,2},
        spell_extension,           TAR_OBJ_INV,            POS_STANDING,
        NULL,                   SLOT(599),      200,    48,
        "",                     "!Extension!",      "",
        SCHOOL_ENCHANTMENT|SCHOOL_ALTERATION,
        SPHERE_TIME
        },


	 {
	"faerie fire",		{ 93, 1, 93, 93,1,93,2,93,93 },     { 1,  1,  2,  2,1,2,1,2,1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",	"",
	SCHOOL_ALTERATION,
	SPHERE_WEATHER
	 },

	 {
	"faerie fog",		{ 93, 2, 93, 93,2,93,2,93,93},     { 1,  1,  2,  2,1,2,1,2,1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!",		"",
	SCHOOL_CONJURATION,
	SPHERE_WEATHER
	 },

	 {
	"farsight",		{ 3, 3, 93, 93,3,3,3,93,3 },	{ 1, 1, 2, 2,1,1,1,2,1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	36,	20,
	"farsight",		"!Farsight!",		"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	"fireball",		{ 2, 93, 93, 93,93,93,4,93,2 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		"",
	SCHOOL_EVOCATION,
	DISC_ELEM_FIRE
	 },

	 {
	"fireproof",		{ 1, 1, 93, 93,1,31,31,93,1 },	{ 1, 1, 2, 2,1,1,1,2,1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades.",
	SCHOOL_ABJURATION|SCHOOL_ENCHANTMENT,
	DISC_ELEM_FIRE
	 },

	 {
	"fire shield",	 { 4, 93, 93, 93,93,93,9,93,4 },  { 2,4,4,4,4,4,4,4,2},
	spell_fire_shield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fire_shield,	SLOT(219),	75,	12,
	"flaming aura",	"The fire around your body dies.", 	"$p's fiery aura fades.",
	SCHOOL_EVOCATION|SCHOOL_ALTERATION,
	DISC_ELEM_FIRE
	 },


	 {
	"frostball",	{ 5, 93, 93, 93, 93, 93, 9,93,5 },		{ 1, 1, 2, 2, 1, 2, 1,2,1 },
	spell_frostball,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(202),	30,	6,
	"frostball",	"!FrostBall!",	"",
	SCHOOL_EVOCATION,
	DISC_ELEM_WATER
	},

	 {
	"flame blade",	{ 3, 93, 93, 93,93,3,93,93,3 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_flame_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(223),	100,	24,
	"",			"!Flame Blade!",	"$p looses its fiery glow.",
	SCHOOL_ENCHANTMENT,
	DISC_ELEM_FIRE
	 },


	 {
	"flamestrike",		{ 93, 4, 93, 93,3,6,93,93,93 },     { 1,  1,  2,  2,1,1,2,2,2},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		"",
	SCHOOL_CONJURATION,
	DISC_ELEM_FIRE
	 },

	 {
	"fly",			{ 2, 93, 93, 93,93,93,4,93,2},     { 1,  1,  2,  2,2,2,4,4,1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground.",	"",
	SCHOOL_ALTERATION,
	0
	 },

	 {
	"floating disc",	{  1, 93, 93, 93,93,93,2,93,1 },	{ 1, 1, 2, 2,1,1,1,2,1},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

	 {
        "frenzy",               { 93, 24, 93, 93,93,93,93,93,93 },{1,1,2,2,1,1,1,1,1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your rage ebbs.",	"",
	SCHOOL_INVOCATION|SCHOOL_SUMMONING,
	0
	 },

	 {
	"frost blade",	{ 3, 93, 93, 93,93,3,3,93,3 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_frost_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(224),	100,	24,
	"",			"!Frost Blade!",	"$p warms back up.",
	SCHOOL_ENCHANTMENT,
	DISC_ELEM_WATER
	 },

	 {
	"gate",			{ 4, 4, 93, 93,4,6,6,93,4 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

	 {
	"giant strength",	{  2, 93, 93, 93,93,93,4,93,2 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker.",	"",
	SCHOOL_ALTERATION,
	0
	 },

	 {
	"harm",			{ 93, 5, 93, 93,5,9,93,93,93 },     { 1,  1,  2,  2,1,2,2,2,1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!",		"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"haste",		{ 3, 93, 93, 93,93,93,7,93,3 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	"",
	SCHOOL_ALTERATION,
	0
	 },

	 {
	"heal",			{ 93, 5, 93, 93,5,9,93,93,93 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"heat metal",		{ 93, 2, 93, 93,93,4,93,93,93 },	{ 1, 1, 2, 2,2,2,2,2,2 },
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		"",
	SCHOOL_INVOCATION,
	DISC_ELEM_FIRE
	 },

	 {
	 "holy aura",   {93,50,93,93,6,7,93,93,93}, {2,4,4,4,2,4,4,4,4},
	 spell_holy_aura, TAR_CHAR_DEFENSIVE, POS_STANDING,
	 NULL,	SLOT(210), 60, 12,
	 "",	 "Your golden glow fades.",	 "$p's golden glow fades.",
	 SCHOOL_ABJURATION,
	 SPHERE_PROTECTION
	 },

	 {
	"holy word",		{ 93, 3, 93, 93,93,7,93,93,93 },	{ 2,  2,  4,  4,4,4,4,4,4},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	SPHERE_COMBAT
	 },

	 {
	 "ice storm", {5,93,93,93,93,93,93,93,5}, {1,2,2,2,2,2,1,2,2},
	 spell_ice_storm, TAR_IGNORE, POS_FIGHTING,
	 NULL, SLOT(217), 75, 36,
	 "ice storm", "The ice melts off your body.", "",
	 SCHOOL_EVOCATION,
	 DISC_ELEM_WATER
	 },


	 {
	"identify",		{ 1, 1, 93, 93,1,2,2,93,1 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	"",			"!Identify!",		"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	 "illusions grandeur", {5,93,93,93,93,93,93,93,5}, {2,2,2,2,2,2,2,2,2},
	 spell_illusions_granduer,  TAR_CHAR_SELF, POS_STANDING,
	 NULL,	SLOT(205), 75, 20,
	 "",  "You do not feel grand anymore.", "$p looks a little less grand.",
	SCHOOL_ILLUSION|SCHOOL_PHANTASM|SCHOOL_ALTERATION,
	0
	 },

	 {
	 "improved phantasm", {3,93,93,93,93,93,8,93,3}, {1,2,2,2,2,2,1,2,1},
	 spell_improved_phantasm,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	 NULL,	SLOT(209), 40, 12,
	 "phantasmal forces", "!Phantasmal Force!", "",
	 SCHOOL_ILLUSION|SCHOOL_PHANTASM,
	 0
	 },

	 {
	 "induce sleep", {2,26,93,93,1,4,3,93,2}, {1,1,2,2,1,1,1,2,1},
	 spell_induce_sleep, TAR_CHAR_DEFENSIVE, POS_RESTING,
	 NULL, SLOT(231), 30, 12,
	 "", "!Induce Sleep!",  "",
	 SCHOOL_CHARM,
	 0
	 },

	 {
	"infravision",		{  2,  1, 93, 93,1,2,2,93,2 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark.",	"",
	SCHOOL_DIVINATION|SCHOOL_ALTERATION,
	0
	 },

	 {
	"invisibility",		{  2, 93, 93, 93,93,93,93,93,2 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",
	"$p fades into view.",
	SCHOOL_ILLUSION,
	0
	 },

	 {
	"know alignment",	{  1,  1, 93, 93,1,2,3,93,1 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	"lightning bolt",	{  1, 2, 93, 93,2,93,39,93,1 }, { 1,  1,  2,  2,2,2,2,2,1},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	"",
	SCHOOL_EVOCATION,
	0
	 },

	 {
	"locate object",	{  2, 2, 93, 93,2,5,4,93,2 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	"",
	SCHOOL_DIVINATION,
	0
	 },

	 {
	 "magic resistance", {5,6,93,93,5,9,9,93,5}, {2,2,2,2,2,2,2,2,2},
	 spell_magic_resistance, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT(215), 30, 24,
	 "", "You are no longer protected from magic.", "",
	SCHOOL_ABJURATION,
	0
	 },

	 {
	"magic missile",	{  1, 93, 93, 93,93,93,1,93,1 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	"",
	SCHOOL_EVOCATION,
	0
	 },

	 {
	"mass healing",		{ 93, 4, 93, 93,4,93,93,93,4 },	{ 2,  2,  4,  4,2,4,4,4,2},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"mass invis",		{ 2, 93, 93, 93,93,93,93,93,2 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",		"",
	SCHOOL_ILLUSION,
	0
	 },

	
	{
	"mithril glaze",   { 2, 93, 93, 93,93,93,93,93,5 }, {2,4,4,4,4,4,4,4,3},
	spell_mithril_glaze,    TAR_OBJ_INV,            POS_STANDING,
	NULL,                   SLOT(304),      200,    24,
	"",                     "!Mithril Glaze!",      "",
	SCHOOL_ENCHANTMENT|SCHOOL_ALTERATION,
	0
	},
 

	 {
	  "nexus",                { 4, 3, 93, 93,3,93,93,93,4 }, { 2,2,4,4,2,4,4,4,2},
	  spell_nexus,            TAR_IGNORE,             POS_STANDING,
	  NULL,                   SLOT(520),       150,   36,
	  "",                     "!Nexus!",		"",
	 SCHOOL_CONJURATION|SCHOOL_ENCHANTMENT,
	 0
	 },

	 {
	"pass door",		{ 2, 3, 93, 93,3,93,6,93,2 },     { 1,  1,  2,  2,1,2,2,2,1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	"",
	SCHOOL_ALTERATION,
	0
	 },

	 {
	 "phantasmal force", {1,93,93,93,93,93,4,93,1}, {1,2,2,2,2,2,1,2,1},
	 spell_phantasmal_force,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	 NULL,	SLOT(209), 15, 12,
	 "phantasmal forces", "!Phantasmal Force!", "",
	SCHOOL_PHANTASM,
	0
	 },

	 {
	"plague",		{ 93, 2, 93, 93,3,4,93,93,93 },     { 1,  1,  2,  2,1,1,2,2,2},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"Your sores vanish.",	"",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	"poison",		{ 93,  2, 93, 93,2,5,6,93,93 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",
	"The poison on $p dries up.",
	SCHOOL_NECROMANCY,
	SPHERE_HEALING
	 },

	 {
	 "poison immunity", {93,93,93,93,2,93,4,93,93}, {2,2,2,2,1,2,1,2,2},
	 spell_poison_immunity, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT(233), 20, 12,
	 "", "You feel less vigerous.", "",
	 SCHOOL_ABJURATION,
	 SPHERE_PLANT
	 },

	 {
	"poison rain",	{ 93, 93, 93, 93, 5, 93, 93,93,93 },		{ 1, 2, 2, 2, 1, 2, 2,2,2 },
	spell_poison_rain,	TAR_IGNORE,			POS_STANDING,
	NULL,		SLOT(222),	90,	12,
	"poison rain",	"!Poison Rain!",		"",
	SCHOOL_CONJURATION,
	SPHERE_PLANT|SPHERE_WEATHER
	},

	 {
        "portal",               { 3, 3, 93, 93,3,93,93,93,3 },  {2,2,4,4,2,4,4,4,2},
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
        "",                     "!Portal!",		"",
	SCHOOL_CONJURATION|SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	 "prismatic spray", {7,93,93,93,93,93,93,93,7}, {2,4,4,4,4,4,4,4,2},
	 spell_prismatic_spray, TAR_IGNORE, POS_FIGHTING,
	 NULL, SLOT(221), 200, 24,
	 "colourful hues", "The color leaves your eyes.", "",
	 SCHOOL_ILLUSION|SCHOOL_EVOCATION,
	 0
	 },

	 {
	 "protection cold", {1,1,93,93,1,2,2,93,1}, {1,1,2,2,1,1,1,2,1},
	 spell_protection_cold,  TAR_CHAR_SELF, POS_STANDING,
	 NULL,	SLOT(207),  10, 12,
	 "",	"You are no longer protected from cold.", "",
	 SCHOOL_ABJURATION,
	 DISC_ELEM_WATER
	 },


	 {
	"protection evil",	{ 2,  1, 93, 93,1,2,4,93,2 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	"",
	SCHOOL_ABJURATION,
	0
	 },

	 {
	 "protection fire", {1,1,93,93,1,2,2,93,1}, {1,1,2,2,1,1,1,2,1},
	 spell_protection_fire,  TAR_CHAR_SELF, POS_STANDING,
	 NULL,	SLOT(206),  10, 12,
	 "",	"You are no longer protected from fire.", "",
	 SCHOOL_ABJURATION,
	 DISC_ELEM_FIRE
	 },

	 {
	  "protection good",      { 1,  9, 93, 93,1,2,4,93,1 }, {1,1,2,2,1,1,1,2,1},
	  spell_protection_good,  TAR_CHAR_SELF,          POS_STANDING,
	  NULL,                   SLOT(514),       5,     12,
	  "",                     "You feel less protected.",	"",
	  SCHOOL_ABJURATION,
	  0
	 },

	 {
	 "protection lightning", {2,2,93,93,2,3,3,93,2}, {1,1,2,2,1,1,1,2,1},
	 spell_protection_lightning,  TAR_CHAR_SELF, POS_STANDING,
	 NULL,	SLOT(208),  10, 12,
	 "",	"You are no longer protected from lightning.", "",
	 SCHOOL_ABJURATION,
	 0
	 },


	 {
	"rage",		{ 4, 3, 93, 93, 93, 6, 5,93,4 },		{ 1, 1, 2, 2, 2, 1, 1,2,1 },
	spell_rage,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(203),	70,	36,
	"",	 "You feel less like killing your fellow man.", "",
	SCHOOL_INVOCATION|SCHOOL_SUMMONING,
	0
	},

	 {
	"recharge",		{ 1, 93, 93, 93,93,93,93,93,1 },{ 1, 1, 2, 2,2,2,2,2,1 },
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		"",
	SCHOOL_INVOCATION|SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	"refresh",		{93,1,93,93,1,2,2,93,93 },      { 1,  1,  2,  2,1,1,1,2,1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!",		"",
	SCHOOL_ALTERATION,
	SPHERE_TIME
         },

	 {
	"regeneration",		{ 93, 2, 93, 93,1,6,6,93,93 },     { 1,  1,  2,  2,1,1,2,2,2},
	spell_regeneration,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(229),	30,	12,
	"regeneration",			"You feel less vibrant.",	"",
	 SCHOOL_ALTERATION,
	 SPHERE_TIME
	 },

	 {
	"rejuvinate",	{ 93, 7, 93, 93, 93, 93, 93,93,93 },	{ 2, 1, 2, 2, 2, 1, 2,2,2 },
	spell_rejuvinate,  TAR_CHAR_DEFENSIVE, 	POS_STANDING,
	NULL,		SLOT(201),	500,	24,
	"",		"!Rejuvinate!",		"",
	SCHOOL_NECROMANCY,	
	SPHERE_HEALING
	 },

	 {
	"remove curse",		{ 93, 2, 93, 93,2,3,93,93,93 },     {1,1,2,2,1,1,2,2,2},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	"",
	SCHOOL_ABJURATION,
	0
         },

	 {
	 "resist poison", {93,93,93,93,1,93,2,93,93}, {2,2,2,2,1,2,1,2,2},
	 spell_resist_poison, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT(232), 10, 12,
	 "", "You feel less vigerous.", "",
	 SCHOOL_ABJURATION,
	 SPHERE_PLANT
	 },

	 {
	"rune edge",	{ 4, 93, 93, 93,93,4,93,93,4 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_rune_edge,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(227),	200,	24,
	"",			"!Rune Edge!",	"$p becomes quite dull.",
	SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	"runic blade",	{ 7, 93, 93, 93,93,7,93,93,7 },     { 2,  2, 4,  4,4,4,4,4,2},
	spell_runic_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(228),	225,	24,
	"",			"!Runic Blade!",	"$p becomes quite dull.",
	SCHOOL_ENCHANTMENT,
	0
	 },

	 {
	"sanctuary",		{ 93, 3, 93, 93,3,8,93,93,93 },     { 1,  1,  2,  2,1,2,1,2,1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	"",
	SCHOOL_ABJURATION,
	SPHERE_PROTECTION
	 },

	 {
	"shield",		{ 2,93,93,93,4,93,93,93,2 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(67),	12,	18,
	"",			"Your force shield shimmers then fades away.",
	"",
	SCHOOL_CONJURATION,
	0
	 },

	 {
	"shocking grasp",	{  1, 93, 93, 93,93,93,2,93,1 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	"",
	SCHOOL_EVOCATION,
	0
	 },

	 {
	"sleep",		{ 1, 93, 93, 93,93,93,2,93,1 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	"",
	SCHOOL_ENCHANTMENT|SCHOOL_CHARM,
	0
	 },

	 {
	  "slow",             {2,3,93,93,3,7,6,93,2},     {1,1,2,2,2,2,2,2,1},
	  spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	  NULL,                   SLOT(515),      30,     12,
	  "",                     "You feel yourself speed up.",	"",
	  SCHOOL_ALTERATION,
	  0
	 },

	 {
	"stone skin",		{ 4, 93, 93,93,3,93,7,93,4 },     {1,1,2,2,2,2,2,2,1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again.",	"",
	SCHOOL_ALTERATION,
	DISC_ELEM_EARTH
	 },

	 {
	"summon",		{ 3,3,93,93,2,4,4,93,3 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!",		"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

    {
	"teleport",		{  1, 2, 93, 93,3,93,93,93,1 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

	 {
	 "true sight", {5,3,93,93,2,7,7,93,5}, {1,1,2,2,1,2,1,2,1},
	 spell_true_sight,  TAR_CHAR_DEFENSIVE, POS_STANDING,
	 NULL, SLOT(212), 50, 24,
	 "", "Your vision blurs once more.", "",
	 SCHOOL_DIVINATION
	 },

	 {
	 "vampiric touch", {3,93,93,93,93,93,93,93,3}, {2,4,4,4,4,4,4,4,2},
	 spell_vampiric_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	 NULL, SLOT(213), 40, 24,
	 "icy grasp", "!vampiric touch!", "",
	 SCHOOL_NECROMANCY,
	 0
	 },

	 {
	"ventriloquate",	{  1, 93, 93, 93,93,93,2,93,1 },     { 1,  1,  2,  2,2,2,1,2,1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	"",
	SCHOOL_PHANTASM,
	0
	 },

	 {
	"weaken",		{  1, 1, 93, 93,2,3,3,93,1 },     { 1,  1,  2,  2,1,2,2,2,1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	"",
	SCHOOL_NECROMANCY,
	0
	 },

	 {
	 "wizard eye", {2,93,93,93,93,1,5,93,2}, {1,2,2,2,2,2,1,2,1},
	 spell_wizard_eye, TAR_IGNORE, POS_RESTING,
	 NULL, SLOT(218), 40, 12,
	 "", "!Wizard Eye!", "",
	 SCHOOL_DIVINATION,
	 0
	 },

	 {
	"word of recall",	{ 3, 2, 93, 93,4,93,93,93,3 },     { 1,  1,  2,  2,2,2,2,2,1},
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	"",
	SCHOOL_CONJURATION|SCHOOL_SUMMONING,
	0
	 },

	{
	"wrath",                { 93, 3, 93, 93,4,5,6,93,93 },{1,1,1,1,1,1,1,1,1},
        spell_wrath,            TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,             SLOT(408),      100,     12,
        "wrath",                "The ill leaves your soul.",
        "$p is no longer disturbed.",
        SCHOOL_ABJURATION,
        0
	},

/*
 * Dragon breath
 */
	 {
	"acid breath",		{ 92, 92, 92, 92,92,92,92,93,93 },     { 1,  1,  2,  2,1,1,1,2,2},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	100,	24,
	"blast of acid",	"!Acid Breath!",	"",
	0,0
	 },

	 {
	"fire breath",		{ 92, 92, 92, 92,92,92,92,93,93 },     { 1,  1,  2,  2,1,1,1,2,1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	"",
	0,0
	 },

	 {
	"frost breath",		{ 92, 92, 92, 92,92,92,92,93,93 },     { 1,  1,  2,  2,1,1,1,2,2},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	125,	24,
	"blast of frost",	"!Frost Breath!",	"",
	0,0
	 },

	 {
	"gas breath",		{ 92, 92, 92, 92,92,92,92,93,93 },     { 1,  1,  2,  2,1,1,1,2,2},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	175,	24,
	"blast of gas",		"!Gas Breath!",		"",
	0,0
	 },

	 {
	"lightning breath",	{ 92, 92, 92, 92,92,92,92,93,93 },     { 1,  1,  2,  2,1,1,1,2,2},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	150,	24,
	"blast of lightning",	"!Lightning Breath!",	"",
	0,0
    },

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
	 {
		  "general purpose",      { 93, 93, 93, 93,93,93,93,93,93 },{ 0, 0, 0, 0,0,0,0,0,0 },
		  spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(401),      0,      12,
		  "general purpose ammo", "!General Purpose Ammo!",	"",0,0
	 },

	 {
		  "high explosive",       { 93, 93, 93, 93,93,93,93,93,93 },	{0,0,0,0,0,0,0,0,0},
		  spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(402),      0,      12,
		  "high explosive ammo",  "!High Explosive Ammo!",	"",0,0
	 },

/* criticals */
/*	 {
		  "cut arm",       { 101,101,101,101,101,101,101,101,101 },	{0,0,0,0,0,0,0,0},
		  crit_none,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(300),      0,      12,
		  "",  "Your arm feels better.",	"",0,0
	 },

	 {
		  "wounded calf",       { 101,101,101,101,101,101,101,101,101 },	{0,0,0,0,0,0,0,0},
		  crit_none,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(301),      0,      12,
		  "",  "Your calf feels better.",	"",0,0
	 },

	 {
		  "wounded thigh",       { 101,101,101,101,101,101,101,101,101 },	{0,0,0,0,0,0,0,0},
		  crit_none,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(302),      0,      12,
		  "",  "Your thigh feels better.",	"",0,0
	 },

	 {
		  "broken shieldarm",       { 101,101,101,101,101,101,101,101,101 },	{0,0,0,0,0,0,0,0},
		  crit_broken_shieldarm,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  NULL,                   SLOT(303),      0,      12,
		  "",  "Your shield arm is finnally healed.",	"",0,0
	 },

*/
/* combat and weapons skills */

        {
        "summoning", {1,1,1,1,1,1,1,1,1}, {5,3,0,0,2,3,2,0,5},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },

               {
        "healing", {1,1,1,1,1,1,1,1,1}, {0,2,0,0,2,2,2,0,0},
        spell_null, TAR_IGNORE, POS_STANDING,
        NULL, SLOT(0), 0, 0,
        "", "", "", 0, 0
        },

        {
        "sorcery", {5,93,93,93,93,93,93,93,93 }, {10,0,0,0,0,0,0,0,0},
        spell_null, TAR_IGNORE, POS_DEAD,
        &gsn_sorcery, SLOT(0), 0, 0,
        "", "!Sorcery!", "", 0, 0
        },

        {
        "mana focusing", {12,8,93,93,7,16,14,93,14 }, {7,6,0,0,4,6,6,0,8},
        spell_null, TAR_IGNORE, POS_DEAD,
        &gsn_mana_focusing, SLOT(0), 0, 0,
        "", "!Mana Focusing!", "", 0, 0
        },
  

	 {
	"axe",			{ 93, 93, 93, 8,93,93,12,1,93 },	{ 0, 0, 0, 4,0,0,5,1,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_axe,            	SLOT( 0),       0,      0,
		  "",                     "!Axe!",		"",0,0
	 },

	 {
		  "dagger",               { 1, 93,1,1,1,1,1,1,1 }, { 2, 0, 2, 2,2,2,2,1,2},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_dagger,            SLOT( 0),       0,      0,
		  "",                     "!Dagger!",		"",0,0
	 },

	 {
	"flail",		{ 93,1,93,1,1,1,1,1,93},	{ 0, 3, 0, 4,3,3,4,3,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_flail,            	SLOT( 0),       0,      0,
		  "",                     "!Flail!",		"",0,0
	 },

	 {
	"mace",			{ 93,1,1,1,1,1,1,1,1 },	{ 0, 2, 3, 3,2,2,3,2,3},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_mace,            	SLOT( 0),       0,      0,
		  "",                     "!Mace!",		"",0,0
	 },

	 {
	"polearm",		{ 93, 93, 93, 1,93,93,1,1,93 },	{ 0, 0, 0, 4,0,0,5,3,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_polearm,           SLOT( 0),       0,      0,
		  "",                     "!Polearm!",		"",0,0
	 },

	 {
	"shield block",		{ 93,30,93,1,93,5,10,8,93 },	{ 0, 4, 0, 2,0,3,3,4,0},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		"",0,0
	 },

	 {
	"spear",		{ 93, 93, 93,1,93,93,5,1,93},	{ 0, 0, 0, 3,0,0,3,2,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_spear,            	SLOT( 0),       0,      0,
        "",                     "!Spear!",		"",0,0
    },

	 {
	"sword",		{ 93, 93,  10,  1,93,1,1,5,12},	{ 0, 0, 5, 2,0,3,3,5,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_sword,            	SLOT( 0),       0,      0,
		  "",                     "!sword!",		"",0,0
	 },

    {
	"whip",			{ 1, 1,  1,  1,1,1,1,1,1},	{ 6, 5, 5, 4,5,4,4,4,5},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_whip,            	SLOT( 0),       0,      0,
        "",                     "!Whip!",	"",0,0
    },

    {
		  "backstab",             { 93, 93,  1, 93,93,93,93,93,1 },  {0,0,5,0,0,0,0,0,5},
		  spell_null,             TAR_IGNORE,             POS_STANDING,
		  &gsn_backstab,          SLOT( 0),        0,     24,
		  "backstab",             "!Backstab!",		"",0,0
	 },

	 {
	"bash",			{ 93, 93, 93,1,93,14,12,1,93},	{ 0, 0, 0, 4,0,4,4,2,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_bash,            	SLOT( 0),       0,      24,
		  "bash",                 "!Bash!",		"",0,0
	 },

	 {
	"berserk",		{ 93, 93, 93, 18,93,93,93,8,93 },	{ 0, 0, 0, 5,0,0,0,4,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_berserk,        	SLOT( 0),       0,      24,
		  "",                     "You feel your pulse slow down.",	"",0,0
	 },

	 {
	"dirt kicking",		{ 93, 93,3,3,93,93,5,5,3 },	{ 0, 0, 4, 4,0,0,4,3,4},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	24,
	"kicked dirt",		"You rub the dirt out of your eyes.",	"",0,0
    },

	 {
		  "disarm",               { 93, 93, 12, 11,93,12,14,20,12},{0,0,6,4,0,4,5,5,6},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_disarm,            SLOT( 0),        0,     24,
		  "",                     "!Disarm!",		"",0,0
    },
 
	 {
		  "dodge",           {20,22,1,13,20,5,6,12,1 },     { 8, 8, 4, 6,7,5,4,7,4},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_dodge,             SLOT( 0),        0,     0,
		  "",                     "!Dodge!",		"",0,0
	 },

	 {
		  "enhanced damage",      { 93, 50, 25,1,55,20,20,1,25 },   {0,9,5,3,10,4,4,2,5},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_enhanced_damage,   SLOT( 0),        0,     0,
		  "",                     "!Enhanced Damage!",	"",0,0
	 },

	 {
		"ultra damage",      { 93, 93, 93,40,93,93,93,30,93 },   {0,0,0,12,0,0,0,8,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_ultra_damage,   SLOT( 0),        0,     0,
        "",                     "!Ultra Damage!",    "",0,0
    },


	 {
	"envenom",		{ 93, 93, 10, 93,93,93,93,93,10 },	{ 0, 0, 4, 0,0,0,0,0,4 },
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	36,
	"",			"!Envenom!",		"",0,0
    },

	 {
	"hand to hand",		{ 25,10,15,6,12,8,9,4,15 },	{ 8, 5, 6, 4,5,4,4,3,6},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	"",0,0
    },

	 {
		  "kick",           {93,12,14,8,14,8,8,6,14 },     { 0, 4, 6, 3,4,3,3,3,6},
		  spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
		  &gsn_kick,              SLOT( 0),        0,     12,
		  "kick",                 "!Kick!",		"",0,0
	 },

	 {
		  "parry",          {42,22,13,1,27,4,3,6,13 },     { 8, 8, 6, 4,8,5,5,5,6},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
        "",                     "!Parry!",		"",0,0
    },

    {
		  "rescue",               { 93, 93, 93,1,93,5,5,20,93 },  { 0, 0, 0, 4,0,4,4,5,0},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_rescue,            SLOT( 0),        0,     12,
		  "",                     "!Rescue!",		"",0,0
    },

    {
	"trip",			{ 93, 93,1,15,93,93,93,10,1 },	{ 0, 0, 4, 8,0,0,0,5,4},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	24,
	"trip",			"!Trip!",		"",0,0
	 },

	 {
		  "second attack",        { 40, 24, 12,5,26,12,8,6,12 },{ 10, 8, 5, 3,8,4,4,5,5},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_second_attack,     SLOT( 0),        0,     0,
		  "",                     "!Second Attack!",	"",0,0
	 },

	 {
		  "third attack",         {93,93,24,12,93,24,16,20,24 },{ 0, 0, 10, 4,0,5,5,5,10},
		  spell_null,             TAR_IGNORE,             POS_FIGHTING,
		  &gsn_third_attack,      SLOT( 0),        0,     0,
		  "",                     "!Third Attack!",	"",0,0
	 },

	{
		  "quad attack",	 {93,93,50,30,93,93,93,93,50}, {0,0,15,10,0,0,0,0,15},
						spell_null,		TAR_IGNORE,   POS_FIGHTING,
		  &gsn_quad_attack,     SLOT( 0), 0, 0,
		  "", 			"!Quad Attack!", "",0,0
	},

/* non-combat skills */


	 {
	"awareness",		{20,20,5,10,20,20,1,10,5 },	{ 3,3,2,2,3,2,1,2,2},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_awareness,	SLOT( 0),	0,	0,
	"",			"!Awareness!",	"",0,0
	 },


	 {
	"channel", {10,5,92,92,10,25,25,93,10}, {4,4,0,0,4,6,6,0,4},
	spell_null, TAR_IGNORE, POS_STANDING,
	&gsn_channel, SLOT(0), 0, 0,
	"", "!Channel!", "",0,0
	},


	 {
	"fast healing",		{45,9,16,6,10,9,9,1,16 },	{ 8, 5, 6, 4,6,5,5,3,6},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	"",0,0
	 },

	 {
	"haggle",		{7,18,1,14,93,93,21,50,1 },	{ 5, 8, 3, 6,0,0,7,9,3},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		"",0,0
	 },

	 {
	"hide",			{ 93, 93,1,12,93,93,8,15,1 },	{ 0, 0, 4, 6,0,0,4,6,4},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	12,
	"",			"!Hide!",		"",0,0
	 },

	 {
	"lore",			{ 10, 10,  6, 20,1,12,8,20,6 },	{ 6, 6, 4, 8,1,6,5,10,4},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		"",0,0
	 },

	 {
	"meditation",		{  6,  6, 93, 93,5,12,12,93,6 },	{ 5, 5, 0, 0,5,8,8,0,5},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		"",0,0
    },

    {
	"peek",			{  8, 21,  1, 14,40,20,20,25,1 },	{ 5, 7, 3, 6,7,6,6,6,3},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		"",0,0
	 },

	 {
	"pick lock",		{ 93, 93,  7, 93,93,93,93,93,7},	{ 0, 0, 4, 0,0,0,0,0,4},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		"",0,0
	 },

	 {
	"scan",			{ 1, 1, 1, 1, 1, 1, 1,1,1},       { 3,2,2,2,2,2,1,2,2 },
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_scan,		SLOT(0),	0,	2,
	"",		"!Scan!",		"",0,0
	 },

	 {
	"sneak",		{ 93, 93,  4,  20,93,93,15,93,4 },	{ 0, 0, 4, 6,0,0,4,0,4},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	"",0,0
	 },

	 {
	"steal",		{ 93, 93,  5, 93,93,93,93,93,5 },	{ 0, 0, 4, 0,0,0,0,0,4},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	24,
	"",			"!Steal!",		"",0,0
	 },

	 {
	"scrolls",		{  1,  1,  1,  1,1,1,1,93,1 },	{ 2, 3, 5, 8,3,4,4,0,2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	24,
	"",			"!Scrolls!",		"",0,0
    },

	 {
	"staves",		{  1,  1,  1,  1,1,1,1,93,1 },	{ 2, 3, 5, 8,3,4,4,0,2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		"",0,0
	 },

	 {
	"wands",		{  1,  1,  1,  1,1,1,1,93,1 },	{ 2, 3, 5, 8,3,4,4,0,2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		"",0,0
    },

	 {
	"recall",{  1,  1,  1,  1,1,1,1,1,1 },	{ 2, 2, 2, 2,2,2,2,2,2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	12,
	"",			"!Recall!",		"",0,0
	 },

         {
        "human",{  1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_human,            SLOT( 0),       0,      0,
        "",                     "!Human!",             "",0,0
         },
         {
        "elven",{  1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_elven,            SLOT( 0),       0,      0,
        "",                     "!Elven!",             "",0,0
         },
         {
        "dwarven",{1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_dwarven,            SLOT( 0),       0,      0,
        "",                     "!Dwarven!",             "",0,0
         },
         {
        "gnomish",{1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_gnomish,            SLOT( 0),       0,      0,
        "",                     "!Gnomish!",             "",0,0
         },
         {
        "halfling",{1,1,1,1,1,1,1,1,1},  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_halfling,            SLOT( 0),       0,      0,
        "",                     "!Halfling!",             "",0,0
         },
         {
        "orcish",{  1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_orcish,            SLOT( 0),       0,      0,
        "",                     "!Orcish!",             "",0,0
         },
         {
        "drow",{1,1,1,1,1,1,1,1,1 },  {3,3,3,3,3,3,3,3,3},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_drow,            SLOT( 0),       0,      0,
        "",                     "!Drow!",             "",0,0
         },
         {
        "avian",{1,1,1,1,1,1,1,1,1 },  {3,3,3,3,3,3,3,3,3},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_avian,            SLOT( 0),       0,      0,
        "",                     "!Avian!",             "",0,0
         },
         {
        "gnoll",{  1,1,1,1,1,1,1,1,1 },  { 2, 2, 2, 2,2,2,2,2,2},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_gnoll,            SLOT( 0),       0,      0,
        "",                     "!Gnoll!",             "",0,0
         }

};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

	 {
	"rom basics",		{ 0, 0, 0, 0,0,0,0,0,0 },
	{ "scrolls", "staves", "wands", "recall" }
	 },

	 {
	"mage basics",		{ 0, -1, -1, -1,-1,-1,-1,-1,-1 },
	{ "dagger" }
	 },

	 {
	"cleric basics",	{ -1, 0, -1, -1,0,-1,-1,-1,-1 },
	{ "mace" }
	 },

	 {
	"thief basics",		{ -1, -1, 0, -1,-1,-1,-1,-1,-1 },
	{ "dagger", "steal" }
	 },

	 {
	"warrior basics",	{ -1, -1, -1, 0,-1,0,0,-1,-1 },
	{ "sword" }
	 },

	 {
	 "barbarian basics", {-1,-1,-1,-1,-1,-1,-1,0,-1},
	 {"berserk", "bash", "axe"}
	 },
  
         {
         "spellfilcher basics", {-1,-1,-1,-1,-1,-1,-1,-1,0},
         {"dagger", "lore", "steal"}
         },

	 {
	"mage default",		{ 40, -1, -1, -1,-1,-1,-1,-1,-1 },
	{ "alteration", "evocation", "dodge", "illusion", "necromancy",
          "scan", "phantasm", "fire"  }
	  
    },

	 {
	"cleric default",	{ -1, 40, -1, -1,-1,-1,-1,-1,-1 },
	{ "abjuration", "summoning", "healing", "combat", "scan",
          "alteration", "divination", "protection", "fast healing",
          "conjuration", "necromancy", "plant", "meditation" } 
	  
	   
	 },

	 {
	"thief default",	{ -1, -1, 40, -1,-1,-1,-1,-1,-1 },
	{ "mace", "sword", "backstab", "disarm", "dodge", "second attack",
	  "trip", "hide", "peek", "pick lock", "sneak" }
	 },

	 {
	"warrior default",	{ -1, -1, -1, 40,-1,-1,-1,-1,-1 },
	{ "weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack" }
	 },

	 {
	"ranger default",	{ -1, -1, -1, -1,-1,-1,40,-1,-1 },
	{ "abjuration", "healing", "combat", "third attack", "divination",
          "scan", "evocation", "spear", "awareness", "sneak"
	   }
	 },

	 {
	"druid default",	{ -1, -1, -1, -1,40,-1,-1,-1,-1 },
	{ "abjuration", "summoning", "earth", "water", "animal", "alteration",
          "charm", "air", "weather", "dagger", "conjuration", "divination",
          "fire", "plant", "hand to hand"
	   }
	 },

	 {
	"paladin default",	{ -1, -1, -1, -1,-1,40,-1,-1,-1 },
	{ "abjuration", "healing", "combat", "third attack", "divination",
          "protection", "shield block", "fast healing", "necromancy",
          "law", "second attack", "scan" 
	  }
	 },

	 {
	 "barbarian default", {-1,-1,-1,-1,-1,-1,-1,40,-1},
	 { "weaponsmaster", "enhanced damage", "dodge", "fast healing",
		"ultra damage", "second attack", "scan", "hand to hand" }
	 },

         {
         "spellfilcher default", {-1,-1,-1,-1,-1,-1,-1,-1,40},
         {
            "abjuration", "alteration", "illusion", "phantasm", "evocation",
            "enchantment", "backstab", "second attack"
         }
         },

	 {
	"weaponsmaster",	{ -1, -1, -1, 20,-1,20,20,10,-1 },
	{ "axe", "dagger", "flail", "mace", "polearm", "spear", "sword","whip" }
	 },

};
