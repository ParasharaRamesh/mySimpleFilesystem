#include "sfs.h"
#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/time.h>

//purpose of this file is to mimic the shell and support the following commands and call the appropriate handlers

int main()
{
	char *command=NULL;
	char *otherpartofinput=NULL;
	char input[50];
	char prompt[3]=">>\0";//print this every line as the prompt of our shell
	printf("Welcome to Simple FileSystem\n");
	printSupportedCommands();

	mkfs();
	while(1)
	{
		//take continous input and switch case each of the supoorted functions

		printf("%s",prompt);
		scanf("%[^\n]%*c", input);
		if(strcmp(input,"quit")==0)
		{
			exit(0);//eventually dumpfs from here!
		}

		command=getCommandFromInput(input);

		if(strcmp(command,"touch")==0)
		{
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfscreate(otherpartofinput))
			{
					printf("cannot create a file!!\n");
			}

		}
		else if(strcmp(input,"ls")==0)
		{
			if(!sfsreaddir())
			{
				printf("empty!!\n");
			}
			showFileTableContents();//debug line
		}

		else if(strcmp(command,"mkdir")==0)
		{
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfsmkdir(otherpartofinput))
			{
				printf("cant create a directory!!\n");
			}
		}
		else if(strcmp(command,"cd")==0)
		{
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfschangedir(otherpartofinput))
			{
				printf("cant create a directory!!\n");
			}
		}
		else if(strcmp(command,"rmdir")==0)
		{
				otherpartofinput=getOtherPartFromInput(input);
				if(!sfsrmdir(otherpartofinput))
					printf("Successfully deleted directory\n");

		}
		else if(strcmp(command,"rm")==0)
		{
				otherpartofinput=getOtherPartFromInput(input);
				if(!sfsdelete(otherpartofinput))
				{
					printf("cant delete this file!!\n");
				}
		}

		else if(strcmp(command,"close")==0)
		{
				char **tokens=split(input,2);
				char *filename=tokens[1];
				if(!sfsclose(filename,getpid()))
				{
					printf("cant close this file!!\n");
				}
				showFileTableContents();//debugline
		}

		else if(strstr(command,"write")==command)
		{

			char ** tokens =split(input,3);
			char * name= tokens[1];
			char * content =(char *)malloc(sizeof(char)*MAX_CONTENT_LIMIT);
			printf("enter the content you want to write into this file\n");
			fgets(content, MAX_CONTENT_LIMIT, stdin);
			//printf("here%s,%s,%d\n",name,content);
			pid_t who=getpid();
			if(!sfsopen(name,who))
			{
				printf("cant open this file!\n");
			}
			showFileTableContents();//debug line
			if(!sfswrite(name,who,content))
			{
				printf("cant write into this file!\n");
			}
			showFileTableContents();//debug line
			free(content);
		}

		else if(strstr(command,"read")==command)
		{
			char ** tokens =split(input,3);
			printf("0\n");
			char * name= tokens[1];
			int nbytes =atoi(tokens[2]);
			pid_t who=getpid();
			printf("%s\t%d\t%d",name,nbytes,who);
			printf("a\n");
			if(!sfsopen(name,who))
			{
				printf("cant open this file!\n");
			}
			printf("b\n");
			if(!sfsread(name,who,nbytes))
			{
				printf("cant write into this file!\n");
			}
			printf("c\n");
		}

		else if(strstr(command,"cat")==command)
		{
			//read filename nbytes
			//otherpartofinput=getOtherPartFromInput(input);
			char ** tokens =split(input,2);
			char * filename= tokens[1];
			inode *file=getInodeFromCurrDirectory(filename,"file");
		  if(file == NULL)
		  {
		    printf("file doesnt exist!\n");
		  }
			//int space=strstr(command," ");
			else
			{
				pid_t who=getpid();
				char *content = readDataBlocks(file);
				if(content == NULL)
				{
					printf("cant read all the content in this file!\n");
				}
				else
				{
					printf("\n%s",content);
				}
			}
			showFileTableContents();//debug line
		}
		//lseek filename offset
		else if(strstr(command,"lseek")==command)
		{
			char ** tokens =split(input,3);
			char *filename=tokens[1];
			int offset=atoi(tokens[2]);
			pid_t who =getpid();

			if(!sfslseek(filename,who,offset))
			{
				printf("Can't set file pointer \n");
			}
			showFileTableContents();//debug line
		}
		//all other commands in elseif ladder c doesnt support switch for strings :(
		command="";
		otherpartofinput="";
	}

	if(command!=NULL)
	{
		free(command);
	}
	if(otherpartofinput!=NULL)
	{
		free(otherpartofinput);
	}

	return 0;
}
