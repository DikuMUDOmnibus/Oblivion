/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "color.h"
#include "critical.h"

DECLARE_DO_FUN(do_drop);

void do_critical args( (CHAR_DATA *actor, CHAR_DATA *victim, CRITICAL_DATA crit) );

void perform_critical(CHAR_DATA *actor, CHAR_DATA *victim, sh_int dam_done,
							 sh_int damage_type)
{
	int num;
	sh_int crit_type;
	sh_int crit_index;

	num=number_range(0,20)+number_range(0,20)+dam_done-10;
	if(num>55) crit_type=4;
	else if(num>45) crit_type=3;
	else if(num>35) crit_type=2;
	else if(num>25) crit_type=1;
	else crit_type=0;

	num=number_range(1,100);

	if(num>98) crit_index=14;
	else if(num>95) crit_index=13;
	else if(num>90) crit_index=12;
	else if(num>85) crit_index=11;
	else if(num>80) crit_index=10;
	else if(num>75) crit_index=9;
	else if(num>70) crit_index=8;
	else if(num>65) crit_index=7;
	else if(num>60) crit_index=6;
	else if(num>55) crit_index=5;
	else if(num>50) crit_index=4;
	else if(num>40) crit_index=3;
	else if(num>30) crit_index=2;
	else if(num>20) crit_index=1;
	else crit_index=0;

	do_critical(actor,victim,critical_table[damage_type][crit_index][crit_type]);
}

void do_critical(CHAR_DATA *actor, CHAR_DATA *victim, CRITICAL_DATA crit)
{
	char bf[MAX_STRING_LENGTH];
	AFFECT_DATA af;
	void * vo = (void *) victim;
	int target = TARGET_CHAR;
	int i;

	if(crit.act_to_actor[0]=='\0')
		return ;

	act(crit.act_to_room, actor, NULL, victim, TO_NOTVICT );
	if(IS_SET(actor->act, PLR_COLOR))
		sprintf(bf,"%s%s%s", FG_LT_BLUE, crit.act_to_actor, FG_LT_GRAY);
	else
		strcpy(bf, crit.act_to_actor);
	act(bf , actor, NULL, victim, TO_CHAR );
	if(IS_SET(victim->act, PLR_COLOR))
		sprintf(bf,"%s%s%s", FG_LT_BLUE, crit.act_to_victim, FG_LT_GRAY);
	else
		strcpy(bf, crit.act_to_victim);
	act(bf, actor, NULL, victim, TO_VICT );

	for(i=0; i<3; i++)
	{
		victim->is_stunned+=crit.victim_effects[i]->stun_for;
		victim->will_die=crit.victim_effects[i]->mortal_in;
		victim->hit-=crit.victim_effects[i]->additional_damage;
		victim->mana-=crit.victim_effects[i]->additional_mana_loss;
		victim->bleeding+=crit.victim_effects[i]->hps_per_turn;

		if(crit.victim_effects[i]->sn>1)
		{
		af.where		 = TO_AFFECTS;
		af.type		 = crit.victim_effects[i]->sn;
		af.level		 = actor->level;
		af.duration  = crit.victim_effects[i]->duration;
		af.location  = crit.victim_effects[i]->aply;
		af.modifier  = crit.victim_effects[i]->modifier;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		if(i==0)
		{
		(*skill_table[crit.victim_effects[i]->sn].spell_fun) (
					crit.victim_effects[i]->sn, actor->level, actor, vo,target);
		}
		else
			if(crit.victim_effects[i]->sn!=crit.victim_effects[i-1]->sn)
				(*skill_table[crit.victim_effects[i]->sn].spell_fun) (
					crit.victim_effects[i]->sn, actor->level, actor, vo,target);
		}
	}
		actor->is_stunned+=crit.actor_effects->stun_for;
		actor->will_die=crit.actor_effects->mortal_in;
		actor->hit-=crit.actor_effects->additional_damage;
		actor->mana-=crit.actor_effects->additional_mana_loss;
		actor->bleeding+=crit.actor_effects->hps_per_turn;

		if(crit.actor_effects->sn>1)
		{
		af.where		 = TO_AFFECTS;
		af.type		 = crit.actor_effects->sn;
		af.level		 = actor->level;
		af.duration  = crit.actor_effects->duration;
		af.location  = crit.actor_effects->aply;
		af.modifier  = crit.actor_effects->modifier;
		af.bitvector = 0;
		affect_to_char( actor, &af );
		vo = (void *) actor;
		(*skill_table[crit.actor_effects->sn].spell_fun) (
			crit.actor_effects->sn, actor->level, victim, vo,target);
		}

	return ;
}

void crit_none( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 return;
}

void crit_broken_shieldarm( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;

	if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) != NULL )
	{
	 unequip_char( ch, obj );
	 do_drop(ch, obj->name);
	}

	if ( ( obj = get_eq_char( ch, WEAR_SECONDARY ) ) != NULL )
	{
	 unequip_char( ch, obj );
	 do_drop(ch, obj->name);
	}

	if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL )
	{
	 unequip_char( ch, obj );
	 do_drop(ch, obj->name);
	}

	return ;
}
