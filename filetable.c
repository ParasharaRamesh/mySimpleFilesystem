#include "globalConstants.h"
#include "utility.h"
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

filetable * FileTable=NULL;

extern int currentshellpid;

filetable* getEntry(int fd,int who)
{
  //traverse the filetable and search for the entry with file's inode id and the who and
  filetable * head= FileTable;
  while(head!=NULL)
  {
    if((head->who==who)&&(head->filedescriptor==fd))
    {
      return head;
    }
    head=head->nextfiletableentry;
  }
  return NULL;
}


int addEntry(inode *file,int who)
{

printf("1\n");
  if(getEntry(file->id,who)!=NULL)
  {
    printf("this process already has this file open!\n");
    return 1;
  }
printf("2\n");
  int fd=file->id;
  int count=0;
  filetable * head;
  if(FileTable == NULL)//create the first filetable nextfiletableentry
  {
    printf("3\n");
    FileTable=(filetable *)malloc(sizeof(filetable));
    if(FileTable==NULL)
    {
      return 0;
    }
    head=FileTable;
    head->filedescriptor=fd;
    head->nextfiletableentry=NULL;
    head->currfilepointer=0;
    head->who=who;
    printf("4\n");
    return 1;
  }
  else
  {
    printf("5\n");
    head=FileTable;
    while(head->nextfiletableentry!= NULL)
    {
      printf("6\n");
      count++;
      head= head->nextfiletableentry;
    }

    printf("7\n");
    if(count>=FILETABLE_SIZE)
    {
      fprintf(stderr,"FILETABLE SIZE EXCEEDED!\n");
      return 0;
    }
    printf("8\n");
    filetable *temp=(filetable *)malloc(sizeof(filetable));
    printf("9\n");
    temp->filedescriptor=fd;
    temp->nextfiletableentry=NULL;
    temp->currfilepointer=0;
    temp->who=who;
    head->nextfiletableentry=temp;
    printf("10\n");
    return 1;
  }

}

int removeEntry(inode *file,int who)
{
  filetable * curr=FileTable;
  filetable * prev=NULL;
  while(curr!=NULL)
  {
    if( (curr->filedescriptor == file->id) && (curr->who == who) )
    {
      if(prev!=NULL)
      {
        prev->nextfiletableentry=curr->nextfiletableentry;
        free(curr);
        file->fdcount--;
        return 1;
      }
      else // curr first node
      {
          FileTable=curr->nextfiletableentry;
          free(curr);
          file->fdcount--;
          return 1;
      }
    }
    else
    {
      prev=curr;
      curr=curr->nextfiletableentry;
    }
  }
  return 0;
}


int flushfiletable(inode *file)
{
  //traverse this table and remove all the entries with the filedescriptor as this file
  int fd=file->id;
  filetable * head= FileTable;
  filetable * prev=NULL;
  if(head==NULL)
  {
    printf("cant flush an empty filetable!\n");
    return 0;
  }
  int count=0;
  while(head!=NULL)
  {
    if(head->filedescriptor == fd)
    {
      if(prev!=NULL)
      {
        prev->nextfiletableentry=head->nextfiletableentry;
        free(head);
      }
      else // curr first node
      {
          FileTable=head->nextfiletableentry;
          free(head);
      }
      count++;
    }
    else
    {
      prev=head;
      head=head->nextfiletableentry;
    }
  }
  if(count==0)
  {
    printf("this file is not open currently in the filetable!\n");
    return 0;//it has gone through the whole table
  }
  else
  {
    return 1;
  }
}
