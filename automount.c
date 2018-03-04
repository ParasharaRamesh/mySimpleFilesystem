#include "globalConstants.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern filetable *FileTable;
extern superblock *sfssuperblock;//new
extern inode * currdirectory;//one for root

superblock *superblockDiskRead(char *disk)
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
