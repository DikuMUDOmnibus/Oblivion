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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "tables.h"

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_recall	);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_scan		);

char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down", "northeast", "southeast",
    "southwest", "northwest"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4, 8, 9, 6, 7
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );



void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;

	 if ( door < 0 || door > MAX_DOORS )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }

    if (IS_SET(pexit->exit_info, EX_CLOSED)
    &&  (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
    &&   !IS_TRUSTED(ch,ANGEL))
    {
	act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	return;
	 }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
	 }

    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) )
    {
	int iClass, iGuild;
	int move;

	int IS_ALLOWED;
	IS_ALLOWED=1;
	
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)	
	    {
	    	if ( iClass != ch->class
			&&   to_room->vnum == class_table[iClass].guild[iGuild]
		&&   IS_ALLOWED!=2	 )
	    	{
		    IS_ALLOWED=0;
		}
		if ( iClass == ch->class
		&&   to_room->vnum == class_table[iClass].guild[iGuild])
		    IS_ALLOWED=2;
	    }
	}
	if (IS_ALLOWED==0)
	{
		send_to_char("You can't go in there, tis not your guild.\n\r", ch);
		return;
	}

	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
	    if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
	    {
		send_to_char( "You can't fly.\n\r", ch );
		return;
	    }
	}

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
  	&&    !IS_AFFECTED(ch,AFF_FLYING))
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
		  * Look for a boat.
	     */
	    found = FALSE;

	    if (IS_IMMORTAL(ch))
		found = TRUE;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
		 {
		if ( obj->item_type == ITEM_BOAT )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
	    {
		send_to_char( "You need a boat to go there.\n\r", ch );
		return;
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

		  move /= 2;  /* i.e. the average */


	/* conditional effects */
	if (IS_AFFECTED(ch,AFF_FLYING) || IS_AFFECTED(ch,AFF_HASTE))
	    move /= 2;

	if (IS_AFFECTED(ch,AFF_SLOW))
	    move *= 2;

	if ( ch->move < move )
	{
	    send_to_char( "You are too exhausted.\n\r", ch );
	    return;
	}

	WAIT_STATE( ch, 2 );
	ch->move -= move;
	 }

	 if ( !IS_AFFECTED(ch, AFF_SNEAK)
	 &&   ch->invis_level < LEVEL_HERO)
	act( "$n leaves $T.", ch, NULL, dir_name[door], TO_ROOM );

	 char_from_room( ch );
	 char_to_room( ch, to_room );

	 if ( !IS_AFFECTED(ch, AFF_SNEAK)
	 &&   ch->invis_level < LEVEL_HERO)
	act( "$n has arrived.", ch, NULL, NULL, TO_ROOM );

	 do_look( ch, "auto" );

	 if (in_room == to_room) /* no circular follows */
	return;

	for(fch=to_room->people; fch != NULL; fch=fch_next)
	{
	fch_next = fch->next_in_room;
	if ( !IS_NPC(fch) )
        {
                if( fch->pcdata->is_trying_aware &&
                         number_percent()<get_skill(fch, gsn_awareness) &&
                         fch->position==POS_SLEEPING &&
                         !IS_AFFECTED(fch,AFF_SLEEP))
                {
                        fch->position=POS_RESTING;
                        send_to_char("You are awakened suddenly by a presence.\n\r",fch);
                        do_stand(fch,"");
                        check_improve(ch,gsn_awareness,TRUE,14);
                }
        }

	}

	 for ( fch = in_room->people; fch != NULL; fch = fch_next )
	 {
	fch_next = fch->next_in_room;

	if ( !IS_NPC(fch) )
	{
		if( fch->pcdata->is_trying_aware &&
			 number_percent()<get_skill(fch, gsn_awareness) &&
			 fch->position==POS_SLEEPING &&
			 !IS_AFFECTED(fch,AFF_SLEEP))
		{
			fch->position=POS_RESTING;
			send_to_char("You are awakened suddenly by a presence.\n\r",fch);
			do_stand(fch,"");
			check_improve(ch,gsn_awareness,TRUE,14);
		}
	}

	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
	&&   fch->position < POS_STANDING)
		 do_stand(fch,"");

	if ( fch->master == ch && fch->position == POS_STANDING
	&&   can_see_room(fch,to_room))
	{

		 if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
		 &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
		 {
		act("You can't bring $N into the city.",
			 ch,NULL,fch,TO_CHAR);
		act("You aren't allowed in the city.",
			 fch,NULL,NULL,TO_CHAR);
		continue;
		 }

		 act( "You follow $N.", fch, NULL, ch, TO_CHAR );
		 move_char( fch, door, TRUE );
	}
	 }

	 return;
}

void do_northeast( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTHEAST, FALSE );
    return;
}

void do_southeast( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTHEAST, FALSE );
    return;
}

void do_southwest( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTHWEST, FALSE );
    return;
}

void do_northwest( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTHWEST, FALSE );
    return;
}

void do_north( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTH, FALSE );
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_EAST, FALSE );
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_WEST, FALSE );
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_UP, FALSE );
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_DOWN, FALSE );
    return;
}



int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else if ( !str_cmp( arg, "ne" ) || !str_cmp( arg, "northeast" ) ) door = 6;
    else if ( !str_cmp( arg, "se" ) || !str_cmp( arg, "southeast" ) ) door = 7;
    else if ( !str_cmp( arg, "sw" ) || !str_cmp( arg, "southwest" ) ) door = 8;
    else if ( !str_cmp( arg, "nw" ) || !str_cmp( arg, "northwest" ) ) door = 9;	
    else
    {
	for ( door = 0; door <= MAX_DOORS; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   pexit->exit_info>0
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( pexit->exit_info==0 )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}



void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char("It's already open.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char("It's locked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act("You open $p.",ch,obj,NULL,TO_CHAR);
	    act("$n opens $p.",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	act("You open $p.",ch,obj,NULL,TO_CHAR);
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
		 { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
		 { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
		 CHAR_DATA *rch;

		 REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
		 for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

	 one_argument( argument, arg );

	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Close what?\n\r", ch );
	return;
	 }

	 if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
	 {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{

		 if (!IS_SET(obj->value[1],EX_ISDOOR)
		 ||   IS_SET(obj->value[1],EX_NOCLOSE))
		 {
		send_to_char("You can't do that.\n\r",ch);
		return;
		 }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's already closed.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
	    act("You close $p.",ch,obj,NULL,TO_CHAR);
	    act("$n closes $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	act("You close $p.",ch,obj,NULL,TO_CHAR);
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
	 }

	 if ( ( door = find_door( ch, arg ) ) >= 0 )
	 {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
		 { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if(IS_SET(pexit->exit_info, EX_NOCLOSE) )
		{ send_to_char("That door may not be closed once opened.\n\r", ch); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
		 CHAR_DATA *rch;

		 SET_BIT(pexit_rev->exit_info, EX_CLOSED);
		 for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Lock what?\n\r", ch );
	return;
	 }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||  IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
	 	return;
		 }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char("It can't be locked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already locked.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
	    act("You lock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n locks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
		 { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	act("You lock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
		 { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 || IS_SET(pexit->exit_info, EX_NOLOCK) )
		 { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
		 { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
		 { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
		 SET_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
	 }

	 return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already unlocked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You unlock $p.",ch,obj,NULL,TO_CHAR);
		 act("$n unlocks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
		 { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
		 { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
		 { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
		 { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
		 SET_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
	 }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
	 OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Pick what?\n\r", ch );
	return;
	 }

	 WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

	 /* look for guards */
	 for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	 {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
		 act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
		 return;
	}
	 }

	 if ( number_percent( ) > get_skill(ch,gsn_pick_lock) || IS_NPC(ch))
	 {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
	 }

	 if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
	 {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {	
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
		 {
		send_to_char("You failed.\n\r",ch);
		return;
		 }

		 REMOVE_BIT(obj->value[1],EX_LOCKED);
		 act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
		 act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
		 check_improve(ch,gsn_pick_lock,TRUE,2);
		 return;
	}




	
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
		 { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	check_improve(ch,gsn_pick_lock,TRUE,2);
	return;
    }

	 if ( ( door = find_door( ch, arg ) ) >= 0 )
	 {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info,EX_CLOSED) )
		 { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
		 { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
		 { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if (
	  (IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	  || (IS_SET(pexit->exit_info, EX_HARD) && number_range(1,4)!=1)
	  || (IS_SET(pexit->exit_info, EX_INFURIATING) && number_range(1,20)!=1)
	  )
		 { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	check_improve(ch,gsn_pick_lock,TRUE,2);

	/* pick the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
		 REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
	    send_to_char("Maybe you should finish fighting first?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
		 send_to_char("You can't seem to find a place to stand.\n\r",ch);
		 return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
		 act_new("There's no room to stand on $p.",
		ch,obj,NULL,TO_ROOM,POS_DEAD);
		 return;
	}
	 }

	 switch ( ch->position )
	 {
	 case POS_SLEEPING:
		send_to_char( "You can't wake up!\n\r", ch ); return;

	 case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
		 send_to_char( "You stand up.\n\r", ch );
		 act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
		 ch->on = NULL;
		 if(!IS_NPC(ch))
			ch->pcdata->is_trying_sleep=FALSE;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
		 act("You stand at $p.",ch,obj,NULL,TO_CHAR);
		 act("$n stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
		 act("You stand on $p.",ch,obj,NULL,TO_CHAR);
		 act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
		 act("You stand in $p.",ch,obj,NULL,TO_CHAR);
		 act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

	 case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	if(!IS_NPC(ch))
		ch->pcdata->is_trying_sleep=FALSE;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

	 return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("You are already fighting!\n\r",ch);
	return;
	 }

    /* okay, now that we know we can rest, find an object to rest on */
	 if (argument[0] != '\0')
	 {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	 }
    else obj = ch->on;

    if (obj != NULL)
    {
        if (!IS_SET(obj->item_type,ITEM_FURNITURE) 
    	||  (!IS_SET(obj->value[2],REST_ON)
    	&&   !IS_SET(obj->value[2],REST_IN)
    	&&   !IS_SET(obj->value[2],REST_AT)))
		{
	    send_to_char("You can't rest on that.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
		 return;
    	}
	
	ch->on = obj;
    }

    switch ( ch->position )
	 {
	 case POS_SLEEPING:
	 send_to_char( "You can't wake yourself up!!\n\r", ch);
	break;

	 case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

	 case POS_STANDING:
	if (obj == NULL)
	{
	    send_to_char( "You rest.\n\r", ch );
	    act( "$n sits down and rests.", ch, NULL, NULL, TO_ROOM );
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You sit down at $p and rest.",ch,obj,NULL,TO_CHAR);
		 act("$n sits down at $p and rests.",ch,obj,NULL,TO_ROOM);
		  }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
        }
		  else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
	if (obj == NULL)
	{
		 send_to_char("You rest.\n\r",ch);
	    act("$n rests.",ch,NULL,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You rest at $p.",ch,obj,NULL,TO_CHAR);
		 act("$n rests at $p.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You rest on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests on $p.",ch,obj,NULL,TO_ROOM);
        }
		  else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_RESTING;
	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("Maybe you should finish this fight first?\n\r",ch);
	return;
    }

	 /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	 }
    else obj = ch->on;

    if (obj != NULL)                                                              
    {
	if (!IS_SET(obj->item_type,ITEM_FURNITURE)
	||  (!IS_SET(obj->value[2],SIT_ON)
	&&   !IS_SET(obj->value[2],SIT_IN)
	&&   !IS_SET(obj->value[2],SIT_AT)))
	{
	    send_to_char("You can't sit on that.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	ch->on = obj;
    }
	 switch (ch->position)
    {
	case POS_SLEEPING:
		 send_to_char("You must wake up first.\n\r",ch);
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char("You stop resting.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits at $p.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SITTING;
		 break;
	case POS_SITTING:
	    send_to_char("You are already sitting down.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char("You sit down.\n\r",ch);
    	        act("$n sits down on the ground.",ch,NULL,NULL,TO_ROOM);
		 }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit down at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down at $p.",ch,obj,NULL,TO_ROOM);
		 }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You sit down in $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down in $p.",ch,obj,NULL,TO_ROOM);
		 }
			 ch->position = POS_SITTING;
    	    break;
    }
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
	 OBJ_DATA *obj = NULL;

	if(IS_NPC(ch))
	{
	  ch->position=POS_SLEEPING;
	  return ;	
	}

	 switch ( ch->position )
	 {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING: 
	if (argument[0] == '\0' && ch->on == NULL)
	{
		 send_to_char( "You attempt to go to sleep.\n\r", ch );
		 if(!IS_NPC(ch))
		 {
			ch->pcdata->is_trying_sleep=TRUE;
			ch->position=POS_RESTING;
		 }
	}
	else  /* find an object and sleep on it */
	{
		 if (argument[0] == '\0')
		obj = ch->on;
		 else
			obj = get_obj_list( ch, argument,  ch->in_room->contents );

		 if (obj == NULL)
		 {
		send_to_char("You don't see that here.\n\r",ch);
		return;
		 }
		 if (obj->item_type != ITEM_FURNITURE
		 ||  (!IS_SET(obj->value[2],SLEEP_ON)
		 &&   !IS_SET(obj->value[2],SLEEP_IN)
		 &&	 !IS_SET(obj->value[2],SLEEP_AT)))
		 {
		send_to_char("You can't sleep on that!\n\r",ch);
		return;
		 }

		 if (ch->on != obj && count_users(obj) >= obj->value[0])
		 {
		act_new("There is no room on $p for you.",
			 ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
		 }

		 ch->on = obj;
		 if (IS_SET(obj->value[2],SLEEP_AT))
		 {
		act("You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act("$n tries to sleep at $p.",ch,obj,NULL,TO_ROOM);
		 }
		 else if (IS_SET(obj->value[2],SLEEP_ON))
		 {
			  act("You try to go to sleep on $p.",ch,obj,NULL,TO_CHAR);
			  act("$n tries to go to sleep on $p.",ch,obj,NULL,TO_ROOM);
		 }
		 else
		 {
		act("You try to go to sleep in $p.",ch,obj,NULL,TO_CHAR);
		act("$n tries to go to sleep in $p.",ch,obj,NULL,TO_ROOM);
		 }
		 ch->position = POS_RESTING;
		 if(!IS_NPC(ch))
		 {
			 ch->pcdata->is_trying_sleep=TRUE;
			 ch->position=POS_RESTING;
		 }
	}
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
	 char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
	 if ( arg[0] == '\0' )
	 {
		if(!IS_NPC(ch))
	        {
		if(ch->pcdata->tired>30 || IS_AFFECTED(ch,AFF_SLEEP) )
		{
		send_to_char("You cannot wake yourself.\n\r",ch);
		}
		else
		{
			ch->position=POS_RESTING;
			do_stand(ch,"");
		}
		}
		else
		{
		   ch->position=POS_RESTING;
		   do_stand(ch,"");
		}	
		 return;
	 }

	 if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

	 if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

	 if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

	 act_new( "$n wakes you.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
	 victim->position=POS_RESTING;
	 do_stand(victim,"");
	 return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
	 AFFECT_DATA af;

	 send_to_char( "You attempt to move silently.\n\r", ch );
	 affect_strip( ch, gsn_sneak );

	 if (IS_AFFECTED(ch,AFF_SNEAK))
	return;

	 if ( number_percent( ) < get_skill(ch,gsn_sneak))
	 {
	check_improve(ch,gsn_sneak,TRUE,3);
	af.where     = TO_AFFECTS;
	af.type      = gsn_sneak;
	af.level     = ch->level;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
	 }
	 else
	check_improve(ch,gsn_sneak,FALSE,3);

	 return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{
	 send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( number_percent( ) < get_skill(ch,gsn_hide))
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    }
    else
	check_improve(ch,gsn_hide,FALSE,3);

    return;
}

/* START WWW */
void do_sscan( CHAR_DATA *ch, char *argument )
{
	 if ( number_percent( ) < get_skill(ch,gsn_scan))
    {
	do_scan( ch, argument);
	check_improve(ch,gsn_scan,TRUE,1);
    }
    else
    {
	check_improve(ch,gsn_hide,FALSE,1);
	send_to_char("Your scanning is insufficient.\n\r",ch);
	 }
	return;
}
/* END WWW */

/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_invis			);
    affect_strip ( ch, gsn_mass_invis			);
	 affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char( "Ok.\n\r", ch );
	 return;
}



void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    static int pvnum;

    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }

	 act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );

    if ( ( location = get_room_index( clan_table[ch->clan].recall_room ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
	 }

	 if (ch->pknorecall>0)
	 {
		send_to_char("You may not recall so soon after conflict.\n\r", ch);
		return ;
	 }

	 if(IS_NPC(ch))
	   location = get_room_index(pvnum);

	 if ( ch->in_room == location )
	return;

	 if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
	 ||   IS_AFFECTED(ch, AFF_CURSE))
	 {
	send_to_char( "Mota has forsaken you.\n\r", ch );
	return;
	 }

	 if ( ( victim = ch->fighting ) != NULL )
	 {
	int lose,skill;

	skill = get_skill(ch,gsn_recall);

	if ( number_percent() < 80 * skill / 100 )
	{
		 check_improve(ch,gsn_recall,FALSE,6);
		 WAIT_STATE( ch, 4 );
		 sprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = (ch->desc != NULL) ? 25 : 50;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );

    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
	 char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    
    if (ch->pet != NULL)
    {
	pvnum = location->vnum;
	do_recall(ch->pet,"");
    }

    return;
}



void do_train( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
	 char *pOutput = NULL;
	 int cost, amount;

    if ( IS_NPC(ch) )
	return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
	if ( IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN) )
	    break;
    }

	 if ( mob == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "You have %d training sessions.\n\r", ch->train );
	send_to_char( buf, ch );
	argument = "foo";
    }

    cost = 1;

    if ( !str_cmp( argument, "st" ) )
    {
	stat        = STAT_ST;
	pOutput     = "strength";
    } else

    if ( !str_cmp( argument, "qu" ) )
    {
        stat        = STAT_QU;
        pOutput     = "quickness";
    } else

    if ( !str_cmp( argument, "pr" ) )
    {
        stat        = STAT_PR;
        pOutput     = "presence";
    } else

    if ( !str_cmp( argument, "em" ) )
    {
        stat        = STAT_EM;
        pOutput     = "empathy";
    } else

    if ( !str_cmp( argument, "in" ) )
    {
        stat        = STAT_IN;
        pOutput     = "intuition";
    } else

    if ( !str_cmp( argument, "co" ) )
    {
        stat        = STAT_CO;
        pOutput     = "constitution";
    } else
    if ( !str_cmp( argument, "ag" ) )
    {
        stat        = STAT_AG;
        pOutput     = "agility";
    } else
    if ( !str_cmp( argument, "sd" ) )
    {
        stat        = STAT_SD;
        pOutput     = "self-discipline";
    } else
    if ( !str_cmp( argument, "me" ) )
    {
        stat        = STAT_ME;
        pOutput     = "memory";
    } else
    if ( !str_cmp( argument, "re" ) )
    {
        stat        = STAT_RE;
        pOutput     = "reasoning";
    } else


     if ( !str_cmp(argument, "hp" ) )
	cost = 1;

	 else if ( !str_cmp(argument, "mana" ) )
	cost = 1;

    else
    {
	strcpy( buf, "You can train:" );
	if ( ch->perm_stats[STAT_ST] < ch->potential_stats[STAT_ST] )  
	    strcat( buf, " st" );

        if ( ch->perm_stats[STAT_QU] < ch->potential_stats[STAT_QU] )
            strcat( buf, " qu" );
 
        if ( ch->perm_stats[STAT_PR] < ch->potential_stats[STAT_PR] )
            strcat( buf, " pr" );

        if ( ch->perm_stats[STAT_EM] < ch->potential_stats[STAT_EM] )
            strcat( buf, " em" );

        if ( ch->perm_stats[STAT_IN] < ch->potential_stats[STAT_IN] )
            strcat( buf, " in" );

        if ( ch->perm_stats[STAT_CO] < ch->potential_stats[STAT_CO] )
            strcat( buf, " co" );

        if ( ch->perm_stats[STAT_AG] < ch->potential_stats[STAT_AG] )
            strcat( buf, " ag" );

        if ( ch->perm_stats[STAT_SD] < ch->potential_stats[STAT_SD] )
            strcat( buf, " sd" );

        if ( ch->perm_stats[STAT_ME] < ch->potential_stats[STAT_ME] )
            strcat( buf, " me" );

        if ( ch->perm_stats[STAT_RE] < ch->potential_stats[STAT_RE] )
            strcat( buf, " re" );

        if ( ch->pcdata->perm_hit < pc_race_table[ch->race].max_hp )
            strcat( buf, " hp" );

	strcat( buf, " mana");

	    strcat( buf, ".\n\r" );
	    send_to_char( buf, ch );

	return;
    }

    if (!str_cmp("hp",argument))
    {
    	if ( cost > ch->train )
    	{
       	    send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }
 
        if(ch->pcdata->perm_hit>pc_race_table[ch->race].max_hp)
	{
           send_to_char( "Your hps are already maxed.\n\r",ch);
 	   return ;
	}

	ch->train -= cost;

	amount = number_range(class_table[ch->class].hp_min,
                                             class_table[ch->class].hp_max);
        ch->pcdata->perm_hit += amount;
        ch->max_hit += amount;
        ch->hit += amount;
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
	 }
 
    if (!str_cmp("mana",argument))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
		  act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( ch->perm_stats[stat]  >= ch->potential_stats[stat] )
    {
	act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost > ch->train )
    {
	send_to_char( "You don't have enough training sessions.\n\r", ch );
	return;
    }

    ch->train		-= cost;
  
    amount = ch->potential_stats[stat] - ch->perm_stats[stat];
    amount = amount * number_range(1,100);
    amount = amount / 100; 
    if(amount<1) amount=1 ;   
    if(amount>9) amount=9 ;
    ch->perm_stats[stat] += amount;
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    reset_char(ch);
    return;
}

void do_walk( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	int count;
	int i;

	argument = one_argument( argument, arg );

	if(arg[0]=='\0')
	{
		send_to_char("Where do you want to walk?",ch);
		return ;
	}

	while(arg[0]!='\0')
	{
		if(is_number(arg))
		{
			count = atoi(arg);
			argument = one_argument( argument, arg );
		}
		else
			count=1;

		switch (arg[0])
		{
		default : send_to_char("Your walking makes no sense.",ch); return ;
		case 'w':
		case 'W': for(i=0; i<count; i++) interpret(ch, "west"); break;
		case 's':
		case 'S': for(i=0; i<count; i++) interpret(ch, "south"); break;
		case 'e':
		case 'E': for(i=0; i<count; i++) interpret(ch, "east"); break;
		case 'n':
		case 'N': for(i=0; i<count; i++) interpret(ch, "north"); break;
		case 'u':
		case 'U': for(i=0; i<count; i++) interpret(ch, "up"); break;
		case 'd':
		case 'D': for(i=0; i<count; i++) interpret(ch, "down"); break;
		}

		argument = one_argument( argument, arg );
	}

	return ;
}

void do_bank( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int number;

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );

  if(arg[0]=='\0')
  {
	send_to_char("Syntax:  bank <deposit|balance|withdraw> <amount>\n\r",ch);
	return ;
  }

  if( (ch->in_room==NULL) || (ch->in_room->vnum!=ROOM_VNUM_BANK) )
  {
	send_to_char("You must be in the bank of midgaard.\n\r",ch);
	return ;
  }

  if(!str_cmp(arg,"balance") )
  {
     sprintf(buf,"You have an account balance of %d gold.\n\r",ch->bank);
     send_to_char(buf,ch);
     return ;
  }	

  if(!is_number(arg2))
  {
     send_to_char("The second argument must be a number.\n\r",ch);
     return ;
  }

  number=atoi(arg2);

  if(!str_cmp(arg,"deposit"))
  {
    if( number+5>ch->gold )
    {
      send_to_char("You do not have that much money.\n\r",ch); 
      return ;
    }
    ch->gold-=number+5;
    ch->bank+=number;   	  
    send_to_char("Deposit made.\n\rThank you for your patronage.\n\r",ch);
    return ;
  }

    if(!str_cmp(arg,"withdraw"))
  {
    if(number<6)
    {
      send_to_char("Due to the surcharge you can not withdraw that small of an amount.\n\r",ch);
      return ;
    }

    if(number>ch->bank-5)
    {
	send_to_char("You do not have that much in the bank.\n\r",ch);
	return ;
    }
    ch->gold+=number;
    ch->bank-=number+5;
    send_to_char("Withdraw made.\n\rThank you for your patronage.\n\r",ch);
 
    return ;
  }

  send_to_char("That is not a valid command.\n\r",ch);

  return ;
}
