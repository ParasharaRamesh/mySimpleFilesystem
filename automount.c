#include "sfs.h"
#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include "globalConstants.h"
/*
extern int currentshellpid;
extern superblock sfssuperblock;

int automount()
{
    char *disk = "PersistentDisk.txt";
    FILE *file = fopen(disk,"r");
    char *command=NULL;
    char input[50];
    while( fgets(input, sizeof(input), file) != NULL )
    {
        input[strlen(input) - 1] = '\0';
        command = getCommandFromInput(input);
        printf("a1\n");
        if(strcmp(command,"touch")==0)
		    {
			       char ** tokens=split(input,2);
             char *otherpartofinput=tokens[1];
			       if(!sfscreate(otherpartofinput))
        		{
        					printf("cannot create a file!!\n");
        		}
		    }
    		else if(strcmp(command,"switch")==0)
    		{
    	    char **tokens=split(input,2);
    			int pid=atoi(tokens[1]);
          currentshellpid=pid;
        }

    		else if(strcmp(command,"mkdir")==0)
    		{
          char ** tokens=split(input,2);
          char *otherpartofinput=tokens[1];
    			if(!sfsmkdir(otherpartofinput))
    			{
    				printf("cant create a directory!!\n");
    			}
    		}

    		else if(strcmp(command,"cd")==0)
    		{
          char ** tokens=split(input,2);
          char *otherpartofinput=tokens[1];
    			if(!sfschangedir(otherpartofinput))
    			{
    				printf("cant create a directory!!\n");
    			}
    		}
    		else if(strcmp(command,"rmdir")==0)
    		{
          char ** tokens=split(input,2);
          char *otherpartofinput=tokens[1];
    			if(!sfsrmdir(otherpartofinput))
    				printf("Successfully deleted directory\n");

    		}
    		else if(strcmp(command,"rm")==0)
    		{
          char ** tokens=split(input,2);
          char *otherpartofinput=tokens[1];
    			if(!sfsdelete(otherpartofinput))
    			{
    				printf("cant delete this file!!\n");
    			}
    		}

    		else if(strcmp(command,"close")==0)
    		{
    			char **tokens=split(input,2);
    			char *filename=tokens[1];
    			if(!sfsclose(filename,currentshellpid))
    			{
    				printf("cant close this file!!\n");
    			}
    		}

    		else if(strstr(command,"write")==command)
    		{
    			char ** tokens =split(input,3);
    			char * name= tokens[1];
    			char * content=tokens[2];
          strcat(content,"\n");
          if(!sfsopen(name,currentshellpid))
    			{
    				printf("cant open this file!\n");
    			}
    			if(!sfswrite(name,currentshellpid,content))
    			{
    				printf("cant write into this file!\n");
    			}
    		}

		//lseek filename offset
  		else if(strstr(command,"lseek")==command)
  		{
  			char ** tokens =split(input,3);
  			char *filename=tokens[1];
  			int offset=atoi(tokens[2]);
  			int who =currentshellpid;

  			if(!sfslseek(filename,who,offset))
  			{
  				printf("Can't set file pointer \n");
  			}
  		}
      printf("a2\n");
  		//all other commands in elseif ladder c doesnt support switch for strings :(
  		command="";
      printf("a3\n");
    }
    fclose(file);
    return 1;
}

*/
extern filetable FileTable[5];
extern superblock sfssuperblock;//new
extern inode *currdirectory;
int superblockDiskRead(char *disk)
{
  //  sfssuperblock = (superblock *)malloc(sizeof(superblock));
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return 0;
    }
    if(fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return 0;
    }
    if(fread(&sfssuperblock , sizeof(superblock), 1 , file) != 1)
    {
        return 0;
    }
    fclose(file);
    printf("closed the file\n");
    printf("shaata --%d\t%d",sfssuperblock.no_of_free_inodes,sfssuperblock.no_of_free_datablocks);
    fflush(stdout); // Will now print everything in the stdout buffer
    currdirectory=&sfssuperblock.inodes[0];
    sfsreaddir();
    return 1;
}

int filetableDiskRead(char *disk)
{
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return 0;
    }
    if(fseek( file , L_FILETABLE , SEEK_SET) != 0 )
    {
        return 0;
    }
    if(fread(FileTable, sizeof(filetable)*5 , 1 , file) != 1)
    {
        return 0;
    }
    fclose(file);
    return 1;
}

int automount()
{
    char *disk="PersistantDisk.txt";

  //memcpy(sfssuperblock,superblockDiskRead(disk),sizeof(superblock));
  if(!superblockDiskRead(disk))
  {
    return 0;
  }
  if(!filetableDiskRead(disk))
  {
    return 0;
  }
  return 1;
}

/*  if(sfssuperblock==NULL)
  {
    printf("automouting failed (1)!\n");
    return 0;
  }
  //memcpy(FileTable,filetableDiskRead(disk),sizeof(filetable));
  FileTable = filetableDiskRead(disk);
  if(FileTable==NULL)
  {
    printf("automouting failed (2)!\n");
    return 0;
  }
  currdirectory=&sfssuperblock.inodes[0]; */
