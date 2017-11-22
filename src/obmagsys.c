/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
/* this section of code is purely oblivion coding */

#if defined(macintosh)^M
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
#include "obmagsys.h"
#include "recycle.h"

const struct magic_flag magic_table[MAX_SCHOOLS] =
{
  { "abjuration", "abjuration", SCHOOL_ABJURATION, 0 },
  { "alteration", "alteration", SCHOOL_ALTERATION, 0 },
  { "conjuration","conjuration", SCHOOL_CONJURATION, 0 },
  { "summoning", "summoning", SCHOOL_SUMMONING, 0 },
  { "illusion", "illusion", SCHOOL_ILLUSION, 0 },
  { "phantasm", "phantasm", SCHOOL_PHANTASM, 0 },
  { "invocation","invocation", SCHOOL_INVOCATION, 0 },
  { "evocation", "evocation", SCHOOL_EVOCATION, 0 },
  { "enchantment","enchantment", SCHOOL_ENCHANTMENT, 0 },
  { "charm", "charm", SCHOOL_CHARM, 0 },
  { "divination", "divination", SCHOOL_DIVINATION, 0 },
  { "necromancy", "necromancy", SCHOOL_NECROMANCY, 0 },
  { "wild magic", "wild magic", DISC_WILD_MAGIC, 1 },
  { "earth", "earth", DISC_ELEM_EARTH, 1 },
  { "air", "air", DISC_ELEM_AIR, 1 },
  { "water", "water", DISC_ELEM_WATER, 1 },
  { "fire", "fire", DISC_ELEM_FIRE, 1 },
  { "healing", "healing", SPHERE_HEALING, 1 },
  { "weather", "weather", SPHERE_WEATHER, 1 },
  { "protection", "protection", SPHERE_PROTECTION, 1 },
  { "plant", "plant", SPHERE_PLANT, 1 },
  { "law", "law", SPHERE_LAW, 1 },
  { "sun", "sun", SPHERE_SUN, 1 },
  { "animal", "animal", SPHERE_ANIMAL, 1 },
  { "combat", "combat", SPHERE_COMBAT, 1 },
  { "creation", "creation", SPHERE_CREATION, 1 },
  { "time", "time", SPHERE_TIME, 1 }
};

void check_realm_imp(CHAR_DATA * ch, int sn, bool success)
{
   int i;

   for(i=0; i<MAX_SCHOOLS; i++)
   {
     if(magic_table[i].is_sphere)
     {
        if(IS_SET(skill_table[sn].sphere_req, magic_table[i].flag))
        {
          check_improve(ch,skill_lookup(magic_table[i].school_name),
                         success,5);
        }
     }
     else
     {
        if(IS_SET(skill_table[sn].realm_req, magic_table[i].flag))
        {
          check_improve(ch,skill_lookup(magic_table[i].school_name),
                         success,5);
        }
     }
   }

   return ;
}

void set_char_magic_bits(CHAR_DATA * ch)
{
  int i;

  if(IS_NPC(ch))
	return ;
  	
  for(i=0; i<MAX_SCHOOLS; i++)
  {
    if(get_skill(ch,skill_lookup(magic_table[i].school_name))>0)
    {
	if(!magic_table[i].is_sphere)
           SET_BIT(ch->pcdata->schools, magic_table[i].flag);
        else
	   SET_BIT(ch->pcdata->spheres, magic_table[i].flag);
    }		

  }
 
  return ;
}

bool has_spell_req(CHAR_DATA * ch, int sn)
{ 
  if(IS_NPC(ch))
    return TRUE;
  
  if((IS_SET(ch->pcdata->schools, skill_table[sn].realm_req) ||
     skill_table[sn].realm_req==0) &&
     (IS_SET(ch->pcdata->spheres, skill_table[sn].sphere_req) ||
     skill_table[sn].sphere_req== 0   ))
       return TRUE;
     return FALSE;
}

int get_avg(CHAR_DATA * ch, int sn)
/*
 * returns the average skills % for the character in all spell realms
 * schools, disciplines and spheres for a given spell
 */
{
   int i;
   int count=0;
   int total=0;
   int chance=0;

   for(i=0; i<MAX_SCHOOLS; i++)
   {
     if(magic_table[i].is_sphere)
     {
 	if(IS_SET(skill_table[sn].sphere_req, magic_table[i].flag))
        {
	  count++;
          total+=get_skill(ch,skill_lookup(magic_table[i].school_name));	
	}
     }
     else
     {
        if(IS_SET(skill_table[sn].realm_req, magic_table[i].flag))
	{
          count++;
	  total+=get_skill(ch,skill_lookup(magic_table[i].school_name));
	}
     }	   	
   }

   if(count==0)
     return 0;

   chance=total/count;
    
   return chance;	
}

bool check_learn_spell(CHAR_DATA * ch, int sn)
{
  int chance;

  if(IS_NPC(ch))
    return FALSE;	

  if(!has_spell_req(ch,sn))
    return FALSE;      

  if(get_skill(ch, sn)>0)
    return FALSE;

  chance=get_avg(ch, sn);

  if(( number_range(1,100) < (chance-(skill_table[sn].skill_level[ch->class])*10) * (ch->level/10+1) / skill_table[sn].rating[ch->class] ) 
   && ( skill_table[sn].skill_level[ch->class]*10-10<ch->level && number_range(1,3)==3 ))
    return TRUE; 

  return FALSE; 	
}

void on_level_learn(CHAR_DATA * ch)
{
  int i;
  char buf[MAX_STRING_LENGTH];

  if(IS_NPC(ch))
	return ;
  
  set_char_magic_bits(ch); 
 	
  for(i=skill_lookup("acid blast"); i<=skill_lookup("wrath"); i++)
  {
    if(check_learn_spell(ch,i))
    {
      check_realm_imp(ch,i,TRUE); 	
      ch->pcdata->learned[i]=1;
      sprintf(buf,"You have mastered the spell %s.\n\r", skill_table[i].name);
      send_to_char(buf,ch);	
    }
  }		

  return ;
}

void do_spinfo(CHAR_DATA *ch, char * argument)
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  int i, j, k;
  bool found=FALSE;

  one_argument(argument,arg);
  
  if(arg[0]=='\0')
  {
    send_to_char("Realms:\n\r",ch);
    send_to_char("Abjuration  Alteration   Conjuration  Summoning    Illusion\n\r",ch);
    send_to_char("Phantasm    Invocation   Evocation	Enchantment  Charm  Necromancy\n\r",ch);
    send_to_char("Spheres:\n\r",ch);
    send_to_char("Wild magic  Earth   Air   Water   Fire   Healing   Weather\n\r",ch);
    send_to_char("Protection  Plant   Law   Sun   Animal  Combat  Creation  Time\n\r",ch);
    return ;
  }
  
  for(i=0; i<MAX_SCHOOLS; i++)
  {
    if(!str_cmp(magic_table[i].school_name, arg))
    {
	found=TRUE;
        break ; 
    }
  }

  if(!found)
  {
    send_to_char("No such school, discipline or sphere of influence.\n\r",ch);
    return ;	
 }

sprintf(buf, "Spell:           Required realms and Spheres of Influence\n\r____________________________________________________________________\n\r");
send_to_char(buf,ch);

  for(j=skill_lookup("acid blast"); j<=skill_lookup("wrath"); j++)
  {
    if(((magic_table[i].is_sphere) && IS_SET(skill_table[j].sphere_req, 
        magic_table[i].flag)) ||
       ((!magic_table[i].is_sphere) && IS_SET(skill_table[j].realm_req,
        magic_table[i].flag)))
    {
       sprintf(buf, "%-20s:", skill_table[j].name);
       send_to_char(buf,ch);
       for(k=0; k<MAX_SCHOOLS; k++)
       {
	if(((magic_table[k].is_sphere) && IS_SET(skill_table[j].sphere_req,
        magic_table[k].flag)) ||
       ((!magic_table[k].is_sphere) && IS_SET(skill_table[j].realm_req,
        magic_table[k].flag)))
	{
	  sprintf(buf, "%s ",magic_table[k].school_name);
          send_to_char(buf,ch);
 	}
       }  		
       send_to_char("\n\r", ch);
    }
  }

  return ;
}

void do_spells(CHAR_DATA *ch, char * argument)
{
  bool found=FALSE;
  int i;
  int counter=0;
  char buf[MAX_STRING_LENGTH];
  char col[MAX_STRING_LENGTH];
  BUFFER *output;

  if(IS_NPC(ch))
    return ;

  output = new_buf();

  for(i=skill_lookup("acid blast"); i<=skill_lookup("wrath"); i++)
  {
     if(ch->pcdata->learned[i])
     {
        found=TRUE;
        if(ch->pcdata->learned[i]>90)
          strcpy(col,FG_LT_BLUE);
        else if(ch->pcdata->learned[i]>70)
          strcpy(col,FG_LT_GREEN);
         else if(ch->pcdata->learned[i]>50)
          strcpy(col,FG_YELLOW);
        else if(ch->pcdata->learned[i]>20)
          strcpy(col,FG_LT_RED);
        else strcpy(col,FG_DK_GRAY);
        sprintf(buf,"%s%-20s %3d ", IS_SET(ch->act, PLR_COLOR) ? col : "",
          skill_table[i].name, skill_table[i].min_mana);
        counter++;
        add_buf( output, buf );

        if(counter>2)
        {
          add_buf( output, "\n\r" );
          counter=0;
        }
     }
  }

  if(!found)
     send_to_char("You have no spells.\n\r",ch);
  else {
     add_buf( output, "\n\r" );
     page_to_char( str_dup( buf_string(output)), ch );
  }
   free_buf(output);
  return ;
}
 

void do_study(CHAR_DATA * ch, char * argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *scroll;
  int i;

  if(IS_NPC(ch))
	return ;
 
  set_char_magic_bits(ch);
 
  argument = one_argument( argument, arg1 );

  if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
  {
    send_to_char( "You do not have that scroll.\n\r", ch );
    return;
  }

  if ( scroll->item_type != ITEM_SCROLL )
  {
    send_to_char( "You can study only scrolls.\n\r", ch );
    return;
  }
  
  for(i=1; i<=3; i++)
  {
    if(scroll->value[i]<1)
      continue ; 
    if(ch->pcdata->learned[scroll->value[i]]>0)
    {
	sprintf(buf, "You already know %s.\n\r", 
                 skill_table[scroll->value[i]].name);
        send_to_char(buf,ch);
        continue ;
    }
    if(!has_spell_req(ch,scroll->value[i]))
    {
      sprintf(buf, "You do not have the requirements to learn %s.\n\r",
                skill_table[scroll->value[i]].name);
      send_to_char(buf,ch);
      continue ;
    }
    if(check_learn_spell(ch,scroll->value[i]))
    {
      ch->pcdata->learned[scroll->value[i]]=1;
      sprintf(buf, "Your studious nature has paid off.\n\rYou have learned %s.\n\r",
               skill_table[scroll->value[i]].name); 
      send_to_char(buf,ch);
      check_realm_imp(ch,scroll->value[i], TRUE);
    }
    else
    {
      sprintf(buf, "You have failed to learn %s.\n\r",
               skill_table[scroll->value[i]].name);
      check_realm_imp(ch,scroll->value[i], FALSE);
    }    

  }
  extract_obj(scroll);
  WAIT_STATE(ch, 300);
  return ;
}

void do_teach(CHAR_DATA *ch, char * argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int sn;
  CHAR_DATA *victim;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if(arg1[0]=='\0' || arg2[0]=='\0')
  {
    send_to_char("Syntax: teach <character> <spell>\n\r",ch);
    return ;	
  }
  
  if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return ; 
  }

  if(IS_SET(victim->act, PLR_NOTEACH))
  {
    send_to_char( "They can not be taught.\n\r", ch);	
    return ;
  }

  if( ( sn = skill_lookup(arg2) ) == -1  || 
        sn < skill_lookup("acid blast") ||
        sn > skill_lookup("wrath") )
  {
    send_to_char("That is not a spell.\n\r", ch);
    return ;
  } 

  if(IS_NPC(ch) || IS_NPC(victim))
  {
    send_to_char("Not with npcs.\n\r", ch);
    return ;
  }

  set_char_magic_bits(victim);
  
  if(ch->pcdata->learned[sn]<1)
  {
    send_to_char("You do not know that spell.\n\r",ch);
    return ;
  }

  if(victim->pcdata->learned[sn]>0)
  {
    send_to_char("They already know tht spell.\n\r",ch);
    return ;
  }

  if(victim->position>POS_RESTING)
  {
    send_to_char("Your student must be in a relaxed state.\n\r",ch);
    return ;
  }

  if(victim->position<POS_RESTING)
  {
    send_to_char("Your student must be awake.\n\r",ch);
    return ;
  }

  if(check_learn_spell(victim,sn))
  {
    victim->pcdata->learned[sn]=1;
    sprintf(buf,"You have learned %s from %s.\n\r",
            skill_table[sn].name, ch->name);
    send_to_char(buf,victim);
    send_to_char("You have taught your student well.\n\r",ch);
    check_realm_imp(ch,sn,TRUE);
    check_realm_imp(ch,sn,TRUE);
  }
  else
  { 
    send_to_char("You have failed.\n\r", ch);
    send_to_char("Your teacher was unable to help you.\n\r", victim);
    check_realm_imp(victim,sn,FALSE);
  }
 
  WAIT_STATE(ch, 400);
  WAIT_STATE(victim, 400);

  return ; 
}
