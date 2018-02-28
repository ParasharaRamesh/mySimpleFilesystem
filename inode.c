#include "globalConstants.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern inode *root;
extern inode *currdirectory;
extern superblock *sfssuperblock;

int createInode(char * name, char * type)
{
    inode *newInode = getInode();//from superblock
    //If the new Inode is the first entry, make it the head.
    if(newInode==NULL)
    {
        return 0;
    }
    if(currdirectory->noOfInodes==0)
    {
      currdirectory->inodeList=newInode;
    }
    //Else append the newInode to the end of the inodeList
    else
    {
      inode *temp=currdirectory->inodeList;

      while(temp->nextdirentry!=NULL)
      {
        if( strcmp(temp-> name,name)==0)
        {
          return 0;
        }
        temp=temp->nextdirentry;
      }
      temp->nextdirentry=newInode;

    }

    strcpy(newInode->name,name);
    strcpy(newInode->type,type);
    newInode->createtime=time(0);
    newInode->datablocksarray=NULL;
    newInode->noOfDatablocks=0;
    newInode->inodeList=NULL;
    newInode->noOfInodes=0;
    newInode->parent=currdirectory;//global
    newInode->fdcount=0;
    newInode->nextdirentry=NULL;
    //Increasing the count of the number of inodes
    currdirectory->noOfInodes+=1;

    return 1;
}
int deleteInode(inode *entry)
{
    if(entry->noOfInodes!=0)
    {
      return 0;
    }
    inode *prev;
    inode *temp=currdirectory->inodeList;
    if(strcmp(entry->type,"file")==0)
    {
      unlinkDataBlock(entry);
    }
    if(temp==entry)
    {
      sfssuperblock->inodelist[entry->id]=0;
      entry->nextdirentry=NULL;
      currdirectory->inodeList=temp->nextdirentry;
      sfssuperblock->no_of_free_inodes++;
      currdirectory->noOfInodes--;
    }
    else{
      while(temp!=entry)
      {
        prev=temp;
        temp=temp->nextdirentry;
      }
      //If there was no matching directory entry
      if(temp==NULL)
        return 0;
      prev->nextdirentry=temp->nextdirentry;
      sfssuperblock->inodelist[entry->id]=0;
      sfssuperblock->no_of_free_inodes++;
      currdirectory->noOfInodes--;
      return 1;
    }

}
