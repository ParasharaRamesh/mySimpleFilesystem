#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#define TOTAL_INODES 64
#define TOTAL_DATABLOCKS 64
#define DATABLOCK_SIZE 2//was 128
#define FILETABLE_SIZE 20
#define MAX_CONTENT_LIMIT 100


#include <sys/time.h>
#include <pthread.h>

int currentshellpid;
//this is the shell workaround we have to modify the code to only pass this to all functions
// and also we gotta include an additional command in the shell called
//switch <pid> which will change this value.

//INODE RELATED
      typedef struct inode
      {

      	int id;
      	char name[32];
      	char type[10];//file or directory
      	time_t accesstime;
      	time_t createtime;
      	time_t modifytime;
      	//datablock ** datablockList;
      	//Making it into an integer array
      	//Each file can have a maximum of 3 datablocks
      	struct datablock * datablocksarray;
      	int noOfDatablocks;
      	//if inode is a directory you can have only a maximum of 5 directory entries
      	struct inode * inodeList;
      	struct inode * nextdirentry;//for traversing this inodeList
      	int noOfInodes;
      	struct inode * parent;
      	int fdcount;//for no of processes accessing this
            int n_link;
      }inode;

      int createInode(char * name, char * type);
      int deleteInode(inode * entry);

//DATABLOCK RELATED
      typedef struct datablock
      {

      	int id;
      	char data[DATABLOCK_SIZE];
      	//Can set this to -1 if it is the last data block
      	int currsize;
      	struct datablock* nextdatablock;
      }datablock;

      char *readDataBlocks(inode *file);
      //mode is 0 for overwrite 1 for append
      datablock *writeDataBlock(char* content);
      //Entire file at once
      int unlinkDataBlock(inode *file);

//FILETABLE RELATED

      typedef struct filetable
      {

      	int filedescriptor;//same as inode no for easier access
      	int who;
      	int currfilepointer;
      	struct filetable * nextfiletableentry;

      }filetable;
      //pid and filedescriptor are the primary key

      //filetable structure


      int addEntry(inode *file,int who);
      int removeEntry(inode *file,int who);
      int flushfiletable(inode *file);
      filetable *getEntry(int fd, int who);

//SUPERBLOCK RELATED

      //there are totally 64 inodes and 64 datablocks only
      typedef struct superblock
      {
          int no_of_free_inodes;
          int no_of_free_datablocks;
          int *inodelist;
          inode *inodes;
          int *datablocklist;
          datablock *datablocks;

      }superblock;

      //superblock of the fs
      //superblock * sfssuperblock;

      //functions related to superblock
      int superblockInit();
      inode *getInode();
      datablock *getDatablock();
      int superblockDestroy();
//DUMPFS related
//int diskWrite(superblock *sfssuperblock,filetable *FileTable,char *disk);
int superblockWrite(superblock *sfssuperblock,char *disk);
int filetableWrite(filetable *FileTable,char *disk);

//AUTOMOUNT related
superblock *superblockDiskRead(char *disk);
filetable *filetableDiskRead(char *disk);

//Locations for disk write/read
#define L_SUPERBLOCK 0
#define L_FILETABLE 64*(sizeof(inode)+sizeof(datablock)+2*sizeof(int))+2*(sizeof(int))//20 is extra hole size


int automount();
int dumpfs();

#endif
