#include "globalConstants.h"
#include "sfs.h"
#include "utility.h"

extern inode * currdirectory;
extern inode * root;
extern filetable * FileTable;

void mkfs()
{
  superblockInit();
}

int sfscreate(char * name)
{
    //first get a free inode and then allocate the necessary inode parameters and set parent to currpointer and return error if currdirectory already has max directory entries
    return createInode(name,"file");
}


int sfsdelete(char *name)
{
  inode *file=getInodeFromCurrDirectory(name,"file");
  if(!flushfiletable(file))
  {
    printf("couldnt flush the filetable of all the rows containing this file!\n");
  }
  if(deleteInode(file))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int sfsopen(char * name,pid_t who)
{
  inode * file= getInodeFromCurrDirectory(name,"file");
  if(getEntry(file->id,who)!=NULL)
  {
    printf("this process already has this file open!\n");
    return 1;
  }
  else
  {
    if(addEntry(file,who)==1)
    {
      file->fdcount++;
      return 1;
    }
    else
    {
      printf("couldnt add the entry into the filetable for some reason! :(\n");
      return 0;
    }

  }

}

int sfsclose(char * name , pid_t who)
{
  inode * file= getInodeFromCurrDirectory(name,"file");
  return removeEntry(file,who);
}


int sfschangedir(char * name)
{
  if(name==NULL)
  {
    //cd into root
    currdirectory=root;
    printf("Current directory is now %s\n",currdirectory->name);
    return 1;
  }
  else if(strcmp(name,"..")==0 && strcmp(currdirectory->name,"root")==0)
    return 0; //Cannot change to parent directory of the root

  else if(strcmp(name,"..")==0)
  {
    //go to parent directory
    currdirectory=currdirectory->parent;
    printf("Current directory is now %s\n",currdirectory->name);
    return  1;
  }
  else
  {
    inode *curr=getInodeFromCurrDirectory(name,"directory");
    if(curr==NULL)
    {
      return 0;
    }
    else
    {
      currdirectory=curr;
      printf("Current directory is now %s\n",currdirectory->name);
      return 1;
    }
  }
}

int sfsreaddir()
{
  inode * currdir= currdirectory;
  inode * head=currdir->inodeList;
  if(head==NULL)
  {
    return 0;
  }
  while(head!=NULL)
  {
    if(strcmp(head->type,"file")==0)
    {
      printf("%s -- %c\n",head->name,'F');
    }
    else if(strcmp(head->type,"directory")==0)
    {
      printf("%s -- %c\n",head->name,'D');
    }
    head=head->nextdirentry;
  }
  return 1;
}

char *sfsread(char *filename,pid_t who,int nbytes)
{

  //get the inode of this filename and using the inode no and the who search the filetable and get the corresponding filetable entry
  //from that filetable entry get the currfilepointer and read nbytes from there
  printf("1\n");
  inode *file = getInodeFromCurrDirectory(filename,"file");
  printf("2\n");
  if(file == NULL)
  {
    printf("cant get this file from the current directory!\n");
    return NULL;
  }
  else
  {
    printf("3\n");
    int fd= file->id;
    printf("%d\t%d\n",fd,who);
    filetable * entry=getEntry(fd,who);
    printf("4\n");
    if(entry==NULL)
    {
      printf("entry doesnt exist!\n");
    }
    printf("5\n");
    int currfilepointer=entry->currfilepointer;
    int filesize=getSizeOfFile(file);
    if(nbytes>filesize)
    {
      printf("6\n");
      printf("cant read %d characters from this file!\n",nbytes);
      return NULL;
    }
    printf("7\n");
    char *content = readDataBlocks(file);
    printf("8\n");
    char *toreturn=(char *)malloc(sizeof(char)*nbytes);
    for(int i=0;i<nbytes;i++)
    {
      toreturn[i]=content[currfilepointer+i];
    }
    //strncpy(toreturn,&content[currfilepointer],nbytes);
    printf("\t\tread thing is %s\n",toreturn);
    return toreturn;
  }

}

int sfswrite(char *filename,pid_t who,char *content)
{
  printf("inside %s\t%d\t%s\n",filename,who,content);
  inode *file=getInodeFromCurrDirectory(filename,"file");
  if(file==NULL)
  {
    printf("File doesn't exist!\n");
    return 0;
  }
  int length2=strlen(content);
  filetable * entry=getEntry(file->id,who);
  int currfilepointer=entry->currfilepointer;
  char *blockcontent=content;
  printf("1\n");
  if(file->datablocksarray!=NULL)
  {
    printf("2\n");
    int filesize=getSizeOfFile(file);
    char *blockcontent=sfsread(filename,who,filesize);
    if(!unlinkDataBlock(file))
    {
      printf("couldnt unlink all the datablocks!\n");
      return 0;
    }
    if(currfilepointer==filesize)
    {
      blockcontent=realloc(blockcontent,filesize+length2);
      strcat(blockcontent,content);
    }
    else if(filesize>(currfilepointer+length2))
    {
      strncpy(&blockcontent[currfilepointer],content,length2-1);
    }
    else
    {
      blockcontent=realloc(blockcontent,currfilepointer+length2);
      strcpy(&blockcontent[currfilepointer],content);
    }
    printf("22\n");
  }
  if(file->datablocksarray!=NULL)
  {
    printf("unlinking error!\n");
    return 0;
  }
  if(file->datablocksarray==NULL)//first time writing
  {
    //all the old code is in readme
    //find out noofblocks required and write into it until the last but one and the last one special case you write
    //then we make sure the currfilepointer is updated and the datablocks are linked
    printf("3\n");
    int contentlen=strlen(blockcontent);
    int noofblocksreq=(int)(floor(((double)contentlen)/DATABLOCK_SIZE))+1;//dont know how to get floor so rewrite correctly
    printf("the no of blocks required for the first time writing is --%d--\n",noofblocksreq);
    char * currblockcontent=(char *)malloc(sizeof(char)*DATABLOCK_SIZE);
    strncpy(currblockcontent,blockcontent,DATABLOCK_SIZE);
    printf("first block contains --%s-- and the len is --%ld--\n",currblockcontent,strlen(currblockcontent));
    file->datablocksarray=writeDataBlock(currblockcontent);
    if(file->datablocksarray==NULL)
    {
      printf("couldt get the first datablock itself!\n");
      return 0;
    }
    file->noOfDatablocks++;
    //first datablock written !
    printf("4\n");
    datablock * head=file->datablocksarray;

    for(int i=1;i<noofblocksreq-1;i++)
    {

      printf("5\n");
      //put the next set of content for the currblockcontent here!
      strncpy(currblockcontent,&blockcontent[i*DATABLOCK_SIZE],DATABLOCK_SIZE);
      printf("curr block contains --%s-- and the len is --%ld--\n",currblockcontent,strlen(currblockcontent));
      head->nextdatablock=writeDataBlock(currblockcontent);
      if(head->nextdatablock==NULL)
      {
        printf("failure!\n");
        return 0;
      }
      head=head->nextdatablock;
      file->noOfDatablocks++;
      printf("6\n");
    }
    //last datablock to be written
    if(noofblocksreq>=2)
    {
      printf("7\n");
      strcpy(currblockcontent,&blockcontent[(noofblocksreq-1)*DATABLOCK_SIZE]);
      printf("curr block contains --%s-- and the len is --%ld--\n",currblockcontent,strlen(currblockcontent));
      head->nextdatablock=writeDataBlock(currblockcontent);
      if(head->nextdatablock==NULL)
      {
        printf("failure 2!\n");
        return 0;
      }
      printf("8\n");
      //go to the last datablock now and set uska next as null
      head=head->nextdatablock;
      head->nextdatablock=NULL;
      file->noOfDatablocks++;
    }
    entry->currfilepointer=contentlen;
    printf("the current filepointer is set to --%d--\n",entry->currfilepointer);
    printf("total no of datblocks in the file now are --%d--\n",file->noOfDatablocks);
    return 1;
  }
  return 0;

}



  int sfsmkdir(char * name)
  {
      return createInode(name,"directory");
  }
  int sfsrmdir(char * name)
  {

    inode *file=getInodeFromCurrDirectory(name,"directory");
    return deleteInode(file);
  }




int sfslseek(char *name,pid_t who,int offset)
{
  //find the inode with this name in currdirectory and then call isPresent in filetable using the filedescriptor and the who and then get that filetable entry and modify the curr file pointer
  inode * file=NULL;
  printf("1\n");
  file=getInodeFromCurrDirectory(name,"file");
  printf("2\n");
  if(file ==NULL)
  {
    printf("3\n");
    return 0;//couldnt get the inode with this name
  }
  else
  {
    printf("4\n");
    filetable * entry=NULL;
    entry =getEntry(file->id,who);
    printf("5\n");
    if(entry==NULL)
    {
      printf("6\n");
      return 0;//not present in the filetable so cant sfslseek
    }
    else
    {
      entry->currfilepointer=offset;
      printf("set currfilepointer to --%d--\n",entry->currfilepointer);
      return 1;
    }
  }
  printf("8\n");
  return 0;
}
