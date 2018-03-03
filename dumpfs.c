#include "globalConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//file must already exist

extern filetable *FileTable;
extern superblock *sfssuperblock;//new
int diskWrite(superblock *sfssuperblock,filetable *FileTable,char *disk)
{
    //before quitting the program, the superblock has to be written to the disk
    //file which has been created in makefile
    //need to write superblock, need to write inodelist, need to write datablocklist,
    //need to write inodes, need to write datablocks, need to write filetable
    FILE *file = fopen(disk, "w+b");
    if( file == NULL)
    {
        printf("failed to open the file for persistence!\n");
        return 0;
    }
    if( fseek( file , L_SUPERBLOCK , SEEK_SET) != 0 )
    //you have just defined L_SUPERBLOCK
    //and other things but shouldnt you have #defined it to some number??
    {
        printf("failed to lseek for writing superblock\n");
        return 0;
    }
    if( fwrite(sfssuperblock, sizeof(superblock), 1 , file) != 1)
    {
        printf("failed to write the superblock!\n");
        return 0;
    }

    if( fseek( file , L_FILETABLE , SEEK_SET) != 0 )
    {
        printf("failed to lseek for writing filetable\n");
        return 0;
    }
    if(fwrite(FileTable, sizeof(filetable) , 1 , file) != 1)
    {
        printf("failed to write filetable!\n");
        return 0;
    }
    fclose(file);
    return 1;
}

int dumpfs()
{
    char *disk="PersistantDisk.txt";
    return diskWrite(sfssuperblock,FileTable,disk);
}
