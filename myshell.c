#include "sfs.h"
#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "globalConstants.h"

//purpose of this file is to mimic the shell 
//and support the following commands and call the appropriate handlers
extern int currentshellpid;
extern superblock *sfssuperblock;

int main()
{
	int choice;
	char *disk = "PersistentDisk.txt";
	FILE *file;
	printf("Do you want to load the previous filesystem state?(1 for yes ,0 for no)\n");
	scanf("%d",&choice);
	if(choice == 0)
	{
		file = fopen(disk,"w+"); //creates fresh file
	}
	else if(choice == 1)
	{
		file = fopen(disk,"r+"); //appends to file
	}
	currentshellpid=getpid();
	printf("\n\ncurrent shell pid is %d\n\n",currentshellpid);
	char *command=NULL;
	char *otherpartofinput=NULL;
	char input[50];
	char prompt[3]=">>\0";//print this every line as the prompt of our shell
	if(!mkfs(choice))
	{
		printf("mkfs failed!\n");
		exit(0);
	}
	if( fseek(file,0,SEEK_END) != 0)
	{
		printf("Fseek Failed\n");
		exit(0);
	}
	printf("Welcome to Simple FileSystem\n");
	printSupportedCommands();

	while(1)
	{
		//take continous input and switch case each of the supoorted functions

		printf("%s",prompt);
		//scanf("%[^\n]%*c", input);
		//fgets(input, 50, stdin);
		//printf("\nInput: %s",input);
		//input[strlen(input)] = '\n';
		gets(input);

		if(strcmp(input,"quit")==0)
		{
			printf("Successfully saved the current state\n");
			exit(0);
		}

		if( fwrite("\n",1,1,file) != 1)
		{
			printf("Write into file failed\n");
		}
		fwrite(input,strlen(input),1,file);

		command=getCommandFromInput(input);

		if(strcmp(command,"touch")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfscreate(otherpartofinput))
			{
					printf("cannot create a file!!\n");
			}

		}
		else if(strcmp(command,"switch")==0)
		{
			printf("1\n");
			char **tokens=split(input,2);
			printf("2\n");
			int pid=atoi(tokens[1]);
			printf("%d\n",pid);
			currentshellpid=pid;
			printf("currentshellpid is changed %d\n",currentshellpid);
		}
		else if(strcmp(input,"ls")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
			if(!sfsreaddir())
			{
				printf("empty!!\n");
			}
			showFileTableContents();//debug line
		}

		else if(strcmp(command,"mkdir")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfsmkdir(otherpartofinput))
			{
				printf("cant create a directory!!\n");
			}
		}
		else if(strcmp(command,"cd")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
			otherpartofinput=getOtherPartFromInput(input);
			if(!sfschangedir(otherpartofinput))
			{
				printf("cant create a directory!!\n");
			}
		}
		else if(strcmp(command,"rmdir")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
				otherpartofinput=getOtherPartFromInput(input);
				if(!sfsrmdir(otherpartofinput))
					printf("Successfully deleted directory\n");

		}
		else if(strcmp(command,"rm")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
				otherpartofinput=getOtherPartFromInput(input);
				if(!sfsdelete(otherpartofinput))
				{
					printf("cant delete this file!!\n");
				}
		}

		else if(strcmp(command,"close")==0)
		{
			printf("current shell pid is %d\n",currentshellpid);
				char **tokens=split(input,2);
				char *filename=tokens[1];
				if(!sfsclose(filename,currentshellpid))
				{
					printf("cant close this file!!\n");
				}
				showFileTableContents();//debugline
		}

		else if(strstr(command,"write")==command)
		{
			printf("current shell pid is %d\n",currentshellpid);
			char ** tokens =split(input,3);
			char * name= tokens[1];
			char * content =(char *)malloc(sizeof(char)*MAX_CONTENT_LIMIT);
			printf("enter the content you want to write into this file\n");
			//fgets(content, MAX_CONTENT_LIMIT, stdin);
			scanf("%[^\n]%*c",content);
			strcat(content,"\n");
			if( fwrite("\n",1,1,file) != 1)
			{
				printf("Write into file failed\n");
			}
			if( fwrite(content,strlen(content),1,file) != 1)
			{
				printf("Write into file failed\n");
			}
			//printf("here%s,%s,%d\n",name,content);
			if(!sfsopen(name,currentshellpid))
			{
				printf("cant open this file!\n");
			}
			//printf("shaa\n");
			showFileTableContents();//debug line
			//printf("shaa\n");
			if(!sfswrite(name,currentshellpid,content))
			{
				printf("cant write into this file!\n");
			}
			showFileTableContents();//debug line
			free(content);
		}

		else if(strstr(command,"read")==command)
		{
			printf("current shell pid is %d\n",currentshellpid);
			char ** tokens =split(input,3);
			printf("0\n");
			char * name= tokens[1];
			int nbytes =atoi(tokens[2]);
			int who=currentshellpid;
			printf("%s\t%d\t%d",name,nbytes,who);
			printf("a\n");
			if(!sfsopen(name,who))
			{
				printf("cant open this file!\n");
			}
			printf("b\n");
			char * readstr=sfsread(name,who,nbytes);
			if(readstr==NULL)
			{
				printf("cant write into this file!\n");
			}
			printf("read string is :%s\n",readstr);
		}

		else if(strstr(command,"cat")==command)
		{
			printf("current shell pid is %d\n",currentshellpid);
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
				int who=currentshellpid;
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
			printf("current shell pid is %d\n",currentshellpid);
			char ** tokens =split(input,3);
			char *filename=tokens[1];
			int offset=atoi(tokens[2]);
			int who =currentshellpid;

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
