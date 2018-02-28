sfs: myshell.c utility.c filetable.c inode.c superblock.c datablock.c sfs.c
	gcc -g -o sfs myshell.c utility.c filetable.c inode.c superblock.c datablock.c sfs.c -lm

