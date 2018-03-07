#include "utility.h"
#include<assert.h>

extern inode *root;
extern inode * currdirectory;
extern filetable FileTable[5];
extern superblock sfssuperblock;//new
extern int currentshellpid;

//myshell uttility functions
void printSupportedCommands()
{
  /*
  commands to support:
  ls,mkdir,rmdir,cd ->,cd ->->,open,write,read,close,create,rm,lseek

  */
  printf("\n");
  printf("Commands supported :\n");//write all the supported commands and its brief usage
  printf("ls     :list the current directory\n");
    printf("\tUsage : ls <dirname>\n");
  printf("cd     :change the current directory\n");
    printf("\tUsage : cd <dirname>\n");
  printf("rmdir  :delete a directory in the current directory\n");
    printf("\tUsage : rmdir <dirname>\n");
  printf("rm     :delete a file in the current directory\n");
    printf("\tUsage : rm <filename>\n");
  printf("lseek  :reset the currentfilepointer of the process opening a particular file \n");
    printf("\tUsage : lseek <filename> <offset>\n");
  printf("touch  :create a file in the current directory\n");
    printf("\tUsage : touch <filename>\n");
  printf("cat    :read a particular file in the current directory\n");
    printf("\tUsage : cat <filename>\n");
  printf("write  :write into a particular file in the current directory\n");
    printf("\tUsage : write <filename> and then enter the content\n");
  printf("read   :read nbytes from a particular file in the current directory\n");
    printf("\tUsage : read <filename> <nbytes>\n");
  printf("close  :close the file which was opened by the current terminal's process\n");
    printf("\tUsage : close <filename>\n");
  printf("\n\n");
}

char ** split(char * input,int noofwords)
{
	//initialize 2d array
	char **result=(char **)malloc(sizeof(char *)*noofwords);
	char *token = strtok(input," ");
	int i=0;
//  printf("aaa\n");
	while (token != NULL)
	{
		//printf("%s\t%d\n",token,i);
		//append into 2d array somehow!
		result[i]=token;
		//printf("\t%s\n",result[i]);
		i++;
		token = strtok(NULL, " ");
	}
	if(token==NULL)
	{
		return result;
	}
}





char * getCommandFromInput(char * input)
{
    //iterate until the first space and get only the command
    int i=0;
    int endpoint=0;
    int inputlen=strlen(input);
    for(i=0;i<inputlen;i++)
    {
        if(*(input+i)==' '||*(input +i)=='\n')
        {
            endpoint=i-1;
            break;
        }
    }
    char *command=(char *)(malloc(sizeof(char)*(endpoint+1)));
    strncpy(command,input,endpoint+1);
    return command;

}

char * getOtherPartFromInput(char * input)
{
    int i=0;
    int endpoint=0;
    int inputlen=strlen(input);
    for(i=0;i<inputlen;i++)
    {
        if(*(input+i)==' ')
        {
            endpoint=i+1;
            break;
        }
    }
    char *command=(char *)(malloc(sizeof(char)*(inputlen-endpoint)));
    for(i=endpoint;i<inputlen;i++)
    {
        *(command+(i-endpoint))=*(input+i);
    }
    return command;
}

//filesystem utility functions
inode * getInodeFromCurrDirectory(char *name,char *type)
{
  //inode * head= currdirectory->inodeList;
  for(int i=0;i<5;i++)
  {
    if(strcmp(sfssuperblock.inodes[currdirectory->inodeList[i]].name,name)==0 && strcmp(sfssuperblock.inodes[currdirectory->inodeList[i]].type,type)==0)
    {
      return &sfssuperblock.inodes[currdirectory->inodeList[i]];
    }
  }
  return NULL;
}

//filetable utility functions

void showFileTableContents()
{
  printf("\n----------Showing all the filetable entries----------\n");
  //filetable * entry=FileTable;
  for(int i=0;i<5;i++)
  {
    if(FileTable[i].used==1)
    {
      printf("\t-->filedescriptor:%d\tprocess_id:%d\tcurrfilepointer:%d\n",FileTable[i].filedescriptor,FileTable[i].who,FileTable[i].currfilepointer);
      //entry=entry->nextfiletableentry;
    }
  }
  printf("-------------------------------------------------------\n");
}


//inode utility functions

int getSizeOfFile(inode * file)
{
  int noOfDatablocks = file->noOfDatablocks;
  int size=0;
  for(int i=0;i<noOfDatablocks;i++)//only the first i datablocks in the datablocksarray should be used
  //while((i<noOfDatablocks)&&(head!=NULL))
  {
    size+=sfssuperblock.datablocks[file->datablocksarray[i]].currsize;
  }
  return size;
}

//datablock utility functions

//superblock utility functions

//automount utility functions

//dumpfs utility functions
void plog(FILE * file,char *input,char *content,int mode)
{
  if(mode==0)//for all other cases
  {
    if( fwrite("\n",1,1,file) != 1)
    {
      printf("Write into file failed\n");
    }
    fwrite(input,strlen(input),1,file);
  }
  else if(mode==1)//only for write case
  {
    if( fwrite("\n",1,1,file) != 1)
    {
      printf("Write into file failed\n");
    }
    if( fwrite(input,strlen(input),1,file) != 1)
    {
      printf("Write into file failed\n");
    }
    if( fwrite(" ",1,1,file) != 1)//all the content is after a space
    {
      printf("Write into file failed\n");
    }
    if( fwrite(content,strlen(content),1,file) != 1)
    {
      printf("Write into file failed\n");
    }
  }
}
