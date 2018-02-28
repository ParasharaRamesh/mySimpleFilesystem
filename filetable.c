#include "globalConstants.h"
#include "utility.h"
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

filetable * FileTable=NULL;

filetable* getEntry(int fd,pid_t who)
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


int addEntry(inode *file,pid_t who)
{
  int fd=file->id;
  int count=0;
  filetable * head;
  if(FileTable == NULL)//create the first filetable nextfiletableentry
  {
    FileTable=(filetable *)malloc(sizeof(filetable));
    if(FileTable==NULL)
    {
      return 0;
    }
    head=FileTable;
  }
  else
  {
    head=FileTable;
    while(head->nextfiletableentry!= NULL)
    {
      count++;
      head= head->nextfiletableentry;
    }
    if(count>=FILETABLE_SIZE)
    {
      fprintf(stderr,"FILETABLE SIZE EXCEEDED!\n");
      return 0;
    }
  }
  head->filedescriptor=fd;
  head->nextfiletableentry=NULL;
  head->currfilepointer=0;
  head->who=who;
  return 1;
}

int removeEntry(inode *file,pid_t who)
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
