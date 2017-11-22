/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "music.h"
#include "lookup.h"

/*Global Variables*/
CRITICAL_DATA critical_table[MAX_DAMAGE_TYPE+1][15][5];

MOOT_DATA *moot;

/* Local functions */
CEFFECT_DATA *new_ceffect ( void );

char			strCrit[MAX_INPUT_LENGTH];
FILE *		fpCrit;
sh_int      fpDam;
int         top_rs;

void load_criticals(void)
{
	FILE *fpList;
	char buf[MAX_STRING_LENGTH];
	int i,j,k,l;

	for(i=0; i<MAX_DAMAGE_TYPE+1; i++)
		for(j=0; j<15; j++)
			for(k=0; k<5; k++)
			{
				critical_table[i][j][k].act_to_victim="\0";
				critical_table[i][j][k].act_to_room="\0";
				critical_table[i][j][k].act_to_actor="\0";
				for(l=0; l<3; l++)
				critical_table[i][j][k].victim_effects[l]=NULL;
				critical_table[i][j][k].actor_effects=NULL;
			}

	if ( ( fpList = fopen( CRITICAL_LIST, "r" ) ) == NULL )
	{
		 perror( CRITICAL_LIST );
		 exit( 1 );
	}


	for ( ; ; )
	{
		 strcpy( strCrit, fread_word( fpList ) );
		 if ( strCrit[0] == '$' )
		break;
		fpDam=fread_number( fpList );
		sprintf(buf, "%s%s", CRITICAL_DIR, strCrit);
		log_string(buf);
		if ( ( fpCrit = fopen( buf, "r" ) ) == NULL )
		{
			 perror( strCrit );
			 exit( 1 );
		}

		for(k=0; k<5; k++)
			for(j=0;j<15;j++)
			{
				critical_table[fpDam][j][k].act_to_victim=fread_string(fpCrit);
				critical_table[fpDam][j][k].act_to_room=fread_string(fpCrit);
				critical_table[fpDam][j][k].act_to_actor=fread_string(fpCrit);
				for(l=0; l<3; l++)
				{
					critical_table[fpDam][j][k].victim_effects[l]=new_ceffect();
					critical_table[fpDam][j][k].victim_effects[l]->sn=
                                                skill_lookup(fread_word(fpCrit));
					critical_table[fpDam][j][k].victim_effects[l]->aply=
                                                fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->modifier=
                                                fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->duration=
						fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->stun_for=
                                                fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->mortal_in=
																fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->additional_damage=
                                                fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->additional_mana_loss=
						fread_number(fpCrit);
					critical_table[fpDam][j][k].victim_effects[l]->hps_per_turn=
						fread_number(fpCrit);
				}
				critical_table[fpDam][j][k].actor_effects=new_ceffect();
				critical_table[fpDam][j][k].actor_effects->sn=skill_lookup(fread_word(fpCrit));
				critical_table[fpDam][j][k].actor_effects->aply=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->modifier=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->duration=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->stun_for=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->mortal_in=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->additional_damage=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->additional_mana_loss=fread_number(fpCrit);
				critical_table[fpDam][j][k].actor_effects->hps_per_turn=fread_number(fpCrit);
				log_string("Critical loaded.");
			}
			fclose(fpCrit);
	 }
	 fclose(fpList);
	 return ;
}

CEFFECT_DATA *new_ceffect(void)
{
        CEFFECT_DATA *af;

        af=alloc_perm(sizeof(*af));
        return af;
}

/*
 * Snarf a room section.
 */
void load_rooms_two( FILE *fp )
{
	 ROOM_INDEX_DATA *pRoomIndex;

	 if ( area_last == NULL )
	 {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
	 }

	 for ( ; ; )
	 {
	sh_int vnum;
	char letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
		 bug( "Load_rooms: # not found.", 0 );
		 exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
		 break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
		 bug( "Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->owner		= str_dup("");
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	pRoomIndex->description		= fread_string( fp );
	/* Area number */		  fread_number( fp );
	pRoomIndex->room_flags		= fread_flag( fp );
	/* horrible hack */
  	if ( 3000 <= vnum && vnum < 3400)
		SET_BIT(pRoomIndex->room_flags,ROOM_LAW);
	pRoomIndex->sector_type		= fread_number( fp );
	pRoomIndex->light		= 0;
	for ( door = 0; door <= 9; door++ )
		 pRoomIndex->exit[door] = NULL;

	/* defaults */
	pRoomIndex->heal_rate = 100;
	pRoomIndex->mana_rate = 100;

	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'S' )
		break;

	    if ( letter == 'H') /* healing room */
		pRoomIndex->heal_rate = fread_number(fp);

		 else if ( letter == 'M') /* mana room */
		pRoomIndex->mana_rate = fread_number(fp);

		else if ( letter == 'C') /* clan */
		{
		if (pRoomIndex->clan)
		{
			 bug("Load_rooms: duplicate clan fields.",0);
			 exit(1);
		}
		pRoomIndex->clan = clan_lookup(fread_string(fp));
		 }


		 else if ( letter == 'D' )
		 {
		EXIT_DATA *pexit;

		door = fread_number( fp );
		if ( door < 0 || door > MAX_DOORS )
		{
			 bug( "Fread_rooms: vnum %d has bad door number.", vnum );
			 exit( 1 );
		}

		pexit			= alloc_perm( sizeof(*pexit) );
		pexit->description	= fread_string( fp );
		pexit->keyword		= fread_string( fp );
		pexit->rs_flags         = fread_flag( fp );
		pexit->key		= fread_number( fp );
		pexit->u1.vnum		= fread_number( fp );
		pexit->orig_door        = door;                 /* OLC */
		pexit->exit_info=pexit->rs_flags;
		pRoomIndex->exit[door]	= pexit;
		pRoomIndex->old_exit[door] = pexit;
		top_exit++;
	    }
		 else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pRoomIndex->extra_descr;
		pRoomIndex->extra_descr	= ed;
		top_ed++;
	    }

		 else if (letter == 'O')
		 {
		if (pRoomIndex->owner[0] != '\0')
		{
		    bug("Load_rooms: duplicate owner.",0);
		    exit(1);
		}

		pRoomIndex->owner = fread_string(fp);
	    }

	    else if ( letter == 'F' )
            {
                ROOM_SPEC_DATA *rs;
                rs                      = alloc_perm( sizeof(*rs) );
                rs->lowhour             = fread_number( fp );
                rs->highhour            = fread_number( fp );
                rs->lowroll             = fread_number( fp );
                rs->highroll            = fread_number( fp );
                rs->special             = fread_string( fp );
                rs->next                = pRoomIndex->spec_data;
                pRoomIndex->spec_data   = rs;
                top_rs++;
            }


	    else
		 {
		bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;
	  top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room; /* OLC */
	  assign_area_vnum( vnum );                                    /* OLC */

    }

	 return;
}

/*
 * Snarf a mob section.  new style
 */
void load_mobiles_two( FILE *fp )
{
	 MOB_INDEX_DATA *pMobIndex;

	 if ( !area_last )   /* OLC */
	 {
		  bug( "Load_mobiles: no #AREA seen yet.", 0 );
		  exit( 1 );
	 }

	 for ( ; ; )
	 {
		  sh_int vnum;
		  char letter;
		  int iHash;

		  letter = fread_letter( fp );
		  if ( letter != '#' )
		  {
		    bug( "Load_mobiles: # not found.", 0 );
		    exit( 1 );
                  }
 
        vnum = fread_number( fp );
        if ( vnum == 0 )
            break;
 
	fBootDb = FALSE;
        if ( get_mob_index( vnum ) != NULL )
        {
	    bug( "Load_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
	fBootDb = TRUE;

	pMobIndex                       = alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum                 = vnum;
	pMobIndex->area                 = area_last;               /* OLC */
	pMobIndex->new_format		= TRUE;
	newmobs++;
	pMobIndex->player_name          = fread_string( fp );
	pMobIndex->short_descr          = fread_string( fp );
	pMobIndex->long_descr           = fread_string( fp );
	pMobIndex->description          = fread_string( fp );
	pMobIndex->race		 	= race_lookup(fread_string( fp ));

	pMobIndex->long_descr[0]        = UPPER(pMobIndex->long_descr[0]);
	pMobIndex->description[0]       = UPPER(pMobIndex->description[0]);

	pMobIndex->act                  = fread_flag( fp ) | ACT_IS_NPC
					| race_table[pMobIndex->race].act;
	pMobIndex->affected_by          = fread_flag( fp )
					| race_table[pMobIndex->race].aff;
	pMobIndex->pShop                = NULL;
	pMobIndex->tendency             = fread_number( fp );
	pMobIndex->alliance             = fread_number( fp );
        pMobIndex->xp_mod               = 100 ;
	pMobIndex->level                = fread_number( fp );
	pMobIndex->hitroll              = fread_number( fp );

	/* read hit dice */
	pMobIndex->hit[DICE_NUMBER]     = fread_number( fp );
	  /* 'd'          */                fread_letter( fp );
	pMobIndex->hit[DICE_TYPE]   	= fread_number( fp );
	  /* '+'          */                fread_letter( fp );
	pMobIndex->hit[DICE_BONUS]      = fread_number( fp );

        pMobIndex->hit[DICE_NUMBER]=pMobIndex->hit[DICE_TYPE]=1;
        pMobIndex->hit[DICE_BONUS] = 6*pMobIndex->level ; 

	/* read mana dice */
	pMobIndex->mana[DICE_NUMBER]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->mana[DICE_TYPE]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->mana[DICE_BONUS]	= fread_number( fp );

	/* read damage dice */
	pMobIndex->damage[DICE_NUMBER]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_TYPE]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_BONUS]	= fread_number( fp );
	pMobIndex->dam_type		= attack_lookup(fread_word(fp));

	/* read armor class */
	pMobIndex->ac[AC_PIERCE]	= fread_number( fp ) * 10;
	pMobIndex->ac[AC_BASH]		= fread_number( fp ) * 10;
	pMobIndex->ac[AC_SLASH]		= fread_number( fp ) * 10;
	pMobIndex->ac[AC_EXOTIC]	= fread_number( fp ) * 10;

	/* read flags and add in data from the race table */
	pMobIndex->off_flags		= fread_flag( fp ) 
					| race_table[pMobIndex->race].off;
	pMobIndex->imm_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].imm;
	pMobIndex->res_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].res;
	pMobIndex->vuln_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].vuln;

	/* vital statistics */
	pMobIndex->start_pos		= position_lookup(fread_word(fp));
	pMobIndex->default_pos		= position_lookup(fread_word(fp));
	pMobIndex->sex			= sex_lookup(fread_word(fp));

	pMobIndex->wealth		= fread_number( fp );
        if(pMobIndex->wealth > pMobIndex->level*10)
          pMobIndex->wealth=pMobIndex->level*10;
          

	pMobIndex->form			= fread_flag( fp )
					| race_table[pMobIndex->race].form;
	pMobIndex->parts		= fread_flag( fp )
					| race_table[pMobIndex->race].parts;
	/* size */
	pMobIndex->size			= size_lookup(fread_word(fp));
	pMobIndex->material		= str_dup(fread_word( fp ));

	for ( ; ; )
        {
            letter = fread_letter( fp );

            if (letter == 'F')
				{
		char *word;
		long vector;

                word                    = fread_word(fp);
		vector			= fread_flag(fp);

		if (!str_prefix(word,"act"))
			 REMOVE_BIT(pMobIndex->act,vector);
                else if (!str_prefix(word,"aff"))
		    REMOVE_BIT(pMobIndex->affected_by,vector);
		else if (!str_prefix(word,"off"))
		    REMOVE_BIT(pMobIndex->affected_by,vector);
		else if (!str_prefix(word,"imm"))
		    REMOVE_BIT(pMobIndex->imm_flags,vector);
		else if (!str_prefix(word,"res"))
		    REMOVE_BIT(pMobIndex->res_flags,vector);
		else if (!str_prefix(word,"vul"))
		    REMOVE_BIT(pMobIndex->vuln_flags,vector);
		else if (!str_prefix(word,"for"))
		    REMOVE_BIT(pMobIndex->form,vector);
		else if (!str_prefix(word,"par"))
		    REMOVE_BIT(pMobIndex->parts,vector);
		else
		{
		    bug("Flag remove: flag not found.",0);
		    exit(1);
		}
	      }
	     else
	     {
		ungetc(letter,fp);
		break;
	     }
	}

		  iHash                   = vnum % MAX_KEY_HASH;
		  pMobIndex->next         = mob_index_hash[iHash];
		  mob_index_hash[iHash]   = pMobIndex;
		  top_mob_index++;
		  kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
	 }

	 return;
}

/*
 * Snarf an obj section. new style
 */
void load_objects_two( FILE *fp )
{
	 OBJ_INDEX_DATA *pObjIndex;

	 if ( !area_last )   /* OLC */
	 {
		  bug( "Load_objects: no #AREA seen yet.", 0 );
		  exit( 1 );
	 }

	 for ( ; ; )
	 {
		  sh_int vnum;
		  char letter;
		  int iHash;
 
        letter = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_objects: # not found.", 0 );
	    exit( 1 );
	}
 
	vnum = fread_number( fp );
        if ( vnum == 0 )
            break;
 
        fBootDb = FALSE;
        if ( get_obj_index( vnum ) != NULL )
        {
            bug( "Load_objects: vnum %d duplicated.", vnum );
				exit( 1 );
        }
	fBootDb = TRUE;
 
        pObjIndex = alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum = vnum;
	pObjIndex->area = area_last;            /* OLC */
	pObjIndex->new_format           = TRUE;
	pObjIndex->reset_num		= 0;
	newobjs++;
        pObjIndex->name                 = fread_string( fp );
        pObjIndex->short_descr          = fread_string( fp );
        pObjIndex->description          = fread_string( fp );
        pObjIndex->material		= fread_string( fp );
 
        pObjIndex->item_type            = item_lookup(fread_word( fp ));
        pObjIndex->extra_flags          = fread_flag( fp );
	pObjIndex->wear_flags           = fread_flag( fp );
	switch(pObjIndex->item_type)
	{
	case ITEM_WEAPON:
	    pObjIndex->value[0]		= weapontype(fread_word(fp));
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= attack_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_flag(fp);
	    break;
	case ITEM_CONTAINER:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_flag(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= fread_number(fp);
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
	case ITEM_DRINK_CON:
	case ITEM_FOUNTAIN:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = liq_lookup(fread_word(fp));
            pObjIndex->value[3]         = fread_number(fp);
            pObjIndex->value[4]         = fread_number(fp);
	    break;
	case ITEM_WAND:
	case ITEM_STAFF:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
	case ITEM_POTION:
	case ITEM_PILL:
	case ITEM_SCROLL:
 	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[2]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= skill_lookup(fread_word(fp));
	    break;
	default:
            pObjIndex->value[0]             = fread_flag( fp );
            pObjIndex->value[1]             = fread_flag( fp );
            pObjIndex->value[2]             = fread_flag( fp );
            pObjIndex->value[3]             = fread_flag( fp );
	    pObjIndex->value[4]		    = fread_flag( fp );
	    break;
	}
	pObjIndex->level		= fread_number( fp );
        pObjIndex->weight               = fread_number( fp );
        pObjIndex->cost                 = fread_number( fp ); 

        /* condition */
        letter 				= fread_letter( fp );
	switch (letter)
	{
		 case ('P') :		pObjIndex->condition = 100; break;
		 case ('G') :		pObjIndex->condition =  90; break;
		 case ('A') :		pObjIndex->condition =  75; break;
		 case ('W') :		pObjIndex->condition =  50; break;
		 case ('D') :		pObjIndex->condition =  25; break;
		 case ('B') :		pObjIndex->condition =  10; break;
		 case ('R') :		pObjIndex->condition =   0; break;
		 default:		pObjIndex->condition = 100; break;
	}
	pObjIndex->absolute_size = fread_number(fp);
	pObjIndex->relative_size = fread_number(fp);
        pObjIndex->class_restriction = 0 ; 

        for ( ; ; )
	{
            char letter;
 
            letter = fread_letter( fp );
 
            if ( letter == 'A' )
		{
                AFFECT_DATA *paf;
 
                paf                     = alloc_perm( sizeof(*paf) );
		paf->where		= TO_OBJECT;
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number( fp );
                paf->modifier           = fread_number( fp );
					 paf->bitvector          = 0;
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
                
                if(paf->location==1) paf->modifier=paf->modifier*2;
                else
                if(paf->location==2)
                {
                  paf->modifier=paf->modifier*2;
                  paf->location=APPLY_RE;  
                }
                else
                if(paf->location==3)
                {
                  paf->modifier=paf->modifier*2;
                  paf->location=APPLY_ME;
                }
                else
                if(paf->location==4)
                {
                  paf->modifier=paf->modifier*2;
                  paf->location=APPLY_QU;
                }
                else
                if(paf->location==5)
                {
                  paf->modifier=paf->modifier*2;
                  paf->location=APPLY_CO;
                }
                else
                if(paf->location==APPLY_HIT)
                {
                  paf->modifier=paf->modifier/3;
                }

                }

	    else if (letter == 'F')
            {
                AFFECT_DATA *paf;
 
                paf                     = alloc_perm( sizeof(*paf) );
		letter 			= fread_letter(fp);
		switch (letter)
	 	{
		case 'A':
                    paf->where          = TO_AFFECTS;
		    break;
		case 'I':
			 paf->where		= TO_IMMUNE;
		    break;
		case 'R':
		    paf->where		= TO_RESIST;
		    break;
		case 'V':
			 paf->where		= TO_VULN;
		    break;
		default:
            	    bug( "Load_objects: Bad where on flag set.", 0 );
            	   exit( 1 );
		}
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number(fp);
		paf->modifier           = fread_number(fp);
                paf->bitvector          = fread_flag(fp);
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
            }
 
            else if ( letter == 'E' )
		{
                EXTRA_DESCR_DATA *ed;
 
                ed                      = alloc_perm( sizeof(*ed) );
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
		ed->next                = pObjIndex->extra_descr;
                pObjIndex->extra_descr  = ed;
                top_ed++;
		}
 
            else
            {
                ungetc( letter, fp );
                break;
            }
        }
 
        iHash                   = vnum % MAX_KEY_HASH;
        pObjIndex->next         = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObjIndex;
	top_obj_index++;
	 }

	 return;
}

void load_mobiles_three( FILE *fp )
{
         MOB_INDEX_DATA *pMobIndex;

         if ( !area_last )   /* OLC */
         {
                  bug( "Load_mobiles: no #AREA seen yet.", 0 );
                  exit( 1 );
         }

         for ( ; ; )
         {
                  sh_int vnum;
                  char letter;
                  int iHash;

                  letter = fread_letter( fp );
                  if ( letter != '#' )
                  {
                    bug( "Load_mobiles: # not found.", 0 );
                    exit( 1 );
                  }

        vnum = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_mob_index( vnum ) != NULL )
        {
            bug( "Load_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pMobIndex                       = alloc_perm( sizeof(*pMobIndex) );
        pMobIndex->vnum                 = vnum;
        pMobIndex->area                 = area_last;               /* OLC */
        pMobIndex->new_format           = TRUE;
        newmobs++;
        pMobIndex->player_name          = fread_string( fp );
        pMobIndex->short_descr          = fread_string( fp );
        pMobIndex->long_descr           = fread_string( fp );
        pMobIndex->description          = fread_string( fp );
        pMobIndex->race                 = race_lookup(fread_string( fp ));

        pMobIndex->long_descr[0]        = UPPER(pMobIndex->long_descr[0]);
        pMobIndex->description[0]       = UPPER(pMobIndex->description[0]);

        pMobIndex->act                  = fread_flag( fp ) | ACT_IS_NPC
                                        | race_table[pMobIndex->race].act;
        pMobIndex->affected_by          = fread_flag( fp )
                                        | race_table[pMobIndex->race].aff;
        pMobIndex->pShop                = NULL;
        pMobIndex->tendency             = fread_number( fp );
        pMobIndex->alliance             = fread_number( fp );
        pMobIndex->xp_mod               = fread_number( fp );
        pMobIndex->level                = fread_number( fp );
        pMobIndex->hitroll              = fread_number( fp );

        /* read hit dice */
        pMobIndex->hit[DICE_NUMBER]     = fread_number( fp );
          /* 'd'          */                fread_letter( fp );
        pMobIndex->hit[DICE_TYPE]       = fread_number( fp );
          /* '+'          */                fread_letter( fp );
        pMobIndex->hit[DICE_BONUS]      = fread_number( fp );

        /* read mana dice */
        pMobIndex->mana[DICE_NUMBER]    = fread_number( fp );
                                          fread_letter( fp );
        pMobIndex->mana[DICE_TYPE]      = fread_number( fp );
                                          fread_letter( fp );
        pMobIndex->mana[DICE_BONUS]     = fread_number( fp );
        /* read damage dice */
        pMobIndex->damage[DICE_NUMBER]  = fread_number( fp );
                                          fread_letter( fp );
        pMobIndex->damage[DICE_TYPE]    = fread_number( fp );
                                          fread_letter( fp );
        pMobIndex->damage[DICE_BONUS]   = fread_number( fp );
        pMobIndex->dam_type             = attack_lookup(fread_word(fp));

        /* read armor class */
        pMobIndex->ac[AC_PIERCE]        = fread_number( fp ) * 10;
        pMobIndex->ac[AC_BASH]          = fread_number( fp ) * 10;
        pMobIndex->ac[AC_SLASH]         = fread_number( fp ) * 10;
        pMobIndex->ac[AC_EXOTIC]        = fread_number( fp ) * 10;

        /* read flags and add in data from the race table */
        pMobIndex->off_flags            = fread_flag( fp )
                                        | race_table[pMobIndex->race].off;
        pMobIndex->imm_flags            = fread_flag( fp )
                                        | race_table[pMobIndex->race].imm;
        pMobIndex->res_flags            = fread_flag( fp )
                                        | race_table[pMobIndex->race].res;
        pMobIndex->vuln_flags           = fread_flag( fp )
                                        | race_table[pMobIndex->race].vuln;

        /* vital statistics */
        pMobIndex->start_pos            = position_lookup(fread_word(fp));
        pMobIndex->default_pos          = position_lookup(fread_word(fp));
        pMobIndex->sex                  = sex_lookup(fread_word(fp));

        pMobIndex->wealth               = fread_number( fp );
        if(pMobIndex->wealth > pMobIndex->level*10)
          pMobIndex->wealth=pMobIndex->level*10;


        pMobIndex->form                 = fread_flag( fp )
                                        | race_table[pMobIndex->race].form;
        pMobIndex->parts                = fread_flag( fp )
                                        | race_table[pMobIndex->race].parts;
        /* size */
        pMobIndex->size                 = size_lookup(fread_word(fp));
        pMobIndex->material             = str_dup(fread_word( fp ));

        for ( ; ; )
        {
            letter = fread_letter( fp );

            if (letter == 'F')
                                {
                char *word;
                long vector;

                word                    = fread_word(fp);
                vector                  = fread_flag(fp);

                if (!str_prefix(word,"act"))
                         REMOVE_BIT(pMobIndex->act,vector);
                else if (!str_prefix(word,"aff"))
                    REMOVE_BIT(pMobIndex->affected_by,vector);
                else if (!str_prefix(word,"off"))
                    REMOVE_BIT(pMobIndex->affected_by,vector);
                else if (!str_prefix(word,"imm"))
                    REMOVE_BIT(pMobIndex->imm_flags,vector);
                else if (!str_prefix(word,"res"))
                    REMOVE_BIT(pMobIndex->res_flags,vector);
                else if (!str_prefix(word,"vul"))
                    REMOVE_BIT(pMobIndex->vuln_flags,vector);
                else if (!str_prefix(word,"for"))
                    REMOVE_BIT(pMobIndex->form,vector);
                else if (!str_prefix(word,"par"))
                    REMOVE_BIT(pMobIndex->parts,vector);
                else
                {
                    bug("Flag remove: flag not found.",0);
                    exit(1);
                }
              }
             else
             {
                ungetc(letter,fp);
                break;
             }
        }

                  iHash                   = vnum % MAX_KEY_HASH;
                  pMobIndex->next         = mob_index_hash[iHash];
                  mob_index_hash[iHash]   = pMobIndex;
                  top_mob_index++;
                  kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;         }

         return;
}

void load_objects_three( FILE *fp )
{
         OBJ_INDEX_DATA *pObjIndex;

         if ( !area_last )   /* OLC */
         {
                  bug( "Load_objects: no #AREA seen yet.", 0 );
                  exit( 1 );
         }

         for ( ; ; )
         {
                  sh_int vnum;
                  char letter;
                  int iHash;

        letter = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_objects: # not found.", 0 );
            exit( 1 );
        }

        vnum = fread_number( fp );
         if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_obj_index( vnum ) != NULL )
        {
            bug( "Load_objects: vnum %d duplicated.", vnum );
                                exit( 1 );
        }
        fBootDb = TRUE;

        pObjIndex = alloc_perm( sizeof(*pObjIndex) );
        pObjIndex->vnum = vnum;
        pObjIndex->area = area_last;            /* OLC */
        pObjIndex->new_format           = TRUE;
        pObjIndex->reset_num            = 0;
        newobjs++;
        pObjIndex->name                 = fread_string( fp );
        pObjIndex->short_descr          = fread_string( fp );
        pObjIndex->description          = fread_string( fp );
        pObjIndex->material             = fread_string( fp );

        pObjIndex->item_type            = item_lookup(fread_word( fp ));
        pObjIndex->extra_flags          = fread_flag( fp );
        pObjIndex->wear_flags           = fread_flag( fp );
        switch(pObjIndex->item_type)
        {
        case ITEM_WEAPON:
            pObjIndex->value[0]         = weapontype(fread_word(fp));
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = fread_number(fp);
            pObjIndex->value[3]         = attack_lookup(fread_word(fp));
            pObjIndex->value[4]         = fread_flag(fp);
            break;
        case ITEM_CONTAINER:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_flag(fp);
            pObjIndex->value[2]         = fread_number(fp);
            pObjIndex->value[3]         = fread_number(fp);
            pObjIndex->value[4]         = fread_number(fp);
            break;
        case ITEM_DRINK_CON:
        case ITEM_FOUNTAIN:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = liq_lookup(fread_word(fp));
            pObjIndex->value[3]         = fread_number(fp);
            pObjIndex->value[4]         = fread_number(fp);
            break;
        case ITEM_WAND:
        case ITEM_STAFF:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = fread_number(fp);
            pObjIndex->value[3]         = skill_lookup(fread_word(fp));
            pObjIndex->value[4]         = fread_number(fp);
            break;
        case ITEM_POTION:
        case ITEM_PILL:
        case ITEM_SCROLL:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = skill_lookup(fread_word(fp));
            pObjIndex->value[2]         = skill_lookup(fread_word(fp));
            pObjIndex->value[3]         = skill_lookup(fread_word(fp));
            pObjIndex->value[4]         = skill_lookup(fread_word(fp));
            break;
        default:
            pObjIndex->value[0]             = fread_flag( fp );
            pObjIndex->value[1]             = fread_flag( fp );
            pObjIndex->value[2]             = fread_flag( fp );
            pObjIndex->value[3]             = fread_flag( fp );
            pObjIndex->value[4]             = fread_flag( fp );
            break;
        }
        pObjIndex->level                = fread_number( fp );
        pObjIndex->weight               = fread_number( fp );
        pObjIndex->cost                 = fread_number( fp );

        /* condition */
        letter                          = fread_letter( fp );
        switch (letter)
        {
                 case ('P') :           pObjIndex->condition = 100; break;
                 case ('G') :           pObjIndex->condition =  90; break;
                 case ('A') :           pObjIndex->condition =  75; break;
                 case ('W') :           pObjIndex->condition =  50; break;
                 case ('D') :           pObjIndex->condition =  25; break;
                 case ('B') :           pObjIndex->condition =  10; break;
                 case ('R') :           pObjIndex->condition =   0; break;
                 default:               pObjIndex->condition = 100; break;
        }
        pObjIndex->absolute_size = fread_number(fp);
        pObjIndex->relative_size = fread_number(fp);
        pObjIndex->class_restriction = fread_flag(fp);

        for ( ; ; )
        {
            char letter;

            letter = fread_letter( fp );

            if ( letter == 'A' )
                {
                AFFECT_DATA *paf;

                paf                     = alloc_perm( sizeof(*paf) );
                paf->where              = TO_OBJECT;
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number( fp );
                paf->modifier           = fread_number( fp );
                                         paf->bitvector          = 0;
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
                }

            else if (letter == 'F')
            {
                AFFECT_DATA *paf;

                paf                     = alloc_perm( sizeof(*paf) );
                letter                  = fread_letter(fp);
                switch (letter)
                {
                case 'A':
                    paf->where          = TO_AFFECTS;
                    break;
                case 'I':
                         paf->where             = TO_IMMUNE;
                    break;
                case 'R':
                    paf->where          = TO_RESIST;
                    break;
                case 'V':
                         paf->where             = TO_VULN;
                    break;
                default:
                    bug( "Load_objects: Bad where on flag set.", 0 );
                   exit( 1 );
                }
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                 paf->location           = fread_number(fp);
                paf->modifier           = fread_number(fp);
                paf->bitvector          = fread_flag(fp);
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
            }

            else if ( letter == 'E' )
                {
                EXTRA_DESCR_DATA *ed;

                ed                      = alloc_perm( sizeof(*ed) );
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
                ed->next                = pObjIndex->extra_descr;
                pObjIndex->extra_descr  = ed;
                top_ed++;
                }

            else
            {
                ungetc( letter, fp );
                break;
            }
        }

        iHash                   = vnum % MAX_KEY_HASH;
        pObjIndex->next         = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObjIndex;
        top_obj_index++;
         }

         return;
}
 
