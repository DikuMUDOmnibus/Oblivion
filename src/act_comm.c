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
 **************************************************************************/

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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "color.h"

/* command procedures needed */
DECLARE_DO_FUN(do_quit	);

/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
	 send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];

   if (IS_NPC(ch))
	return;
  
   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("Delete status removed.\n\r",ch);
		 ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
		 sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
		 wiznet("$N turns $Mself into line noise.",ch,NULL,0,0,0);

	    do_quit(ch,"");
	    unlink(strsave);
	    return;
	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("Just type delete. No argument.\n\r",ch);
	return;
    }

    send_to_char("Type delete again to confirm this command.\n\r",ch);
    send_to_char("WARNING: this command is irreversible.\n\r",ch);
	 send_to_char("Typing delete with an argument will undo delete status.\n\r",
	ch);
	 ch->pcdata->confirm_delete = TRUE;
	 wiznet("$N is contemplating deletion.",ch,NULL,0,0,get_trust(ch));
}
	    

/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
	 char buf[MAX_STRING_LENGTH];

	 /* lists all channels and their status */
	 send_to_char("   channel     status\n\r",ch);
	 send_to_char("---------------------\n\r",ch);

	 if(IS_SET(ch->act,PLR_COLOR))
		 sprintf (buf, "%sOOC            ", FG_CYAN);
	 else
		 strcpy (buf,    "OOC            ");
			send_to_char(buf ,ch);

	 if (!IS_SET(ch->comm,COMM_NOGOSSIP))
		send_to_char("ON\n\r",ch);
	 else
		send_to_char("OFF\n\r",ch);

	 if(IS_SET(ch->act,PLR_COLOR))
		 sprintf (buf, "%sQ/A            ", FG_GREEN);
	 else
		 strcpy (buf,    "Q/A            ");
			send_to_char(buf ,ch);

	 if (!IS_SET(ch->comm,COMM_NOQUESTION))
		send_to_char("ON\n\r",ch);
	 else
		send_to_char("OFF\n\r",ch);

	 if (IS_IMMORTAL(ch))
	 {
		if(IS_SET(ch->act,PLR_COLOR))
		 sprintf (buf, "%sgod channel    ", FG_LT_GREEN);
	 else
		 strcpy (buf,    "god_channel    ");
			send_to_char(buf ,ch);


		if(!IS_SET(ch->comm,COMM_NOWIZ))
		  send_to_char("ON\n\r",ch);
		else
		  send_to_char("OFF\n\r",ch);
	 }

	 if (IS_SET(ch->act, PLR_COLOR))
		send_to_char (FG_LT_GRAY, ch);

	 send_to_char("quiet mode     ",ch);
	 if (IS_SET(ch->comm,COMM_QUIET))
		send_to_char("ON\n\r",ch);
	 else
		send_to_char("OFF\n\r",ch);

	 if (IS_SET(ch->comm,COMM_AFK))
	send_to_char("You are AFK.\n\r",ch);

	 if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
	send_to_char("You are immune to snooping.\n\r",ch);

	 if (ch->lines != PAGELEN)
	 {
	if (ch->lines)
	{
		 sprintf(buf,"You display %d lines of scroll.\n\r",ch->lines+2);
		 send_to_char(buf,ch);
	}
	else
		 send_to_char("Scroll buffering is off.\n\r",ch);
	 }

	 if (ch->prompt != NULL)
    {
	sprintf(buf,"Your current prompt is: %s\n\r",ch->prompt);
	send_to_char(buf,ch);
    }

	 if (IS_SET(ch->comm,COMM_NOSHOUT))
      send_to_char("You cannot shout.\n\r",ch);
  
    if (IS_SET(ch->comm,COMM_NOTELL))
		send_to_char("You cannot use tell.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("You cannot use channels.\n\r",ch);

	 if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("You cannot show emotions.\n\r",ch);

}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
	 if (IS_SET(ch->comm,COMM_QUIET))
	 {
		send_to_char("Quiet mode removed.\n\r",ch);
		REMOVE_BIT(ch->comm,COMM_QUIET);
	 }
	else
	{
	  send_to_char("From now on, you will only hear says and emotes.\n\r",ch);
	  SET_BIT(ch->comm,COMM_QUIET);
	}
}

void do_ntalk( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;
	 CHAR_DATA *victim;

	 if (argument[0] == '\0' )
	 {
		send_to_char("What do you wish to ntalk?\n\r",ch);
		return ;
	 }

	 if (IS_SET(ch->comm,COMM_NOCHANNELS))
	 {
		 send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
		 return;
	 }

	if(IS_NPC(ch))
		return;

	sprintf (buf, "%sYou ntalk: %s\n\r",
		IS_SET(ch->act, PLR_COLOR) ? FG_LT_PURPLE : "",
		argument);
	send_to_char(buf ,ch);

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	 victim = d->original ? d->original : d->character;

	 if ( (d->connected == CON_PLAYING) &&
			!IS_NPC(victim) &&
			victim->pcdata->diplomacy>0 &&
			ch!=victim )
	 {
		 if(ch->pcdata->diplomacy)
                 
		 sprintf (buf, "%s<noble talk>: '%s'\n\r",
			 (IS_SET(victim->act,PLR_COLOR)) ? FG_LT_PURPLE : "",
			  argument);
		else
		 sprintf(buf, "%s<%s noble talks>: '%s'\n\r",
                         (IS_SET(victim->act, PLR_COLOR)) ? FG_LT_PURPLE : "",
			 ch->name,
			 argument);		
		 send_to_char(buf,victim);
	 }
	 }

}


void  broadcast_moot(void)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;
	char mt_type[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	switch (moot->moot_type)
	{
	default: strcpy(mt_type,"none"); break;
	case 1: strcpy(mt_type,"reward"); break;
	case 2: strcpy(mt_type,"induction"); break;
	case 3: strcpy(mt_type,"outcasting"); break;
	}

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	 victim = d->original ? d->original : d->character;

	 if ( (d->connected == CON_PLAYING) &&
			!IS_NPC(victim) &&
			victim->pcdata->diplomacy>0 )
	 {
		 sprintf (buf, "%sMoot in progress of type %s called against %s of value:%d will be resolved in %d seconds.%s\n\r",
			 (IS_SET(victim->act,PLR_COLOR)) ? FG_YELLOW : "",
			 mt_type,
			 moot->moot_victim->name,
			 moot->scope,
			 moot->pulse/4,
			 (IS_SET(victim->act,PLR_COLOR)) ? FG_LT_GRAY : "");
		 send_to_char(buf,victim);
	 }
	 }

	return ;
}

/* afk command */

void do_afk ( CHAR_DATA *ch, char * argument)
{
	 if (IS_SET(ch->comm,COMM_AFK))
	 {
		send_to_char("AFK mode removed. Type 'replay' to see tells.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_AFK);
	 }
   else
	{
     send_to_char("You are now in AFK mode.\n\r",ch);
	  SET_BIT(ch->comm,COMM_AFK);
	}
}

void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
	 {
	send_to_char("You can't replay.\n\r",ch);
	return;
	 }

	 page_to_char(buf_string(ch->pcdata->buffer),ch);
	 clear_buf(ch->pcdata->buffer);
}

/* RT chat replaced with ROM gossip */
void do_ooc( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;

	 if (argument[0] == '\0' )
	 {
		if (IS_SET(ch->comm,COMM_NOGOSSIP))
		{
		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sOOC channel is now ON.\n\r", FG_CYAN);
		else
			 strcpy (buf,    "OOC channel is now ON.\n\r");
		send_to_char(buf ,ch);
		  REMOVE_BIT(ch->comm,COMM_NOGOSSIP);
		}
		else
		{
		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sOOC channel is now OFF.\n\r", FG_CYAN);
		else
			 strcpy (buf,    "OOC channel is now OFF.\n\r");
		send_to_char(buf ,ch);
		  SET_BIT(ch->comm,COMM_NOGOSSIP);
		}
	 }
	 else  /* gossip message sent, turn gossip on if it isn't already */
	 {
		  if (IS_SET(ch->comm,COMM_QUIET))
		  {
			 send_to_char("You must turn off quiet mode first.\n\r",ch);
			 return;
		  }

		  if (IS_SET(ch->comm,COMM_NOCHANNELS))
		  {
			 send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
			 return;

			}

		REMOVE_BIT(ch->comm,COMM_NOGOSSIP);

		if(!IS_NPC(ch))
			ch->pcdata->did_ooc=TRUE;

		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%s<You OOC>: '%s'\n\r", FG_CYAN, argument);
		else
			 sprintf (buf,    "<You OOC>: '%s'\n\r", argument);
		send_to_char(buf ,ch);

		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		  CHAR_DATA *victim;

		  victim = d->original ? d->original : d->character;

		  if ( d->connected == CON_PLAYING &&
				 d->character != ch &&
				 !IS_SET(victim->comm,COMM_NOGOSSIP) &&
				 !IS_SET(victim->comm,COMM_QUIET) )
		  {
			if(IS_SET(victim->act, PLR_COLOR))
			  sprintf (buf, "%s<$n OOC>: '$t'%s", FG_CYAN, FG_LT_GRAY);
			else
			  strcpy (buf, "<$n OOC>: '$t'");
			act_new(buf,ch,argument,d->character,TO_VICT,POS_DEAD);
		  }
		}
	 }
	 if(IS_SET(ch->act,PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);
}

void do_pray( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;

	 if (argument[0] == '\0' )
	 {
		send_to_char("Pray What?", ch);
		return ;
	 }

		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sYou pray to any who would listen: '%s'\n\r", FG_LT_PURPLE, argument);
		else
			 sprintf (buf,    "You pray to any who would listen: '%s'\n\r", argument);
		send_to_char(buf ,ch);

		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		  CHAR_DATA *victim;

		  victim = d->original ? d->original : d->character;

		  if ( d->connected == CON_PLAYING &&
				 d->character != ch &&
				 !IS_SET(victim->comm,COMM_NOGRATS) &&
				 !IS_SET(victim->comm,COMM_QUIET) &&
				 ch->in_room==get_room_index(victim->temple))
		  {
			if(!IS_NPC(ch))
				ch->pcdata->did_ic=TRUE;

			if(IS_SET(victim->act, PLR_COLOR))
			  sprintf (buf, "%s$n prays '$t'%s", FG_LT_PURPLE, FG_LT_GRAY);
			else
			  strcpy (buf, "$n prays '$t'");
			act_new(buf,ch,argument,d->character,TO_VICT,POS_DEAD);
		  }
	}
}

/* RT question channel */
void do_question( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;

	 if (argument[0] == '\0' )
	 {
		if (IS_SET(ch->comm,COMM_NOQUESTION))
		{
		  if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sQ/A channel is now ON.\n\r", FG_GREEN);
		  else
			 strcpy (buf,    "Q/A channel is now ON.\n\r");
		  send_to_char(buf ,ch);

		  REMOVE_BIT(ch->comm,COMM_NOQUESTION);
		}
		else
		{
		  if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sQ/A channel is now OFF.\n\r", FG_GREEN);
		  else
			 strcpy (buf,    "Q/A channel is now OFF.\n\r");
		  send_to_char(buf ,ch);

		  SET_BIT(ch->comm,COMM_NOQUESTION);
		}
	 }
	 else  /* question sent, turn Q/A on if it isn't already */
	 {
		  if (IS_SET(ch->comm,COMM_QUIET))
		  {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }

		  if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
			 send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}
 
        REMOVE_BIT(ch->comm,COMM_NOQUESTION);

		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sYou question '%s'\n\r", FG_GREEN, argument);
		else
			 sprintf (buf,    "You question '%s'\n\r", argument);
		send_to_char(buf ,ch);

		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		  CHAR_DATA *victim;

		  victim = d->original ? d->original : d->character;

		  if ( d->connected == CON_PLAYING &&
             d->character != ch &&
				 !IS_SET(victim->comm,COMM_NOQUESTION) &&
				 !IS_SET(victim->comm,COMM_QUIET) )
		  {
		 if(IS_SET(victim->act, PLR_COLOR))
			sprintf (buf, "%s$n questions '$t'%s", FG_GREEN, FG_LT_GRAY);
		 else
			strcpy (buf, "$n questions '$t'");
		 act_new(buf,ch,argument,d->character,TO_VICT,POS_DEAD);
		  }
		}
	 }
		 if(IS_SET(ch->act,PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);
}

/* RT answer channel - uses same line as questions */
void do_answer( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;

	 if (argument[0] == '\0' )
	 {
		if (IS_SET(ch->comm,COMM_NOQUESTION))
		{
		  if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sQ/A channel is now ON.\n\r", FG_GREEN);
		  else
			 strcpy (buf,    "Q/A channel is now ON.\n\r");
		  send_to_char(buf ,ch);

		  REMOVE_BIT(ch->comm,COMM_NOQUESTION);
		}
		else
		{
		  if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sQ/A channel is now OFF.\n\r", FG_GREEN);
		  else
			 strcpy (buf,    "Q/A channel is now OFF.\n\r");
		  send_to_char(buf ,ch);

		  SET_BIT(ch->comm,COMM_NOQUESTION);
		}
	 }
	 else  /* answer sent, turn Q/A on if it isn't already */
	 {
		  if (IS_SET(ch->comm,COMM_QUIET))
		  {
			 send_to_char("You must turn off quiet mode first.\n\r",ch);
			 return;
		  }

		  if (IS_SET(ch->comm,COMM_NOCHANNELS))
		  {
			 send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
			 return;
	}

		  REMOVE_BIT(ch->comm,COMM_NOQUESTION);
		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sYou answer '%s'\n\r", FG_GREEN, argument);
		else
			 sprintf (buf,    "You answer '%s'\n\r", argument);
		send_to_char(buf ,ch);

		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		  CHAR_DATA *victim;

		  victim = d->original ? d->original : d->character;

		  if ( d->connected == CON_PLAYING &&
				 d->character != ch &&
				 !IS_SET(victim->comm,COMM_NOQUESTION) &&
				 !IS_SET(victim->comm,COMM_QUIET) )
		  {
		 if(IS_SET(victim->act, PLR_COLOR))
			sprintf (buf, "%s$n answers '$t'%s", FG_GREEN, FG_LT_GRAY);
		 else
			strcpy (buf, "$n answers '$t'");
		 act_new(buf,ch,argument,d->character,TO_VICT,POS_DEAD);

		  }
		}
	 }
		 if(IS_SET(ch->act,PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 DESCRIPTOR_DATA *d;

	 if ( argument[0] == '\0' )
	 {
		if (IS_SET(ch->comm,COMM_NOWIZ))
		{
	 if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sImmortal channel is now ON.\n\r", FG_LT_GREEN);
		  else
			 strcpy (buf,    "Immortal channel is now ON.\n\r");
		  send_to_char(buf ,ch);

	REMOVE_BIT(ch->comm,COMM_NOWIZ);
		}
		else
		{
	 if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%sImmortal channel is now OFF.\n\r", FG_LT_GREEN);
		  else
			 strcpy (buf,    "Immortal channel is now OFF.\n\r");
		  send_to_char(buf ,ch);

	SET_BIT(ch->comm,COMM_NOWIZ);
		}
		return;
	 }

	 REMOVE_BIT(ch->comm,COMM_NOWIZ);

		if(IS_SET(ch->act,PLR_COLOR))
			 sprintf (buf, "%s $n: $t%s", FG_LT_GREEN, FG_LT_GRAY);
		else
			 strcpy(buf, "$n: $t");

	 act_new(buf,ch,argument,NULL,TO_CHAR,POS_DEAD);
	 for ( d = descriptor_list; d != NULL; d = d->next )
	 {
	if ( d->connected == CON_PLAYING &&
		  IS_IMMORTAL(d->character) &&
				 !IS_SET(d->character->comm,COMM_NOWIZ) )
	{
		if(IS_SET(d->character->act, PLR_COLOR))
			sprintf (buf, "%s $n : $t%s", FG_LT_GREEN, FG_LT_GRAY);
		 else
			strcpy (buf, "$n : $t");
		 act_new(buf,ch,argument,d->character,TO_VICT,POS_DEAD);
	}
	 }
	 if(IS_SET(ch->act,PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);
	 return;
}

void do_say( CHAR_DATA *ch, char *argument )
{
         DESCRIPTOR_DATA *d;
         char buf[MAX_STRING_LENGTH];
 
	 if ( argument[0] == '\0' )
	 {
	send_to_char( "Say what?\n\r", ch );
	return;
   }

	if(!IS_NPC(ch))
		ch->pcdata->did_ic=TRUE;
	if(room_is_private(ch->in_room))
		{ ch->pcdata->did_ooc=TRUE; ch->pcdata->did_ic=FALSE; }

         for ( d = descriptor_list; d != NULL; d = d->next )
         {
        if ( d->connected == CON_PLAYING
        &&   d->character != ch
        &&   d->character->in_room != NULL
        &&   d->character->in_room == ch->in_room
           )
        {
           trans_language(ch,d->character,argument,buf); 
           act("$n says '$t'",ch,buf,d->character,TO_VICT);
        }
         }
 
   

    act( "You say '$T'", ch, NULL, argument, TO_CHAR );
	 return;
}


void do_tell( CHAR_DATA *ch, char *argument )
{
	 char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

	 if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
	 }

	 argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
	 {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

	 /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
	  */
	 if ( ( victim = get_char_world( ch, arg ) ) == NULL
	 || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
	 {
	send_to_char( "They aren't here.\n\r", ch );
	return;
	 }

	if(!IS_IMMORTAL(victim)&&!IS_IMMORTAL(ch))
	{
	  send_to_char( "Tells may only be exchanged with an immortal.\n\r",ch);
	  return ;
	}

	 if ( victim->desc == NULL && !IS_NPC(victim))
	 {
	act("$N seems to have misplaced $S link...try again later.",
		 ch,NULL,victim,TO_CHAR);
		  sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
		  buf[0] = UPPER(buf[0]);
		  add_buf(victim->pcdata->buffer,buf);
	return;
	 }

	 if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
	 && !IS_IMMORTAL(ch))
	 {
	act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );
	return;
	 }

    if (IS_SET(victim->comm,COMM_AFK))
	 {
	if (IS_NPC(victim))
	{
	    act("$E is AFK, and not receiving tells.",ch,NULL,victim,TO_CHAR);
	    return;
	}

	act("$E is AFK, but your tell will go through when $E returns.",
		 ch,NULL,victim,TO_CHAR);
	sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
	 }

			if(!IS_NPC(ch))
			ch->pcdata->did_ooc=TRUE;
	 if (IS_SET (ch->act, PLR_COLOR))
		send_to_char(FG_LT_CYAN, ch);
	 sprintf( buf, "You tell %s '%s'\n\r", victim->name, argument);
	 send_to_char(buf, ch);
	 if (IS_SET (ch->act, PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);

	 if (IS_SET (victim->act, PLR_COLOR))
		sprintf(buf,"%s%s tells you '$t'%s", FG_LT_CYAN, ch->name, FG_LT_GRAY);
	 else
		sprintf(buf,"%s tells you '$t'", ch->name);
	 act_new(buf,ch,argument,victim,TO_VICT,POS_DEAD);
	 victim->reply	= ch;

	 return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
	 CHAR_DATA *victim;
	 char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->comm, COMM_NOTELL) )
	 {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
	 }

	 if ( victim->desc == NULL && !IS_NPC(victim))
	 {
		  act("$N seems to have misplaced $S link...try again later.",
				ch,NULL,victim,TO_CHAR);
		  sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
		  buf[0] = UPPER(buf[0]);
		  add_buf(victim->pcdata->buffer,buf);
		  return;
	 }


	 if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
	 &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
	 {
		  act_new( "$E is not receiving tells.", ch, 0, victim, TO_CHAR,POS_DEAD);
		  return;
	 }


    if (IS_SET(victim->comm,COMM_AFK))
    {
        if (IS_NPC(victim))
		  {
				act_new("$E is AFK, and not receiving tells.",
		ch,NULL,victim,TO_CHAR,POS_DEAD);
            return;
        }

		  act_new("$E is AFK, but your tell will go through when $E returns.",
				ch,NULL,victim,TO_CHAR,POS_DEAD);
		  sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
		  add_buf(victim->pcdata->buffer,buf);
		  return;
	 }
	if(!IS_NPC(ch))
           ch->pcdata->did_ooc=TRUE;
        if(IS_SET(ch->act, PLR_COLOR))
	send_to_char(FG_LT_PURPLE, ch);
        sprintf(buf, "You tell %s '%s'\n\r", victim->name, argument);
        send_to_char(buf,ch);
	 if (IS_SET (ch->act, PLR_COLOR))
		send_to_char(FG_LT_GRAY, ch);

	 if (IS_SET (victim->act, PLR_COLOR))
		sprintf(buf,"%s%s tells you '$t'%s", FG_LT_PURPLE, ch->name, FG_LT_GRAY);
	 else
		sprintf(buf,"%s tells you '$t'", ch->name);
	 act_new(buf,ch,argument,victim,TO_VICT,POS_DEAD);
	 victim->reply	= ch;

	 return;
}

void do_yell( CHAR_DATA *ch, char *argument )
{
	 DESCRIPTOR_DATA *d;
         char buf[MAX_INPUT_LENGTH];

	 if ( IS_SET(ch->comm, COMM_NOSHOUT) )
	 {
		  send_to_char( "You can't yell.\n\r", ch );
		  return;
	 }

	 if ( argument[0] == '\0' )
	 {
	send_to_char( "Yell what?\n\r", ch );
	return;
	 }

	 		if(!IS_NPC(ch))
			ch->pcdata->did_ic=TRUE;

	 act("You yell '$t'",ch,argument,NULL,TO_CHAR);
	 for ( d = descriptor_list; d != NULL; d = d->next )
	 {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area 
	   )
	
        {
           trans_language(ch,d->character,argument,buf);
	    act("$n yells '$t'",ch,buf,d->character,TO_VICT);
	}
	 }

    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{
	 if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
	 {
		  send_to_char( "You can't show your emotions.\n\r", ch );
		  return;
	 }

	 if ( argument[0] == '\0' )
	 {
		  send_to_char( "Emote what?\n\r", ch );
		  return;
	 }

			if(!IS_NPC(ch))
			ch->pcdata->did_ic=TRUE;
       if(room_is_private(ch->in_room))
               { ch->pcdata->did_ooc=TRUE; ch->pcdata->did_ic=FALSE; }

	 act( "$n $T", ch, NULL, argument, TO_ROOM );
	 act( "$n $T", ch, NULL, argument, TO_CHAR );
	 return;
}


void do_pmote( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    int matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
	 {
        send_to_char( "You can't show your emotions.\n\r", ch );
        return;
	 }

	 if ( argument[0] == '\0' )
	 {
		  send_to_char( "Emote what?\n\r", ch );
		  return;
    }
 
    act( "$n $t", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
		 continue;

	if ((letter = strstr(argument,vch->name)) == NULL)
	{
		 act("$N $t",vch,argument,ch,TO_CHAR);
		 continue;
	}

	strcpy(temp,argument);
	temp[strlen(argument) - strlen(letter)] = '\0';
		last[0] = '\0';
	name = vch->name;

	for (; *letter != '\0'; letter++)
	{ 
	    if (*letter == '\'' && matches == strlen(vch->name))
	    {
		strcat(temp,"r");
		continue;
	    }

	    if (*letter == 's' && matches == strlen(vch->name))
		 {
		matches = 0;
		continue;
	    }
	    
		 if (matches == strlen(vch->name))
	    {
		matches = 0;
		 }

	    if (*letter == *name)
	    {
		matches++;
		name++;
		if (matches == strlen(vch->name))
		{
		    strcat(temp,"you");
		    last[0] = '\0';
		    name = vch->name;
		    continue;
		}
		strncat(last,letter,1);
		continue;
	    }

		 matches = 0;
	    strcat(temp,last);
	    strncat(temp,letter,1);
	    last[0] = '\0';
		 name = vch->name;
	}

	act("$N $t",vch,temp,ch,TO_CHAR);
	 }

			if(!IS_NPC(ch))
			ch->pcdata->did_ic=TRUE;
       if(room_is_private(ch->in_room))
               { ch->pcdata->did_ooc=TRUE; ch->pcdata->did_ic=FALSE; }

	 return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
	 append_file( ch, BUG_FILE, argument );
	 send_to_char( "Bug logged.\n\r", ch );
	 return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
	 append_file( ch, TYPO_FILE, argument );
	 send_to_char( "Typo logged.\n\r", ch );
	 return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}



void do_quit( CHAR_DATA *ch, char *argument )
{
	 DESCRIPTOR_DATA *d,*d_next;
    int id;

	 if ( IS_NPC(ch) )
	return;

	 if ( ch->position == POS_FIGHTING )
	 {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
	 }

	 if(ch->pcdata->diplomacy && moot->moot_type)
	 {
		send_to_char("You can not quit while a moot is in progress.\n\r",ch);
		return ;
	 }

	 if(moot->moot_victim==ch)
	 {
		send_to_char("You can not quit while a moot is called against you.\n\r",ch);
		return ;
	 }

	 if(ch->pknorecall>0)
	 {
		send_to_char( "You may not leave this world so soon after conflict.\n\r", ch);
		return ;
	 }

	 if ( ch->position  < POS_STUNNED  )
	 {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
	 }

	 send_to_char(
	"Alas, all good things must come to an end.\n\r",ch);
	 act( "$n has left the game.", ch, NULL, NULL, TO_ROOM );
	 sprintf( log_buf, "%s has quit.", ch->name );
	 log_string( log_buf );
	  wiznet("$N rejoins the real world.",ch,NULL,WIZ_LOGINS,0,get_trust(ch));

	 /*
     * After extract_char the ch is no longer valid!
     */
	 save_char_obj( ch );
    id = ch->id;
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
	close_socket( d );

    /* toast evil cheating bastards */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char(tch,TRUE);
		 close_socket(d);
	} 
    }

    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
	 if ( IS_NPC(ch) )
	return;

	 save_char_obj( ch );
	 send_to_char("Saving. Remember that Oblivion has automatic saving now\n\r", ch);
	 WAIT_STATE(ch,4 * PULSE_VIOLENCE);
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Follow whom?\n\r", ch );
	return;
	 }

	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
	act("$N doesn't seem to want any followers.\n\r",
             ch,NULL,victim, TO_CHAR);
		  return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    
    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
	 return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
	 {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

	 if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

	 if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
		act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
	 }
	 if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{    
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
		stop_follower(pet);
		if (pet->in_room != NULL)
			 act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
		extract_char(pet,TRUE);
	 }
	 ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
	stop_follower( ch );
    }

	 ch->leader = NULL;

	 for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
		 stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
	 CHAR_DATA *victim;
	 CHAR_DATA *och;
	 CHAR_DATA *och_next;
	 bool found;
	 bool fAll;

	 argument = one_argument( argument, arg );
	 one_argument(argument,arg2);

	 if (!str_cmp(arg2,"delete") && ch->level!=MAX_LEVEL)
	 {
		  send_to_char("That will NOT be done.\n\r",ch);
		  return;
	 }

	 if ( arg[0] == '\0' || argument[0] == '\0' )
	 {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
	 }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
	 }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
		 send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch 
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    sprintf( buf, "$n orders you to '%s'.", argument );
	    act( buf, ch, NULL, och, TO_VICT );
		 interpret( och, argument );
	}
	 }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	send_to_char( "Ok.\n\r", ch );
    }
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

	 one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
		 {
		sprintf( buf,
		"[%2d %s] %-16s %4d/%4d hp %4d/%4d mana %4d/%4d mv %5d xp\n\r",
			 gch->level,
		    IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
		    capitalize( PERS(gch, ch) ),
			 gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
	 }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
	 }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }
    
    if (IS_AFFECTED(victim,AFF_CHARM))
    {
        send_to_char("You can't remove charmed mobs from your group.\n\r",ch);
        return;
    }
    
	 if (IS_AFFECTED(ch,AFF_CHARM))
    {
    	act("You like your master too much to leave $m!",ch,NULL,victim,TO_VICT);
    	return;
    }

	 if (victim->level - ch->level > 8)
    {
	send_to_char("They are to high of a level for your group.\n\r",ch);
	return;
    }

    if (victim->level - ch->level < -8)
    {
	send_to_char("They are to low of a level for your group.\n\r",ch);
	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
	return;
    }

    victim->leader = ch;
    act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
	 act( "You join $n's group.", ch, NULL, victim, TO_VICT    );
    act( "$N joins your group.", ch, NULL, victim, TO_CHAR    );
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0, amount_silver = 0;
	 int share_gold, share_silver;
    int extra_gold, extra_silver;

    argument = one_argument( argument, arg1 );
			 one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }
    
    amount_silver = atoi( arg1 );

	 if (arg2[0] != '\0')
	amount_gold = atoi(arg2);

    if ( amount_gold < 0 || amount_silver < 0)
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount_gold == 0 && amount_silver == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold || ch->silver < amount_silver)
    {
	send_to_char( "You don't have that much to split.\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	 {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
	 }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
	 }
	    
    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

	 if ( share_gold == 0 && share_silver == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

	 ch->silver	-= amount_silver;
	 ch->silver	+= share_silver + extra_silver;
    ch->gold 	-= amount_gold;
    ch->gold 	+= share_gold + extra_gold;

    if (share_silver > 0)
    {
	sprintf(buf,
	    "You split %d silver coins. Your share is %d silver.\n\r",
 	    amount_silver,share_silver + extra_silver);
	send_to_char(buf,ch);
    }

    if (share_gold > 0)
	 {
	sprintf(buf,
	    "You split %d gold coins. Your share is %d gold.\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

    if (share_gold == 0)
    {
	sprintf(buf,"$n splits %d silver coins. Your share is %d silver.",
		amount_silver,share_silver);
	 }
    else if (share_silver == 0)
    {
	sprintf(buf,"$n splits %d gold coins. Your share is %d gold.",
		amount_gold,share_gold);
    }
    else
    {
	sprintf(buf,
"$n splits %d silver and %d gold coins, giving you %d silver and %d gold.\n\r",
	 amount_silver,amount_gold,share_silver,share_gold);
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

	 /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
	 sprintf( buf, "%s tells the group '%s'\n\r", ch->name, argument );
	 for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	    send_to_char( buf, gch );
    }

	 return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
	 if ( bch->leader != NULL ) bch = bch->leader;
	 return ach == bch;
}

