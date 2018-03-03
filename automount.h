#ifndef AUTOMOUNT_H
#define AUTOMOUNT_H

#include "globalConstants.h"
#include <unistd.h>

superblock *superblockDiskRead(char *disk);
filetable *filetableDiskRead(char *disk);

#endif
