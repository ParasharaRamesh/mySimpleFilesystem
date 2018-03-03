#ifndef DUMPFS_H
#define DUMPFS_H

#include "globalConstants.h"
#include <stdio.h>

int superblockDiskWrite(superblock *sfssuperblock,char *disk);
int filetableDiskWrite(filetable *FileTable);

#endif
