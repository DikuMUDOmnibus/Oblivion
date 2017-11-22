/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
/* this section of code is purely oblivion coding */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "color.h"
#include "recycle.h"

DECLARE_DO_FUN(do_look);

/* by Airius WWW */
void send_hue_mess(char *clmess, char *clcode, CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MAX_STRING_LENGTH];
        CHAR_DATA	*vch;
	CHAR_DATA   *vch_next;

	if(IS_SET(ch->act, PLR_COLOR))
		sprintf(buf, "%sA %s hue strikes you!%s\n\r", clcode, clmess, FG_LT_GRAY);
	else
		sprintf(buf,"A %s hue strikes you!\n\r", clmess);
	send_to_char(buf, victim);

	for ( vch = char_list; vch!=NULL; vch = vch_next )
	{
	vch_next=vch->next;
	if (vch->in_room == NULL)
		continue ;
	if ( vch->in_room == ch->in_room )
	{
		if ( vch != victim )
		{
			if(IS_SET(vch->act, PLR_COLOR))
			  sprintf(buf, "%sA %s hue strikes %s!%s\n\r", clcode, clmess, victim->short_descr, FG_LT_GRAY);
			else
			  sprintf(buf, "A %s hue strikes %s!\n\r", clmess, victim->short_descr);
			send_to_char(buf, vch);
		}
		continue ;
	}
	}

}

/* by Airius WWW */
void strike_with_hue( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim)
{
	int dtoss;
	ROOM_INDEX_DATA *pRoomIndex;
	AFFECT_DATA af;

	dtoss=dice(1,7);
	switch (dtoss)
	{
	case 1 :
	  send_hue_mess("red", FG_RED, ch, victim);
	  damage_old(ch, victim, saves_spell( level, victim, DAM_LIGHT) ? 50 : 25, sn, DAM_LIGHT, TRUE);
	 break;
	case 2 :
	  send_hue_mess("orange", FG_LT_RED, ch, victim);
	  damage_old(ch, victim, saves_spell( level, victim, DAM_ENERGY) ? 100 : 50, sn, DAM_ENERGY, TRUE);
	 break;
	case 3 :
	  send_hue_mess("yellow", FG_YELLOW, ch, victim);
	  damage_old(ch, victim, saves_spell( level, victim, DAM_FIRE) ? 150 : 75, sn, DAM_FIRE, TRUE);
	 break;
	case 4 :
	  send_hue_mess("green", FG_GREEN, ch, victim);
	 if(!saves_spell(level, victim, DAM_POISON))
	 {
	 af.where     = TO_AFFECTS;
	 af.type      = sn;
	 af.level     = level;
	 af.duration  = level;
	 af.location  = APPLY_ST;
	 af.modifier  = -4;
	 af.bitvector = AFF_POISON;
	 affect_join( victim, &af );
	 send_to_char( "You feel very sick.\n\r", victim );
	 act("$n looks very ill.",victim,NULL,NULL,TO_ROOM);
	 }
	 else
	  damage_old(ch, victim, 20, sn, DAM_POISON, TRUE);
	 break;
	case 5 :
	  send_hue_mess("blue", FG_BLUE, ch, victim);
	 if ( victim->in_room == NULL
	 || IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
	 || ( victim != ch && IS_SET(victim->imm_flags,IMM_SUMMON))
	 || ( !IS_NPC(ch) && victim->fighting != NULL )
	 || ( victim != ch
	 && ( saves_spell( level - 5, victim,DAM_OTHER))))
	 {
		break;
	 }

	 pRoomIndex = get_random_room(victim);

	 if (victim != ch)
	send_to_char("You have been teleported!\n\r",victim);

	 act( "$n vanishes!", victim, NULL, NULL, TO_ROOM );
	 char_from_room( victim );
	 char_to_room( victim, pRoomIndex );
	 act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM );
	 do_look( victim, "auto" );
	 break;
	case 6 :
	  send_hue_mess("indigo", FG_LT_PURPLE, ch, victim);
	  if (saves_spell(level,victim,DAM_OTHER)
	 ||  IS_SET(victim->imm_flags,IMM_MAGIC))
	 {
	if (victim != ch)
				send_to_char("Nothing seemed to happen.\n\r",ch);
		  send_to_char("You feel momentarily lethargic.\n\r",victim);
		  break;
	 }

	 if (IS_AFFECTED(victim,AFF_HASTE))
	 {
		  if (!check_dispel(level,victim,skill_lookup("haste")))
		  {
		 if (victim != ch)
					send_to_char("Spell failed.\n\r",ch);
	 affect_to_char( victim, &af );
	 send_to_char( "You feel yourself slowing d o w n...\n\r", victim );
	 act("$n starts to move in slow motion.",victim,NULL,NULL,TO_ROOM);
			  send_to_char("You feel momentarily slower.\n\r",victim);
				break;
		  }

		  act("$n is moving less quickly.",victim,NULL,NULL,TO_ROOM);
		  break;
	 }


	 af.where     = TO_AFFECTS;
	 af.type      = sn;
	 af.level     = level;
	 af.duration  = level/2;
	 af.location  = APPLY_QU;
	 af.modifier  = -1 - (level/5);
	 af.bitvector = AFF_SLOW;
	 affect_join( victim, &af );
	 send_to_char( "You feel yourself slowing d o w n...\n\r", victim );
	 act("$n starts to move in slow motion.",victim,NULL,NULL,TO_ROOM);
	 break;
	case 7 :
	  send_hue_mess("violet", FG_PURPLE, ch, victim);
	  damage_old(ch, victim, saves_spell( level, victim, DAM_NEGATIVE) ? 200 : 150, sn, DAM_NEGATIVE, TRUE);
	 break;
	}
	return ;
}

/* By Wynn */
void spell_animal_essence( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA 	*victim = (CHAR_DATA *) vo;
	AFFECT_DATA	af;

	if ( !IS_AFFECTED( victim, AFF_FLYING ) )
		{
		af.where		 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = AFF_FLYING;
		affect_to_char( victim, &af );
		send_to_char( "Your feet rise of the ground.\n\r", victim );
		act( "$n's feet rise off the ground.", ch, NULL, victim, TO_NOTVICT );
		}

	if ( !IS_AFFECTED( victim, AFF_DETECT_HIDDEN ) )
		{
		af.where 	 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_DETECT_HIDDEN;
		affect_to_char( victim, &af );
		send_to_char( "Your awarness improves.\n\r", victim );
		}

	if ( !IS_AFFECTED( victim, AFF_DETECT_INVIS ) )
		{
		af.where		 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_DETECT_INVIS;
		affect_to_char( victim, &af );
		send_to_char( "Your eyes tingle with detect invisability.\n\r", victim );
		}
	return ;
}

/*by Airius WWW */
void spell_barkskin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;

	 if ( is_affected( victim, sn ) )
	 {
	if (victim == ch)
	  send_to_char("Your skin is already as bark.\n\r",ch);
	else
	  act("$N already has bark for skin.",ch,NULL,victim,TO_CHAR);
	return;
	 }

	 af.where     = TO_AFFECTS;
	 af.type      = sn;
	 af.level     = level;
	 af.duration  = level;
	 af.location  = APPLY_AC;
	 af.modifier  = -40;
	 af.bitvector = 0;
	 affect_to_char( victim, &af );
	 act( "$n's skin turns to bark.", victim, NULL, NULL, TO_ROOM );
	 send_to_char( "Your skin turns to bark.\n\r", victim );
	 af.where	  = TO_VULN;
	 af.location  = APPLY_NONE;
	 af.modifier  = 0;
	 af.bitvector = VULN_FIRE;
	 affect_to_char( victim, &af );

	 return;
}

/* airius WWW */
void spell_chill_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;

	 if ( is_affected( victim, sn ))
	 {
	if (victim == ch)
	  send_to_char("You already have a chill shield.\n\r",ch);
	else
	  act("$N already has a shield of icy magic.",
			ch,NULL,victim,TO_CHAR);
		  return;
	 }

	 if (is_affected(victim,skill_lookup("fire shield")))
	 {
	if (!check_dispel(level,victim,skill_lookup("fire shield")))
	{
		 if (victim != ch)
			  send_to_char("Spell failed.\n\r",ch);
		 send_to_char("Your fire shield is destroyed.\n\r",victim);
		 return;
	}
		  act("$n's fire shield is snuffed out.",victim,NULL,NULL,TO_ROOM);
		  return;
	 }

	 af.where     = TO_AFFECTS;
	 af.type      = gsn_chill_shield;
	 af.level     = level;
	 if (victim == ch)
		af.duration  = level/5;
	 else
		af.duration  = level/10;
	 af.location  = APPLY_NONE;
	 af.modifier  = 0;
	 af.bitvector = 0;
	 affect_to_char( victim, &af );
	 af.where	  = TO_RESIST;
	 af.bitvector = RES_FIRE;
	 affect_to_char( victim, &af );
	 af.where	  = TO_VULN;
	 af.bitvector = VULN_COLD;
	 send_to_char( "You are surrounded by an icy aura.\n\r", victim );
	 act("$n is surrounded by an icy aura.",victim,NULL,NULL,TO_ROOM);
	 if ( ch != victim )
		  send_to_char( "Ok.\n\r", ch );
	 return;
}


/*by Airius WWW */
void spell_cone_cold( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA	*vch;
	CHAR_DATA       *vch_next;
	int			 dam;
	AFFECT_DATA af;

	dam = dice( level , 3 )+50;

	af.where     = TO_AFFECTS;
	af.type		 = sn;
	af.level	    = level;
	af.duration  = level;
	af.location  = APPLY_ST;
	af.modifier  = -level/3;
	af.bitvector = 0;

	send_to_char( "You send forth a cone of cold!\n\r", ch );
	act( "$n places out his hands and blasts forth a cone of cold!",
	ch, NULL, NULL, TO_ROOM );

	for ( vch = char_list; vch!=NULL; vch = vch_next )
	{
	vch_next=vch->next;
	if (vch->in_room == NULL)
		continue ;
	if ( vch->in_room == ch->in_room )
	{
		if ( vch != ch
		&& ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
		{
		damage_old( ch, vch, saves_spell( level, vch, DAM_COLD ) ? dam /2 : dam, sn, DAM_COLD, TRUE );
		if(!saves_spell(level,vch, DAM_COLD))
		{
		  affect_join( vch, &af );
		  send_to_char("The cold seeps into your bones.", vch);
		}
		}
		continue ;
	}
	}

	return ;
}

/*Airius WWW*/
void spell_create_buffet( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	 OBJ_DATA *mushroom;
	 int counter=0;
	 for(counter=0; counter<level/5+1; counter++)
	 {
	 mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
	 mushroom->value[0] = level / 2;
	 mushroom->value[1] = level;
	 obj_to_room( mushroom, ch->in_room );
	 act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
	 act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
	 }
	 return;
}



/*Airius WWW*/
void spell_drain_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

   
	if(IS_GOOD(ch))
	{
		send_to_char("You are far too nice to use such majiks.\n\r",ch);
		return ;
	}

	if(!IS_EVIL(ch))
	{
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	}
	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{


		  if(IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC))
		  {
			send_to_char("That weapon is already quite evil.\n\r", ch);
			return ;
		  }
		  if(IS_OBJ_STAT(obj,ITEM_BLESS))
		  {
			send_to_char("That weapon is too holy to be touched by your majiks.\n\r", ch);
			return ;
		  }
		  if(!IS_OBJ_STAT(obj,ITEM_EVIL))
			SET_BIT(obj->extra_flags, ITEM_EVIL);
		  if(!IS_OBJ_STAT(obj,ITEM_ANTI_GOOD))
			SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
		  if(!IS_OBJ_STAT(obj,ITEM_ANTI_NEUTRAL))
			SET_BIT(obj->extra_flags, ITEM_ANTI_NEUTRAL);

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level/2;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_VAMPIRIC;
		  affect_to_obj(obj, &af);

		  act("$p becomes dark and evil.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}


/*Airius WWW*/
void spell_empower_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
		  if(IS_WEAPON_STAT(obj,WEAPON_SHOCKING))
		  {
			send_to_char("That weapon is already imbued with power.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_SHOCKING;
		  affect_to_obj(obj, &af);

		  act("$p sparks with electricity.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}


/*Airius WWW*/
void spell_flame_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
		  if(IS_WEAPON_STAT(obj,WEAPON_FLAMING))
		  {
			send_to_char("That weapon is already flaming.\n\r", ch);
			return ;
		  }
		  if(IS_WEAPON_STAT(obj,WEAPON_FROST))
		  {
			send_to_char("That weapon is too cold to accept the magic.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_FLAMING;
		  affect_to_obj(obj, &af);

		  act("$p gets a fiery aura.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}

/*Airius WWW*/
void spell_fire_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;

	 if ( is_affected( victim, sn ))
	 {
	if (victim == ch)
	  send_to_char("You already have a fire shield.\n\r",ch);
	else
	  act("$N already has a shield of fire.",
			ch,NULL,victim,TO_CHAR);
		  return;
	 }

	 if (is_affected(victim,skill_lookup("chill shield")))
	 {
	if (!check_dispel(level,victim,skill_lookup("chill shield")))
	{
		 if (victim != ch)
			  send_to_char("Spell failed.\n\r",ch);
		 send_to_char("Your chill shield is destroyed.\n\r",victim);
		 return;
	}
		  send_to_char("You failed!", ch);
		  return;
	 }

	 af.where     = TO_AFFECTS;
	 af.type      = sn;
	 af.level     = level;
	 if (victim == ch)
		af.duration  = level/5;
	 else
		af.duration  = level/10;
	 af.location  = APPLY_NONE;
	 af.modifier  = 0;
	 af.bitvector = 0;
	 affect_to_char( victim, &af );
	 af.where	  = TO_RESIST;
	 af.bitvector = RES_COLD;
	 affect_to_char( victim, &af );
	 af.where	  = TO_VULN;
	 af.bitvector = VULN_FIRE;
	 send_to_char( "You are surrounded by a fire shield.\n\r", victim );
	 act("$n is surrounded by a fire shield.",victim,NULL,NULL,TO_ROOM);
	 if ( ch != victim )
		  send_to_char( "Ok.\n\r", ch );
	 return;
}

/*Airius WWW*/
void spell_frost_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
		  if(IS_WEAPON_STAT(obj,WEAPON_FROST))
		  {
			send_to_char("That weapon is already wickedly cold.\n\r", ch);
			return ;
		  }
		  if(IS_WEAPON_STAT(obj,WEAPON_FROST))
		  {
			send_to_char("That weapon is too warm to accept the magic.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_FROST;
		  affect_to_obj(obj, &af);

		  act("$p grows wickedly cold.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}


/* by Airius WWW */
void spell_holy_aura( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!IS_GOOD(victim))
	{
		if(ch == victim)
			send_to_char("You are not holy enough to cast this spell.\n\r", ch);
		else
			send_to_char("They are too unrighteous!\n\r", ch);
		return ;
	}
	if(!is_affected(victim,sn))
	{
		af.where      = TO_AFFECTS;
		af.type		  = sn;
		af.level		  = level;
		af.duration	  = level/3;
		af.location	  = APPLY_AC;
		af.bitvector  = 0;
		af.modifier	  = -level;
		affect_to_char( victim, &af);
		af.where		  = TO_IMMUNE;
		af.modifier	  = 0;
		af.location	  = APPLY_NONE;
		af.bitvector	  = IMM_NEGATIVE;
		affect_to_char( victim, &af);
		send_to_char("You are surrounded by a gold aura.\n\r",victim);
		act("$n is surrounded with a gold aura.", ch, NULL, victim, TO_NOTVICT);
	}
	else
	{
		if(ch == victim)
			send_to_char("You are already protected by divine magic.\n\r", ch);
		else
			send_to_char("They are already protected.\n\r", ch);
		return ;
	}

	return ;
}

void spell_ice_storm( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA	*vch;
	CHAR_DATA       *vch_next;
	int			 dam;
	AFFECT_DATA af;

	dam = 150;

	 af.where     = TO_AFFECTS;
	 af.type      = sn;
	 af.level     = level;
	 af.duration  = level/10;
	 af.location  = APPLY_QU;
	 af.modifier  = -1 - (level/5);
	 af.bitvector = AFF_SLOW;

	send_to_char( "You conjure up an ice storm!\n\r", ch );
	act( "$n raises $s hands and an ice storm appears from nowhere!",
	ch, NULL, NULL, TO_ROOM );

	for ( vch = char_list; vch!=NULL; vch = vch_next )
	{
	vch_next=vch->next;
	if (vch->in_room == NULL)
		continue ;
	if ( vch->in_room == ch->in_room )
	{
		if ( vch != ch
		&& ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
		{
		damage_old( ch, vch, saves_spell( level, vch, DAM_COLD ) ? dam /2 : dam, sn, DAM_COLD, TRUE );
		if(!saves_spell(level,vch, DAM_COLD))
		{
		  affect_join( vch, &af );
		  send_to_char( "You get covered in ice and have a hard time moving.\n\r", vch );
		  act("$n is covered in ice.",vch,NULL,NULL,TO_ROOM);
		}
		}
		continue ;
	}
	}

	return ;
}


/* by Airius  WWW  */
void spell_illusions_granduer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;

	 if(!is_affected(victim,sn))
	 {
	 af.where       = TO_AFFECTS;
	 af.type			 = sn;
	 af.level		 = level;
	 af.duration	 = level/10;
	 af.location	 = APPLY_HIT;
	 af.modifier    = level;
	 af.bitvector	= 0;
	 affect_to_char( victim, &af );
         af.modifier    = level*3;
	 af.location    = APPLY_MANA;
	 affect_to_char( victim, &af );
	 send_to_char( "You appear mightier then you are!\n\r", victim );
	 act( "$n grows in stature and appears quite mighty now!\n\r", victim, NULL, NULL, TO_ROOM );
	 }
	 else
		send_to_char("You are having delusions of granduer now.\n\r", victim );
	 return ;
}

/* by Airius */
void spell_improved_phantasm( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	dam = dice(level, 9);
	if(saves_spell( level, victim, DAM_ILLUSION))
		dam = dam/3;
	damage_old( ch, victim, dam, sn, DAM_ILLUSION, TRUE);
}


void spell_induce_sleep( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;

	 if((ch==victim)&&(!IS_NPC(ch)))
	 {
		send_to_char("You put yourself to sleep.\n\r", ch);
		ch->pcdata->is_trying_sleep=TRUE;
		ch->position=POS_SLEEPING;
		act( "$n goes quietly to sleep.", ch, NULL, NULL, TO_ROOM );
	 }

	 if(IS_NPC(victim))
	 {
		send_to_char("You cannot put to sleep unwilling creatures.\n\r",ch);
		return ;
	 }

	 if(victim->pcdata->is_trying_sleep)
	 {
		send_to_char("You drift off into dreamland.\n\r",victim);
		victim->position=POS_SLEEPING;
		act( "$n goes quietly to sleep.", victim, NULL, NULL, TO_ROOM );
		return ;
	 }

	 send_to_char("You cannot put to sleep unwilling players.\n\r",ch);

	 return;
}




/* by Airius */
void spell_magic_resistance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/10+1;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_MAGIC;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from magic.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from magic.\n\r", victim);

	return ;
}


/* by Airius */
void spell_phantasmal_force( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	dam = dice(level, 4);
	if(saves_spell( level, victim, DAM_ILLUSION))
		dam = 0;
	damage_old( ch, victim, dam, sn, DAM_ILLUSION, TRUE);
}

/* by Airius WWW */
void spell_prismatic_spray( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA	*vch;
	CHAR_DATA   *vch_next;
	int number_hits;
	int i;

	send_to_char( "You put out your hands and send forth a dazzling pristmatic spray!\n\r", ch );
	act( "$n raises $s hands and sends out a dazzling prismatic spray!",
	ch, NULL, NULL, TO_ROOM );

	for ( vch = char_list; vch!=NULL; vch = vch_next )
	{
	vch_next=vch->next;
	if (vch->in_room == NULL)
		continue ;
	if ( vch->in_room == ch->in_room )
	{
		if ( vch != ch && ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ) )
		{
			if(dice(1,8)==8)
				number_hits=2;	else number_hits=1;
			if(dice(1,8)==8)
				number_hits+=2; else number_hits+=1;
			for(i=1; i<=number_hits; i+=1)
			{
			strike_with_hue(sn,level,ch,vch);
			}
		}
		continue ;
	}
	}
	return ;
}

/* by Airius */
void spell_protection_fire( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_FIRE;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from flames.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from fire.\n\r", victim);

	return ;
}

/* by Airius */
void spell_protection_cold( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_COLD;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from cold.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from cold.\n\r", victim);

	return ;
}

/* by Airius */
void spell_protection_lightning( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_LIGHTNING;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from lightning.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from lightning.\n\r", victim);

	return ;
}

/* by Airius */
void spell_poison_immunity( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_IMMUNE;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/4+10;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = IMM_POISON;
	affect_to_char(victim, &af);
	send_to_char( "You are immune to all forms of poison.\n\r", victim);
	}
	else
		send_to_char( "You are already immune to poison.\n\r", victim);

	return ;
}


/*by Wynn */
void spell_poison_rain( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA	*vch;
	CHAR_DATA       *vch_next;
	int			 dam;
	AFFECT_DATA af;

	if ( !IS_OUTSIDE( ch ) )
	{
	send_to_char( "You must be out of doors.\n\r", ch );
	return ;
	}

	dam = dice( level , 8 );

	af.where		 = TO_AFFECTS;
	af.type		 = gsn_poison;
	af.level		 = level;
	af.duration  = level;
	af.location  = APPLY_ST;
	af.modifier  = -4;
	af.bitvector = AFF_POISON;

	send_to_char( "A burning rain falls on your enemy's heads!\n\r", ch );
	act( "$n brings forth a rain of poison!",
	ch, NULL, NULL, TO_ROOM );

	for ( vch = char_list; vch!=NULL; vch = vch_next )
	{
	vch_next=vch->next;
	if (vch->in_room == NULL)
		continue ;
	if ( vch->in_room == ch->in_room )
	{
		if ( vch != ch
		&& ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
		&& vch->class!=5 )
		{
		damage_old( ch, vch, saves_spell( level, vch, DAM_POISON ) ? dam /2 : dam, sn, DAM_POISON, TRUE );
		if(!saves_spell(level,vch, DAM_POISON))
		  affect_join( vch, &af );
		}
		continue ;
	}
	}

	return ;
}

/* by Wynn */
void spell_rage ( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA	*victim = (CHAR_DATA *)	vo;
	AFFECT_DATA	af;

	if ( is_affected( victim, sn ) )
	return ;
	af.where		= TO_AFFECTS;
	af.type		= sn;
	af.level	 	= level;
	af.duration = level / 4;
	af.location = APPLY_DAMROLL;
	af.modifier = level / 5;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location = APPLY_AC;
	af.modifier = level*3;
	affect_to_char( victim, &af );

	af.location = APPLY_HIT;
	af.modifier = level/2;
	affect_to_char( victim, &af );
	victim->hit = UMIN( victim->hit + level*2, victim->max_hit );
	update_pos( victim );

	if (ch != victim )
	send_to_char( "Ok.\n\r", ch );
	send_to_char( "You feel enraged!\n\r", victim );
	return ;
}

void spell_regeneration( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA 	*victim = (CHAR_DATA *) vo;
	AFFECT_DATA	af;

	if (IS_AFFECTED( victim, AFF_REGENERATION ))
	{
		if(ch==victim)
			send_to_char("You are already healing at an accelerated rate.\n\r",ch);
		else
			send_to_char("They are already regenerating.\n\r",ch);
		return;
	}

	af.where		= TO_AFFECTS;
	af.type		= sn;
	af.level	 	= level;
	af.duration = level / 4;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_REGENERATION;
	affect_to_char( victim, &af );

	if(victim==ch)
		send_to_char("You feel more vibrant!!!\n\r", ch);
	else
	{
		send_to_char("You feel more vibrant!!!\n\r", victim);
		send_to_char("Ok.", ch);
	}

	return ;
}

/*contributed by Wynn, Modified by Airius */
void spell_rejuvinate( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA 	*victim = (CHAR_DATA *) vo;

	victim->hit = victim->max_hit;
	update_pos( victim );
	send_to_char( "You feel rejuvinated...\n\r", victim );
	if ( ch != victim )
		send_to_char( "Ok.\n\r", ch );
	if ( is_affected( victim, gsn_curse ) )
		{
		affect_strip( victim, gsn_curse );
		send_to_char( "A warm feeling fills your body.\n\r", victim);
		act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
		}

	if ( is_affected( victim, gsn_poison ) )
		{
		affect_strip( victim, gsn_poison );
		send_to_char( "You feel better.\n\r", victim);
		}
	return ;
}
/* end contribution */

/* by Airius */
void spell_resist_poison( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/4+10;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_POISON;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from all forms of poison.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from poison.\n\r", victim);

	return ;
}



/*Airius WWW*/
void spell_rune_edge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
		  if(IS_WEAPON_STAT(obj,WEAPON_SHARP)||IS_WEAPON_STAT(obj,WEAPON_VORPAL))
		  {
			send_to_char("That weapon is already quite sharp.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_SHARP;
		  affect_to_obj(obj, &af);

		  act("$p looks newly honed.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}


/*Airius WWW*/
void spell_runic_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
		  if(IS_WEAPON_STAT(obj,WEAPON_VORPAL) || IS_WEAPON_STAT(obj,WEAPON_SHARP))
		  {
			send_to_char("That weapon is already magically sharp.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_VORPAL;
		  affect_to_obj(obj, &af);

		  act("$p gleams with magical strength.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}

/*Airius WWW*/
void spell_true_sight( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_IMMUNE;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = IMM_ILLUSION;
	affect_to_char(victim, &af);
	send_to_char( "You see more clearly now.\n\r", victim);
	if(victim!=ch)
		send_to_char("OK.", ch);
	}
	else
	{
	  if(victim!=ch)
		send_to_char( "He already can see clearly.\n\r", ch);
	  else
		send_to_char( "You can already see clearly.\n\r", ch);
	}
	if ( !is_affected( victim, AFF_DETECT_HIDDEN ) )
		{
		af.where 	 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_DETECT_HIDDEN;
		affect_to_char( victim, &af );
		send_to_char( "Your awarness improves.\n\r", victim );
		}
	if ( !is_affected( victim, AFF_DETECT_INVIS ) )
		{
		af.where		 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_DETECT_INVIS;
		affect_to_char( victim, &af );
		send_to_char( "Your eyes tingle with detect invisability.\n\r", victim );
		}


	return ;
}

/*Airius WWW*/
void spell_vampiric_touch( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;
	int temp_hps;

	if(ch==victim)
	{
		send_to_char("You cannot target yourself!", ch);
		return ;
	}

	dam=dice(level, 7)/abs(4+ch->alliance);

	if(saves_spell(level, victim, DAM_NEGATIVE))
		dam /= 2;
        temp_hps=victim->hit;
	damage_old(ch, victim, dam, sn, DAM_NEGATIVE, TRUE);
	if(victim->hit<-10) victim->hit=-10;
	/*gives caster victims hps*/
        if(victim!=NULL)
          ch->hit+=temp_hps-victim->hit;      
	return ;
}

/* by Airius */
void spell_wizard_eye( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 char buf[MAX_STRING_LENGTH];

	 if ( ( victim = get_char_world( ch, target_name ) ) == NULL
	 ||   victim == ch
	 ||   victim->in_room == NULL
	 ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
	 ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
	 ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
         ||   ((victim->in_room->vnum > 20999) && (victim->in_room->vnum<21031)))
	 {
		  send_to_char( "You failed.\n\r", ch );
		  return;
	 }

	 sprintf(buf, "%s look", target_name);
	 do_at(ch, buf);

	return ;
}

/* By Bonhomme */
void spell_chaos_lace( int sn, int level, CHAR_DATA *ch, 
void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    int result, fail;

    if ( !CAN_WEAR( obj, ITEM_WEAR_BODY) &&
         !CAN_WEAR( obj, ITEM_WEAR_HEAD) &&
         !CAN_WEAR( obj, ITEM_WEAR_LEGS) &&
         !CAN_WEAR( obj, ITEM_WIELD) )
      {
        send_to_char("Chaos lace works only on torso \
armor, leggings, helmets, and weapons.\n\r",ch);
        return;
      }

    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be enchanted.\n\r",ch);
        return;
    }
/* Only one weird enchant per item */

    if (obj->chaos) {
        send_to_char("That item is already as weird as it can \
get!\n\r",ch);
        return;
    }

/* Items with attitudes don't like weirdness, and resist it */

    if ( (IS_OBJ_STAT(obj,ITEM_ANTI_EVIL)) ||
         (IS_OBJ_STAT(obj,ITEM_ANTI_GOOD)) ||
         (IS_OBJ_STAT(obj,ITEM_ANTI_NEUTRAL)) ) {
       fail = 50;
       fail += (level - obj->level);
       result = number_percent();
       if (result < fail) {
            act("$p weirds out completely!",ch,obj,NULL,TO_CHAR);
            act("$p weirds out completely!",ch,obj,NULL,TO_ROOM);
            extract_obj(obj);
            return;
       }
       REMOVE_BIT(obj->extra_flags,ITEM_ANTI_GOOD);
       REMOVE_BIT(obj->extra_flags,ITEM_ANTI_EVIL);
       REMOVE_BIT(obj->extra_flags,ITEM_ANTI_NEUTRAL);
    }

/* weirdness ruins all flags */

    obj->enchanted = TRUE;
    for (paf = obj->affected; paf != NULL; paf = paf_next)
    {
       paf_next = paf->next;
       free_affect(paf);
    }
    obj->affected = NULL;
    obj->extra_flags = 0;

    obj->chaos = TRUE;

    act("$p suddenly looks rather weird.",ch,obj,NULL,TO_CHAR);
    act("$p suddenly looks rather weird.",ch,obj,NULL,TO_ROOM);
    SET_BIT(obj->extra_flags,ITEM_CHAOS);

/* Add affects, often bad, depending on what type item this is */

    paf = new_affect();
    paf->where       = TO_OBJECT;
    paf->type        = sn;
    paf->level       = level;
    paf->duration    = -1;
    paf->bitvector   = 0;

    if ( CAN_WEAR(obj, ITEM_WEAR_BODY) ) {
       if (number_percent() <= 60) {
          paf->location = APPLY_HIT;
          paf->modifier = (((number_percent() % 6) - 3) * 10);
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else {
          paf->location = APPLY_ST;
          paf->modifier = ((number_percent() % 6) - 3) * 2 ;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
    }
    if ( CAN_WEAR(obj, ITEM_WEAR_HEAD) ) {
       if (number_percent() <= 60) {
          paf->location = APPLY_MANA;
          paf->modifier = (((number_percent() % 6) - 3) * 10);
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else if (number_percent() <= 50) {
          paf->location = APPLY_IN;
          paf->modifier = ((number_percent() % 6) - 3) * 2;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else {
          paf->location = APPLY_ME;
          paf->modifier = ((number_percent() % 6) - 3) * 2;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
    }
    if ( CAN_WEAR(obj, ITEM_WEAR_LEGS) ) {
       if (number_percent() <= 60) {
          paf->location = APPLY_MOVE;
          paf->modifier = (((number_percent() % 6) - 3) * 10);
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else {
          paf->location = APPLY_CO;
          paf->modifier = ((number_percent() % 6) - 3) * 2;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
    }

/* weird weapons are weird, and have three different 10% chances;
   they can easily wind up with no affect, though still weird */

    if ( CAN_WEAR(obj, ITEM_WIELD) ) {
       if (number_percent() <= 10) {
          paf->location = APPLY_HIT;
          paf->modifier = (((number_percent() % 6) - 3) * 10);
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else if (number_percent() <= 10) {
          paf->location = APPLY_ST;
          paf->modifier = ((number_percent() % 6) - 3) * 2;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
       else if (number_percent() <= 10) {
          paf->location = APPLY_QU;
          paf->modifier = ((number_percent() % 6) - 3) * 2;
          paf->next     = obj->affected;
          obj->affected = paf;
       }
    }

    return ;	
}

/* By Bonhomme */
void spell_mithril_glaze( int sn, int level, CHAR_DATA *ch, \
void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    OBJ_DATA *mit, *mit_next;
    AFFECT_DATA *paf;


    if ( !CAN_WEAR( obj, ITEM_WEAR_ABOUT) )
    {
        send_to_char("You can only apply a mithril glaze to objects \
which are worn about the body.\n\r",ch);
        return;
    }
    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be glazed.\n\r",ch);
        return;
    }
/*
 * Have to put (at least some) mithril items in manually until
 * someone gets around to fixing obj->pIndexData->material ;)
 */
    for ( mit = ch->carrying; mit != NULL; mit = mit_next)
    {
        mit_next = mit->next_content;
        if ( (mit->wear_loc == -1) &&
           ( (mit->pIndexData->vnum == 5200) ||
             (mit->pIndexData->vnum == 5201) ||
             (mit->pIndexData->vnum == 5202) ||
             (mit->pIndexData->vnum == 5203) ||
             (mit->pIndexData->vnum == 5204) ||
             (mit->pIndexData->vnum == 5205) ||
             (mit->pIndexData->vnum == 5206) ||
             (mit->pIndexData->vnum == 1312) ||
             (mit->pIndexData->vnum == 1314) ||
             (mit->pIndexData->vnum == 1355) ||
             (mit->pIndexData->vnum == 1357) ||
             (mit->pIndexData->vnum == 2323) ||
             (mit->pIndexData->vnum == 3505) ||
             (mit->pIndexData->vnum == 1101) ||
             (mit->pIndexData->vnum == 1107) ||
             (mit->pIndexData->vnum == 1614) ||
             (mit->pIndexData->vnum == 1615) ) )
             break;
    }
    if (mit == NULL) {
        send_to_char("You must have a mithril item to be consumed by \
this spell.\n\r",ch);
        return;
    }
    if (ch->pcdata->tired > 16) {
       send_to_char("You are not well-rested enough to cast this \
spell just now. Have a nap first.\n\r",ch);
       return;
    }
    act("$p effervesces into a mithril cloud...",ch,mit,NULL,TO_CHAR);
    act("$p effervesces into a mithril cloud...",ch,mit,NULL,TO_ROOM);
    extract_obj(mit);
    if ( number_percent() <= 40 ) {
        act("Which settles on $p and sets it afire!",ch,obj,\
NULL,TO_CHAR);
        act("Which settles on $p and sets it afire!",ch,obj,\
NULL,TO_ROOM);
        extract_obj(obj);
    }
    else {
        ch->mana = 0;
        ch->pcdata->tired += 60;
        ch->pcdata->condition[COND_THIRST] = 0;
        ch->pcdata->condition[COND_HUNGER] = 0;
        act("Which slowly condenses at your command upon $p.",ch,obj,\
NULL,TO_CHAR);
        act("Which slowly condenses upon $p.",ch,obj,\
NULL,TO_ROOM);
    }
    for ( paf = obj->affected; paf != NULL; paf = paf->next ) {
        if ( !str_cmp( skill_table[paf->type].name, "mithril glaze" ) ) {
            paf->modifier = (paf->modifier += dice(level / 3, 3));
            paf->level = UMAX(paf->level,level);
            obj->level = obj->level + 5;
            return;
        }
    }
    paf = new_affect();
    paf->where       = TO_OBJECT;
    paf->type        = sn;
    paf->level       = level;
    paf->duration    = -1;
    paf->bitvector   = 0;
    paf->location    = APPLY_MANA;
    paf->modifier    = dice(level / 3, 3);
    paf->next        = obj->affected;
    obj->level       = obj->level + 5;
    obj->affected    = paf;
    return ;
 }

/* By Bonhomme */
void spell_extension( int sn, int level, CHAR_DATA *ch, \
void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf, *afpaf;
    char buf[MAX_STRING_LENGTH];

    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be extended.\n\r",ch);
        return;
    }
    for ( afpaf = paf = obj->affected; paf != NULL; paf = paf->next ) {
        if ( paf->duration < 0 )
                continue;
        if ( afpaf->duration < 0 ) {
                afpaf = paf;
                continue;
        }
        if ( afpaf->duration > paf->duration )
                afpaf = paf;
    }
    if ( ( afpaf != NULL ) && ( afpaf->duration > 0 ) ) {
        afpaf->duration += dice(((ch->pcdata->learned[sn] * level ) \
/ 100), 10);
        if ( afpaf->duration > 9999 )
            afpaf->duration = 9999;
        sprintf( buf, "The %s affect seems steadier.\n\r", \
skill_table[afpaf->type].name );
        send_to_char( buf, ch );
    }
    else
        act("You couldn't find any effect on $p to extend.",ch,obj,\
NULL,TO_CHAR);
    return;

}

void spell_cobble( int sn, int level, CHAR_DATA *ch, \
void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    char ittype[5];
    int i, a;

    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be made into footwear.\n\r",ch);
        return;
    }
    act("$n's hands fly with mystic guidance, cutting up $p...",ch,obj,NULL,TO_ROOM);
    act("Your hands fly with mystic guidance, cutting up $p...",ch,obj,NULL,TO_CHAR);
    if ( number_percent() <= ( 10 - ch->level + obj->level ) ) {
        act("And assemble a useless pile of rags!",ch,obj,NULL,TO_CHAR);
        act("And assemble a useless pile of rags!",ch,obj,NULL,TO_ROOM);
        extract_obj(obj);
        return;
    }
     for ( i = 0; ( (i < ( MAX_STRING_LENGTH - 1 )) && ( obj->short_descr[i] \
!= '\0') ); i++ )

    sprintf(ittype, "%c%c%c%c", obj->short_descr[i - 3],
                                obj->short_descr[i - 2],
                                obj->short_descr[i - 1],
                                obj->short_descr[i] );

/* 2 lines for testing */

    sprintf( buf, "Working on a %s.\n\r", ittype );
    send_to_char( buf, ch );

    if ( ( str_cmp(ittype, "robe" ) ) && ( str_cmp(ittype, "hide" ) ) &&
         ( str_cmp(ittype, "pelt" ) ) && ( str_cmp(ittype, "skin" ) ) &&
         ( str_cmp(ittype, "loak" ) ) && ( str_cmp(ittype, "ncho" ) ) ) {
       send_to_char("You can only make footwear from things best described as\n\robes, cloaks, ponchos, hides, pelts, or skins.\n\r", ch );
       return;
    }
    if ( !str_cmp(ittype, "loak" ) )
       i -= 6;
    else if ( !str_cmp(ittype, "ncho" ) )
       i -= 7;
    else
       i -= 5;
    output = new_buf();
    add_buf( output, "a pair of " );
    a = 0;
    if ( ( obj->short_descr[0] == 'a' )
      && ( obj->short_descr[1] == ' ' ) )
       a += 2;
    if ( ( obj->short_descr[0] == 'a' )
      && ( obj->short_descr[1] == 'n' )
      && ( obj->short_descr[2] == ' ' ) )
       a += 3;
    for ( ; a <= i; a++ ) {
       sprintf(buf, "%c", obj->short_descr[a] );
       add_buf( output, buf );
    }
    if ( !str_cmp(ittype, "robe" ) )
       add_buf( output, "slippers" );
    if ( !str_cmp(ittype, "loak" ) )
       add_buf( output, "slippers" );
    if ( !str_cmp(ittype, "ncho" ) )
       add_buf( output, "shoes" );
    if ( !str_cmp(ittype, "skin" ) )
       add_buf( output, "shoes" );
    if ( !str_cmp(ittype, "pelt" ) )
       add_buf( output, "boots" );
    if ( !str_cmp(ittype, "hide" ) )
       add_buf( output, "boots" );

    obj->wear_flags = ITEM_WEAR_FEET;
    if ( obj->item_type != ITEM_ARMOR ) {
       obj->item_type = ITEM_ARMOR;
       if ( ( !str_cmp(ittype, "robe") ) || ( !str_cmp(ittype, "loak") ) )
          a = 1;
       else if ( ( !str_cmp(ittype, "ncho") ) || ( !str_cmp(ittype, "skin") ) )
          a = 2;
       else
          a = 3;
       a += ( obj->level / 10 );
       obj->value[0] = obj->value[1] = obj->value[2] = a;
    }
    obj->value[0] = ( obj->value[0] * ch->pcdata->learned[sn] / 100 );
    obj->value[1] = ( obj->value[1] * ch->pcdata->learned[sn] / 100 );
    obj->value[2] = ( obj->value[2] * ch->pcdata->learned[sn] / 100 );

/* test line */
    send_to_char(str_dup( buf_string(output)), ch);
    if ( !str_cmp(ittype, "pelt" ) )
       add_buf( output, "boots" );
    if ( !str_cmp(ittype, "hide" ) )
       add_buf( output, "boots" );

    obj->wear_flags = ITEM_WEAR_FEET;
    if ( obj->item_type != ITEM_ARMOR ) {
       obj->item_type = ITEM_ARMOR;
       if ( ( !str_cmp(ittype, "robe") ) || ( !str_cmp(ittype, "loak") ) )
          a = 1;
       else if ( ( !str_cmp(ittype, "ncho") ) || ( !str_cmp(ittype, "skin") ) )
          a = 2;
       else
          a = 3;
       a += ( obj->level / 10 );
       obj->value[0] = obj->value[1] = obj->value[2] = a;
    }
    obj->value[0] = ( obj->value[0] * ch->pcdata->learned[sn] / 100 );
    obj->value[1] = ( obj->value[1] * ch->pcdata->learned[sn] / 100 );
    obj->value[2] = ( obj->value[2] * ch->pcdata->learned[sn] / 100 );

/* test line */
    send_to_char( buf_string(output), ch);

    obj->short_descr = str_dup( buf_string(output));

    act("And assemble $p.",ch,obj,NULL,TO_ROOM);
    act("And assemble $p.",ch,obj,NULL,TO_CHAR);
    free_buf(output);
}


void spell_wrath( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        AFFECT_DATA af;
	CHAR_DATA *victim;

	victim=(CHAR_DATA *) vo;

	if(!IS_EVIL(ch))
	{
	  send_to_char("You must be evil to wield such majiks.\n\r",ch);
	  return ;
	}

	if(IS_EVIL(victim))
	{
	  send_to_char("You may not cast that against those who are evilly aligned.\n\r",ch);
	  return ;
	}

	if(!saves_spell( level, victim, DAM_NEGATIVE) )
	{
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/5;
    af.location  = APPLY_HIT;
    af.modifier  = (-level / 4) - 15;
    af.bitvector = 0;
    affect_join( victim, &af );
    send_to_char( "You feel a force of great Ill invade your soul.\n\r", victim );
    act("$n drops to $s knees in horror.",victim,NULL,NULL,TO_ROOM);
    return;
	}
        else
	{          
        act("$n shivers against a great force, but it passes.",victim,NULL,NULL,TO_ROOM);      
        send_to_char("You feel a great Ill, but it passes.\n\r",victim);
        return ;
        } 
}
