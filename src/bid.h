/***************************************************************************
 *  OBLIVION 1.2 is copyright by Wes Wagner August, 1996                   *
 *  by using this code you have agreed to the terms of the Oblivion License*
 **************************************************************************/
 
#include <stdlib.h>

/*
  This function allows the following kinds of bids to be made:

  Absolute bid
  ============

  bid 14k, bid 50m66, bid 100k

  Relative bid
  ============

  These bids are calculated relative to the current bid. The '+' symbol adds
  a certain number of percent to the current bid. The default is 25, so
  with a current bid of 1000, bid + gives 1250, bid +50 gives 1500 etc.
  Please note that the number must follow exactly after the +, without any
  spaces!

  The '*' or 'x' bid multiplies the current bid by the number specified,
  defaulting to 2. If the current bid is 1000, bid x  gives 2000, bid x10
  gives 10,000 etc.

*/

int advatoi (const char *s)
/*
  util function, converts an 'advanced' ASCII-number-string into a number.
  Used by parsebid() but could also be used by do_give or do_wimpy.

  Advanced strings can contain 'k' (or 'K') and 'm' ('M') in them, not just
  numbers. The letters multiply whatever is left of them by 1,000 and
  1,000,000 respectively. Example:

  14k = 14 * 1,000 = 14,000
  23m = 23 * 1,000,0000 = 23,000,000

  If any digits follow the 'k' or 'm', the are also added, but the number
  which they are multiplied is divided by ten, each time we get one left. This
  is best illustrated in an example :)

  14k42 = 14 * 1000 + 14 * 100 + 2 * 10 = 14420

  Of course, it only pays off to use that notation when you can skip many 0's.
  There is not much point in writing 66k666 instead of 66666, except maybe
  when you want to make sure that you get 66,666.

  More than 3 (in case of 'k') or 6 ('m') digits after 'k'/'m' are automatically
  disregarded. Example:

  14k1234 = 14,123

  If the number contains any other characters than digits, 'k' or 'm', the
  function returns 0. It also returns 0 if 'k' or 'm' appear more than
  once.

*/

{

/* the pointer to buffer stuff is not really necessary, but originally I
   modified the buffer, so I had to make a copy of it. What the hell, it 
   works:) (read: it seems to work:)
*/

  char string[MAX_INPUT_LENGTH]; /* a buffer to hold a copy of the argument */
  char *stringptr = string; /* a pointer to the buffer so we can move around */
  char tempstring[2];       /* a small temp buffer to pass to atoi*/
  int number = 0;           /* number to be returned */
  int multiplier = 0;       /* multiplier used to get the extra digits right */


  strcpy (string,s);        /* working copy */

  while ( stringptr[0]>='0' && stringptr[0]<='9') /* as long as the current character is a digit */
  {
      strncpy (tempstring,stringptr,1);           /* copy first digit */
      number = (number * 10) + atoi (tempstring); /* add to current number */
      stringptr++;                                /* advance */
  }

  switch (UPPER(*stringptr)) {
      case 'K'  : multiplier = 1000;    number *= multiplier; stringptr++; break;
      case 'M'  : multiplier = 1000000; number *= multiplier; stringptr++; break;
      case '\0' : break;
      default   : return 0; /* not k nor m nor NUL - return 0! */
  }

  while ( stringptr[0]>='0' && stringptr[0]<='9' && (multiplier > 1)) /* if any digits follow k/m, add those too */
  {
      strncpy (tempstring,stringptr,1);           /* copy first digit */
      multiplier = multiplier / 10;  /* the further we get to right, the less are the digit 'worth' */
      number = number + (atoi (tempstring) * multiplier);
      stringptr++;
  }

  if (stringptr[0]<'0' && stringptr[0]>'9' && stringptr!=NULL && stringptr!='\0') /* a non-digit character was found, other than NUL */
    return 0; /* If a digit is found, it means the multiplier is 1 - i.e. extra
                 digits that just have to be ignore, liked 14k4443 -> 3 is ignored */


  return (number);
}


int parsebid (const int currentbid, const char *argument)
{

  int newbid = 0;                /* a variable to temporarily hold the new bid */
  char string[MAX_INPUT_LENGTH]; /* a buffer to modify the bid string */
  char *stringptr = string;      /* a pointer we can move around */

  strcpy (string,argument);      /* make a work copy of argument */


  if (*stringptr)               /* check for an empty string */
  {

    if (stringptr[0]>='0' && stringptr[0]<='9') /* first char is a digit assume e.g. 433k */
		newbid = advatoi (stringptr); /* parse and set newbid to that value */

    else
      if (*stringptr == '+') /* add ?? percent */
      {
        if (strlen (stringptr) == 1) /* only + specified, assume default */
          newbid = (currentbid * 125) / 100; /* default: add 25% */
        else
          newbid = (currentbid * (100 + atoi (++stringptr))) / 100; /* cut off the first char */
      }
      else
        {
		  if ((*stringptr == '*') || (*stringptr == 'x')) /* multiply */
          if (strlen (stringptr) == 1) /* only x specified, assume default */
            newbid = currentbid * 2 ; /* default: twice */
          else /* user specified a number */
            newbid = currentbid * atoi (++stringptr); /* cut off the first char */
        }
  }

  return newbid;        /* return the calculated bid */
}

