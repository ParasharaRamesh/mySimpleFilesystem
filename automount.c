#include "automount.h"

superblock *superblockDiskRead(char *disk)
{
    superblock* sfssuperblock;
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return NULL;
    }
    if(fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return 0;
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
    filetable *FileTable;
    FILE *file = fopen(disk, "rb");
    if(file == NULL)
    {
        return NULL;
    }
    if(fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return 0;
    } 
    if(fread(FileTable, sizeof(filetable) , 1 , file) != sizeof(filetable))
    {
        return NULL;
    }
    fclose(file);
    return FileTable;
}