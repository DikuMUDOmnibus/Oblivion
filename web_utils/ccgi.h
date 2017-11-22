int get_var(const char *source, char *dest, const char *var);
int decode_hex (const char *hex, char *des);


/*		Created 3/5/96 to look up variables in a stream from a fill out form.
		Last Modified 4/13/96 to fix a couple of relatively minor bugs
		© 1996 Brant Sears 

		returns -1 on failure and the number of characters copied to dest if successful
		this is useful if your variable is a checkbox
		
		If you like these functions, use them. If you use them 
		send me email and tell me how you like them, report problems, etc.
		to:
		
		 searsbe@sluvca.slu.edu
		
		** If you really like this, the going rate is to send me 
		some coffee from your corner of the world (thanks, Ronan) **
		Brant Sears
		c/o IBM
		555 Bailey Ave
		San Jose, CA 95150
*/	
		
int get_var(const char *source, char *dest, const char *var)
{
int c;
char h,i;
int size;
char *p_copy;




p_copy=strstr(source, var);

if (p_copy)
        ;
else /* A test for a Null pointer. If one is there, it is a checkbox which is off */
        {
        return(-1);
        }

size = strlen (var);


i=0;
c=0;
while (i != '&')
{
        i = *(p_copy+size+c);

        switch(i)
                {
                case '+':
                *(dest+c) = ' ';
                c++;
                break;

                case '&':
                break;
        
                case '%':
                if (decode_hex((p_copy+size+c), &i))
                        *(dest+c) = i;
                i= '$';
                p_copy += 2;
                c++;
                break;

                default:
                *(dest+c) = i;
                c++;
                break;
                } /*end switch*/
/*c++;*/
} /* end while */
*(dest+c) = '\0';
/*if (c==1)
	*(dest) = '\0';
*/
return (c);
}

/*      decode_hex takes a pointer to the spot in the array to decode and a pointer to the destination
        Added 3/7/96 at 1:30am ©1996 Brant Sears
*/
int decode_hex (const char *hex, char *des)
{
char string[3];
int i;
long l;
if ((sscanf(hex, "%%%2s", &string))==0)
        return(0);
/*printf("%s ", string);*/
l=strtol(string, NULL, 16);
i = (int) l;
/*printf("%c<BR>", i);*/
*des = (char) i;
return(1);
}
