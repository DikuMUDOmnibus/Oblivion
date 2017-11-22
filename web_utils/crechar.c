#include <stdio.h>
#include "ccgi.h"

int main()
{
	int i, allowed;
	FILE *outfile,*charfile;
	char DATA[30000];
	char buf[256],buf2[256];
	double length;
	const char * str_len = getenv("CONTENT_LENGTH");
	char stream[30000];
	int pid;

	scanf("%s", &stream);
	stream[strlen(stream)]= '&';

	length = strtod(str_len, NULL);

	for(i=0; i<length; i++)
	{ fscanf(stdin, "%c", &DATA[i]); }

	if((outfile=fopen("sendtext.txt","w"))==NULL)
	{
		printf("Content-type: text/html\n\n Script not working."\
			" Write error.");
		return 0;
	}

	printf("Content-type: text/html\n\n Script is done.");

	if((get_var(stream,buf,"PC_Allow="))==-1)
	{
	 allowed=0;
	 fprintf(outfile,"We regretfully inform you that your application for a character has been\ndenied. Below is a summery of your application and a reason as to why it was\nunacceptable.  If your application was denied for roleplaying reasons you may\n");
	 fprintf(outfile,"wish to contact voltron@cs.wisc.edu for help.\n");
	}
	else
	{
	 allowed=1;
	 fprintf(outfile,"Congratulations, your application has been accepted and your character created.\n Please telnet to harker.dnaco.net 1234 and type in your character name.\n You will be prompted to put in a password, just type in anything and after\n");
	 fprintf(outfile,"you are done creating use the command \"password (null) <password>\" to set your\npassword permenantly, if you do not do this, anyone could log in as your\ncharacter.\n");
	 fprintf(outfile,"After which you will be prompted to choose your various skills.  These are only\nthe skills which you begin with, and you can gain more later as you gain levels.\nMost players find that an xp per level range of between 1200 and 2500 is best.\nThank you for applying, and have fun roleplaying.\nYour application was as follows:\n"); 
	}
	
	get_var(stream,buf,"PC_Name=");
	fprintf(outfile,"Name: %s\n",buf);	
	get_var(stream,buf,"PC_Race=");
	fprintf(outfile,"Race: %s\n",buf);
	get_var(stream,buf,"PC_Class=");
	fprintf(outfile,"Class: %s\n",buf);
	get_var(stream,buf,"PC_Sex=");
	fprintf(outfile,"Gender: %s\n",buf);
	get_var(stream,buf,"PC_Alliance=");
	fprintf(outfile,"Alliance: %s\n", buf);
	get_var(stream,buf,"PC_Tendency=");
	fprintf(outfile,"Tendency: %s\n",buf);
	get_var(stream,buf,"PC_Description=");
	fprintf(outfile,"Description:\n%s\n",buf);
	get_var(stream,buf,"PC_History=");
	fprintf(outfile,"History:\n%s\n",buf);

	if(allowed==0)
	{
	get_var(stream,buf,"PC_Reason=");
	fprintf(outfile,"Reason for Rejection:\n%s\n",buf);
	}

	fprintf(outfile,"Thank you for applying, and even if you were rejected, feel free to try again.\n-The Administration of Oblivion MIRPG\n");

	close(outfile);

	pid=fork();

	if(pid==0)
	{
	  get_var(stream,buf,"PC_Email=");
	  sprintf(buf2,"mail %s < sendtext.txt",buf);
	  system(buf2);
          return 0;
	}
		
	

        if(allowed==1)
	{
	 get_var(stream,buf,"PC_Name=");
	 sprintf(buf2,"pfiles/%s",buf);
	 charfile=fopen(buf2,"w");
	 fprintf(charfile,"#PLAYER\nVersion 0\n");
	 get_var(stream,buf,"PC_Name=");
	 fprintf(charfile,"Name %s~\n",buf);
	 get_var(stream,buf,"PC_Race=");
	 fprintf(charfile,"Race %s~\n",buf);
	 get_var(stream,buf,"PC_Description=");
	 fprintf(charfile,"Desc %s~\n",buf);
	 get_var(stream,buf,"PC_Sex=");
	 fprintf(charfile,"TSex %d\n", strcmp(buf,"Male") ? 2 : 1);
	 get_var(stream,buf,"PC_Class=");
	if(strcmp(buf,"Mage")==0)
	 fprintf(charfile,"Cla 0");
	else if(strcmp(buf,"Cleric")==0)
	 fprintf(charfile,"Cla 1");
	else if(strcmp(buf,"Thief")==0)
	 fprintf(charfile,"Cla 2");
	else if(strcmp(buf,"Warrior")==0)
	 fprintf(charfile,"Cla 3");
	else if(strcmp(buf,"Druid")==0)
	 fprintf(charfile,"Cla 4");
	else if(strcmp(buf,"Paladin")==0)
	 fprintf(charfile,"Cla 5");
	else if(strcmp(buf,"Ranger")==0)
	 fprintf(charfile,"Cla 6");
	else if(strcmp(buf,"Barbarian")==0)
	 fprintf(charfile,"Cla 7");
	else if(strcmp(buf,"Spellfilcher")==0)
	 fprintf(charfile,"Cla 8");
	 get_var(stream,buf,"PC_Tendency=");
	 get_var(stream,buf2,"PC_Alliance=");
	 fprintf(charfile,"\nAlig %d %d\n",
		atoi(buf),atoi(buf2));
	 fprintf(charfile,"End\n#END\n");
	 close(charfile);
	}
	exit(1);
	return 0;
}
