#include "dumpfs.h"

//file must already exist
int superblockDiskWrite(superblock *sfssuperblock,char *disk);
{
    //before quitting the program, the superblock has to be written to the disk
    //file which has been created in makefile
    //need to write superblock, need to write inodelist, need to write datablocklist, 
    //need to write inodes, need to write datablocks, need to write filetable
    FILE *file = open(disk, "r+b");
    if( file == NULL)
    {
        return 0;
    }
    if( fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    {
        return 0;
    } 
    if( fwrite(sfssuperblock, sizeof(superblock), 1 , file) != sizeof(superblock))
    {
        return 0;
    }
    fclose(file);
    return 1;
}

int filetableDiskWrite(filetable *FileTable,char *disk)
{
    FILE *file = fopen(disk, "r+b");
    if( file == NULL )
    {
        return 0;
    }    
    if( fseek( file , L_FILETABLE , SEEK_SET) != 0 )
    {
        return 0;
    } 
    if(fwrite(FileTable, sizeof(filetable) , 1 , file) != sizeof(filetable))
    {
        return 0;
    }
    fclose(file);
    return 1;
}