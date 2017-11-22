/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

void do_add( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );
	if(ch->clan==0)
	{
		send_to_char("You are not in a clan yourself!", ch);
		return ;
	}
	if(!clan_table[ch->clan].can_add[ch->clanrank])
	{
		send_to_char("You have not the authority to do that.\n\r",ch);
		return ;
	}
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
		  send_to_char( "They aren't playing.\n\r", ch );
		  return;
	 }
	if(victim->clan>0)
	{
		send_to_char( "They are already in a clan.\n\r",ch);
		return ;
	}
	if(victim->seeks!=ch->clan)
	{
		send_to_char("They are not seeking to join your clan.\n\r",ch);
		return ;
	}
	victim->clan=ch->clan;
	victim->clanrank=0;
	victim->seeks=0;

	sprintf(buf,"They are now a %s of your clan.\n\r",
		 clan_table[ch->clan].clan_rank_title[0]);
	send_to_char(buf,ch);
	sprintf(buf,"You are now a %s of clan %s.\n\r",
		 clan_table[ch->clan].clan_rank_title[0],
		 clan_table[ch->clan].name);
	send_to_char(buf,victim);

	return ;
}

void do_demote( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );
	if(ch->clan==0)
	{
		send_to_char("You are not in a clan yourself!", ch);
		return ;
	}
	if(!clan_table[ch->clan].can_promote[ch->clanrank])
	{
		send_to_char("You have not the authority to do that.\n\r",ch);
		return ;
	}
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
		  send_to_char( "They aren't playing.\n\r", ch );
		  return;
	 }
	if(victim->clan!=ch->clan)
	{
		send_to_char( "They are not in your clan.\n\r",ch);
		return ;
	}
	if(ch->clanrank<victim->clanrank+1)
	{
		send_to_char("They are too high in rank for your authority.\n\r",ch);
		return ;
	}

	if(victim->clanrank<1)
	{
		send_to_char("They are already of the lowest rank.\n\r",ch);
		return;
	}

	victim->clanrank-=1;

	sprintf(buf,"They are now a %s of your clan.\n\r",
		 clan_table[ch->clan].clan_rank_title[victim->clanrank]);
	send_to_char(buf,ch);
	sprintf(buf,"You are now a %s of clan %s.\n\r",
		 clan_table[ch->clan].clan_rank_title[0],
		 clan_table[ch->clan].name);
	send_to_char(buf,victim);

	return ;
}

void do_outcast( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );
	if(ch->clan==0)
	{
		send_to_char("You are not in a clan yourself!", ch);
		return ;
	}
	if(!clan_table[ch->clan].can_remove[ch->clanrank])
	{
		send_to_char("You have not the authority to do that.\n\r",ch);
		return ;
	}
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
		  send_to_char( "They aren't playing.\n\r", ch );
		  return;
	 }
	if(victim->clan!=ch->clan)
	{
		send_to_char( "They are not in your clan.\n\r",ch);
		return ;
	}
	if(ch->clanrank<victim->clanrank+1)
	{
		send_to_char( "You have not the authority to do that.\n\r",ch);
		return ;
	}

	victim->clan=0;
	victim->clanrank=0;

	sprintf(buf,"They are no longer a %s of your clan.\n\r",
		 clan_table[ch->clan].clan_rank_title[0]);
	send_to_char(buf,ch);
	sprintf(buf,"You are no longer a %s of clan %s.\n\r",
		 clan_table[ch->clan].clan_rank_title[0],
		 clan_table[ch->clan].name);
	send_to_char(buf,victim);

	return ;
}


void do_promote( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );
	if(ch->clan==0)
	{
		send_to_char("You are not in a clan yourself!", ch);
		return ;
	}
	if(!clan_table[ch->clan].can_promote[ch->clanrank])
	{
		send_to_char("You have not the authority to do that.\n\r",ch);
		return ;
	}
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
		  send_to_char( "They aren't playing.\n\r", ch );
		  return;
	 }
	if(victim->clan!=ch->clan)
	{
		send_to_char( "They are not in your clan.\n\r",ch);
		return ;
	}
	if(ch->clanrank<victim->clanrank+2)
	{
		send_to_char("They are already too high in rank for your authority.\n\r",ch);
		return ;
	}

	victim->clanrank+=1;

	sprintf(buf,"They are now a %s of your clan.\n\r",
		 clan_table[ch->clan].clan_rank_title[victim->clanrank]);
	send_to_char(buf,ch);
	sprintf(buf,"You are now a %s of clan %s.\n\r",
		 clan_table[ch->clan].clan_rank_title[victim->clanrank],
		 clan_table[ch->clan].name);
	send_to_char(buf,victim);

	return ;
}

void do_seek( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int clan;

	argument = one_argument( argument, arg1 );

	if(ch->clan!=0)
	{
		send_to_char("You are already in a clan.\n\r", ch);
		return ;
	}
	if ((clan = clan_lookup(arg1)) == 0)
	 {
	send_to_char("No such clan exists.\n\r",ch);
	return;
	 }

	ch->seeks=clan;
	
	sprintf(buf, "You are seeking to join %s.\n\r",
			  clan_table[clan].name);
	send_to_char(buf,ch);

	return ;
}
