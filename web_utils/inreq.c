#include <stdio.h>
#include "ccgi.h"

int main()
{
	int i;
	FILE *outfile;
	char DATA[30000];
	char buf[256];
	double length;
	const char *str_len = getenv("CONTENT_LENGTH");
	char stream[30000];

        scanf("%s", &stream);
	stream[strlen(stream)]= '&';
 
        length = strtod(str_len, NULL);

	for(i=0; i<length; i++)
	{ fscanf(stdin, "%c", &DATA[i]); }

	if((outfile=fopen("req.html","a"))==NULL)
	{
		printf("Content-type: text/html\n\n Sorry, but the script is not working right now. Please Try Again later.\n");
		exit(1);
	}
	
	get_var(stream,buf,"PC_Email=");
	fprintf(outfile,"<FORM value=\"%s\" METHOD=POST ACTION=\"crechar.cgi\">\n",buf);
	fprintf(outfile,"Email: <input type=\"text\" maxlength=80 name=\"PC_Email\" value=\"%s\"><P>", buf);
	get_var(stream,buf,"PC_Name=");    
 	fprintf(outfile,"Name: <input type=\"text\" name=\"PC_Name\" value=\"%s\"><P>", buf);
	get_var(stream,buf,"PC_Race=");
	fprintf(outfile,"Race: <input type=\"text\" name=\"PC_Race\" value=\"%s\"><P>", buf);
	get_var(stream,buf,"PC_Class=");
	fprintf(outfile,"Class: <input type=\"text\" name=\"PC_Class\" value=\"%s\"><P>", buf);
	get_var(stream,buf,"PC_Sex=");
	fprintf(outfile,"Sex: <input type=\"text\" name=\"PC_Sex\" value=\"%s\"><P>", buf);
	get_var(stream,buf,"PC_Alliance=");
	fprintf(outfile,"Alliance: <input type=\"text\" name=\"PC_Alliance\" value=\"%d\"><P>", atoi(buf));
	get_var(stream,buf,"PC_Tendency=");
	fprintf(outfile,"Tendency: <input type=\"text\" name=\"PC_Tendency\" value=\"%d\"><P>", atoi(buf));
	get_var(stream,buf,"PC_Description=");
	fprintf(outfile,"Description:<P><TEXTAREA name=\"PC_Description\" ROWS=10 COLS=50>%s</TEXTAREA><P>",buf);
	get_var(stream,buf,"PC_History=");
	fprintf(outfile,"History:<P><TEXTAREA name=\"PC_History\" ROWS=10 COLS=50>%s</TEXTAREA><P>",buf);
	fprintf(outfile,"REASON:<P><TEXTAREA name=\"PC_Reason\" ROWS=10 COLS=50></TEXTAREA><P>");
	fprintf(outfile,"<input type=\"checkbox\" name=\"PC_Allow\" value=\"yes\">Allow Character<BR>");
    	fprintf(outfile,"<input type=submit value=\"Submit Character\"><P>");
	fprintf(outfile,"<CENTER><IMG SRC=\"devider4.gif\"></CENTER><P>\n");


   	printf(
"Content-type: text/html\n\n");
	printf(
"<HEAD>\n<TITLE>Submission Accepted</TITLE>\n<HEAD>");
	printf(
"<BODY BACKGROUND=\"../images/back2.jpg\" TEXT=\"FFFFFF\">\n\n");
	printf(
"<CENTER><IMG SRC=\"../images/oblivion.gif\"><P></CENTER>");
	printf(
"<CENTER><IMG SRC=\"../images/divide2.jpg\"><P></CENTER>");
	printf(
"Your application has been recieved, and you should recieve notice ");
	printf(
"usually within 24 hours but in no more then 1 week.<P>");


	return 0;
	

}
