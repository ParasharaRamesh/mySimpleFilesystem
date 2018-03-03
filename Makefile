sfs: myshell.c utility.c filetable.c inode.c superblock.c datablock.c sfs.c dumpfs.c automount.c
	gcc -g -o sfs myshell.c utility.c filetable.c inode.c superblock.c datablock.c sfs.c dumpfs.c automount.c -lm
