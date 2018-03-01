#include "utility.h"
#include<assert.h>

extern inode *root;
extern inode * currdirectory;
extern filetable *FileTable;
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
  inode * head= currdirectory->inodeList;
  while(head!=NULL)
  {
    if(strcmp(head->name,name)==0 && strcmp(head->type,type)==0)
    {
      return head;
    }
    else
    {
      head= head->nextdirentry;
    }
  }
  return NULL;
}

//filetable utility functions

void showFileTableContents()
{
  printf("\n----------Showing all the filetable entries----------\n");
  filetable * entry=FileTable;
  while(entry!=NULL)
  {
    printf("\t-->filedescriptor:%d\tprocess_id:%d\tcurrfilepointer:%d\n",entry->filedescriptor,entry->who,entry->currfilepointer);
    entry=entry->nextfiletableentry;
  }
  printf("-------------------------------------------------------\n");
}


//inode utility functions

int getSizeOfFile(inode * file)
{
  int noOfDatablocks = file->noOfDatablocks;
  datablock * head = file->datablocksarray;
  int i=0;
  int size=0;
  while((i<noOfDatablocks)&&(head!=NULL))
  {
    size+=head->currsize;
    i++;
    head=head->nextdatablock;
  }
  return size;
}

//datablock utility functions

//superblock utility functions

//automount utility functions

//dumpfs utility functions
