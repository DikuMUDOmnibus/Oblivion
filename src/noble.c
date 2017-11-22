/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 /******************************************************************************
* Oblivion 1.0 Alpha Code written by Wes Wagner                               *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include "merc.h"




void do_noble(CHAR_DATA *ch, char * argument)
{
	 char arg1[MAX_INPUT_LENGTH];
	 CHAR_DATA *victim;

	 argument = one_argument( argument, arg1 );

	 if ( arg1[0] == '\0')
	 {
		  send_to_char( "Syntax: noble <char>\n\r",ch);
		  return;
	 }
	 if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
		  send_to_char( "They aren't playing.\n\r", ch );
		  return;
	 }

	 if(victim->level>91)
		victim->pcdata->diplomacy=50;
	 else
		victim->pcdata->diplomacy=10;

	 send_to_char("They are now a noble.\n\r",ch);
	 send_to_char("You are now a noble.\n\r",victim);

	 return;
}

void do_moothelp(CHAR_DATA *ch, char * argument)
{
	send_to_char("Anything after the | is optional aruguments.\n\r",ch);
	send_to_char("moot <character> reward <amount>\n\r",ch);
	send_to_char("moot <character> induct\n\r",ch);
	send_to_char("moot <character> outcast\n\r",ch);
	send_to_char("moot <character> change <alliance|tendency> <+|->\n\r",ch);

	return ;
}

void do_moot(CHAR_DATA *ch, char * argument)
{

	char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH],arg3[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int cost=0;
	int scope=0;
	int type=0;

	if(IS_NPC(ch))
		return ;

	if(ch->pcdata->diplomacy<1)
	{
		send_to_char("You are not a noble.\n\r",ch);
		return ;
	}

	if(moot->moot_type!=0)
	{
		send_to_char("A moot is already in progress.\n\r", ch);
		return ;
	}

	argument = one_argument( argument, arg1 );
	if(arg1[0]=='\0')
	{
		do_moothelp(ch,"auto");
		return ;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	 {
	send_to_char( "They aren't here.\n\r", ch );
	return;
	 }

	if(ch==victim)
	{
		send_to_char("You cannot call moots on your on behalf.\n\r",ch);
		return ;
	}

	argument = one_argument( argument, arg2 );
	

	if ( !str_cmp( arg2, "reward" ) )
	{
		argument = one_argument( argument, arg3 );
		if(is_number(arg3))
		  scope=atoi(arg3);
		else
		{
			do_moothelp(ch,"auto");
			return ;
		}
		cost=cost+abs(scope);
		type=1;
	}

	if ( !str_cmp( arg2, "induct" ) )
	{
		if(victim->pcdata->diplomacy)
		{
		 send_to_char("They are already a noble.\n\r",ch);
		 return ;
		}	
		cost=2000;
		type=2;
	}

	if ( !str_cmp( arg2, "outcast" ) )
	{
		if(victim->pcdata->diplomacy==0)
                {
                 send_to_char("They are not a noble.\n\r",ch);
                 return ;
                }
               
		cost=4000;
		type=3;
	}

	if(type==0)
	{
		send_to_char("That is not a valid moot type.\n\r",ch);
		return ;
	}

	if(ch->pcdata->dip_points<cost)
	{
		send_to_char("You have not enough diplomacy points to call a moot.\n\r",ch);
		return ;
	}

	switch(type)
	{
	default: break;
	case 1: sprintf(buf,"A moot has been called on your behalf for %+d.\n\r", scope);
				send_to_char(buf,victim); break;
	case 2: send_to_char("The nobles are attempting to recruit you!!!\n\r",victim);
				break;
	case 3: send_to_char("Your fellow nobles are trying to outcast you!\n\r",victim);
				break;
	}

	ch->pcdata->dip_points-=cost;

	moot->called_by=ch;
	moot->moot_victim=victim;
	moot->moot_type=type;
	moot->scope=scope;
	moot->votes_for=0;
	moot->votes_against=0;
	moot->pulse=PULSE_MOOT;

	broadcast_moot();

	return ;
}

void resolve_moot(void)
{
	bool passed=FALSE;
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA * d;

	if(moot->moot_type==1)
	{
		if(moot->votes_for>moot->votes_against)
		{
			passed=TRUE;
			gain_exp(moot->moot_victim, moot->scope);
		}
		sprintf(buf, "A moot which %s was called against %s for %d exp.\n\r",
			passed ? "passed" : "failed",
			moot->moot_victim->name,
			moot->scope);
	}
	if(moot->moot_type==2)
	{
		if(100*moot->votes_for/((moot->votes_for+moot->votes_against)+1)>70)
		{
			passed=TRUE;
			moot->moot_victim->pcdata->diplomacy=10;
			moot->called_by->pcdata->diplomacy+=2;
		}
		sprintf(buf, "A moot to induct a new noble has %s.\n\r",
		  passed ? "suceeded" : "failed");
	}
	if(moot->moot_type==3)
	{
		if(100*moot->votes_for/((moot->votes_for+moot->votes_against)+1)>80 &&
                    moot->votes_for>5000)
		{
			passed=TRUE;
			moot->moot_victim->pcdata->diplomacy=0;
			moot->called_by->pcdata->diplomacy+=5;
		}
		else
		{
			moot->called_by->pcdata->diplomacy=
				moot->called_by->pcdata->diplomacy/2-1;
		}
		sprintf(buf, "A moot to outcast a noble has %s.\n\r",
		  passed ? "suceeded" : "failed");
		if(moot->called_by->pcdata->diplomacy<0)
			moot->called_by->pcdata->diplomacy=0;
	}
	for ( d = descriptor_list; d; d = d->next )
	{
	if ( d->connected == CON_PLAYING )
	{
		 send_to_char( buf,   d->character );
		 if(!IS_NPC(d->character))
		 if(d->character->pcdata->diplomacy)
		 {
			if( (d->character->pcdata->in_favor==1 && passed) ||
				 (d->character->pcdata->in_favor==-1 && !passed))
			{
			  if(number_range(1,100)<(d->character->pcdata->votes_cast*50/((
					moot->votes_for+moot->votes_against)+1))
				&&number_range(1,20)<d->character->pcdata->votes_cast)
				d->character->pcdata->diplomacy++;
				if(d->character->pcdata->diplomacy>50)
				 d->character->pcdata->diplomacy=50;
			}
			else
			{
			  if(number_range(1,100)<(d->character->pcdata->votes_cast*100/((
					moot->votes_for+moot->votes_against)+1)))
				d->character->pcdata->diplomacy--;
			}
			d->character->pcdata->in_favor=0;
			d->character->pcdata->votes_cast=0;
		 }
	}
	}

	moot->called_by=NULL;
	moot->moot_victim=NULL;
	moot->moot_type=0;
	moot->scope=0;
	moot->votes_for=0;
	moot->votes_against=0;

	return ;
}

void do_vote(CHAR_DATA *ch, char * argument)
{
	char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
	int votes;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if(IS_NPC(ch))
		return;

	if(moot->moot_type==0)
	{
		send_to_char("There is no moot in progress.\n\r",ch);
		return ;
	}

	if(arg1[0]=='\0' || !is_number(arg2) ||
		!(!str_cmp( arg1, "favor" ) || !str_cmp( arg1, "oppose" )))
	{
		send_to_char("Syntax: vote favor <votes>\n\rSyntax: vote oppose <votes>\n\r",ch);
		return ;
	}

	votes=atoi(arg2);
	if(votes>ch->pcdata->dip_points)
	{
		send_to_char("You don't have that many votes.\n\r",ch);
		return;
	}
	if(votes<=0)
	{
		send_to_char("Ho ho ho.\n\r",ch);
		return ;
	}
	if(ch->pcdata->votes_cast)
	{
		send_to_char("You already voted.\n\r",ch);
		return;
	}

	if(!str_cmp( arg1, "favor" ))
	{
		ch->pcdata->in_favor=1;
		ch->pcdata->dip_points-=votes;
		ch->pcdata->votes_cast=votes;
		moot->votes_for+=votes;
	}
	else
	{
		ch->pcdata->in_favor=-1;
		ch->pcdata->dip_points-=votes;
		ch->pcdata->votes_cast=votes;
		moot->votes_against+=votes;
	}
	send_to_char("Your votes have been cast\n\r",ch);

	return ;
}

void do_dinfo(CHAR_DATA *ch, char * argument)
{
	char buf[MAX_STRING_LENGTH];

	if(IS_NPC(ch))
		return;

	sprintf(buf,"Diplomacy:%d Votes:%d\n\r",
		ch->pcdata->diplomacy,
		ch->pcdata->dip_points);
	send_to_char(buf,ch);
	return ;
}

