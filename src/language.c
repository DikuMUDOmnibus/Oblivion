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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "color.h"

const char language_table[MAX_LANGUAGE][10] =
	{ "human", "elven", "dwarven", "gnomish", "halfling", "orcish",
	  "drow", "avian", "gnoll" };

int language_lookup(const char *name)
{
         int lang;

    for (lang = 0; lang < MAX_LANGUAGE; lang++)
         {
        if (LOWER(name[0]) == LOWER(language_table[lang][0])
        &&  !str_prefix(name,language_table[lang]))
                 return lang;
         }

         return -1;
}


struct lang_set
{
	char *old;
	char *new;
};

const struct lang_set trans_table[MAX_LANGUAGE+1][500] =
{
   {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
   },

 {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "weapon", "saalah" }, { "battle", "thaaleh" },
     { "treasure", "beulaern" }, { "friend", "abbinaa" },
     { "magician", "faeerban" }, { "magical", "faarn" },
     { "magic", "faar" }, { "strenght", "altrun" },
     { "meeting", "aenfin" }, { "darkness", "velkiin" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},
    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},
    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},
    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},

    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},

    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "confrontation", "qua'laelay" },
     { "disagreement", "qua'laelay" },
     { "traditional", "elend" }, { "illithids", "haszakkin" },
     { "illithid", "haszak" }, { "recklessness", "streeaka" },
     { "fearlessness", "streeaka" }, { "continuing", "elendar" },
     { "concealment", "veldrin" }, { "achievement", "xundus" },
     { "highpriestess", "yathtallar" },
     { "expedition", "z'hind" }, { "continued", "elendar" },
     { "continue", "elendar" }, { "enduring", "elendar" },
     { "stratagem", "inth" }, { "brightness", "ssussun" },
     { "authority", "quarth" }, { "blockage", "klug" },
     { "agreement", "inthigg" }, { "confidence", "sargh" },
     { "battlemight", "sargh" }, { "opponent", "ogglin" },
     { "Goddess", "Quar'valsharess" },
     { "priestess", "yathrin" }, { "knowledge", "zhaunil" },
     { "wanderer", "noamuth" }, { "commanded", "quarthen" },
     { "darkness", "oloth" }, { "discover", "ragar" },
     { "dominance", "z'ress" }, { "accomplish", "xun" },
     { "learning", "zhaunil" }, { "conquering", "ultrinnan" },
     { "encounter", "thalra" }, { "honored", "malla" },
     { "inspiration", "mrimm" }, { "conqueror", "ultrin" },
     { "excrement", "iblith" }, { "rampart", "kluggen" },
     { "watching", "kyorlin" }, { "waiting", "kyorlin" },
     { "guarding", "kyorlin" }, { "yourself", "dosstan" },
     { "surprises", "brorna" }, { "surprise", "brorn" },
     { "treasure", "belaern" }, { "comrade", "abbil" },
     { "coinage", "belaern" }, { "traitors", "darthiir" },
     { "bravery", "honglath" }, { "carrion", "iblith" },
     { "barrier", "kluggen" }, { "magical", "faerl" },
     { "wizard", "faern" }, { "goblin", "gol" },
     { "scheme", "inth" }, { "destiny", "elamshin" },
     { "destroy", "elgg" }, { "second", "drada" },
     { "matrons", "ilharessen" }, { "matron", "ilharess" },
     { "superior", "alur" }, { "foremost", "alurl" },
     { "facing", "alust" },   { "treaty", "inthigg" },
     { "foolish", "waela" }, { "unaware", "waela" },
     { "unwary", "waela" }, { "striving", "xund" },
     { "journey", "z'hind" }, { "strength", "z'ress" },
     { "ordered", "quarthen" }, { "argument", "qua'laelay" },
     { "monster", "phindar" }, { "creature", "phindar" },
     { "shadows", "veldrin" }, { "supreme", "ultrin" },
     { "highest", "ultrin" }, { "destiny", "ul-Ilindith" },
     { "longing", "ssinssrigg" }, { "uncover", "ragar" },
     { "behind", "rathrae" }, { "humans", "rivvin" },
     { "human", "rivvil" }, { "meeting", "talthalra" },
     { "council", "talthalra" },  { "parley", "talthalra" },
     { "caution", "olist" }, { "stealth", "olist" },
     { "rival", "ogglin" }, { "enemy", "ogglin" },
     { "unknown", "noamuth" }, { "passion", "ssinssrigg" },
     { "warning", "sarn" }, { "beware", "sarn" },
     { "warrior", "sargtlin" }, { "Lloth", "Quarvalsharess" },
     { "order", "quarth" }, { "spider", "orbb" },
     { "sieze", "plynn" }, { "sacred", "orthae" },
      { "elves", "darthiir" }, { "faeries", "darthiir" },
     { "outcast", "dobluth" }, { "coinage", "belaern" },
     { "wealth", "belaern" }, { "dodge", "bautha" },
     { "around", "bauth" }, { "about", "bauth" },
     { "trick", "golhyrr" }, { "ruse", "golhyrr" },
     { "master", "jabbuk" }, { "trust", "khaless" },
     { "shield", "kluggen" }, { "alert", "kyone" },
     { "wary", "kyone" }, { "careful", "kyone" },
     { "third", "llarnbuss" }, { "three", "llar" },
     { "apart", "maglust" }, { "alone", "maglust" },
     { "guide", "mrimm" }, { "patron", "ilharn" },
     { "mother", "ilhar" }, { "consider", "talinth" },
     { "light", "ssussun" }, { "first", "ust" },
     { "queen", "valsharess" }, { "invisible", "velkyn" },
     { "hidden", "velkyn" },  { "unseen", "velkyn" },
     { "black", "vel'oloth" }, { "blade", "velve" },
     { "magic", "faer" }, { "goal", "ilindith" },
     { "watch", "kyrol" }, { "wait", "kyrol" },
     { "guard", "kyrol" }, { "hand", "kyonss" },
     { "calm", "honglath" }, { "raid", "tha-lackz'hind" },
     { "think", "talinth" }, { "perhaps", "xal" },
     { "might", "xal" }, { "temple", "yath" },
     { "doing", "xundus" }, { "climb", "z'orr" },
     { "power", "z'ress" }, { "force", "z'ress" },
     { "walk", "z'hin" }, { "ride", "z'har" },
     { "wisdom", "zhaunil" }, { "trip", "z'hind" },
     { "fool", "wael" }, { "shit", "iblith" },
     { "backs", "ratha" }, { "back", "rath" },
     { "find", "ragar" }, { "House", "qu'ellar" },
     { "upon", "pholor" }, { "holy", "orthae" },
     { "unknown", "noamuth" }, { "lost", "noamuth" },
     { "those", "nind" }, { "grave", "phalar" },
     { "danger", "sreen" }, { "than", "taga" },
     { "both", "tuth" }, { "aim", "ilindith" },
     { "may", "xal" }, { "one", "uss" }, { "to", "uss" },
     { "in", "wun" }, { "do", "xun" }, { "yours", "dosst" },
     { "two", "draa" }, { "more", "mzild" }, { "kill", "ellg" },
     { "slay", "ellg" }, { "alive", "dro" }, { "life", "dro" },
     { "you", "dos" }, { "path", "colbauth" }, { "from", "dal" },
     { "band", "akh" }, { "group", "akh" }, { "ally", "abban" },
     { "best", "alurl" }, { "give", "belbau" },
     { "gift", "belbol" }, { "thing", "bol" }, { "item", "bol" },
     { "food", "cahallin" }, { "out", "doeb" },
     { "meet", "thalra" }, { "take", "plynn" },
     { "death", "elghinn" }, { "fun", "jivvin" },
     { "play", "jivvin" }, { "valor", "sargh" },
     { "war", "thalack" }, { "no", "nau" },
     { "are", "phuul" }, { "of", "del" }, { "is", "zhah" },
     { "all", "jal" }, { "down", "harl" }, { "under", "harl" },
     { "below", "harl" }, { "as", "izil" }, { "the", "lil" },
     { "and", "lueth" }, { "be", "tlu" },
     { "him", "usstil" }, { "her", "usstils" },
     { "they", "usstin" }, { "them", "usstina" },
     { "us", "usstens" }, { "I", "usstan" },
     { "es", "ir" }, { "ess", "th" }, { "on", "pholor" },
     { "th", "nn" }, { "al", "ol" }, { "ing", "in" },
     { "ion", "mm" }, { "ment", "lay" }, { "er", "uk" },
     { "a", "natha" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},
    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
     { "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
     { "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
     { "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
     { "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
     { "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
     { "y", "l" }, { "z", "k" },
     { "", "" }
},
    {
     { " ", " " },
     { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
     { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
     { "9", "9" }, { "0", "0" },
     { ",", "," }, { ".", "." }, { "!", "!" }, { "'", "'" },
     { "_", "_" }, { ";", ";" }, { ":", ":" }, { "=", "=" },
     { "\"", "\"" }, { ")", ")" }, { "(", "(" },
     { "*", "*" }, { "&", "&" }, { "^", "^" }, { "%", "%" },
     { "$", "$" }, { "#", "#" }, { "<", "<" }, { ">", ">" },
     { "-", "-" },
     { "attractive", "shulamushula" }, { "slime", "sleta" },
     { "pleasure", "nugnukkaah" }, { "orgasm", "nugnukkaah" },
     { "Takhisis", "Drakanna" }, { "Airius", "Drakan" },
     { "strenght", "grauul" }, { "might", "grauul" },
     { "tribe", "kullek" }, { "group", "kullek" },
     { "bitch", "catich" }, { "curse", "nakkad" },
     { "horny", "leanna" }, { "posess", "kallak" },
     { "sword", "lierakla" }, { "blade", "lierakla" },
     { "torso", "crruta" }, { "body", "crruta" },
     { "armor", "tkkula" }, { "fight", "ka" },
     { "battle", "ralak" }, { "war", "sutta" },
     { "attack", "daah" }, { "Thera", "Gaia" },
     { "victory", "chaaug" }, { "magic", "ilde" },
     { "human", "aan" }, { "avian", "aruken" },
     { "dragon", "drakon" }, { "gnoll", "lutteah" },
     { "weapon", "tsittsa" }, { "gnome", "chaa" },
     { "orc", "ellueh" }, { "murder", "leucth" },
     { "thief", "lecutta" }, { "cleric", "shanan" },
     { "pack", "kullek" }, { "mage", "errma" },
     { "snout", "hieda" }, { "foot", "utta" },
     { "hand", "utta" }, { "head", "serreck" },
     { "arm", "gurrt" }, { "leg", "lauck" },
     { "flea", "tilkk" }, { "fool", "tchakl" },
     { "weak", "guatch" }, { "spit", "nilgitch" },
     { "shit", "karnaloghtch" }, { "dung", "karnaloghtch" },
     { "damn", "kkuleh" }, { "lust", "kellat" },
     { "soft", "suvvt" }, { "sexy", "shulamushula" },
     { "joy", "nugnukkaah" }, { "give", "kallak" },
     { "take", "kallak" }, { "have", "kallak" },
     { "own", "kallak" }, { "like", "aark" },
     { "want", "aark" }, { "meat", "chaach" },
     { "food", "chaach" }, { "with", "cullek" },
     { "good", "rauuh" }, { "eat", "chaach" },
     { "yes", "rau" }, { "no", "nug" }, { "you", "hy" },
     { "I", "Ra" }, { "love", "" }, { "me", "ra" },
     { "us", "ayn" }, { "he", "ta" }, { "she", "ta" },
     { "that", "ta" }, { "them", "tauk" }, { "they", "tauk" },
     { "trust", "" }, { "was", "a" }, { "is", "et" },
     { "will", "uh" }, { "ory", "aug" }, { "er", "th" },
     { "ea", "ugn" },
     { "a", "uh" }, { "b", "gr" }, { "c", "cha" },
     { "d", "dah" },{ "e", "eh" }, { "f", "kk" }, { "g", "ga" },
     { "h", "ash" }, { "i", "u" }, { "j", "tch" }, { "k", "t" },
     { "l", "r" }, { "m", "chk" }, { "n", "i" }, { "o", "ec" },
     { "p", "vt" }, { "q", "d" }, { "r", "f" }, { "s", "uk" },
     { "t", "tch" }, { "u", "uuh" }, { "v", "z" },
     { "w", "wch" }, { "x", "n" }, { "y", "l" }, { "z", "k" },
     { "", "" }
}
 
};

void trans_language(CHAR_DATA * ch, CHAR_DATA * victim, char * message, 
                  char *output)
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [ MAX_STRING_LENGTH ];
    char *pName;
    int iSyl;
    int length;
    bool checked;

    buf[0]      = '\0';
    for ( pName = message; *pName != '\0'; pName += length )
    {
        checked=FALSE;
        for ( iSyl = 0; (length = 
              strlen(trans_table[ch->language][iSyl].old)) != 0; iSyl++ )
        {
            
            if ( !str_prefix( trans_table[ch->language][iSyl].old, pName ) &&
                 !checked)
            {
		checked=TRUE;
		if( ((number_percent()>
                     get_skill(ch,skill_lookup(language_table[ch->language])))
		   ||
		    (number_percent()>
                     get_skill(victim,skill_lookup(language_table[ch->language]))))
                   &&
                    (!IS_SET(ch->act, PLR_HOLYLIGHT))
                   &&
                    (!IS_SET(victim->act, PLR_HOLYLIGHT)
		   && !IS_NPC(ch) && !IS_NPC(victim) && !IS_CHAT(ch))

                  )
                  strcat( buf, trans_table[ch->language][iSyl].new );
		  else
		  strcat( buf, trans_table[ch->language][iSyl].old);
                  break;
            }
        }

        if ( length == 0 )
            length = 1;
    }

    if(get_skill(victim,skill_lookup(language_table[ch->language])) > 10)
	sprintf(buf2,"%s(%s)%s%s", IS_SET(victim->act, PLR_COLOR) ?
              FG_LT_PURPLE : "" , language_table[ch->language], 
              IS_SET(victim->act, PLR_COLOR) ? FG_LT_GRAY : "", buf);
    else
	strcpy(buf2,buf);
    
    strcpy(output,buf2);	
    
    if( number_range(1,100) < strlen(buf2))
    {
       if(ch->race == race_lookup("drow"))
	 check_improve(ch, skill_lookup(language_table[ch->language]),TRUE,4);
       if(victim->race == race_lookup("drow"))
         check_improve(victim, skill_lookup(language_table[ch->language]),TRUE,4);
         check_improve(ch, skill_lookup(language_table[ch->language]),TRUE,4);
         check_improve(victim, skill_lookup(language_table[ch->language]),TRUE,4);
    }	
    	
    return;
}

void do_language(CHAR_DATA *ch, char *argument)
{
   int lang;

   if( ( lang = language_lookup(argument) ) == -1 )
	{
	send_to_char("No such tongue exists\n\r",ch);	
	return ;
	}
   
   if ( get_skill(ch,skill_lookup(language_table[lang]))<1)
	{
	send_to_char("You are not learned in that language.\n\r",ch);
	return ;
	}

   ch->language=lang;
   send_to_char("Ok.\n\r",ch);
   return ;
}

 


