#include "globalConstants.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern filetable *FileTable;
extern superblock *sfssuperblock;//new

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
    if(fread(sfssuperblock , sizeof(superblock), 1 , file) != sizeof(superblock))
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
    if(fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return NULL;
    }
    if(fread(FileTable, sizeof(filetable) , 1 , file) != sizeof(filetable))
    {
        return NULL;
    }
    fclose(file);
    return FileTable;
}

int automount()
{
  memcpy(sfssuperblock,superblockDiskRead(disk),sizeof(superblock));
  if(sfssuperblock==NULL)
  {
    printf("automouting failed (1)!\n");
    return 0;
  }
  memcpy(FileTable,filetableDiskRead(disk),sizeof(filetable));
  if(FileTable==NULL)
  {
    printf("automouting failed (2)!\n");
    return 0;
  }
  return 1;

}
