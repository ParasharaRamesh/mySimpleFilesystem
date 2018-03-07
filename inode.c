#include "globalConstants.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern inode *root;
extern inode *currdirectory;
extern superblock sfssuperblock;

int createInode(char * name, char * type)
{
    //printf("1\n");
    inode *newInode = getInode();//from superblock
    //If the new Inode is the first entry, make it the head.
    if(newInode==NULL)
    {
        return 0;
    }
    //printf("2\n");
    if(currdirectory->noOfInodes==0)
    {
        //printf("3\n");
        //printf("newinode->id is %d\n",newInode->id);
      currdirectory->inodeList[0]=newInode->id;
      currdirectory->noOfInodes++;
      strcpy(newInode->name,name);
      strcpy(newInode->type,type);
      newInode->createtime=time(0);
      newInode->noOfDatablocks=0;
      newInode->noOfInodes=0;
      newInode->parent=currdirectory->id;//global
      newInode->fdcount=0;
      //Increasing the count of the number of inodes
        //printf("4\n");
        return 1;
    }
    //Else append the newInode to the end of the inodeList
    else
    {
      //inode *temp=currdirectory->inodeList;
      int i=0;
      while(currdirectory->inodeList[i]!=-1)
      {
        if(strcmp(sfssuperblock.inodes[currdirectory->inodeList[i]].name,name)==0)
        {
          return 0;
        }
        i++;
      }
      if(i<5)
      {
        currdirectory->inodeList[i]=newInode->id;
        strcpy(newInode->name,name);
        strcpy(newInode->type,type);
        newInode->createtime=time(0);
        newInode->noOfDatablocks=0;
        newInode->noOfInodes=0;
        newInode->parent=currdirectory->id;//global
        newInode->fdcount=0;
        //Increasing the count of the number of inodes
        currdirectory->noOfInodes+=1;
        return 1;
      }
      return 0;
    }


}

int deleteInode(inode *entry)
{
    if(entry->noOfInodes!=0)
    {
      return 0;
    }
    //inode *prev;
    //inode *temp=currdirectory->inodeList;
    if(strcmp(entry->type,"file")==0)
    {
      unlinkDataBlock(entry);
    }
    for(int i=0;i<5;i++)
    {
      if(currdirectory->inodeList[i]==entry->id)
      {
        sfssuperblock.inodelist[entry->id]=0;
        sfssuperblock.no_of_free_inodes++;
        currdirectory->noOfInodes--;
        for(int k=i;k<4;k++)
        {
          currdirectory->inodeList[k]=currdirectory->inodeList[k+1];
        }
        currdirectory->inodeList[4]=-1;
        return 1;
      }
    }
    return 0;
}
