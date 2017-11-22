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
#include "magic.h"

void do_channel(CHAR_DATA *ch, char *argument)
{
	 char arg1 [MAX_INPUT_LENGTH];
	 char arg2 [MAX_INPUT_LENGTH];
	 CHAR_DATA *victim;
	 int roll, roll2;

	 argument = one_argument( argument, arg1 );
	 argument = one_argument( argument, arg2 );

	 if (get_skill(ch,gsn_channel) < 1)
	 {
	send_to_char("You have not the power in your soul.\n\r",ch);
	return;
	 }

	 if ( arg1[0] == '\0' )
	 {
		send_to_char("Channel how much?\n\r", ch);
		return ;
	 }

	 if (is_number(arg1) )
	 {
	  int amount;

	  amount = atoi(arg1);

	  if (amount<1)
	  {
	    send_to_char("Yeah right!\n\r",ch);
	    return ;
	  }

	  if(amount>ch->hit)
	  {
		send_to_char("You can not channel more power then you have.\n\r",ch);
		return ;	
	  }
	
	  if(arg2[0] == '\0' ) 
	  {
			ch->hit-=amount;
			update_pos(ch);

			roll=number_percent();
			if(roll<get_skill(ch,gsn_channel))
			{
				ch->mana+=amount;
				send_to_char("You feel physically exhausted.\n\r",ch);
				if(amount>99) check_improve(ch,gsn_channel,TRUE,14);
				return ;
			}
			else
			{
				send_to_char("You feel no gain from your rigorous attempts at power.\n\r",ch);
				if(amount>99) check_improve(ch,gsn_channel,FALSE,14);
				return ;
			}
	  }
	  else
	  {
		roll=number_percent();
		roll2=number_percent();
		if( ( victim = get_char_world(ch, arg2)) == NULL )
		{
		send_to_char("You send your power to nobody.\n\r",ch);
		return ;
		}

		if(amount>ch->mana)
		{
		 send_to_char("You have not enough mana.\n\r", ch);
		 return ;
		}

		if(roll<get_skill(ch,gsn_channel))
		{
		  if(get_skill(victim,gsn_channel)<1)
		  {
			send_to_char("Your target had not the skill to recieve your power.\n\r",ch);
			ch->mana-=amount;
			return ;
		  }
		  if(roll2<get_skill(victim,gsn_channel))
		  {
			ch->mana-=amount;
			victim->mana+= amount*roll2/90;
			send_to_char("You send your power to him.\n\r",ch);
			send_to_char("You recieve magical power from an outer source.\n\r",victim);
			if(amount>99) check_improve(ch,gsn_channel,TRUE,14);
			if(amount>99) check_improve(victim,gsn_channel,TRUE,14);
			return ;
		  }
		  else
		  {
			ch->mana-=amount;
			send_to_char("Your power is recieved by the inept who can not harnass it.\n\r",ch);
			if(amount>99) check_improve(ch,gsn_channel,FALSE,14);
			return ;
		  }
		}
		else
		{
			send_to_char("You failed to send any power.\n\r",ch);
			if(amount>99) check_improve(ch,gsn_channel,TRUE,14);
			return ;
		}
	  }
	 }
	 else
	 {
		send_to_char("You must specify how much to channel in hit points or mana.\n\r",ch);
		return ;
	 }
	 return ;
}

void do_awareness(CHAR_DATA *ch, char *argument)
{

	 if(IS_NPC(ch)) return ;

	 if (get_skill(ch,gsn_awareness) < 1)
	 {
	send_to_char("Your sense are not that acute.\n\r",ch);
	return;
	 }

	 if(ch->pcdata->is_trying_aware)
	 {
		ch->pcdata->is_trying_aware=FALSE;
		send_to_char("You let your guard down.\n\r",ch);
	 }
	 else
	 {
		ch->pcdata->is_trying_aware=TRUE;
		send_to_char("You make yourself more paranoid.\n\r",ch);
	 }

	 return ;
}
