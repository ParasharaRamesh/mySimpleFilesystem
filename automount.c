#include "sfs.h"
#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include "globalConstants.h"
<<<<<<< HEAD

int automount()
{
    char *disk = "PersistentDisk.txt";
    FILE *file = fopen(disk,"r");
    char *command;
    char *otherpartofinput;
    char input[50];
    while( fgets(input, sizeof(input), file) != NULL )
    {
        input[strlen(input) - 1] = '\0';
        command = getCommandFromInput(input);
        if(strcmp(command,"touch")==0)
		{
			otherpartofinput=getOtherPartFromInput(input);
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
			if(!sfsclose(filename,currentshellpid))
			{
				printf("cant close this file!!\n");
			}
		}

		else if(strstr(command,"write")==command)
		{
			char ** tokens =split(input,3);
			char * name= tokens[1];
			char * content =(char *)malloc(sizeof(char)*MAX_CONTENT_LIMIT);
			if( fgets(content,sizeof(content),file) == NULL)
            {
                printf("ERROR : Failed to read content\n");
            }
            if(!sfsopen(name,currentshellpid))
			{
				printf("cant open this file!\n");
			}
			if(!sfswrite(name,currentshellpid,content))
			{
				printf("cant write into this file!\n");
			}
			free(content);
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
			showFileTableContents();//debug line
		}
		//all other commands in elseif ladder c doesnt support switch for strings :(
		command="";
		otherpartofinput="";
    }
    fclose(file);
    return 1;
}


//extern filetable *FileTable;
//extern superblock *sfssuperblock;//new
//extern inode *currdirectory;
/*superblock *superblockDiskRead(char *disk)
=======
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern filetable *FileTable;
extern superblock *sfssuperblock;//new
extern inode * currdirectory;//one for root

superblock *superblockDiskRead(char *disk)
>>>>>>> b512ff022842b8fb807261ed6a83b895a056d856
{
    superblock* sfssuperblock = (superblock *)malloc(sizeof(superblock));
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return NULL;
    }
    if(fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return NULL;
    }
    if(fread(sfssuperblock , sizeof(superblock), 1 , file) != 1)
    {
        return NULL;
    }
    fclose(file);
    return sfssuperblock;
}

filetable *filetableDiskRead(char *disk)
{
    filetable *FileTable = (filetable *)malloc(sizeof(filetable));
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return NULL;
    }
    if(fseek( file , L_FILETABLE , SEEK_SET) != 0 )
    {
        return NULL;
    }
    if(fread(FileTable, sizeof(filetable) , 1 , file) != 1)
    {
        return NULL;
    }
    fclose(file);
    return FileTable;
}

int automount()
{
    char *disk="PersistantDisk.txt";

  //memcpy(sfssuperblock,superblockDiskRead(disk),sizeof(superblock));
  sfssuperblock = superblockDiskRead(disk);
  if(sfssuperblock==NULL)
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
<<<<<<< HEAD
  currdirectory=&sfssuperblock->inodes[0];
  return 1; 
} */
=======
  inode *root=&sfssuperblock->inodes[0];
  currdirectory=root;
  printf("previous currdirectory assigned!\n");
  printf("name of current directory is %s\n",currdirectory->name);
  printf("%d\t%d\n",sfssuperblock->no_of_free_inodes,sfssuperblock->no_of_free_datablocks);
  if(FileTable==NULL)
  {
    printf("yeah bro its supposed to be null!\n");
  }
  else
  {
    printf("looks like something is there!\n");
    printf("\t-->filedescriptor:%d\tprocess_id:%d\tcurrfilepointer:%d\n",FileTable->filedescriptor,FileTable->who,FileTable->currfilepointer);
  }
  //showFileTableContents();
  return 1;
}
>>>>>>> b512ff022842b8fb807261ed6a83b895a056d856
